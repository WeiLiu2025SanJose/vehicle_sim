#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include "BaseStats.h"

class Vehicle;

/**
 * @brief A singleton responsible for storing and updating statistical
 *        information for all vehicle types in the simulation.
 *
 * The manager uses a map of:
 *      vehicleType → BaseStats-derived object
 *
 * By default, VehicleStatsData is used, but any class deriving from BaseStats
 * can be installed via setStatData() for unit testing or extended behaviors.
 *
 * Thread safety:
 *   - All operations modifying the statsMap are protected by statsMutex.
 */
class VehicleStatsManager {
public:

    /**
     * @brief Returns the global singleton instance.
     */
    static VehicleStatsManager& getInstance();

    /**
     * @brief Records a statistic event for a specific vehicle type.
     *
     * @param type     The vehicle type string (e.g., "Sedan", "EVBus").
     * @param v        Reference to the vehicle whose stats are to be recorded.
     * @param statType Type of statistic event (RUN or CHARGE).
     *
     * If no statistics object exists for the given type, one is created
     * using the default implementation (VehicleStatsData).
     *
     */
    void record(const std::string& type,
                const Vehicle& v,
                StatType statType);

    /**
     * @brief Registers (or overwrites) a BaseStats implementation for a vehicle type.
     *
     * @param type  The vehicle type.
     * @param stats A unique_ptr to a BaseStats-derived object.
     *
     * This allows:
     *   - Replacing VehicleStatsData with a custom stats collector
     *
     */
    void setStatData(const std::string& type,
                     std::unique_ptr<BaseStats> stats);

    /**
     * @brief Logs all stored statistics to the console and/or output file.
     *
     * Iterates through every vehicle type registered in statsMap and calls
     * BaseStats::log() on each.
     *
     */
    void printAll();

protected:
    /**
     * @brief Mapping from vehicle type string to a statistics storage object.
     *
     * Each value is a BaseStats-derived instance, stored in a unique_ptr.
     */
    std::unordered_map<std::string, std::unique_ptr<BaseStats>> statsMap;

    /**
     * @brief Mutex to protect statsMap for thread-safe operations.
     */
    std::mutex statsMutex;

private:
    VehicleStatsManager() = default;
    ~VehicleStatsManager() = default;

    // Disable copying and assignment to enforce singleton
    VehicleStatsManager(const VehicleStatsManager&) = delete;
    VehicleStatsManager& operator=(const VehicleStatsManager&) = delete;

#ifdef UNIT_TESTING
    friend class VehicleStatsManagerTest;
    friend class VehicleRegisterStatsTest;
#endif
};

