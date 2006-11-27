/**
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3CalibrationDefaults.h 6917 2005-04-26 02:56:37Z olivas $
 * 
 * @file I3CalibrationDefaults.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3CALIBRATIONDEFAULTS_H
#define I3CALIBRATIONDEFAULTS_H

#include "dataclasses/I3Units.h"

namespace I3CalibDefaults{
  /**
   *Default vaules for DOM temperature
   */

  const double TEMPERATURE = 250.*I3Units::kelvin;

  /**
   *Start of the valid time range of the calibration
   */
  const int32_t START_YEAR = 2000;
  const int64_t START_DAQTIME = 0;

  /**
   *End of the valid time range of the calibration
   */
  const int32_t END_YEAR = 3000;
  const int64_t END_DAQTIME = 0;
  /**
   *Slope and intercept if FADC Baseline Fit
   */
  const double FADC_BASELINE_FIT_SLOPE = 1.248; //NO CHANGE
  const double FADC_BASSLINE_FIT_INTERCEPT = -864.32; //NO CHANGE
  /**
   *FADC Gain
   */
  const double FADC_GAIN = 8.961e-5*I3Units::V; // OLD: 9.733e-5*I3Units::V
  /**
   *Gain for ATWD Channels 0,1,and 2
   */
  const double ATWD0_GAIN = -15.9; //OLD: -16.0
  const double ATWD1_GAIN = -1.7;  //OLD: -2.0
  const double ATWD2_GAIN = -0.17; //OLD: -0.25
  /**
   *Quadratic fit values for ATWD a and b
   */
  const double ATWD_A_FREQFIT_A = 26.04;  //OLD: 2.5858788
  const double ATWD_A_FREQFIT_B = 0.3202; //OLD: 0.013337472
  const double ATWD_A_FREQFIT_C = -2.842e-5;    //From DOMCal files
  const double ATWD_B_FREQFIT_A = 26.46;  //OLD: 2.3853257
  const double ATWD_B_FREQFIT_B = 0.3202; //OLD: 0.014224272
  const double ATWD_B_FREQFIT_C = -2.853e-5;    //From DOMCal files

  /**
   *High Voltage gain slope and intercept
   */
  const double HV_GAIN_FIT_SLOPE = 7.39; //OLD: 7.0842533
  const double HV_GAIN_FIT_INTERCEPT = -15.99; //OLD: -15.1997

  /**
   *ATWD bin calibration slope and intercept
   */
  const double ATWD_BINCALIB_FIT_SLOPE = -0.002*I3Units::V; //NO CHANGE
  const double ATWD_BINCALIB_FIT_INTERCEPT = 2.88*I3Units::V; //OLD: 2.9*I3Units::V

}

#endif
