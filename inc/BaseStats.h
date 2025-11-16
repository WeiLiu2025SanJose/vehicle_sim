#pragma once
#include <string>
#include "Vehicle.h"

/**
 * @brief Enumerates all statistical categories recorded during simulation.
 *
 * These values specify which metric is being updated.
 */
enum class StatType {
    TotalTestVehicle,   /**< Counts vehicles participating in simulation. */
    TotalTime,          /**< Records running time accumulated by vehicles. */
    TotalChargeCycle,   /**< Counts the number of charge cycles completed. */
    TotalChargeTime     /**< Records total time spent charging. */
};

/**
 * @brief Abstract interface for collecting and reporting vehicle statistics.
 *
 * Concrete implementations (e.g., VehicleStatsData) record simulation metrics
 * such as total run time, distance travelled, charging time, etc. The
 * VehicleStatsManager holds one instance per vehicle type.
 */
class BaseStats {
public:
    /**
     * @brief Virtual destructor for safe polymorphic cleanup.
     */
    virtual ~BaseStats() = default;

    /**
     * @brief Records a statistical update for a vehicle.
     *
     * @param v     Reference to the vehicle generating the metric.
     * @param type  Type of metric being updated.
     */
    virtual void record(const Vehicle& v, StatType type) = 0;

    /**
     * @brief Logs summary results for this vehicle type.
     *
     * Implementations may print to console or also write to log files.
     *
     * @param type  Vehicle type name associated with this statistics instance.
     */
    virtual void log(const std::string& type) const = 0;
};

