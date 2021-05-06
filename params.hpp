#ifndef PARAMS_HPP
#define PARAMS_HPP

#include "Box2D/Box2D.h"

namespace params {
    float time_step = 1.;
    float inv_time_step = 1.;
    float strain_compliance = 0.;
    float normalized_strain_compliance = 0.;
    int constraint_iters = 1;

    void setTimeStep(float in_time_step);
    void setStrainCompliance(float in_strain_compliance);
    void updateNormalizedCompliances();
    void setConstraintIters(int in_constraint_iters);
}

inline void params::setTimeStep(float in_time_step) {
    assert(in_time_step > 0.);
    params::time_step = in_time_step;
    params::inv_time_step = 1./in_time_step;
    void updateNormalizedCompliances();
}

inline void params::setStrainCompliance(float in_strain_compliance) {
    params::strain_compliance = in_strain_compliance;
    void updateNormalizedCompliances();
}

inline void params::updateNormalizedCompliances() {
    params::normalized_strain_compliance = params::strain_compliance * pow(params::inv_time_step, 2.);
}

inline void params::setConstraintIters(int in_constraint_iters) {
    params::constraint_iters = in_constraint_iters;
}

#endif