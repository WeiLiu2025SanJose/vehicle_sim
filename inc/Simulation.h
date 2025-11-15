#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <chrono>
#include <random>
#include "ThreadSafeQueue.h"
#include "Vehicle.h"
#include "ChargeStationManager.h"
#include "VehicleStatsManager.h"
#include "Factories.h"

// Defines the strategy for how simulation vehicles are created and deployed.
class VehicleDeployment {
public:
    virtual ~VehicleDeployment() = default;
    virtual std::vector<std::unique_ptr<Vehicle>> deployVehicles() = 0;
};

// Concrete implementation that produces a randomized list of vehicle objects using various factories.
class VehicleRandomDeployment : public VehicleDeployment {
public:
    VehicleRandomDeployment();
    std::vector<std::unique_ptr<Vehicle>> deployVehicles() override;
private:
    std::vector<std::unique_ptr<VehicleFactory>> factories;
    std::mt19937 gen{std::random_device{}()};
};

//Central controller for the multi-threaded EV simulation.
class Simulation {
public:
    // timeSliceMs controls real ms per simulated second
    Simulation(int stations, int timeSliceMs = 100);

    void setDeployment(std::unique_ptr<VehicleDeployment> deploy);
    void runSimulation(std::chrono::seconds simulatedDuration);

private:
    // thread functions
    //Runs vehicles for one time slice and decides if they need charging.
    void runnerThreadFunc();
    //Moves depleted vehicles to charging stations.
    void needChargeDispatcherFunc();
    //Simulates charging and returns vehicles to the run queue.
    void chargerThreadFunc();

    ThreadSafeQueue<Vehicle*> runQueue;
    ThreadSafeQueue<Vehicle*> needChargeQueue;
    ThreadSafeQueue<Vehicle*> chargeQueue;

    std::vector<std::unique_ptr<Vehicle>> vehicles; 
    ChargeStationManager stationManager;

    std::atomic<bool> stopFlag{false};
    std::unique_ptr<VehicleDeployment> deployment;

    // threads
    std::thread runnerThread;
    std::thread needChargeThread;
    std::thread chargerThread;

    int msTimeSlice;
#ifdef UNIT_TESTING
    friend class RunnerLogicTest;
#endif
};

