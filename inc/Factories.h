#pragma once
#include <memory>
#include "Vehicle.h"

//Implements factory classes for different vehicle types.
class VehicleFactory {
public:
    virtual ~VehicleFactory() = default;
    virtual std::unique_ptr<Vehicle> createVehicle() = 0;
};

//we may consider for Generic Vehicle Factory
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
