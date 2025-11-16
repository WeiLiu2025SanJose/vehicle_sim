#pragma once
#include <string>
#include <mutex>
#include <condition_variable>
#include <atomic>

/**
 * @brief Coordinates access to a finite number of charging stations.
 *
 * This class regulates concurrent vehicle access to shared charging resources.
 * It implements a blocking acquire/release model using a mutex and
 * condition variable, ensuring that no more than the configured number of
 * vehicles may charge simultaneously.
 */
class ChargeStationManager {
public:
    /**
     * @brief Constructs a manager controlling the specified number of stations.
     *
     * @param totalStations  The total number of charging stations available.
     */
    explicit ChargeStationManager(int totalStations);

    /**
     * @brief Attempts to acquire a charging station slot.
     *
     * This method blocks until a station becomes available or the simulation
     * requests termination. If stopFlag becomes true during the wait,
     * the call returns immediately without acquiring a slot.
     *
     * @param stopFlag  External termination flag monitored during blocking wait.
     */
    void acquire(std::atomic<bool>& stopFlag);

    /**
     * @brief Releases a previously acquired charging station slot.
     *
     * This operation wakes one waiting thread, if any.
     */
    void release();

    /**
     * @brief Returns the current number of unoccupied charging stations.
     *
     * @return Count of available charging stations.
     */
    int getAvailable() const;

    /**
     * @brief Notifies all waiting threads to terminate blocking waits.
     *
     * Used when the simulation is shutting down to unblock calls to acquire().
     */
    void stopAll();

private:
    int availableStations;                   ///< Number of unoccupied charging stations.
    mutable std::mutex mtx;                  ///< Protects station counter.
    std::condition_variable cv;              ///< Coordinates waiting and wakeup events.
};
