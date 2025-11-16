#pragma once
#include <string>
#include <functional>

/**
 * @brief Represents a single electric vehicle in the simulation.
 *
 * A Vehicle models the behavior under simulated driving and charging
 * conditions. It maintains runtime metrics such as battery level, running time,
 * charging time, and fault probability. Concrete vehicle types (Alpha, Bravo,
 * etc.) may extend this class and override registerStats() for custom behavior.
 *
 * A vehicle participates in a lifecycle of:
 * - Running for the time which battery can support
 * - Reducing battery
 * - Triggering a charging cycle when battery is empty
 * - Charging until full
 * - Re-entering the run queue
 */
class Vehicle {
public:
    /**
     * @brief Constructs a vehicle with the given configuration parameters.
     *
     * @param type          String identifier for the vehicle type.
     * @param speed         Cruise speed in miles per hour.
     * @param capacity      Battery capacity in kWh.
     * @param timeHours     Time required to fully charge (in hours).
     * @param energy        Energy consumption in kWh per mile at Cruise speed.
     * @param passenger     Number of passengers the vehicle carries.
     * @param fault         Probability of a fault per running hour.
     */
    Vehicle(const std::string& type,
            int speed,
            int capacity,
            double timeHours,
            double energy,
            int passenger,
            double fault);

    /**
     * @brief Virtual destructor for safe inheritance.
     */
    virtual ~Vehicle() = default;

    /**
     * @brief Simulates one time slice of running.
     *
     * This updates:
     * - runningTime
     * - batteryRatio
     */
    void run();

    /**
     * @brief Simulates one time slice of charging.
     *
     * This updates:
     * - chargingTime
     * - batteryRatio
     */
    void charge();

    /**
     * @brief Checks if the battery is depleted enough to require charging.
     *
     * @return true if the vehicle should enter the charging queue.
     */
    bool needsCharge() const;

    /**
     * @brief Checks if the vehicle is fully charged.
     *
     * @return true if charging is complete.
     */
    bool isFullyCharged() const;

    // ----------------------------------------------------------------------
    // Getters
    // ----------------------------------------------------------------------

    /** @return Vehicle type string. */
    const std::string& getType() const { return vehicleType; }

    /** @return Cruise speed in mph. */
    int getCruiseSpeed() const { return cruiseSpeed; }

    /** @return Battery capacity in kWh. */
    int getBatteryCapacity() const { return batteryCapacity; }

    /** @return Time needed for full charge (hours). */
    double getTimeToCharge() const { return timeToCharge; }

    /** @return Energy usage rate in kWh per mile. */
    double getEnergyUse() const { return energyUse; }

    /** @return Number of passengers carried. */
    int getPassengers() const { return passengers; }

    /** @return Probability of fault per running hour. */
    double getFaultPerHour() const { return faultPerHour; }

    /** @return Running time accumulated for current cycle. */
    double getRunningTime() const { return runningTime; }

    /** @return Charging time for current cycle. */
    double getChargingTime() const { return chargingTime; }

    /**
     * @brief Sets the time slice duration used during simulation.
     *
     * @param ms Real milliseconds representing one simulated second.
     */
    void setTimeSliceMs(int ms) { timeSliceMs = ms; }

    /**
     * @brief Resets the running time at the beginning of a new cycle.
     */
    void resetRunningTime() { runningTime = 0; }

    /**
     * @brief Resets the charging time at the beginning of a new cycle.
     */
    void resetChargingTime() { chargingTime = 0; }

private:

    std::string vehicleType;     ///< Identifier: "Alpha", "Bravo", etc.
    int cruiseSpeed;             ///< Cruise speed (mph)
    int batteryCapacity;         ///< Battery capacity (kWh)
    double timeToCharge;         ///< Hours required for a full charge
    double energyUse;            ///< Energy use (kWh per mile)
    int passengers;              ///< Passenger count
    double faultPerHour;         ///< Fault probability per hour

    double runningTime = 0;      ///< Accumulated runtime for current cycle
    double chargingTime = 0;     ///< Accumulated charging time for current cycle
    double batteryRatio = 1.0;   ///< Battery level ratio (1.0 = full)
    int timeSliceMs = 100;       ///< Real milliseconds per simulation-second

protected:
    /**
     * @brief Registers runtime statistics for the vehicle.
     *
     * Concrete vehicle types may override this to customize stat reporting.
     * This is typically invoked from run() and charge().
     */
    virtual void registerStats();

#ifdef UNIT_TESTING
    friend class VehicleRegisterStatsTest;
    friend class VehicleStatsManagerTest;
    friend class RunnerLogicTest;
#endif
};
