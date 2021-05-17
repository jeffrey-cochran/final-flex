#ifndef PARAMS_HPP
#define PARAMS_HPP

namespace params {
    extern double time_step;
    extern double inv_time_step;
    extern double strain_compliance;
    extern double normalized_strain_compliance;
    extern int constraint_iters;

    extern float friction;
    extern float density;

    void setTimeStep(double in_time_step);
    void setStrainCompliance(double in_strain_compliance);
    void updateNormalizedCompliances();
    void setConstraintIters(int in_constraint_iters);

    void setFriction(float pf_friction);
    void setDensity(float pf_Density);
}

#endif
