#include "params.hpp"
#include <cmath>
#include <cassert>

namespace params{
    double time_step = 1.;
    double inv_time_step = 1.;
    double strain_compliance = 1.;
    double normalized_strain_compliance = 1.;
    int constraint_iters = 1;
    double friction = 1.;
    double density = 1.;
    double linear_damping = 20.;
    double fracture_strain = 0.5;
    double yield_strain = 0.04;


    void setTimeStep(double in_time_step) {
        assert(in_time_step > 0.);
        params::time_step = in_time_step;
        params::inv_time_step = 1./in_time_step;
        void updateNormalizedCompliances();
    }

    void setStrainCompliance(double in_strain_compliance) {
        params::strain_compliance = in_strain_compliance;
        void updateNormalizedCompliances();
    }

    void updateNormalizedCompliances() {
        params::normalized_strain_compliance = params::strain_compliance * pow(params::inv_time_step, 2.);
    }

    void setConstraintIters(int in_constraint_iters) {
        params::constraint_iters = in_constraint_iters;
    }

    void setFriction(float pf_friction) {
        params::friction = pf_friction;
    }

    void setDensity(float pf_density) {
        params::density = pf_density;
    }
}
