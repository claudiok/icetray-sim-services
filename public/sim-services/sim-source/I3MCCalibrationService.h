/**
 * Definition of I3MCRawDOMStatusFiller class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3MCRawDOMStatusFiller.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3MCRawDOMStatusFiller.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author pretz
 * @author tmccauley
 */

#ifndef I3MCCALIBRATIONSERVICE_H
#define I3MCCALIBRATIONSERVICE_H

#include "phys-services/I3CalibrationService.h"
#include "phys-services/I3GeometryService.h"

class I3Geometry;
I3_POINTER_TYPEDEFS(I3Geometry);
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

  I3MCCalibrationService(I3GeometryServicePtr g);

  virtual I3CalibrationConstPtr GetCalibration(I3Time time);
  virtual ~I3MCCalibrationService() { };

  void SetStartYear(int32_t t){startYear_= t;};
  void SetStartDAQTime(int64_t t){startDAQTime_= t;};
  void SetEndYear(int32_t t){endYear_= t;};
  void SetEndDAQTime(int64_t t){endDAQTime_= t;};

  void SetFADCBaselineSlope(double x){fadcBaselineFit_slope_=x; };
  void SetFADCBaselineIntercept(double x){fadcBaselineFit_intercept_=x;};
  void SetFADCGain(double x){fadcGain_=x;};
  void SetATWD0(double x){atwd0Gain_=x;};
  void SetATWD1(double x){atwd1Gain_=x;};
  void SetATWD2(double x){atwd2Gain_=x;};
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

  SET_LOGGER("I3MCCalibrationService");

 private:
  I3MCCalibrationService();
  I3GeometryServicePtr geo_service_;

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

};

#endif
