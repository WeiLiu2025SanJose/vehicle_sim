#include "Factories.h"
#include "VehicleStats.h"
#include "VehicleStatsData.h"
#include <vector>

using namespace std;

struct VehicleParams {
    std::string type;
    int cruiseSpeed;
    int batteryCapacity;
    double timeToCharge;
    double energyUse;
    int passengers;
    double faultPerHour;
};

inline const std::vector<VehicleParams> vehicleDataSource = {
    {"Alpha", 120, 320, 0.6, 1.6, 4, 0.25},
    {"Bravo", 100, 100, 0.2, 1.5, 5, 0.10},
    {"Charlie", 160, 220, 0.8, 2.2, 3, 0.05},
    {"Dela", 90, 120, 0.62, 0.8, 2, 0.22},
    {"Echo", 30, 150, 0.3, 5.8, 2, 0.61}
    // {"Fly", 500, 200, 0.5, 15.0, 4, 0.01}, // add FlyVehicle if needed
};

class AlphaVehicle : public Vehicle {
public:
    AlphaVehicle()
        : Vehicle(vehicleDataSource[0].type,
                  vehicleDataSource[0].cruiseSpeed,
                  vehicleDataSource[0].batteryCapacity,
                  vehicleDataSource[0].timeToCharge,
                  vehicleDataSource[0].energyUse,
                  vehicleDataSource[0].passengers,
                  vehicleDataSource[0].faultPerHour)
    {
        registerStats();
    }
};
class BravoVehicle : public Vehicle {
public:
    BravoVehicle()
        : Vehicle(vehicleDataSource[1].type,
                  vehicleDataSource[1].cruiseSpeed,
                  vehicleDataSource[1].batteryCapacity,
                  vehicleDataSource[1].timeToCharge,
                  vehicleDataSource[1].energyUse,
                  vehicleDataSource[1].passengers,
                  vehicleDataSource[1].faultPerHour)
    {
        registerStats();
    }
};
class CharlieVehicle : public Vehicle {
public:
    CharlieVehicle()
        : Vehicle(vehicleDataSource[2].type,
                  vehicleDataSource[2].cruiseSpeed,
                  vehicleDataSource[2].batteryCapacity,
                  vehicleDataSource[2].timeToCharge,
                  vehicleDataSource[2].energyUse,
                  vehicleDataSource[2].passengers,
                  vehicleDataSource[2].faultPerHour)
    {
        registerStats();
    }
};
class DelaVehicle : public Vehicle {
public:
    DelaVehicle()
        : Vehicle(vehicleDataSource[3].type,
                  vehicleDataSource[3].cruiseSpeed,
                  vehicleDataSource[3].batteryCapacity,
                  vehicleDataSource[3].timeToCharge,
                  vehicleDataSource[3].energyUse,
                  vehicleDataSource[3].passengers,
                  vehicleDataSource[3].faultPerHour)
    {
        registerStats();
    }
};
class EchoVehicle : public Vehicle {
public:
    EchoVehicle()
        : Vehicle(vehicleDataSource[4].type,
                  vehicleDataSource[4].cruiseSpeed,
                  vehicleDataSource[4].batteryCapacity,
                  vehicleDataSource[4].timeToCharge,
                  vehicleDataSource[4].energyUse,
                  vehicleDataSource[4].passengers,
                  vehicleDataSource[4].faultPerHour)
    {
        registerStats();
    }
};
/*class FlyVehicle : public Vehicle {
public:
    FlyVehicle()
        : Vehicle(vehicleDataSource[5].type,
                  vehicleDataSource[5].cruiseSpeed,
                  vehicleDataSource[5].batteryCapacity,
                  vehicleDataSource[5].timeToCharge,
                  vehicleDataSource[5].energyUse,
                  vehicleDataSource[5].passengers,
                  vehicleDataSource[5].faultPerHour)
    {
        registerStats();
    }
protected:
    void registerStats() override;  // override to register FlyVehicleStatsData
};
void FlyVehicle::registerStats() {
    std::cout<<"call fly"<<std::endl;
    auto& vs = VehicleStatsManager::getInstance();
    vs.setStatData("Fly", std::make_unique<FlyVehicleStatsData>());
}*/
std::unique_ptr<Vehicle> AlphaFactory::createVehicle() { return std::make_unique<AlphaVehicle>(); }
std::unique_ptr<Vehicle> BravoFactory::createVehicle() { return std::make_unique<BravoVehicle>(); }
std::unique_ptr<Vehicle> CharlieFactory::createVehicle() { return std::make_unique<CharlieVehicle>(); }
std::unique_ptr<Vehicle> DelaFactory::createVehicle() { return std::make_unique<DelaVehicle>(); }
std::unique_ptr<Vehicle> EchoFactory::createVehicle() { return std::make_unique<EchoVehicle>(); }
//std::unique_ptr<Vehicle> FlyFactory::createVehicle() {  return std::make_unique<FlyVehicle>();}
