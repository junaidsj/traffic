#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// Vehicle class
class Vehicle {
public:
    int id; // Unique ID for each vehicle
    int lane; // Lane number
    int position; // Current position of the vehicle in the lane
    int speed; // Speed of the vehicle

    Vehicle(int id, int lane, int position, int speed)
        : id(id), lane(lane), position(position), speed(speed) {
    }
};

// Traffic Simulation Class
class TrafficSimulator {
private:
    std::vector<Vehicle> vehicles;
    const int roadLength = 100; // Length of the road
    const int numLanes = 3; // Number of lanes
    int totalVehicles = 0;

public:
    void addVehicle(int lane, int speed) {
        int position = 0; // Vehicles start at the beginning of the road
        Vehicle v(totalVehicles++, lane, position, speed);
        vehicles.push_back(v);
    }

    void updatePositions() {
        for (auto& vehicle : vehicles) {
            vehicle.position += vehicle.speed;
            // Remove vehicles that have exited the road
            if (vehicle.position > roadLength) {
                vehicle.position = -1; // Mark vehicle as exited
            }
        }
        vehicles.erase(std::remove_if(vehicles.begin(), vehicles.end(),
            [](const Vehicle& v) { return v.position == -1; }),
            vehicles.end());
    }

    void displayTraffic() {
        std::cout << "Traffic State:\n";
        for (int lane = 0; lane < numLanes; ++lane) {
            std::cout << "Lane " << lane + 1 << ": ";
            for (const auto& vehicle : vehicles) {
                if (vehicle.lane == lane) {
                    std::cout << "[ID: " << vehicle.id << " Pos: " << vehicle.position << "] ";
                }
            }
            std::cout << "\n";
        }
    }

    int countVehicles() {
        return vehicles.size();
    }

    void simulate(int iterations, int vehicleSpawnRate) {
        for (int i = 0; i < iterations; ++i) {
            // Randomly add vehicles at the spawn rate
            if (rand() % 100 < vehicleSpawnRate) {
                int lane = rand() % numLanes;
                int speed = rand() % 5 + 1; // Speed between 1 and 5 units
                addVehicle(lane, speed);
            }

            // Update vehicle positions
            updatePositions();

            // Display the traffic state
            std::cout << "\n--- Iteration " << i + 1 << " ---\n";
            displayTraffic();

            // Show total vehicle count
            std::cout << "Total Vehicles on Road: " << countVehicles() << "\n";

            // Wait for a short time to simulate real-time processing
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    TrafficSimulator simulator;
    int iterations = 20; // Number of simulation steps
    int vehicleSpawnRate = 30; // Percentage chance to spawn a vehicle each step

    std::cout << "Starting Traffic Flow Simulation...\n";
    simulator.simulate(iterations, vehicleSpawnRate);

    std::cout << "\nSimulation Complete!\n";
    return 0;
}
