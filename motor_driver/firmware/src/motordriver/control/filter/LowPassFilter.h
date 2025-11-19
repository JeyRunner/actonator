#pragma once


template<class TYPE, int RESOLUTION_BITS>
class LowPassFilter {
  public:
    /// filter time constant in microseconds (larger = more smoothing)
    unsigned int Tf = 0;

    /// the current filtered value
    TYPE value;


    /// @param tf filter time constant in microseconds (larger = more smoothing)
    LowPassFilter(unsigned int tf): Tf(tf) {
    }

    void update(TYPE valueNew, uint32_t dTimeMs) {
      // resolution corresponds to the 1.0 value
      constexpr int resolution = RESOLUTION_BITS; // to work with fixed point
      TYPE alpha = (Tf*resolution) / (Tf + dTimeMs);
      if (Tf == 0) {
        value = valueNew;
        return;
      }
      /*
      printf("%ld , %ld, \n",
             (alpha*value + (resolution - alpha)*valueNew),
             (alpha*value + (resolution - alpha)*valueNew) / resolution);
             */
      //value = (alpha*value + (resolution - alpha)*valueNew) / resolution;

      int Q = RESOLUTION_BITS;
      TYPE alphaScale = 1 << Q;
      TYPE alpha_ = (dTimeMs*alphaScale)/Tf;
      value += (alpha_* (valueNew - value) ) >> Q;
    }
};