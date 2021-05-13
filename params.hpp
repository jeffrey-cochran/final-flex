#ifndef PARAMS_HPP
#define PARAMS_HPP

namespace params {
    extern float time_step;
    extern float inv_time_step;
    extern float strain_compliance;
    extern float normalized_strain_compliance;
    extern int constraint_iters;

    void setTimeStep(float in_time_step);
    void setStrainCompliance(float in_strain_compliance);
    void updateNormalizedCompliances();
    void setConstraintIters(int in_constraint_iters);
}

#endif
