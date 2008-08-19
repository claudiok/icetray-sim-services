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
  const double FADC_BASELINE_FIT_SLOPE = 1.248; 
  const double FADC_BASSLINE_FIT_INTERCEPT = -864.32; 
  /**
   *FADC Gain
   */
  const double FADC_GAIN = 7.685e-5*I3Units::V; //to match season III strings 
  /**
   *Gain for ATWD Channels 0,1,and 2
   */
  const double ATWD0_GAIN = -18.24; //to match season III strings 
  const double ATWD1_GAIN = -2.0; //to match season III strings   
  const double ATWD2_GAIN = -0.17; 
  /**
   *Quadratic fit values for ATWD a and b
   */
  const double ATWD_A_FREQFIT_A = 26.04; 
  const double ATWD_A_FREQFIT_B = 0.3202; 
  const double ATWD_A_FREQFIT_C = -2.842e-5;
  const double ATWD_B_FREQFIT_A = 26.46; 
  const double ATWD_B_FREQFIT_B = 0.3202;
  const double ATWD_B_FREQFIT_C = -2.853e-5;

  /**
   *High Voltage gain slope and intercept
   */
  const double HV_GAIN_FIT_SLOPE = 7.39; 
  const double HV_GAIN_FIT_INTERCEPT = -15.99; 

  /**
   *ATWD bin calibration slope and intercept
   */
  const double ATWD_BINCALIB_FIT_SLOPE = -0.002*I3Units::V; 
  const double ATWD_BINCALIB_FIT_INTERCEPT = 2.88*I3Units::V; 

  const double ATWD_RESPONSE_WIDTH = 0.41; //god-given units
  const double FADC_RESPONSE_WIDTH = 0.49; //god-given units

  const double TAU_P0_POST_2006 = 10960;
  const double TAU_P1_POST_2006 = 56665;
  const double TAU_P2_POST_2006 = 6.5;
  const double TAU_P3_POST_2006 = 500;
  const double TAU_P4_POST_2006 = 0;
  const double TAU_P5_POST_2006 = 1;
  const double TAU_FRACTION_POST_2006 = -0.5;

  //const double FADC_DELTA_T = -77 * I3Units::ns;
  const double FADC_DELTA_T = -100 * I3Units::ns;
  const double FRONT_END_IMPEDANCE = 43 * I3Units::ohm;

  const double PMT_TRANSIT_TIME_SLOPE = 1983; //units are ns/sqrt(V)
  const double PMT_TRANSIT_TIME_INTERCEPT = 85.8 * I3Units::ns;

  const string DOMCAL_VERSION("7.1.0");

  const double ATWDA0_BASELINE = 2.778 * I3Units::mV;
  const double ATWDA1_BASELINE = -0.4321 * I3Units::mV;
  const double ATWDA2_BASELINE = 0.9336 * I3Units::mV;

  const double ATWDB0_BASELINE = 1.821 * I3Units::mV;
  const double ATWDB1_BASELINE = 0.8368 * I3Units::mV;
  const double ATWDB2_BASELINE = 0.8206 * I3Units::mV;

//  const double ATWDA0_BASELINE = 1.944 * I3Units::mV;
//  const double ATWDA1_BASELINE = -0.572 * I3Units::mV;
//  const double ATWDA2_BASELINE = 0.6698 * I3Units::mV;
//
//  const double ATWDB0_BASELINE = -3.505 * I3Units::mV;
//  const double ATWDB1_BASELINE = -16.37 * I3Units::mV;
//  const double ATWDB2_BASELINE = -16.47 * I3Units::mV;

  const double SPE_DISCRIMINATOR_INTERCEPT = -7.209 ;
  const double SPE_DISCRIMINATOR_SLOPE = 0.01339 ;
  const double MPE_DISCRIMINATOR_INTERCEPT = -71.9 ;
  const double MPE_DISCRIMINATOR_SLOPE = 0.1347 ;

  const double ATWDA_DELTAT = 0. * I3Units::ns;
  const double ATWDB_DELTAT = -0.3571 * I3Units::ns;

}

#endif
