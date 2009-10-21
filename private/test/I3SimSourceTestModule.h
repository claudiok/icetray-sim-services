#ifndef I3SIMSOURCETESTMODULE_H
#define I3SIMSOURCETESTMODULE_H
/**
 * class: I3SimSourceTestModule
 *
 * Version $Id: I3SimSourceTestModule.h,v 1.11 2005/03/03 00:43:41 olivas Exp $
 *
 * date:  2 July 2004
 *
 * (c) 2004 IceCube Collaboration
 */

#include "icetray/I3Module.h"
#include "dataclasses/I3Position.h"

/** 
 *@class I3SimSourceTestModule I3SimSourceTestModule.h 
 */

class I3SimSourceTestModule : public I3Module
{
 public:

  I3SimSourceTestModule(const I3Context& ctx);
  ~I3SimSourceTestModule();
  
  void Configure();
  void Physics(I3FramePtr frame);

 private:
  I3SimSourceTestModule();
  I3SimSourceTestModule(const I3SimSourceTestModule&);
  I3SimSourceTestModule& operator=(const I3SimSourceTestModule&);

  SET_LOGGER("I3SimSourceTestModule");

  string stringsToUse_;
  string stationsToUse_;
  string stringsToExclude_;
  string stationsToExclude_;

  /**
   *Detector Status parameters
   */

  /**
   *InIce trigger parameters
   */
  int ic_configID_;
  int ic_threshold_;
  double ic_timeWindow_;

  /**
   *IceTop trigger parameters
   */
  int it_configID_;
  int it_threshold_;
  double it_timeWindow_;

  /**
   *Start of the valid time range of the detector status
   */
  int32_t ds_startYear_;
  int64_t ds_startDAQTime_;

  /**
   *End of the valid time range of the detector status
   */
  int32_t ds_endYear_;
  int64_t ds_endDAQTime_;

  /**
   * Icetop local coincidence trigger windows
   */
  double ds_icetopLCWindowPre_;
  double ds_icetopLCWindowPost_;

  /**
   * IceTop PMT voltages for high and low gain
   */
  double ds_icetopHighGainVoltage_;
  double ds_icetopLowGainVoltage_;

  /**
   * InIce local coincidence trigger windows
   */
  double ds_iniceLCWindowPre_;
  double ds_iniceLCWindowPost_;

  /**
   * Number of neighbors required to satisfy local coincidence
   */
  int ds_iniceLCSpan_;
  int ds_icetopLCSpan_;

  /**
   * InIce PMT voltage
   */
  double ds_iniceVoltage_;

  int ds_iniceTriggerMode_;
  int ds_icetopHGTriggerMode_;
  int ds_icetopLGTriggerMode_;
  int ds_lcMode_inice_first_;
  int ds_lcMode_inice_bulk_;
  int ds_lcMode_inice_last_;
  int ds_lcMode_icetop_;
  int ds_statusATWDa_;
  int ds_statusATWDb_;

  int ds_statusFADC_InIce_;
  int ds_statusFADC_IceTop_;

  double ds_speThreshold_;
  double ds_fePedestal_;
  int ds_dacTriggerBias0_;
  int ds_dacTriggerBias1_;
  int ds_dacFADCRef_;

  /**
   *Number of FADC bins
   */
  unsigned int ds_nBinsATWD0_InIce_;
  unsigned int ds_nBinsATWD1_InIce_;
  unsigned int ds_nBinsATWD2_InIce_;
  unsigned int ds_nBinsFADC_InIce_;

  unsigned int ds_nBinsATWD0_IceTop_;
  unsigned int ds_nBinsATWD1_IceTop_;
  unsigned int ds_nBinsATWD2_IceTop_;
  unsigned int ds_nBinsFADC_IceTop_;


  /**
   *Calibration Parameters
   */
  /**
   *Start of the valid time range of the detector status
   */
  int32_t cal_startYear_;
  int64_t cal_startDAQTime_;

  /**
   *End of the valid time range of the detector status
   */
  int32_t cal_endYear_;
  int64_t cal_endDAQTime_;

  double cal_temperature_;

  double cal_fadcBaselineFit_slope_;
  double cal_fadcBaselineFit_intercept_;

  double cal_fadcGain_;
  double cal_atwd0Gain_;
  double cal_atwd1Gain_;
  double cal_atwd2Gain_;

  double cal_atwd_a_FreqFit_A_;
  double cal_atwd_a_FreqFit_B_;
  double cal_atwd_a_FreqFit_C_;

  double cal_atwd_b_FreqFit_A_;
  double cal_atwd_b_FreqFit_B_;
  double cal_atwd_b_FreqFit_C_;

  double cal_hvGainFit_slope_;
  double cal_hvGainFit_intercept_;

  double cal_atwdBinCalibFit_slope_;
  double cal_atwdBinCalibFit_intercept_;

  double cal_atwd_response_width_;

  double cal_atwda_deltat_;
  double cal_atwdb_deltat_;
  double cal_spe_disc_thresh_int_;
  double cal_spe_disc_thresh_slope_;
  double cal_mpe_disc_thresh_int_;
  double cal_mpe_disc_thresh_slope_;

  double cal_pmt_disc_thresh_slope_;
  double cal_pmt_disc_thresh_intercept_;

  double cal_inice_relative_efficiencies_;
  double cal_deepcore_relative_efficiencies_;

  double cal_inice_dom_noise_rates_;
  double cal_deepcore_dom_noise_rates_;

};

#endif //I3SIMSOURCETESTMODULE_H
