#include "sim-services/tweak-sources/I3TweakCalibrationService.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3_SERVICE_FACTORY(I3TweakCalibrationService);

I3TweakCalibrationService::I3TweakCalibrationService(const I3Context& context) :
  I3ServiceFactory(context),
  oldServiceName_(I3DefaultName<I3CalibrationService>::value()),
  tweakedServiceName_(I3DefaultName<I3TweakCalibrationService>::value()),
  temperature_(NAN),
  fadcBaselineFit_slope_(NAN),
  fadcBaselineFit_intercept_(NAN),
  fadcGain_(NAN),
  atwd0Gain_(NAN),
  atwd1Gain_(NAN),
  atwd2Gain_(NAN),
  atwd_a_FreqFit_A_(NAN),
  atwd_a_FreqFit_B_(NAN),
  atwd_a_FreqFit_C_(NAN),
  atwd_b_FreqFit_A_(NAN),
  atwd_b_FreqFit_B_(NAN),
  atwd_b_FreqFit_C_(NAN),
  hvGainFit_slope_(NAN),
  hvGainFit_intercept_(NAN),
  atwdBinCalibFit_slope_(NAN),
  atwdBinCalibFit_intercept_(NAN)
{
  AddParameter("OldServiceName","Name of service to tweak",oldServiceName_);
  AddParameter("TweakedServiceName","Name of tweaked service",tweakedServiceName_);
  AddParameter("Temperature","",temperature_);
  AddParameter("FADCBaselineFit_slope","",fadcBaselineFit_slope_);
  AddParameter("FADCBaselineFit_intercept","",fadcBaselineFit_intercept_);
  AddParameter("FADCGain","",fadcGain_);
  AddParameter("ATWD0Gain","",atwd0Gain_);
  AddParameter("ATWD1Gain","",atwd1Gain_);
  AddParameter("ATWD2Gain","",atwd2Gain_);
  AddParameter("ATWD_a_FreqFit_A","",atwd_a_FreqFit_A_);
  AddParameter("ATWD_a_FreqFit_B","",atwd_a_FreqFit_B_);
  AddParameter("ATWD_a_FreqFit_C","",atwd_a_FreqFit_C_);
  AddParameter("ATWD_b_FreqFit_A","",atwd_b_FreqFit_A_);
  AddParameter("atwd_b_FreqFit_B","",atwd_b_FreqFit_B_);
  AddParameter("ATWD_b_FreqFit_C","",atwd_b_FreqFit_C_);
  AddParameter("HVGainFit_slope","",hvGainFit_slope_);
  AddParameter("HVGainFit_intercept","",hvGainFit_intercept_);
  AddParameter("ATWDBinCalibFit_slope","",atwdBinCalibFit_slope_);
  AddParameter("ATWDBinCalibFit_intercept","",atwdBinCalibFit_intercept_);
}


void I3TweakCalibrationService::Configure()
{  
  GetParameter("OldServiceName",oldServiceName_);
  GetParameter("TweakedServiceName",tweakedServiceName_);
  GetParameter("Temperature",temperature_);
  GetParameter("FADCBaselineFit_slope",fadcBaselineFit_slope_);
  GetParameter("FADCBaselineFit_intercept",fadcBaselineFit_intercept_);
  GetParameter("FADCGain",fadcGain_);
  GetParameter("ATWD0Gain",atwd0Gain_);
  GetParameter("ATWD1Gain",atwd1Gain_);
  GetParameter("ATWD2Gain",atwd2Gain_);
  GetParameter("ATWD_a_FreqFit_A",atwd_a_FreqFit_A_);
  GetParameter("ATWD_a_FreqFit_B",atwd_a_FreqFit_B_);
  GetParameter("ATWD_a_FreqFit_C",atwd_a_FreqFit_C_);
  GetParameter("ATWD_b_FreqFit_A",atwd_b_FreqFit_A_);
  GetParameter("ATWD_b_FreqFit_B",atwd_b_FreqFit_B_);
  GetParameter("ATWD_b_FreqFit_C",atwd_b_FreqFit_C_);
  GetParameter("HVGainFit_slope",hvGainFit_slope_);
  GetParameter("HVGainFit_intercept",hvGainFit_intercept_);
  GetParameter("ATWDBinCalibFit_slope",atwdBinCalibFit_slope_);
  GetParameter("ATWDBinCalibFit_intercept",atwdBinCalibFit_intercept_);
}

bool I3TweakCalibrationService::InstallService(I3Context& services)
{
  if(!cal_service_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>(oldServiceName_);
    cal_service_ = 
      I3TweakCalibrationPtr
      (new I3TweakCalibration(old_cal));
    log_debug("Made new I3TweakCalibration.");
    //Configure the calibration
    if(!isnan(temperature_))
      cal_service_->SetTemperature(temperature_);
    if(!isnan(fadcBaselineFit_slope_))
      cal_service_->SetFADCBaselineSlope(fadcBaselineFit_slope_);
    if(!isnan(fadcBaselineFit_intercept_))
      cal_service_->SetFADCBaselineIntercept(fadcBaselineFit_intercept_);
    if(!isnan(fadcGain_))
      cal_service_->SetFADCGain(fadcGain_);
    if(!isnan(atwd0Gain_))
      cal_service_->SetATWD0Gain(atwd0Gain_);
    if(!isnan(atwd1Gain_))
      cal_service_->SetATWD1Gain(atwd1Gain_);
    if(!isnan(atwd2Gain_))
      cal_service_->SetATWD2Gain(atwd2Gain_);
    if(!isnan(atwd_a_FreqFit_A_))
      cal_service_->SetATWDaFreqFitA(atwd_a_FreqFit_A_);
    if(!isnan(atwd_a_FreqFit_B_))
      cal_service_->SetATWDaFreqFitB(atwd_a_FreqFit_B_);
    if(!isnan(atwd_a_FreqFit_C_))
      cal_service_->SetATWDaFreqFitC(atwd_a_FreqFit_C_);
    if(!isnan(atwd_b_FreqFit_A_))
      cal_service_->SetATWDbFreqFitA(atwd_b_FreqFit_A_);
    if(!isnan(atwd_b_FreqFit_B_))
      cal_service_->SetATWDbFreqFitB(atwd_b_FreqFit_B_);
    if(!isnan(atwd_b_FreqFit_C_))
      cal_service_->SetATWDbFreqFitC(atwd_b_FreqFit_C_);
    if(!isnan(hvGainFit_slope_))
      cal_service_->SetHVGainSlope(hvGainFit_slope_);
    if(!isnan(hvGainFit_intercept_))
      cal_service_->SetHVGainIntercept(hvGainFit_intercept_);
    if(!isnan(atwdBinCalibFit_slope_))
      cal_service_->SetATWDBinCalibSlope(atwdBinCalibFit_slope_);
    if(!isnan(atwdBinCalibFit_intercept_))
      cal_service_->SetATWDBinCalibIntercept(atwdBinCalibFit_intercept_);
  }

  bool good_calib = services.Put<I3CalibrationService>(tweakedServiceName_,cal_service_);
  log_debug("good_calib %d",good_calib);

  return good_calib;
}



