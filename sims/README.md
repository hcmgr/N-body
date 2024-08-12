`sims/` is where we dump output files (`.sim` files) from our simulation.

`.sim` - Custom Output File Format for N-body Simulations

### Format
[N] // number of particles
[X] // width of simulation area
[Y] // height of simulation area
[MIN_MASS] // minimum particle mass
[MAX_MASS] // maximum particle mass

[timestep 0]
[x1 y1 m1] // first particle
[x2 y2 m2] // second particle
...
[xN yN mN] // Nth particle

[timestep 1]
[x1 y1 m1] // first particle
[x2 y2 m2] // second particle
...
[xN yN mN] // Nth particle

...
EOF