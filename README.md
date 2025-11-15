Build and Test

1. Build simulation:
make
./bin/simulation

2. Build test runner:
make test
./bin/test_runner

3. Clean:
make clean

4. test result:
test result will log to console as well as to the file "stats_log.txt".


Components and class description

1. BaseStats (Interface)
Provides an abstract interface for recording and logging statistical data of vehicles.
Record per-vehicle metric updates record()
Log summary data log()

2. ChargeStationManager
Manages the charging station resources and synchronizes access using a mutex and condition variable.
Blocking acquire() waits until a station is available
release() frees a station

3. Vehicle Factories
Implements factory classes for different vehicle types.

4. VehicleDeployment  and VehicleRandomDeployment
VehicleDeployment (abstract)
Defines the strategy for how simulation vehicles are created and deployed.
VehicleRandomDeployment
Concrete implementation that produces a randomized list of vehicle objects using various factories.

5. Simulation
Central controller for the multi-threaded EV simulation.

Three internal worker threads:
runnerThread (runs vehicles)
needChargeThread (dispatches depleted vehicles to charger)
chargerThread (charges vehicles)

Three thread-safe queues:
runQueue
needChargeQueue
chargeQueue

Thread Functions:
runnerThreadFunc()
Runs vehicles for one time slice and decides if they need charging.
needChargeDispatcherFunc()
Moves depleted vehicles to charging stations.
chargerThreadFunc()
Simulates charging and returns vehicles to the run queue.

6. ThreadSafeQueue
lock-based FIFO queue with safe multi-thread access.

7. Vehicle
Represents a single electric vehicle in simulation.
Vehicle parameter: speed, battery capacity, charge time, passengers, energy consumption, fault rate.
Runtime status: running time, charging time, battery ratio.
Simulate running (run())
Simulate charging (charge())
Determine when to charge (needsCharge())
Determine when charging is complete (isFullyCharged())

8. VehicleStatsData
Stores statistical metrics for one vehicle type and computes averages:
Total & average running time
Total & average distance
Charge cycles & charge time
Fault accumulations
Passenger miles

9. VehicleStatsManager (Singleton)
Global singleton managing all vehicle statistics.
Allow vehicles to record events (record())
Provide debug/log output (printAll())

