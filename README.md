# Fluid Simulation Project for PBS

Simulation of mixing fluids for Physically-based Simulation at ETH Zurich

## Group members

|Name | E-Mail | Legi No.  |
|----------------|------------------|------------|
| Viviane Yang   | vyang@ethz.ch    | 16-944-530 |
| Daniel Sparber | dsparber@ethz.ch | 17-948-191 |
| Jela Kovacevic | jelak@ethz.ch    | 15-923-428 |


## Install & Run

```shell
# Build
mkdir build
cd build
cmake ..
make -j8

# Run
cd cocktails
./coktails
```

## Code Structure

We used the dummy project from the lecture as a template.

- Headers: [cocktails/include](cocktails/include)
- Source: [cocktails/src](cocktails/src)
- Main class: [main.cpp](cocktails/src/main.cpp)

The main class handles the initialization of the simulation and starts a simple GUI that visualizes the simulation and can be used to tweak various parameters. 

### Simulations

You can choose between two different types of simulation: 
- [SphSimulation](cocktails/include/SphSimulation.h) - the vanilla SPH simulation 
- [DCSPHSimulation](cocktails/include/DCSPHSimulation.h) - the density contrast SPH simulation

### Fluids

A fluid is described by a [Fluid](cocktails/include/Fluid.h) instance and defines SPH parameters.
Each fluid instance contains a collection of [Particles](cocktails/include/Particle.h).


### Sources

A [Source](cocktails/include/Source.h) generates particles. Sources that create particles at initialization are:

- [BlockSource](cocktails/include/BlockSource.h) - generates a block of particles
- [CustomSource](cocktails/include/CustomSource.h) - generates particles based on a `.xyz` file
- [StateSource](cocktails/include/StateSource.h) -  generates particles from a saved state (i.e. restores particles)

Additionally, there are two sources that continuously generate particles

- [GeneratingSource](cocktails/include/GeneratingSource.h) - Continuously releases new particles
- [EmittingSource](cocktails/include/EmittingSource.h) - Emits blocks of particles at some interval





## Features

### Basic SPH Simulation
Main reference: [Particle-Based Fluid Simulation for Interactive Applications][mueller03]

### Density Contrast SPH
For better interaction of fluids with different densities. 
Main reference: [Density Contrast SPH Interfaces](https://www.zora.uzh.ch/id/eprint/9734/2/Solenthaler_sca08.pdf)

### Boundary Handling
Boundaries are modeled as particles. This allows us to use arbitrary shapes. Given a Mesh, we use Possion-Disk-Sampling 
to generate particles. 

For better boundary handling, we implemented a radial repulsion. Our main reference: 
[SPH particle boundary forces for arbitrary boundaries](https://www.sciencedirect.com/science/article/pii/S0010465509001544?casa_token=YGFEttNytVgAAAAA:CGjWZReNwk5orC4WUZWbjSsogiBcs_-GzpyxiqwOHzSNtcn3yR3lH0zH2PrYYdvR__G3_ercAeQ)

### Surface tension
Main reference: [Particle-Based Fluid Simulation for Interactive Applications][mueller03]


### Performance
- Multithreading: Run calculations for a simulation step on all available CPU cores.
- Optimized kernels: Precompute all constant parameters
- Uniform grid neighbor search
- Skip unnecessary computations for boundary particles


## Additional References

- [Multiple-Fluid SPH Simulation Using a Mixture Model](https://dl.acm.org/doi/pdf/10.1145/2645703)

- [SPH treatment of boundaries and application to moving objects](http://www.unige.ch/math/folks/sutti/SPH_2019.pdf)
  
- [Smoothed Particle Hydrodynamics Techniques for the Physics Based Simulation of Fluids and Solids](https://interactivecomputergraphics.github.io/SPH-Tutorial/pdf/SPH_Tutorial.pdf)


[mueller03]: https://matthias-research.github.io/pages/publications/sca03.pdf