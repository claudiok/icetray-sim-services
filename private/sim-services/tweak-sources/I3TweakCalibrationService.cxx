#include "sim-services/tweak-sources/I3TweakCalibrationService.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "icetray/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3_SERVICE_FACTORY(I3TweakCalibrationService);

I3TweakCalibrationService::I3TweakCalibrationService(const I3Context& context) :
  I3ServiceFactory(context),
  oldServiceName_(I3DefaultName<I3CalibrationService>::value()),
  geometryServiceName_(I3DefaultName<I3GeometryService>::value()),
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
  atwdBinCalibFit_intercept_(NAN),
  atwd_response_width_(NAN),
  fadc_response_width_(NAN),
  tauparam_P0_(NAN),
  tauparam_P1_(NAN),
  tauparam_P2_(NAN),
  tauparam_P3_(NAN),
  tauparam_P4_(NAN),
  tauparam_P5_(NAN),
  tauparam_TauFrac_(NAN),
  fadcDeltaT_(NAN),
  frontendImpedance_(NAN),
  pmtTransitTimeSlope_(NAN),
  pmtTransitTimeIntercept_(NAN),
  domcalVersion_(""),
  atwda0_baseline_(NAN),
  atwda1_baseline_(NAN),
  atwda2_baseline_(NAN),
  atwdb0_baseline_(NAN),
  atwdb1_baseline_(NAN),
  atwdb2_baseline_(NAN),
  atwda_deltat_(NAN),
  atwdb_deltat_(NAN),
  spe_disc_thresh_slope_(NAN),
  spe_disc_thresh_int_(NAN),
  mpe_disc_thresh_slope_(NAN),
  mpe_disc_thresh_int_(NAN),
  pmt_disc_thresh_slope_(NAN),
  pmt_disc_thresh_int_(NAN)
{
  AddParameter("OldServiceName","Name of service to tweak",oldServiceName_);
  AddParameter("GeometryServiceName","Name of geometry service",geometryServiceName_);
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

  AddParameter("ATWDResponseWidth","",atwd_response_width_);
  AddParameter("FADCResponseWidth","",fadc_response_width_);
  AddParameter("TauParamP0","",tauparam_P0_);
  AddParameter("TauParamP1","",tauparam_P1_);
  AddParameter("TauParamP2","",tauparam_P2_);
  AddParameter("TauParamP3","",tauparam_P3_);
  AddParameter("TauParamP4","",tauparam_P4_);
  AddParameter("TauParamP5","",tauparam_P5_);
  AddParameter("TauParamTauFrac","",tauparam_TauFrac_);
  AddParameter("FADCDeltaT","",fadcDeltaT_);
  AddParameter("FrontEndImpedence","",frontendImpedance_);
  AddParameter("PMTTransitTimeSlope","",pmtTransitTimeSlope_);
  AddParameter("PMTTransitTimeIntercept","",pmtTransitTimeIntercept_);
  AddParameter("DOMCalVersion","",domcalVersion_);
  AddParameter("ATWDa0Baseline","",atwda0_baseline_);
  AddParameter("ATWDa1Baseline","",atwda1_baseline_);
  AddParameter("ATWDa2Baseline","",atwda2_baseline_);
  AddParameter("ATWDb0Baseline","",atwdb0_baseline_);
  AddParameter("ATWDb1Baseline","",atwdb1_baseline_);
  AddParameter("ATWDb2Baseline","",atwdb2_baseline_);

  AddParameter("ATWDADeltaT","",atwda_deltat_);
  AddParameter("ATWDBDeltaT","",atwdb_deltat_);
  AddParameter("SPEDiscThreshIntercept","",spe_disc_thresh_int_);
  AddParameter("SPEDiscThreshSlope","",spe_disc_thresh_slope_);
  AddParameter("MPEDiscThreshIntercept","",mpe_disc_thresh_int_);
  AddParameter("MPEDiscThreshSlope","",mpe_disc_thresh_slope_);
  AddParameter("PMTDiscThreshIntercept","",pmt_disc_thresh_int_);
  AddParameter("PMTDiscThreshSlope","",pmt_disc_thresh_slope_);
}


void I3TweakCalibrationService::Configure()
{  
  GetParameter("OldServiceName",oldServiceName_);
  GetParameter("GeometryServiceName",geometryServiceName_);
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
  GetParameter("ATWDResponseWidth",atwd_response_width_);

  GetParameter("ATWDResponseWidth",atwd_response_width_);
  GetParameter("FADCResponseWidth",fadc_response_width_);
  GetParameter("TauParamP0",tauparam_P0_);
  GetParameter("TauParamP1",tauparam_P1_);
  GetParameter("TauParamP2",tauparam_P2_);
  GetParameter("TauParamP3",tauparam_P3_);
  GetParameter("TauParamP4",tauparam_P4_);
  GetParameter("TauParamP5",tauparam_P5_);
  GetParameter("TauParamTauFrac",tauparam_TauFrac_);
  GetParameter("FADCDeltaT",fadcDeltaT_);
  GetParameter("FrontEndImpedence",frontendImpedance_);
  GetParameter("PMTTransitTimeSlope",pmtTransitTimeSlope_);
  GetParameter("PMTTransitTimeIntercept",pmtTransitTimeIntercept_);
  GetParameter("DOMCalVersion",domcalVersion_);
  GetParameter("ATWDa0Baseline",atwda0_baseline_);
  GetParameter("ATWDa1Baseline",atwda1_baseline_);
  GetParameter("ATWDa2Baseline",atwda2_baseline_);
  GetParameter("ATWDb0Baseline",atwdb0_baseline_);
  GetParameter("ATWDb1Baseline",atwdb1_baseline_);
  GetParameter("ATWDb2Baseline",atwdb2_baseline_);

  GetParameter("ATWDADeltaT",atwda_deltat_);
  GetParameter("ATWDBDeltaT",atwdb_deltat_);
  GetParameter("SPEDiscThreshIntercept",spe_disc_thresh_int_);
  GetParameter("SPEDiscThreshSlope",spe_disc_thresh_slope_);
  GetParameter("MPEDiscThreshIntercept",mpe_disc_thresh_int_);
  GetParameter("MPEDiscThreshSlope",mpe_disc_thresh_slope_);
  GetParameter("PMTDiscThreshIntercept",pmt_disc_thresh_int_);
  GetParameter("PMTDiscThreshSlope",pmt_disc_thresh_slope_);
}

