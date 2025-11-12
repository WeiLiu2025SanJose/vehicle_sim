#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "BaseStats.h"
#include "Vehicle.h"
#include <mutex>

//Singleton Vehicle Stats to store all the accumulated stat per type to statsMap
class VehicleStats {
public:
    static VehicleStats& getInstance();
    // Record a vehicle's run
    void record(const std::string& type, double ratio, const Vehicle& v);
    // it will set to common VehicleStatsData, it may be set to specific stat data per type
    void setStatData(const std::string& type, std::unique_ptr<BaseStats> stats);

    // Log all stats
    void logAll();

protected:
    // Map vehicle type -> polymorphic stats object for each type
    std::unordered_map<std::string, std::unique_ptr<BaseStats>> statsMap;
    std::mutex statsMutex;
};

