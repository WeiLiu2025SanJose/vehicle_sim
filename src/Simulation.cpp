#include "Simulation.h"
#include <iostream>

// -------------------------------------------------------
// VehicleRandomDeployment Implementation
// -------------------------------------------------------
VehicleRandomDeployment::VehicleRandomDeployment() {
    factories.emplace_back(std::make_unique<AlphaFactory>());
    factories.emplace_back(std::make_unique<BravoFactory>());
    factories.emplace_back(std::make_unique<CharlieFactory>());
    factories.emplace_back(std::make_unique<DelaFactory>());
    factories.emplace_back(std::make_unique<EchoFactory>());
    //factories.emplace_back(std::make_unique<FlyFactory>());
}

std::vector<std::unique_ptr<Vehicle>> VehicleRandomDeployment::deployVehicles() {
    std::uniform_int_distribution<> dis(0, factories.size() - 1);

    std::vector<std::unique_ptr<Vehicle>> result;
    result.reserve(20);

    for (int i = 0; i < 20; ++i) {
        result.push_back(factories[dis(gen)]->createVehicle());
    }
    return result;
}

// -------------------------------------------------------
// Simulation Implementation
// -------------------------------------------------------
Simulation::Simulation(const int& stations)
    : chargeStation(stations),
      deployment(std::make_unique<VehicleRandomDeployment>()) {} // default strategy

void Simulation::setDeployment(std::unique_ptr<VehicleDeployment> deploy) {
    deployment = std::move(deploy);  // replace existing one
}

void Simulation::runSimulation(std::chrono::seconds duration) {
    // default deployment is guaranteed
    vehicles = deployment->deployVehicles();

    std::vector<std::thread> threads;
    stopFlag = false;

    for (auto& v : vehicles)
        threads.emplace_back(&Vehicle::run, v.get(), std::ref(stopFlag), std::ref(chargeStation));

    std::this_thread::sleep_for(duration);
    stopFlag = true;

    for (auto& t : threads)
        t.join();

    std::cout << "\n=== Simulation End ===\n";
    VehicleStatsManager::getInstance().printAll();
}