bool I3TweakCalibrationService::InstallService(I3Context& services)
{
  if(!cal_service_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>(oldServiceName_);

    I3GeometryServicePtr geo_service = context_.Get<I3GeometryServicePtr>(geometryServiceName_);

    cal_service_ = 
      I3TweakCalibrationPtr
      (new I3TweakCalibration(old_cal,geo_service));
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
    if(!isnan(atwd_response_width_))
      cal_service_->SetATWDResponseWidth(atwd_response_width_);

    if(!isnan(fadc_response_width_))
      cal_service_->SetFADCResponseWidth(fadc_response_width_);

    if(!isnan(tauparam_P0_))
      cal_service_->SetTauParamP0(tauparam_P0_);
    if(!isnan(tauparam_P1_))
      cal_service_->SetTauParamP1(tauparam_P1_);
    if(!isnan(tauparam_P2_))
      cal_service_->SetTauParamP2(tauparam_P2_);
    if(!isnan(tauparam_P3_))
      cal_service_->SetTauParamP3(tauparam_P3_);
    if(!isnan(tauparam_P4_))
      cal_service_->SetTauParamP4(tauparam_P4_);
    if(!isnan(tauparam_P5_))
      cal_service_->SetTauParamP5(tauparam_P5_);
    if(!isnan(tauparam_TauFrac_))
      cal_service_->SetTauParamTauFrac(tauparam_TauFrac_);


    if(!isnan(fadcDeltaT_))
      cal_service_->SetFADCDeltaT(fadcDeltaT_);
    if(!isnan(frontendImpedance_))
      cal_service_->SetFrontEndImpedance(frontendImpedance_);
    if(!isnan(pmtTransitTimeSlope_))
      cal_service_->SetPMTTransitTimeSlope(pmtTransitTimeSlope_);
    if(!isnan(pmtTransitTimeIntercept_))
      cal_service_->SetPMTTransitTimeIntercept(pmtTransitTimeIntercept_);
    if(domcalVersion_.size())
      cal_service_->SetDOMCalVersion(domcalVersion_);

    if(!isnan(atwda0_baseline_))
      cal_service_->SetATWDa0Baseline(atwda0_baseline_);
    if(!isnan(atwda1_baseline_))
      cal_service_->SetATWDa1Baseline(atwda1_baseline_);
    if(!isnan(atwda2_baseline_))
      cal_service_->SetATWDa2Baseline(atwda2_baseline_);
    if(!isnan(atwdb0_baseline_))
      cal_service_->SetATWDb0Baseline(atwdb0_baseline_);
    if(!isnan(atwdb1_baseline_))
      cal_service_->SetATWDb1Baseline(atwdb1_baseline_);
    if(!isnan(atwdb2_baseline_))
      cal_service_->SetATWDb2Baseline(atwdb2_baseline_);

    if(!isnan(atwda_deltat_))
      cal_service_->SetATWDADeltaT(atwda_deltat_);
    if(!isnan(atwdb_deltat_))
      cal_service_->SetATWDBDeltaT(atwdb_deltat_);
    if(!isnan(spe_disc_thresh_slope_))
      cal_service_->SetSPEDiscThreshSlope(spe_disc_thresh_slope_);
    if(!isnan(spe_disc_thresh_int_))
      cal_service_->SetSPEDiscThreshIntercept(spe_disc_thresh_int_);
    if(!isnan(mpe_disc_thresh_slope_))
      cal_service_->SetMPEDiscThreshSlope(mpe_disc_thresh_slope_);
    if(!isnan(mpe_disc_thresh_int_))
      cal_service_->SetMPEDiscThreshIntercept(mpe_disc_thresh_int_);
    if(!isnan(pmt_disc_thresh_slope_))
      cal_service_->SetPMTDiscThreshSlope(pmt_disc_thresh_slope_);
    if(!isnan(pmt_disc_thresh_int_))
      cal_service_->SetPMTDiscThreshIntercept(pmt_disc_thresh_int_);
    

  }

  bool good_calib = services.Put<I3CalibrationService>(tweakedServiceName_,cal_service_);
  log_debug("good_calib %d",good_calib);

  return good_calib;
}



