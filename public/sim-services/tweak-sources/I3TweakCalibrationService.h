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

#ifndef I3TWEAKCALIBRATIONSERVICE_H
#define I3TWEAKCALIBRATIONSERVICE_H

#include "sim-services/tweak-sources/I3TweakCalibration.h"
#include "icetray/I3ServiceFactory.h"
#include <icetray/I3DefaultName.h>
/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3TweakCalibrationService : public I3ServiceFactory
{
public:

  I3TweakCalibrationService(const I3Context& context);

  virtual ~I3TweakCalibrationService(){};

  virtual bool InstallService(I3Context& services);

  virtual void Configure();

  SET_LOGGER("I3TweakCalibrationService");

 private:
  I3TweakCalibrationService();
  I3TweakCalibrationService
    (const I3TweakCalibrationService& rhs); // stop default
  I3TweakCalibrationService operator=
    (const I3TweakCalibrationService& rhs); // stop default

  I3TweakCalibrationPtr cal_service_;

  std::string oldServiceName_;
  std::string tweakedServiceName_;

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

};
I3_POINTER_TYPEDEFS(I3TweakCalibrationService);
I3_DEFAULT_NAME(I3TweakCalibrationService);

#endif
