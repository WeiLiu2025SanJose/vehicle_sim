#pragma once
#include "Vehicle.h"
#include <memory>

// factory to create specific vehicle per type
class VehicleFactory {
public:
    virtual ~VehicleFactory() = default;
    virtual std::unique_ptr<Vehicle> createVehicle() = 0;
};

class AlphaFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

class BravoFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

class CharlieFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

class DelaFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

class EchoFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};
