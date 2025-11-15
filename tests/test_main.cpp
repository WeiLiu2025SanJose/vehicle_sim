#include "VehicleStatsManager.h"
#include "VehicleStatsData.h"
#include "ChargeStationManager.h"
#include "Simulation.h"
#include "Factories.h"
#include <cassert>
#include <thread>
#include <iostream>

struct VehicleParams {
    std::string type;
    int cruiseSpeed;
    int batteryCapacity;
    double timeToCharge;
    double energyUse;
    int passengers;
    double faultPerHour;
};
// ------------------------------------------
// VehicleStatsManager tests
// ------------------------------------------
class VehicleStatsManagerTest {
public:
    static void run() {
        std::cout << "[TEST] VehicleStatsManager basic..." << std::endl;
        //resetVehicleStatsManager();

        auto& mgr = VehicleStatsManager::getInstance();
        mgr.setStatData("Alpha", std::make_unique<VehicleStatsData>());

        Vehicle v("Alpha", 60, 100, 1.0, 2.0, 2, 0.1);

        v.runningTime=3000;
        mgr.record(v.getType(), v,StatType::TotalTestVehicle);
        mgr.record(v.getType(), v,StatType::TotalTime);
        v.resetRunningTime();
        v.runningTime=1000;
        mgr.record(v.getType(), v,StatType::TotalTestVehicle);
        mgr.record(v.getType(), v,StatType::TotalTime);


        auto* stats = dynamic_cast<VehicleStatsData*>(mgr.statsMap["Alpha"].get());
        assert(stats != nullptr);
        //std::cout << stats->getAverageTime()<<std::endl";
        assert(stats->getAverageTime() == 2000);

        std::cout << " VehicleStatsManagerTest passed\n";
    }
};

// ------------------------------------------
// ChargeStationManager concurrency test
// ------------------------------------------
class ChargeStationManagerTest {
public:
    static void run() {
        std::cout << "[TEST] ChargeStationManager concurrency..." << std::endl;
        std::atomic<bool> stopFlag=false;
        ChargeStationManager mgr(2);

        auto job = [&]() {
            mgr.acquire(stopFlag);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            mgr.release();
        };

        std::thread t1(job);
        std::thread t2(job);
        std::thread t3(job);

        t1.join();
        t2.join();
        t3.join();

        std::cout << " ChargeStationManagerTest passed\n";
    }
};

// ------------------------------------------
// Vehicle::registerStats test
// ------------------------------------------
class VehicleRegisterStatsTest {
public:
    static void run() {
        std::cout << "[TEST] Vehicle::registerStats..." << std::endl;
        //resetVehicleStatsManager();

        Vehicle v("Bravo", 80, 150, 1.5, 3.0, 4, 0.05);
        v.registerStats();

        auto& mgr = VehicleStatsManager::getInstance();
        assert(mgr.statsMap.find("Bravo") != mgr.statsMap.end());

        std::cout << " VehicleRegisterStatsTest passed\n";
    }
};

// ------------------------------------------
// Vehicle factories test
// ------------------------------------------
class FactoryTest {
public:
    static void run() {
        std::cout << "[TEST] Vehicle factories..." << std::endl;

        AlphaFactory a; BravoFactory b; CharlieFactory c; DelaFactory d; EchoFactory e;
        auto va = a.createVehicle();
        auto vb = b.createVehicle();
        auto vc = c.createVehicle();
        auto vd = d.createVehicle();
        auto ve = e.createVehicle();

        assert(va->getCruiseSpeed() ==120);// it will fail,not equal to 120
        assert(!vb->getType().compare("Bravo"));
        assert(vc->getBatteryCapacity() ==220);
        assert(vd->getTimeToCharge() == 0.62);
        assert(ve->getFaultPerHour() == 0.61);

        std::cout << " FactoryTest passed\n";
    }
};

// ------------------------------------------
// Simulation integration test
// ------------------------------------------
class SimulationIntegrationTest {
public:
    static void run() {
        std::cout << "[TEST] Simulation integration..." << std::endl;
        //resetVehicleStatsManager();

        Simulation sim(2);
        sim.setDeployment(std::make_unique<VehicleRandomDeployment>());

        sim.runSimulation(std::chrono::seconds(2));
        VehicleStatsManager::getInstance().printAll();

        std::cout << " SimulationIntegrationTest passed\n";
    }
};

class RunnerLogicTest {
public:
    static void run() {
        std::cout << "[TEST] Runner logic..." << std::endl;

        Simulation sim(2, 1);
        AlphaFactory a;
        auto v = a.createVehicle();;
        Vehicle* vp = v.get();
        sim.runQueue.push(vp);
        //runnerThread = std::thread(&Simulation::runnerThreadFunc, this);
        vp->batteryRatio = 0.0;
        std::thread testThread = std::thread(&Simulation::runnerThreadFunc, &sim);
	//sim.runnerThreadFunc();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
	sim.stopFlag=true;
        //vp->run();
        if (testThread.joinable()) testThread.join();

        assert(sim.needChargeQueue.size() == 1);
        assert(sim.runQueue.size() == 0);

        std::cout << " RunnerLogicTest passed\n";
    }
};
// ------------------------------------------
// Test Runner
// ------------------------------------------
int main() {
    VehicleStatsManagerTest::run();
    ChargeStationManagerTest::run();
    VehicleRegisterStatsTest::run();
    FactoryTest::run();
    SimulationIntegrationTest::run();
    RunnerLogicTest::run();

    std::cout << "\n All tests passed successfully!\n";
    return 0;
}
