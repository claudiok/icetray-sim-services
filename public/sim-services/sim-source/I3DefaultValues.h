/**
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3DefaultValues.h 6917 2005-04-26 02:56:37Z olivas $
 * 
 * @file I3DefaultValues.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3DEFAULTDETECTORSTATUSVALUES_H
#define I3DEFAULTDETECTORSTATUSVALUES_H

#include "dataclasses/I3Units.h"
#include "dataclasses/status/I3DOMStatus.h"

namespace I3DetStatDefaults{
  /**
   *Start of the valid time range of the detector status
   */
  const int32_t START_YEAR = 2000;
  const int64_t START_DAQTIME = 0;

  /**
   *End of the valid time range of the detector status
   */
  const int32_t END_YEAR = 3000;
  const int64_t END_DAQTIME = 0;

  /**
   * Icetop local coincidence trigger windows
   */
  const double ICETOP_LCWINDOW_PRE = 125.0*I3Units::ns;
  const double ICETOP_LCWINDOW_POST = 125.0*I3Units::ns;

  /**
   * IceTop PMT voltages for high and low gain
   */
  const double ICETOP_HIGHGAIN_VOLTAGE = 1172*I3Units::volt;
  const double ICETOP_LOWGAIN_VOLTAGE = 698*I3Units::volt;

  /**
   * InIce local coincidence trigger windows
   */
  const double INICE_LCWINDOW_PRE = 1000.0*I3Units::ns;
  const double INICE_LCWINDOW_POST = 1000.0*I3Units::ns;

  /**
   * Number of neighbors required to satisfy local coincidence
   */
  const int LCSPAN = 1;

  /**
   * InIce PMT voltage
   */
  const double INICE_VOLTAGE = 1350*I3Units::volt;

  const I3DOMStatus::TrigMode TRIGGER_MODE = I3DOMStatus::SPE;
  const I3DOMStatus::LCMode LCMODE = I3DOMStatus::UpOrDown;
  const I3DOMStatus::OnOff STATUS_ATWDa = I3DOMStatus::On;
  const I3DOMStatus::OnOff STATUS_ATWDb = I3DOMStatus::On;
  const I3DOMStatus::OnOff STATUS_FADC = I3DOMStatus::On;

  const double SPE_THRESHOLD = 1.422*I3Units::mV;
  const double FE_PEDESTAL = 2.6*I3Units::volt;
  const int DAC_TRIGGER_BIAS0 = 850;
  const int DAC_TRIGGER_BIAS1 = 850;
  const int DAC_FADC_REF = 800;

  /**
   *Number of FADC bins
   */
  const int NBINS_ATWD0 = 128;
  const int NBINS_ATWD1 = 32;
  const int NBINS_ATWD2 = 32;
  const int NBINS_FADC = 50;

}

namespace I3CalibDefaults{
  /**
   *Default vaules for Detector Status
   */

  const double TEMPERATURE = 250.*I3Units::kelvin;

  /**
   *Start of the valid time range of the detector status
   */
  const int32_t START_YEAR = 2000;
  const int64_t START_DAQTIME = 0;

  /**
   *End of the valid time range of the detector status
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
  const double FADC_GAIN = 9.733e-5*I3Units::V;
  /**
   *Gain for ATWD Channels 0,1,and 2
   */
  const double ATWD0_GAIN = -16.0;
  const double ATWD1_GAIN = -2.0;
  const double ATWD2_GAIN = -0.25;
  /**
   *Quadratic fit values for ATWD a and b
   */
  const double ATWD_A_FREQFIT_A = 2.5858788;
  const double ATWD_A_FREQFIT_B = 0.013337472;
  const double ATWD_A_FREQFIT_C = NAN;
  const double ATWD_B_FREQFIT_A = 2.3853257;
  const double ATWD_B_FREQFIT_B = 0.014224272;
  const double ATWD_B_FREQFIT_C = NAN;

  /**
   *High Voltage gain slope and intercept
   */
  const double HV_GAIN_FIT_SLOPE = 7.0842533;
  const double HV_GAIN_FIT_INTERCEPT = -15.1997;

  /**
   *ATWD bin calibration slope and intercept
   */
  const double ATWD_BINCALIB_FIT_SLOPE = -0.002*I3Units::V;
  const double ATWD_BINCALIB_FIT_INTERCEPT = 2.9*I3Units::V;

}

#endif
