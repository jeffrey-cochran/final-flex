#ifndef PARAMS_HPP
#define PARAMS_HPP

namespace params {
    extern double time_step;
    extern double inv_time_step;
    extern double strain_compliance;
    extern double normalized_strain_compliance;
    extern int constraint_iters;
    extern double linear_damping;
    extern double fracture_strain;
    extern double yield_strain;

    extern double fixture_friction;
    extern double fixture_density;

    extern double particle_friction;
    extern double particle_density;

    void setTimeStep(double in_time_step);
    void setStrainCompliance(double in_strain_compliance);
    void updateNormalizedCompliances();
    void setFractureStrain(double f_strain);
    void setYieldStrain(double y_strain);
    void setConstraintIters(int in_constraint_iters);

    void setFixtureFriction(float f_friction);
    void setFixtureDensity(float f_Density);

    void setParticleFriction(float p_friction);
    void setParticleDensity(float p_Density);
}

#endif
