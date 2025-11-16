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

/**
 * @brief Strategy interface for defining how vehicles are created and deployed into the simulation.
 *
 * Implementations of this class decide what types of vehicles exist and how many
 * should be created before the simulation begins.
 */
class VehicleDeployment {
public:
    virtual ~VehicleDeployment() = default;

    /**
     * @brief Creates and returns a list of vehicles ready to be inserted into the simulation.
     *
     * @return A vector of unique_ptr-managed Vehicle objects.
     */
    virtual std::vector<std::unique_ptr<Vehicle>> deployVehicles() = 0;
};

/**
 * @brief Deployment strategy that creates a randomized list of vehicles
 *        using the various VehicleFactory implementations.
 *
 * This class holds a list of vehicle factories and randomly chooses which vehicles
 * to generate, providing diversity in the simulated environment.
 */
class VehicleRandomDeployment : public VehicleDeployment {
public:
    /**
     * @brief Constructs the randomized deployment strategy and initializes the factories.
     */
    VehicleRandomDeployment();

    /**
     * @brief Creates a randomized set of vehicles from the available factories.
     *
     * @return Vector of vehicles generated through vehicle factories.
     */
    std::vector<std::unique_ptr<Vehicle>> deployVehicles() override;

private:
    std::vector<std::unique_ptr<VehicleFactory>> factories; ///< Registered factories
    std::mt19937 gen{std::random_device{}()};               ///< Random number generator
};

/**
 * @brief Central controller for the multi-threaded EV simulation.
 *
 * The Simulation class launches three worker threads:
 *  - **Runner thread:** run vehicle.
 *  - **Need-charge dispatcher:** acquire station and push them to chargers.
 *  - **Charger thread:** charge vehicle and reintroduces charged vehicles into the run queue.
 *
 * It coordinates these threads through several ThreadSafeQueue instances and synchronizes access
 * to limited charging-station resources via ChargeStationManager.
 */
class Simulation {
public:
    /**
     * @brief Constructs a simulation environment.
     *
     * @param stations Number of available charging stations.
     * @param timeSliceMs Real-time milliseconds per simulated second (time granularity).
     */
    Simulation(int stations, int timeSliceMs = 100);

    /**
     * @brief Specifies the deployment strategy used to generate starting vehicles.
     *
     * @param deploy Deployment strategy providing an initial set of vehicles.
     */
    void setDeployment(std::unique_ptr<VehicleDeployment> deploy);

    /**
     * @brief Launches the simulation for a specified duration.
     *
     * This function:
     *  - Builds the initial vehicle list using the deployment strategy.
     *  - Starts three worker threads.
     *  - Runs until the simulated time expires.
     *  - Signals all threads to stop, joins them, and produces statistics.
     *
     * @param simulatedDuration How long the simulated world should run.
     */
    void runSimulation(std::chrono::seconds simulatedDuration);

private:
    /** @brief Worker thread that runs vehicles for each time slice and checks whether they require charging. */
    void runnerThreadFunc();

    /** @brief Worker thread that transfers depleted vehicles into the charging queue. */
    void needChargeDispatcherFunc();

    /** @brief Worker thread that performs the charging simulation and returns vehicles to the run queue. */
    void chargerThreadFunc();

    ThreadSafeQueue<Vehicle*> runQueue;          ///< Vehicles that are actively running
    ThreadSafeQueue<Vehicle*> needChargeQueue;   ///< Vehicles that require charging
    ThreadSafeQueue<Vehicle*> chargeQueue;       ///< Vehicles currently charging

    std::vector<std::unique_ptr<Vehicle>> vehicles; ///< All vehicles created for the simulation
    ChargeStationManager stationManager;            ///< Manages access to limited charging stations

    std::atomic<bool> stopFlag{false};              ///< Global stop condition for all threads
    std::unique_ptr<VehicleDeployment> deployment;  ///< Vehicle creation strategy

    std::thread runnerThread;        ///< Thread responsible for running vehicles
    std::thread needChargeThread;    ///< Thread responsible for dispatching depleted vehicles
    std::thread chargerThread;       ///< Thread responsible for charging vehicles

    int msTimeSlice;                 ///< Real-time milliseconds per simulated in-world second

#ifdef UNIT_TESTING
    friend class RunnerLogicTest;    ///< For unit-test access to internals
#endif
};
