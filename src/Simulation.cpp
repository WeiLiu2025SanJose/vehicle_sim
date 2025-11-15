#include "Simulation.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
using namespace std;

VehicleRandomDeployment::VehicleRandomDeployment() {
    factories.emplace_back(std::make_unique<AlphaFactory>());
    factories.emplace_back(std::make_unique<BravoFactory>());
    factories.emplace_back(std::make_unique<CharlieFactory>());
    factories.emplace_back(std::make_unique<DelaFactory>());
    factories.emplace_back(std::make_unique<EchoFactory>());
}

//produces a randomized list of vehicle objects using various factories.
std::vector<std::unique_ptr<Vehicle>> VehicleRandomDeployment::deployVehicles() {
    std::uniform_int_distribution<> dis(0, static_cast<int>(factories.size()) - 1);
    std::vector<std::unique_ptr<Vehicle>> result;
    result.reserve(20);
    for (int i = 0; i < 20; ++i) {
        result.push_back(factories[dis(gen)]->createVehicle());
    }
    return result;
}

// in constructor, we set the deployment stategy
Simulation::Simulation(int stations, int timeSliceMs)
    : stationManager(stations),
      deployment(std::make_unique<VehicleRandomDeployment>()),
      msTimeSlice(timeSliceMs)
{}

// if potential to change another one
void Simulation::setDeployment(std::unique_ptr<VehicleDeployment> deploy) {
    deployment = std::move(deploy);
}

void Simulation::runSimulation(std::chrono::seconds simulatedDuration) {
    // Create vehicles via deployment strategy
    vehicles = deployment->deployVehicles();

    // init run queue and set vehicle time-slice
    for (auto& v : vehicles) {
        v->setTimeSliceMs(msTimeSlice);
        runQueue.push(v.get());
        VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalTestVehicle);
    }

    stopFlag = false;

    // start three threads
    runnerThread = std::thread(&Simulation::runnerThreadFunc, this);
    needChargeThread = std::thread(&Simulation::needChargeDispatcherFunc, this);
    chargerThread = std::thread(&Simulation::chargerThreadFunc, this);

    // run for simulatedDuration seconds, scaled by msTimeSlice real ms per sim-second
    auto totalMs = simulatedDuration.count() * msTimeSlice;
    std::this_thread::sleep_for(std::chrono::milliseconds(totalMs));

    // request stop
    stopFlag = true;
    // wake all threads blocked in acquire()
    stationManager.stopAll();
    // wake threads
    runQueue.notifyAll();
    needChargeQueue.notifyAll();
    chargeQueue.notifyAll();

    // join threads
    if (runnerThread.joinable()) runnerThread.join();
    if (needChargeThread.joinable()) needChargeThread.join();
    if (chargerThread.joinable()) chargerThread.join();

    std::cout << "\n=== Simulation End ===\n";
    
    // for the vehicle not complete one running cycle or charging cycle,store the running/charging time.
    // assume we should still store the data which not fully complete.
    for (auto& v : vehicles){
        VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalChargeTime);
        VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalTime);
    }
    VehicleStatsManager::getInstance().printAll();
}

// Runner thread: run the vechicle in the queue and 1) requeue in runner or needCharge
void Simulation::runnerThreadFunc() {
    while (!stopFlag) {
    	int size = runQueue.size();
    	for (int i=0;i<size;i++){
    	    if(stopFlag) break;
    	    auto opt = runQueue.tryPop();

            if (!opt) continue;
            Vehicle* v =  *opt;
            v->run();

            if (v->needsCharge()) {
                VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalTime);
                v->resetRunningTime();
                needChargeQueue.push(v);
            } else {
                // requeue for next second
                runQueue.push(v);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(msTimeSlice));
    }
}

// needCharge thread: check whether station is available and enqueue charge queue if any.
void Simulation::needChargeDispatcherFunc() {
    while (!stopFlag) {
        auto opt = needChargeQueue.tryPop();

        if (!opt) {
            std::this_thread::sleep_for(std::chrono::milliseconds(msTimeSlice));
            continue;
        }
        Vehicle* v  = *opt;
        // blocks until a station is available
        stationManager.acquire(this->stopFlag);
        // now vehicle holds a station; recored total charge cycle per type and push to chargingQueue for charger thread to process
        if(stopFlag) break;
        VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalChargeCycle);
        chargeQueue.push(v);
    }
}

// Charger thread: charge the vehicle and requeue, or push to runner if charge is complete
void Simulation::chargerThreadFunc() {
    while (!stopFlag) {
    	int size = chargeQueue.size();
    	for (int i=0;i<size;i++){
    	   if(stopFlag) break;
           auto opt = chargeQueue.tryPop();
            if (!opt) continue;
            
            Vehicle* v = *opt;
            v->charge();
            
            if(v->isFullyCharged()){
                // release station
                stationManager.release();
                // recorder charge time and increase test Vehicle cycle per tyoe, then push back to run queue
                VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalChargeTime);
                VehicleStatsManager::getInstance().record(v->getType(), *v,StatType::TotalTestVehicle);
                v->resetChargingTime();
                runQueue.push(v);
            } else {
                chargeQueue.push(v);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(msTimeSlice));
    }
}
