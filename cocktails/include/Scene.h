#ifndef PBS_SCENE_H
#define PBS_SCENE_H

#include <Eigen/Dense>
#include <igl/opengl/glfw/Viewer.h>
#include "Particle.h"

class Scene {
public:
	Scene() {};

	virtual ~Scene() {}

	virtual void draw(igl::opengl::glfw::Viewer& viewer) = 0;

	virtual bool outOfBoundary(const Eigen::Vector3d& pos) = 0;

	void updateOnBoundaryCollision(Particle& particle, const double dt) {
		Eigen::Vector3d intersection, normal;
		getIntersectionPointAndSurfaceNormal(particle.m_pos, dt * particle.m_vel, intersection, normal);
		particle.m_vel = 2 * normal.dot(-particle.m_vel) * normal + particle.m_vel; // reflect on normal
		double remainingDt = dt - (intersection - particle.m_pos).norm();
		particle.m_pos = intersection;

		if (remainingDt > 0) {
			Eigen::Vector3d nextPosition = particle.m_pos + remainingDt * particle.m_vel;
			if (outOfBoundary(nextPosition)) { // check for another boundary collision and recurse while dt > 0
				updateOnBoundaryCollision(particle, remainingDt);
			}
			else {
				particle.m_pos = nextPosition;
			}
		}
	}

protected:
	virtual void getIntersectionPointAndSurfaceNormal(
		const Eigen::Vector3d& pos,
		const Eigen::Vector3d& dir,
		Eigen::Vector3d& intersection,
		Eigen::Vector3d& normal
	) = 0;
};

#endif