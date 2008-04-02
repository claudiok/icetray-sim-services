/**
 * Definition of I3MCCalibrationService class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3MCCalibrationService.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3MCCalibrationService.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author pretz
 * @author tmccauley
 */

#ifndef I3MCCALIBRATIONSERVICE_H
#define I3MCCALIBRATIONSERVICE_H

#include "interfaces/I3CalibrationService.h"
#include <vector>
#include <string>
#include <icetray/I3Logging.h>

class I3GeometryService;
I3_POINTER_TYPEDEFS(I3GeometryService);
/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3MCCalibrationService : public I3CalibrationService
{
public:

  I3MCCalibrationService(I3GeometryServicePtr g,
			 I3CalibrationServicePtr c);

  virtual I3CalibrationConstPtr GetCalibration(I3Time time);
  virtual ~I3MCCalibrationService() { };

  void SetSkipStrings(std::vector<int>& v){ skipStrings_ = v;};
  void SetSkipStations(std::vector<int>& v){ skipStations_ = v;};
  void SetTWRChargeFile(std::string s){ twrChargeFile_ = s;};

  SET_LOGGER("I3MCCalibrationService");

 private:
  I3MCCalibrationService();
  I3GeometryServicePtr geo_service_;
  I3CalibrationServicePtr cal_service_;

  /**
   *Start of the valid time range of the detector status
   */
  int32_t startYear_;
  int64_t startDAQTime_;

  /**
   *End of the valid time range of the detector status
   */
  int32_t endYear_;
  int64_t endDAQTime_;

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
  std::string twrChargeFile_;

  /**
   * Don't modify these strings/stations
   */
  std::vector<int> skipStrings_;
  std::vector<int> skipStations_;

};
I3_POINTER_TYPEDEFS(I3MCCalibrationService);
#endif
