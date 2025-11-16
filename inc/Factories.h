#pragma once
#include <memory>
#include "Vehicle.h"

/**
 * @brief Abstract factory interface for constructing vehicles.
 *
 * Factories allow the simulation to instantiate heterogeneous vehicle types
 * without coupling to concrete constructors. The deployment strategy selects
 * specific factories at runtime.
 */
class VehicleFactory {
public:
    virtual ~VehicleFactory() = default;

    /**
     * @brief Creates a new vehicle instance.
     *
     * @return A unique_ptr containing a dynamically constructed Vehicle.
     */
    virtual std::unique_ptr<Vehicle> createVehicle() = 0;
};

/**
 * @brief Factory for producing Alpha-type vehicles.
 */
class AlphaFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

/**
 * @brief Factory for producing Bravo-type vehicles.
 */
class BravoFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

/**
 * @brief Factory for producing Charlie-type vehicles.
 */
class CharlieFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

/**
 * @brief Factory for producing Dela-type vehicles.
 */
class DelaFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};

/**
 * @brief Factory for producing Echo-type vehicles.
 */
class EchoFactory : public VehicleFactory {
public:
    std::unique_ptr<Vehicle> createVehicle() override;
};
