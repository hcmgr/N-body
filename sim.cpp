#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>

#include "utils.hpp"

#define G           6.67430e-11 // gravitational constant
#define EPSILON     1e-10       // avoid dividing by zero

typedef struct {
    double mass;
    double x, y; // position
    double vx, vy; // velocity
} Particle;

class Model {
public:
    int N; // num particles
    int X, Y; // sim dimensions
    int MIN_MASS, MAX_MASS;
    double dt;
    double timesteps; 

    std::vector<Particle> particles; // N-d particle-state vector

    std::ofstream outputFile;

    Model(int N, int X, int Y, int MIN_MASS, int MAX_MASS, 
        double dt, double timesteps, std::string outputFileName) {

        this->N = N;
        this->X = X;
        this->Y = Y;
        this->MIN_MASS = MIN_MASS;
        this->MAX_MASS = MAX_MASS;
        this->dt = dt;
        this->timesteps = timesteps;

        this->particles = std::vector<Particle>(N);
        init_particles_random(MIN_MASS, MAX_MASS);

        // dump model metadata and initial state to output file
        this->outputFile.open(outputFileName, std::ios::out | std::ios::trunc);
        dump_metadata();
        dump_timestep_data(0);
    }

    /**
    Random initalisation of 'particles'. 
    
    Each particle has:
        - random position within X x Y area
        - random mass within range [0, MAX_MASS]
        - 0 velocity 
    */
    void init_particles_random(int MIN_MASS, int MAX_MASS) {
        for (int i = 0; i < N; i++) {
            Particle p;

            p.mass = Utils::bounded_rand(MIN_MASS, MAX_MASS);
            p.x = Utils::bounded_rand(0, X);
            p.y = Utils::bounded_rand(0, Y);
            p.vx = 0;
            p.vy = 0;

            particles[i] = p;
        }
    }

    /**
    Updates positions of all particles over our timestep (dt)
    */
    void update_positions(int timestepNum) {
        size_t N = particles.size();
        std::vector<std::vector<double>> acc(N, std::vector<double>(2, 0.0));

        // Calculate forces and update acceleration
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = i + 1; j < N; ++j) {
                double dx = particles[j].x - particles[i].x;
                double dy = particles[j].y - particles[i].y;
                double r = std::sqrt(dx * dx + dy * dy) + EPSILON;
                double force = G * particles[i].mass * particles[j].mass / (r * r);

                acc[i][0] += force * (dx / r) / particles[i].mass;
                acc[i][1] += force * (dy / r) / particles[i].mass;

                acc[j][0] -= force * (dx / r) / particles[j].mass;
                acc[j][1] -= force * (dy / r) / particles[j].mass;
            }
        }

        // Update velocities and positions
        for (size_t i = 0; i < N; ++i) {
            particles[i].vx += acc[i][0] * dt;
            particles[i].vy += acc[i][1] * dt;

            particles[i].x += particles[i].vx * dt;
            particles[i].y += particles[i].vy * dt;
        }

        dump_timestep_data(timestepNum);
    }

    void dump_metadata() {
        outputFile << this->N << std::endl;
        outputFile << this->X << std::endl;
        outputFile << this->Y << std::endl;
        outputFile << this->MIN_MASS << std::endl;
        outputFile << this->MAX_MASS << std::endl;
        outputFile << std::endl;
    }

    void dump_timestep_data(int timestepNum) {
        outputFile << timestepNum << std::endl;
        for (auto &p : this->particles) {
            outputFile << p.x << " " << p.y << " " << p.mass << std::endl;
        }
        if (timestepNum != timesteps) {
            outputFile << std::endl;
        }
    }
};

/**
Main model runner
*/
void run_model(Model &model) {
    for (int i = 1; i <= model.timesteps; i++) {
        model.update_positions(i);
    }
}

int main() {
    // seed random
    srand(time(0));

    // particle setup
    int N = 10;
    int X = 5, Y = 5; // m
    int MIN_MASS = 1000, MAX_MASS = 10000;

    // time
    double timesteps = 10;
    double dt = 0.1;

    // dump file
    std::string outputFileName = "sims/code_out.sim";

    Model model = Model(N, X, Y, MIN_MASS, MAX_MASS, dt, timesteps, outputFileName);
    run_model(model);
}