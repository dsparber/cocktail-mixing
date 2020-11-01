#include "Simulation.h"
#include "Particle.h"
#include "UniformGridNeighborSearch.h"

class ParticleSimulation : public Simulation {
public:
	ParticleSimulation();

	void init() override;
	void resetMembers() override;
	void updateRenderGeometry() override;
	bool advance() override;
	void renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) override;

	void postAdvance();

protected:
    std::vector<Particle> particles;
    UniformGridNeighborSearch neighborSearch;

private:
	Eigen::MatrixXd m_V;  // vertex positions
	Eigen::MatrixXi m_F;  // face indices
	Eigen::MatrixXd m_C;  // colors per face

	Eigen::MatrixXd m_renderV;  // vertex positions for rendering
	Eigen::MatrixXi m_renderF;  // face indices for rendering
	Eigen::MatrixXd m_renderC;  // colors per face for rendering

};