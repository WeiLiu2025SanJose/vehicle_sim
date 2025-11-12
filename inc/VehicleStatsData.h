#pragma once
#include "BaseStats.h"
#include <iostream>

// common Vehicle Stat dataset and the way to accumulate
class VehicleStatsData : public BaseStats {
public:
    double totalTime = 0;
    double totalTestVehicle = 0;
    double totalDistance = 0;
    double totalChargeTime = 0;
    double totalPassengersMiles = 0;
    double totalFaults = 0;
    std::mutex statsMutex;
    
    void record(double ratio, const Vehicle& v) override;
    void log(const std::string& type) const override;
};
