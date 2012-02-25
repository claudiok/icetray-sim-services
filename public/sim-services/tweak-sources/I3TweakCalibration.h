/**
 * Definition of I3TweakCalibration class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3TweakCalibration.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3TweakCalibration.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3TWEAKCALIBRATION_H
#define I3TWEAKCALIBRATION_H

#include "interfaces/I3CalibrationService.h"
#include <icetray/I3Logging.h>
#include "interfaces/I3GeometryService.h"

#include <string>

/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3TweakCalibration : public I3CalibrationService
{
public:

  I3TweakCalibration(I3CalibrationServicePtr c, I3GeometryServicePtr g);

  virtual I3CalibrationConstPtr GetCalibration(I3Time time);
  virtual ~I3TweakCalibration() { };

  void SetTemperature(double x){ temperature_ = x; };
  void SetFADCBaselineSlope(double x){fadcBaselineFit_slope_=x; };
  void SetFADCBaselineIntercept(double x){fadcBaselineFit_intercept_=x;};
  void SetFADCGain(double x){fadcGain_=x;};
  void SetATWD0Gain(double x){atwd0Gain_=x;};
  void SetATWD1Gain(double x){atwd1Gain_=x;};
  void SetATWD2Gain(double x){atwd2Gain_=x;};
  void SetATWDaFreqFitA(double x){atwd_a_FreqFit_A_=x;};
  void SetATWDaFreqFitB(double x){atwd_a_FreqFit_B_=x;};
  void SetATWDaFreqFitC(double x){atwd_a_FreqFit_C_=x;};
  void SetATWDbFreqFitA(double x){atwd_b_FreqFit_A_=x;};
  void SetATWDbFreqFitB(double x){atwd_b_FreqFit_B_=x;};
  void SetATWDbFreqFitC(double x){atwd_b_FreqFit_C_=x;};
  void SetHVGainSlope(double x){hvGainFit_slope_=x;};
  void SetHVGainIntercept(double x){hvGainFit_intercept_=x;};
  void SetATWDBinCalibSlope(double x){atwdBinCalibFit_slope_=x;};
  void SetATWDBinCalibIntercept(double x){atwdBinCalibFit_intercept_=x;};
  void SetATWDResponseWidth(double x){atwd_response_width_=x;};
  void SetFADCResponseWidth(double x){fadc_response_width_=x;};

  void SetFADCDeltaT(double x){fadcDeltaT_=x;};
  void SetFrontEndImpedance(double x){frontendImpedance_=x;};
  void SetPMTTransitTimeSlope(double x){pmtTransitTimeSlope_=x;};
  void SetPMTTransitTimeIntercept(double x){pmtTransitTimeIntercept_=x;};
  void SetDOMCalVersion(std::string x){domcalVersion_=x;};
  void SetATWDa0Baseline(double x){atwda0_baseline_=x;};
  void SetATWDa1Baseline(double x){atwda1_baseline_=x;};
  void SetATWDa2Baseline(double x){atwda2_baseline_=x;};
  void SetATWDb0Baseline(double x){atwdb0_baseline_=x;};
  void SetATWDb1Baseline(double x){atwdb1_baseline_=x;};
  void SetATWDb2Baseline(double x){atwdb2_baseline_=x;};

  void SetTauParamP0(double x){tauparam_P0_=x;};
  void SetTauParamP1(double x){tauparam_P1_=x;};
  void SetTauParamP2(double x){tauparam_P2_=x;};
  void SetTauParamP3(double x){tauparam_P3_=x;};
  void SetTauParamP4(double x){tauparam_P4_=x;};
  void SetTauParamP5(double x){tauparam_P5_=x;};
  void SetTauParamTauFrac(double x){tauparam_TauFrac_=x;};

  void SetATWDADeltaT(double x){atwda_deltat_ = x;};
  void SetATWDBDeltaT(double x){atwdb_deltat_ = x;};
  void SetSPEDiscThreshIntercept(double x){spe_disc_thresh_int_ = x;};
  void SetSPEDiscThreshSlope(double x){spe_disc_thresh_slope_ = x;};
  void SetMPEDiscThreshIntercept(double x){mpe_disc_thresh_int_ = x;};
  void SetMPEDiscThreshSlope(double x){mpe_disc_thresh_slope_ = x;};
  void SetPMTDiscThreshIntercept(double x){pmt_disc_thresh_int_ = x;};
  void SetPMTDiscThreshSlope(double x){pmt_disc_thresh_slope_ = x;};

  SET_LOGGER("I3TweakCalibration");

 private:
  I3TweakCalibration();
  I3CalibrationServicePtr cal_service_;

  I3GeometryServicePtr geometry_service_;

  double temperature_;

  double fadcBaselineFit_slope_;
  double fadcBaselineFit_intercept_;

  double fadcGain_;
  double atwd0Gain_;
  double atwd1Gain_;
  double atwd2Gain_;

  double atwd_a_FreqFit_A_;
  double atwd_a_FreqFit_B_;
  double atwd_a_FreqFit_C_;

  double atwd_b_FreqFit_A_;
  double atwd_b_FreqFit_B_;
  double atwd_b_FreqFit_C_;

  double hvGainFit_slope_;
  double hvGainFit_intercept_;

  double atwdBinCalibFit_slope_;
  double atwdBinCalibFit_intercept_;

  double atwd_response_width_;
  double fadc_response_width_;

  double tauparam_P0_;
  double tauparam_P1_;
  double tauparam_P2_;
  double tauparam_P3_;
  double tauparam_P4_;
  double tauparam_P5_;
  double tauparam_TauFrac_;

  double fadcDeltaT_;
  double frontendImpedance_;
  double pmtTransitTimeSlope_;
  double pmtTransitTimeIntercept_;
  std::string domcalVersion_;
  double atwda0_baseline_;
  double atwda1_baseline_;
  double atwda2_baseline_;
  double atwdb0_baseline_;
  double atwdb1_baseline_;
  double atwdb2_baseline_;

  double atwda_deltat_;
  double atwdb_deltat_;

  double spe_disc_thresh_slope_;
  double spe_disc_thresh_int_;
  double mpe_disc_thresh_slope_;
  double mpe_disc_thresh_int_;
  double pmt_disc_thresh_slope_;
  double pmt_disc_thresh_int_;

};

I3_POINTER_TYPEDEFS(I3TweakCalibration);

#endif
