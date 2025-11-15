#pragma once
#include "BaseStats.h"
#include <mutex>
#include <iostream>

// Stores statistical metrics for one vehicle type and computes averages:
class VehicleStatsData : public BaseStats {
public:
    double getAverageTime() const { return averageTime; }
    double getAverageDistance() const { return averageDistance; }
    double getAverageChargeTime() const { return averageChargeTime; }
    double getTotalFaults() const { return totalFaults; }
    double getTotalPassengersMiles() const { return totalPassengersMiles; }

    void record(const Vehicle& v,StatType type) override;
    void log(const std::string& type) const override;

private:
    double totalTime = 0;
    double averageTime = 0;
    double totalTestVehicle = 0;

    double totalDistance = 0;
    double averageDistance = 0;

    double totalChargedVehicle = 0;
    double totalChargeTime = 0;
    double averageChargeTime = 0;

    double totalFaults = 0;
    double totalPassengersMiles = 0;

    mutable std::mutex statsMutex;
};
