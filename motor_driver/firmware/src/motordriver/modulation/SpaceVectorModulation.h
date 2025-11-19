#pragma once

#include "motordriver/util/util.h"
#include "generated/ModulationDataParams.h"
#include "generated/ModulationData_sinus.h"
#include "generated/ModulationData_third_harmonic_injection.h"


enum class MODULATION_TYPE {
    SINUS,
    THIRD_HARMONIC_INJECTION
};


struct Vec3 {
    int a;
    int b;
    int c;
};


constexpr int ROTATION_120_DEGREE = MODULATION_DATA_FULLROTATION_VALUE / 3;


/**
 * Get modulation values for the three motor phases.
 * The returned values correspond to the maximum throttle (MODULATION_DATA_MAX_PWM_VALUE)
 * @param angle The angle as integer (360° ^= MODULATION_DATA_FULLROTATION_VALUE).
 *              Will be normalized, thus can also be negative or lager than MODULATION_DATA_FULLROTATION_VALUE.
 * @param modulationType the type of modulation
 * @return The three phase values (between -MODULATION_DATA_MAX_PWM_VALUE and +MODULATION_DATA_MAX_PWM_VALUE)
 */
template<MODULATION_TYPE modulationType>
Vec3 getModulationValues(int angle) {
  int angleNorm = normalizeAngle(angle, MODULATION_DATA_FULLROTATION_VALUE);
  int angleNormPlus120 = normalizeAngle(angle + ROTATION_120_DEGREE, MODULATION_DATA_FULLROTATION_VALUE);
  int angleNormPlus240 = normalizeAngle(angle + ROTATION_120_DEGREE*2, MODULATION_DATA_FULLROTATION_VALUE);

  //printf("angleNorm %d \n", angleNorm);

  // phase value lookup
  if constexpr(modulationType == MODULATION_TYPE::SINUS) {
    return Vec3{
        .a = MODULATION_DATA_SINUS[angleNorm],
        .b = MODULATION_DATA_SINUS[angleNormPlus120],
        .c = MODULATION_DATA_SINUS[angleNormPlus240]
    };
  }
  else {
    //return Vec3{};
    return Vec3{
        .a = MODULATION_DATA_THIRD_HARMONIC_INJECTION[angleNorm],
        .b = MODULATION_DATA_THIRD_HARMONIC_INJECTION[angleNormPlus120],
        .c = MODULATION_DATA_THIRD_HARMONIC_INJECTION[angleNormPlus240]
    };
  }
}