#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include "BaseStats.h"

class VehicleStatsManager {
public:
    static VehicleStatsManager& getInstance();
    //Stores statistical metrics for one vehicle type
    void record(const std::string& type, const class Vehicle& v, StatType statType);
    //if we have statsdata other than VehicleStatsData
    void setStatData(const std::string& type, std::unique_ptr<BaseStats> stats);
    // log to console as well as file
    void printAll();

protected:
    // mapping type to stats
    std::unordered_map<std::string, std::unique_ptr<BaseStats>> statsMap;
    std::mutex statsMutex;

private:
    VehicleStatsManager() = default;
    ~VehicleStatsManager() = default;
    VehicleStatsManager(const VehicleStatsManager&) = delete;
    VehicleStatsManager& operator=(const VehicleStatsManager&) = delete;
#ifdef UNIT_TESTING
    friend class VehicleStatsManagerTest;
    friend class VehicleRegisterStatsTest;
#endif
};

