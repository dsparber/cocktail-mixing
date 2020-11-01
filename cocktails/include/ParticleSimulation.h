#include "Simulation.h"
#include "Particle.h"
#include "UniformGridNeighborSearch.h"

class ParticleSimulation : public Simulation {
public:
    explicit ParticleSimulation(double gridWidth);

    virtual void init() override;
	virtual void resetMembers() override;
	virtual bool advance() override;

	void updateRenderGeometry() override;
    void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

	void postAdvance();

protected:
    std::vector<Particle> particles;
    NeighborSearch* neighborSearch;

private:
	Eigen::MatrixXd V;  // Vertex positions
	Eigen::MatrixXi F;  // Faces (indices of vertices)
	Eigen::MatrixXd C;  // Colors per face
};