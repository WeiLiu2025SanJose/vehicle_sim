#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include "Vehicle.h"
#include "VehicleStats.h"
#include "ChargeStationManager.h"
#include "Factories.h"

class Simulation {
public:
    Simulation();
    void runSimulation(std::chrono::seconds duration);

private:
    std::unique_ptr<Vehicle> createRandomVehicle();

    std::vector<std::unique_ptr<Vehicle>> vehicles;
    ChargeStationManager chargeStation;
    std::atomic<bool> stopFlag{false};
};
