#pragma once
#include <string>
#include "Vehicle.h"

enum class StatType {
    TotalTestVehicle,
    TotalTime,
    TotalChargeCycle,
    TotalChargeTime,
    // Add more as needed
};

// Provides an abstract interface for recording and logging statistical data of vehicles.
class BaseStats {
public:
    virtual ~BaseStats() = default;
    virtual void record(const Vehicle& v,StatType type) = 0;
    virtual void log(const std::string& type) const = 0;
};
