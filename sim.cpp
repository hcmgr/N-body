#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>

#define G           6.67430e-11 // gravitational constant
#define EPSILON     1e-10       // avoid dividing by zero

/**
Represents 2-D state of a particle

NOTE: can easily add 3-D later
*/
typedef struct {
    double mass;
    double x, y; // position
    double vx, vy; // velocity
} Particle;

/**
Generates N particles at:
    - random positions in an X x Y area
    - mass MASS
    - 0 velocity 
*/
std::vector<Particle> init_config_random(int N, int X, int Y, int MASS) {
    std::vector<Particle> init_config(N);
    for (int i = 0; i < N; i++) {
        Particle p;

        p.mass = MASS;
        p.x = rand() % X, p.y = rand() % Y, 
        p.x = 0, p.y = 0;
        p.vx = 0, p.vy = 0;

        init_config[i] = p;
    }
    return init_config;
}

/**
Updates positions of all particles over given timestep 'dt'
*/
void update_positions(std::vector<Particle> &particles, double dt) {
    size_t N = particles.size();

    std::vector<std::vector<double>> acc(N, std::vector<double>(2, 0.0));
    
    // Calculate forces and update acceleration
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double r_mag = std::sqrt(dx * dx + dy * dy) + EPSILON;
            double force = G * particles[i].mass * particles[j].mass / (r_mag * r_mag);

            acc[i][0] += force * (dx / r_mag) / particles[i].mass;
            acc[i][1] += force * (dy / r_mag) / particles[i].mass;

            acc[j][0] -= force * (dx / r_mag) / particles[j].mass;
            acc[j][1] -= force * (dy / r_mag) / particles[j].mass;
        }
    }

    // Update velocities and positions
    for (size_t i = 0; i < N; ++i) {
        particles[i].vx += acc[i][0] * dt;
        particles[i].vy += acc[i][1] * dt;

        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
    }
}

/**
Main model runner
*/
void run_model(std::vector<Particle> state, int timesteps, int dt) {
    for (int i = 0; i < timesteps; i++) {
        update_positions(state, dt);
    }
}

int main() {
    // seed random
    srand(time(0));

    // time
    double timesteps = 50;
    double dt = 0.1;
    double time = timesteps / dt;

    // particle setup
    int N = 50;
    int MASS = 1; // kg
    int X = 100, Y = 100; // m
    std::vector<Particle> init_state = init_config_random(N, X, Y, MASS);

    run_model(init_state, timesteps, dt);
}