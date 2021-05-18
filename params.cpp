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

    float particles_per_unit_length = 0.2;
    double link_stiffness = 1.f;
    double damping_strength = 1.f;
    double radius_influence = 1.f;

    void setLinearDamping(double ld) {
        params::linear_damping = ld;
    }

    void setLinkStiffness(double ls) {
        params::link_stiffness = ls;
    }
    void setDampingStrength(double ds) {
        params::damping_strength = ds;
    }
    void setRadiusInfluence(double ri) {
        params::radius_influence = ri;
    }

    void setParticlePerUnitLength(float ppul) {
        params::particles_per_unit_length = ppul;
    }


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
