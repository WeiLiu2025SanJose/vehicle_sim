#pragma once
#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <chrono>
#include <random>
#include "Vehicle.h"
#include "VehicleStatsManager.h"
#include "ChargeStationManager.h"
#include "Factories.h"

// -------------------------------------------------------
// Base class for vehicle deployment strategy
// -------------------------------------------------------
class VehicleDeployment {
public:
    virtual ~VehicleDeployment() = default;
    virtual std::vector<std::unique_ptr<Vehicle>> deployVehicles() = 0;
};

// -------------------------------------------------------
// Random vehicle deployment implementation
// -------------------------------------------------------
class VehicleRandomDeployment : public VehicleDeployment {
public:
    VehicleRandomDeployment();
    std::vector<std::unique_ptr<Vehicle>> deployVehicles() override;

private:
    std::vector<std::unique_ptr<VehicleFactory>> factories;
    std::mt19937 gen{std::random_device{}()};
};

// -------------------------------------------------------
// Simulation class
// -------------------------------------------------------
class Simulation {
public:
    Simulation(const int& stations);

    void runSimulation(std::chrono::seconds duration);
    void setDeployment(std::unique_ptr<VehicleDeployment> deploy);

private:
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    ChargeStationManager chargeStation;
    std::atomic<bool> stopFlag{false};
    std::unique_ptr<VehicleDeployment> deployment;
};
