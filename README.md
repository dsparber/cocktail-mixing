# Fluid Simulation Project for PBS

Simulation of mixing fluids for Physically-based Simulation at ETH Zurich

## Group members

|Name | E-Mail | Legi No.  |
|----------------|------------------|------------|
| Viviane Yang   | vyang@ethz.ch    | 16-944-530 |
| Daniel Sparber | dsparber@ethz.ch | 17-948-191 |
| Jela Kovacevic | jelak@ethz.ch    | 15-923-428 |


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