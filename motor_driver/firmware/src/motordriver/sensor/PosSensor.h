#pragma once


#include "motordriver/control/filter/LowPassFilter.h"
#include "motordriver/control/filter/LowPassFilterFloat.h"
#include "stm32_custom/StmUtil.h"
#include "util/FixedPoint.h"


/**
 * Wraps Encoder.
 * Does smoothing over position of encoder and
 * calculates velocity.
 */
class PosSensor {
  private:



    uint32_t timeUsLast;


    long angleAbsoluteLast = 0;



  public:
    //LowPassFilter<long, 50> angleAbsolute;
    Encoder &encoder;

    /// filtered abs angle [in 100*MODULATION_DATA_FULLROTATION_VALUE per rotation]
    LowPassFilter<long, 6> angleAbsolute;
    //LowPassFilter<long, 1000> angleAbsoluteVelocity;

    /// filtered angle velocity [in 10'000'000*MODULATION_DATA_FULLROTATION_VALUE per rotation per 1 microsecond]
    LowPassFilter<long, 8> angleAbsoluteVelocity;

    /// filtered angle velocity
    LowPassFilterFloat<float> angleAbsoluteVelocityFloat;
    float angleAbsoluteVelocityFloatRaw = 0;



    PosSensor( Encoder &encoder)
    : encoder(encoder),
      //angleAbsolute(3000), angleAbsoluteVelocity(3000)
      angleAbsolute(20'000), angleAbsoluteVelocity(40'000),
      angleAbsoluteVelocityFloat(10'000)
    {
      timeUsLast = getCurrentMicros();
    }


    uint32_t timeUsLast_velCalc = 0;


    /**
     * Call this at each iteration of the programs main loop.
     * This updates the encoder values and calcs velocity.
     */
    void update() {

      // get delta time
      uint32_t timeUs = getCurrentMicros();
      uint32_t timeDeltaUs = timeUs - timeUsLast;
      timeUsLast = timeUs;

      //printf("timeDeltaUs: %lu\n", timeDeltaUs);

      /// update angle and apply filters
      EncoderAngle encoderAngle = encoder.update();
      // increase resolution by factor 100
      angleAbsolute.update(encoderAngle.angle*100, timeDeltaUs);

      /*
      printf("%f , %ld\n",
             (float)currentAngle.angle,
             (angleAbsolute.value)///100.0f
             );
      return;
       */

      if (timeUsLast_velCalc >= timeUs - 1'000) {
        return;
      }
      uint32_t timeDeltaUs_velCalc = timeUs - timeUsLast_velCalc;

      /// calculate angular velocity
      // do not use filtered absAngle for now
      /// code for fixed point calculation version
      /*
      FixedPoint<long, 10'000*100*MODULATION_DATA_FULLROTATION_VALUE> angle_velocity{};
      angle_velocity.value = ((int)(encoderAngle.angle - (int)angleAbsoluteLast)*10'000'000) / ((int)timeDeltaUs_velCalc); // timeDeltaUs_velCalc
      angleAbsoluteVelocity.update(angle_velocity.value, timeDeltaUs_velCalc);

      float velRaw =  ((float)angle_velocity.value / (1000.0f*100.0f));
      float velFiltered = ((float)angleAbsoluteVelocity.value / (1000.0f*100.0f));
       */

      float angle_velocity_raw = (((float)encoderAngle.angle - (float)angleAbsoluteLast) / (float)timeDeltaUs_velCalc) * 1'000'000 /* us to sec */;
      angleAbsoluteVelocityFloatRaw = angle_velocity_raw;
      angleAbsoluteVelocityFloat.update(angle_velocity_raw, timeDeltaUs_velCalc);
      //printf("angleAbsolute: %ld\n", currentAngle.angle);
      //printf("angleAbsoluteLast: %ld\n\n", angleAbsoluteLast);
      ///*
      /*
      printf("%f, %f , %ld\n",
             velRaw,
             velFiltered,
             //angleAbsolute.value
             encoderAngle.angle);
             //*/
      //printf("%ld, %ld \n", angleAbsolute.value, angleAbsoluteLast);
      //printf("%ld, %ld \n", ((long)encoderAngle.angle- (long)angleAbsoluteLast), timeDeltaUs_velCalc);

      angleAbsoluteLast = encoderAngle.angle;
      timeUsLast_velCalc = getCurrentMicros();
    }


    /**
     * Get normalized to MODULATION_DATA_FULLROTATION_VALUE per rotation per 1 second
     * @return
     */
    float getVelocityFilteredFloat() const {
      //return angleAbsoluteVelocity.value / (10'000'000.0f/100'000.0f);
      //return angleAbsoluteVelocity.value /  (1000.0f*100.0f);
      return angleAbsoluteVelocityFloat.value;
    }
};