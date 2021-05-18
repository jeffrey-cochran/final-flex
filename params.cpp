#include "params.hpp"
#include <cmath>
#include <cassert>

namespace params{
    double time_step = 1.;
    double inv_time_step = 1.;
    double strain_compliance = 1.;
    double normalized_strain_compliance = 1.;
    int constraint_iters = 2;

    double linear_damping = 10.;
    double fracture_strain = 0.5;
    double yield_strain = 0.04;

    double fixture_friction = 1.f;
    double fixture_density = 100.f;

    double particle_friction = 0.1f;
    double particle_density = 1.f;


    void setTimeStep(double in_time_step) {
        assert(in_time_step > 0.);
        params::time_step = in_time_step;
        params::inv_time_step = 1./in_time_step;
        updateNormalizedCompliances();
    }

    void setStrainCompliance(double in_strain_compliance) {
        params::strain_compliance = in_strain_compliance;
        updateNormalizedCompliances();
    }

    void updateNormalizedCompliances() {
        params::normalized_strain_compliance = params::strain_compliance * pow(params::inv_time_step, 2.);
    }

    void setFractureStrain(double f_strain) {
        params::fracture_strain = f_strain;
    }

    void setYieldStrain(double y_strain) {
        params::yield_strain = y_strain;
    };

    void setConstraintIters(int in_constraint_iters) {
        params::constraint_iters = in_constraint_iters;
    }

    void setFixtureFriction(float f_friction) {
        params::fixture_friction = f_friction;
    }

    void setFixtureDensity(float f_density) {
        params::fixture_density = f_density;
    }

    void setParticleFriction(float p_friction) {
        params::particle_friction = p_friction;
    }

    void setParticleDensity(float p_density) {
        params::particle_density = p_density;
    }
}
