#pragma once


template<class TYPE>
class LowPassFilterFloat {
  public:
    /// filter time constant in microseconds (larger = more smoothing)
    float Tf = 0;

    /// the current filtered value
    TYPE value;


    /// @param tf filter time constant in microseconds (larger = more smoothing)
    LowPassFilterFloat(float tf): Tf(tf) {
    }

    void update(TYPE valueNew, uint32_t dTimeMs) {
      // resolution corresponds to the 1.0 value
      TYPE alpha = Tf / (Tf + dTimeMs);
      if (Tf == 0) {
        value = valueNew;
        return;
      }
      /*
      printf("%ld , %ld, \n",
             (alpha*value + (resolution - alpha)*valueNew),
             (alpha*value + (resolution - alpha)*valueNew) / resolution);
             */
      value = (alpha*value + (1-alpha)*valueNew);
    }
};