#pragma once
#include <string>
#include "Vehicle.h"

class BaseStats {
public:
    virtual ~BaseStats() = default;

    // Record data from a vehicle run and accumulate data per type
    virtual void record(double ratio, const Vehicle& v) = 0;

    // retrieve the accumulated stat per type
    virtual void log(const std::string& type) const = 0;
};
