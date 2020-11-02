#ifndef PBS_PARTICLE_SIMULATION_H
#define PBS_PARTICLE_SIMULATION_H

#include "Simulation.h"
#include "Particle.h"
#include "UniformGridNeighborSearch.h"
#include "Scene.h"

class ParticleSimulation : public Simulation {
public:
    explicit ParticleSimulation(double gridWidth);

    virtual void init() override;
	virtual void resetMembers() override;
	virtual bool advance() override;

	void updateRenderGeometry() override;
    void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

    void initParticles();
	void postAdvance();

protected:
    std::vector<Particle> m_particles;
    NeighborSearch* m_neighborSearch;
	Scene* m_scene;

private:
	Eigen::MatrixXd V;  // Vertex positions
	Eigen::MatrixXi F;  // Faces (indices of vertices)
	Eigen::MatrixXd C;  // Colors per face
};

#endif