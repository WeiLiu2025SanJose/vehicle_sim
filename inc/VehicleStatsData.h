#pragma once
#include "BaseStats.h"
#include <mutex>
#include <iostream>
/**
 * @brief Defines the VehicleStatsData class, which stores aggregated
 *        statistical metrics for a specific vehicle type.
 *
 * This class is responsible for collecting and computing:
 *   - Average running time
 *   - Average distance traveled
 *   - Average charging time
 *   - Total vehicle faults
 *   - Total passenger-miles
 *
 * It is thread-safe and can be called concurrently by multiple worker threads.
 */
class VehicleStatsData : public BaseStats {
public:
    /**
     * @brief Gets the average total running time of vehicles of this type.
     */
    double getAverageTime() const { return averageTime; }

    /**
     * @brief Gets the average distance traveled across all completed runs.
     */
    double getAverageDistance() const { return averageDistance; }

    /**
     * @brief Gets the average charging time for vehicles of this type.
     */
    double getAverageChargeTime() const { return averageChargeTime; }

    /**
     * @brief Gets the total number of fault events recorded.
     */
    double getTotalFaults() const { return totalFaults; }

    /**
     * @brief Gets the sum of (passengers × miles) across all completed runs.
     *        Useful for load efficiency statistics.
     */
    double getTotalPassengersMiles() const { return totalPassengersMiles; }

    /**
     * @brief Records a completed vehicle cycle (running or charging).
     * @param v    Vehicle whose stats are being recorded.
     * @param type Indicates whether this is a run cycle or charge event.
     *
     * This function updates aggregated totals and recomputes averages.
     * It is thread-safe and locks the internal mutex.
     */
    void record(const Vehicle& v, StatType type) override;

    /**
     * @brief Logs the final computed stats.
     * @param type The vehicle type.
     */
    void log(const std::string& type) const override;

private:
    double totalTime = 0;        ///< Total accumulated running time (hours)
    double averageTime = 0;      ///< Average running time (hours)
    double totalTestVehicle = 0; ///< Number of vehicles that completed running

    double totalDistance = 0;    ///< Total miles traveled by all vehicles
    double averageDistance = 0;  ///< Mean miles per run

    double totalChargedVehicle = 0; ///< Number of vehicles that completed charging
    double totalChargeTime = 0;     ///< Total charge time across all cycles
    double averageChargeTime = 0;   ///< Average charge duration (hours)

    double totalFaults = 0;           ///< Sum of all faults encountered
    double totalPassengersMiles = 0;  ///< Sum of (passengers × miles)

    mutable std::mutex statsMutex; ///< Guards access to all stored statistics
};
