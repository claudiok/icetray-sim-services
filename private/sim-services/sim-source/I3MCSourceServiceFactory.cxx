#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3MCDetectorStatusService.h"
#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/I3DefaultValues.h"

I3_SERVICE_FACTORY(I3MCSourceServiceFactory);

I3MCSourceServiceFactory::
I3MCSourceServiceFactory(const I3Context& context) : 
  I3ServiceFactory(context),
  ic_configID_(0),
  ic_threshold_(8),
  ic_timeWindow_(5000.*I3Units::ns),
  it_configID_(15),
  it_threshold_(6),
  it_timeWindow_(2000.*I3Units::ns),
  ds_startYear_(I3DetStatDefaults::START_YEAR),
  ds_startDAQTime_(I3DetStatDefaults::START_DAQTIME),
  ds_endYear_(I3DetStatDefaults::END_YEAR),
  ds_endDAQTime_(I3DetStatDefaults::END_DAQTIME),
  ds_icetopLCWindowPre_(I3DetStatDefaults::ICETOP_LCWINDOW_PRE),
  ds_icetopLCWindowPost_(I3DetStatDefaults::ICETOP_LCWINDOW_POST),
  ds_icetopHighGainVoltage_(I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE),
  ds_icetopLowGainVoltage_(I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE),
  ds_iniceLCWindowPre_(I3DetStatDefaults::INICE_LCWINDOW_PRE),
  ds_iniceLCWindowPost_(I3DetStatDefaults::INICE_LCWINDOW_POST),
  ds_lcSpan_(I3DetStatDefaults::LCSPAN),
  ds_iniceVoltage_(I3DetStatDefaults::INICE_VOLTAGE),
  ds_triggerMode_(I3DetStatDefaults::TRIGGER_MODE),
  ds_lcMode_(I3DetStatDefaults::LCMODE),
  ds_statusATWDa_(I3DetStatDefaults::STATUS_ATWDa),
  ds_statusATWDb_(I3DetStatDefaults::STATUS_ATWDb),
  ds_statusFADC_InIce_(I3DetStatDefaults::STATUS_FADC_INICE),
  ds_statusFADC_IceTop_(I3DetStatDefaults::STATUS_FADC_ICETOP),
  ds_speThreshold_(I3DetStatDefaults::SPE_THRESHOLD),
  ds_fePedestal_(I3DetStatDefaults::FE_PEDESTAL),
  ds_dacTriggerBias0_(I3DetStatDefaults::DAC_TRIGGER_BIAS0),
  ds_dacTriggerBias1_(I3DetStatDefaults::DAC_TRIGGER_BIAS1),
  ds_dacFADCRef_(I3DetStatDefaults::DAC_FADC_REF),
  ds_nBinsATWD0_InIce_(I3DetStatDefaults::NBINS_ATWD0_INICE),
  ds_nBinsATWD1_InIce_(I3DetStatDefaults::NBINS_ATWD1_INICE),
  ds_nBinsATWD2_InIce_(I3DetStatDefaults::NBINS_ATWD2_INICE),
  ds_nBinsFADC_InIce_(I3DetStatDefaults::NBINS_FADC_INICE),
  ds_nBinsATWD0_IceTop_(I3DetStatDefaults::NBINS_ATWD0_ICETOP),
  ds_nBinsATWD1_IceTop_(I3DetStatDefaults::NBINS_ATWD1_ICETOP),
  ds_nBinsATWD2_IceTop_(I3DetStatDefaults::NBINS_ATWD2_ICETOP),
  ds_nBinsFADC_IceTop_(I3DetStatDefaults::NBINS_FADC_ICETOP),
  cal_startYear_(I3CalibDefaults::START_YEAR),
  cal_startDAQTime_(I3CalibDefaults::START_DAQTIME),
  cal_endYear_(I3CalibDefaults::END_YEAR),
  cal_endDAQTime_(I3CalibDefaults::END_DAQTIME),
  cal_temperature_(I3CalibDefaults::TEMPERATURE),
  cal_fadcBaselineFit_slope_(I3CalibDefaults::FADC_BASELINE_FIT_SLOPE),
  cal_fadcBaselineFit_intercept_(I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT),
  cal_fadcGain_(I3CalibDefaults::FADC_GAIN),
  cal_atwd0Gain_(I3CalibDefaults::ATWD0_GAIN),
  cal_atwd1Gain_(I3CalibDefaults::ATWD1_GAIN),
  cal_atwd2Gain_(I3CalibDefaults::ATWD2_GAIN),
  cal_atwd_a_FreqFit_A_(I3CalibDefaults::ATWD_A_FREQFIT_A ),
  cal_atwd_a_FreqFit_B_(I3CalibDefaults::ATWD_A_FREQFIT_B),
  cal_atwd_a_FreqFit_C_(I3CalibDefaults::ATWD_A_FREQFIT_C),
  cal_atwd_b_FreqFit_A_(I3CalibDefaults::ATWD_B_FREQFIT_A),
  cal_atwd_b_FreqFit_B_(I3CalibDefaults::ATWD_B_FREQFIT_B),
  cal_atwd_b_FreqFit_C_(I3CalibDefaults::ATWD_B_FREQFIT_C),
  cal_hvGainFit_slope_(I3CalibDefaults::HV_GAIN_FIT_SLOPE),
  cal_hvGainFit_intercept_(I3CalibDefaults::HV_GAIN_FIT_INTERCEPT),
  cal_atwdBinCalibFit_slope_(I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE),
  cal_atwdBinCalibFit_intercept_(I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
{
  calServiceName_ = I3DefaultName<I3CalibrationService>::value();
  statusServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  geoServiceName_ = I3DefaultName<I3GeometryService>::value();

  AddParameter("Threshold","InIce Multiplicity Trigger Threshold",ic_threshold_);
  AddParameter("TimeWindow","InIce Multiplicity Trigger Time Window",ic_timeWindow_);
  AddParameter("ConfigID","InIce Trigger Status Config ID",ic_configID_);
  AddParameter("IceTop_Threshold","IceTop Multiplicity Trigger Threshold",it_threshold_);
  AddParameter("IceTop_TimeWindow","IceTop Multiplicity Trigger Time Window",it_timeWindow_);
  AddParameter("IceTop_ConfigID","IceTop Trigger Status Config ID",it_configID_);
  AddParameter("CalServiceName","Name of calibration service to install",calServiceName_);
  AddParameter("StatusServiceName","Name of detector status service to install",statusServiceName_);
  AddParameter("GeoServiceName","Name of geometry service",geoServiceName_);

  AddParameter("DetStat_StartYear","",ds_startYear_);
  AddParameter("DetStat_StartDAQTime","",ds_startDAQTime_);
  AddParameter("DetStat_EndYear","",ds_endYear_);
  AddParameter("DetStat_EndDAQTime","",ds_endDAQTime_);
  AddParameter("DetStat_IceTopLCWindowPre","",ds_icetopLCWindowPre_);
  AddParameter("DetStat_IceTopLCWindowPost","",ds_icetopLCWindowPost_);
  AddParameter("DetStat_IceTopHighGainVoltage","",ds_icetopHighGainVoltage_);
  AddParameter("DetStat_IceTopLowGainVoltage","",ds_icetopLowGainVoltage_);
  AddParameter("DetStat_InIceLCWindowPre","",ds_iniceLCWindowPre_);
  AddParameter("DetStat_InIceLCWindowPost","",ds_iniceLCWindowPost_);
  AddParameter("DetStat_LCSpan","",ds_lcSpan_);
  AddParameter("DetStat_InIceVoltage","",ds_iniceVoltage_);
  AddParameter("DetStat_TriggerMode","",ds_triggerMode_);
  AddParameter("DetStat_LCMode","",ds_lcMode_);
  AddParameter("DetStat_StatusATWDa","",ds_statusATWDa_);
  AddParameter("DetStat_StatusATWDb","",ds_statusATWDb_);
  AddParameter("DetStat_StatusFADCInIce","",ds_statusFADC_InIce_);
  AddParameter("DetStat_StatusFADCIceTop","",ds_statusFADC_IceTop_);
  AddParameter("DetStat_SPEThreshold","",ds_speThreshold_);
  AddParameter("DetStat_FEPedestal","",ds_fePedestal_);
  AddParameter("DetStat_DACTriggerBias0","",ds_dacTriggerBias0_);
  AddParameter("DetStat_DACTriggerBias1","",ds_dacTriggerBias1_);
  AddParameter("DetStat_DACFADCRef","",ds_dacFADCRef_);

  AddParameter("DetStat_NBinsATWD0InIce","",ds_nBinsATWD0_InIce_);
  AddParameter("DetStat_NBinsATWD1InIce","",ds_nBinsATWD1_InIce_);
  AddParameter("DetStat_NBinsATWD2InIce","",ds_nBinsATWD2_InIce_);
  AddParameter("DetStat_NBinsFADCInIce","",ds_nBinsFADC_InIce_);

  AddParameter("DetStat_NBinsATWD0IceTop","",ds_nBinsATWD0_IceTop_);
  AddParameter("DetStat_NBinsATWD1IceTop","",ds_nBinsATWD1_IceTop_);
  AddParameter("DetStat_NBinsATWD2IceTop","",ds_nBinsATWD2_IceTop_);
  AddParameter("DetStat_NBinsFADCIceTop","",ds_nBinsFADC_IceTop_);

  AddParameter("Calib_StartYear","",cal_startYear_);
  AddParameter("Calib_StartDAQTime","",cal_startDAQTime_);
  AddParameter("Calib_EndYear","",cal_endYear_);
  AddParameter("Calib_EndDAQTime","",cal_endDAQTime_);
  AddParameter("Calib_Temperature","",cal_temperature_);
  AddParameter("Calib_fadcBaselineFit_slope","",cal_fadcBaselineFit_slope_);
  AddParameter("Calib_fadcBaselineFit_intercept","",cal_fadcBaselineFit_intercept_);
  AddParameter("Calib_fadcGain","",cal_fadcGain_);
  AddParameter("Calib_atwd0Gain","",cal_atwd0Gain_);
  AddParameter("Calib_atwd1Gain","",cal_atwd1Gain_);
  AddParameter("Calib_atwd2Gain","",cal_atwd2Gain_);
  AddParameter("Calib_atwd_a_FreqFit_A","",cal_atwd_a_FreqFit_A_);
  AddParameter("Calib_atwd_a_FreqFit_B","",cal_atwd_a_FreqFit_B_);
  AddParameter("Calib_atwd_a_FreqFit_C","",cal_atwd_a_FreqFit_C_);
  AddParameter("Calib_atwd_b_FreqFit_A","",cal_atwd_b_FreqFit_A_);
  AddParameter("Calib_atwd_b_FreqFit_B","",cal_atwd_b_FreqFit_B_);
  AddParameter("Calib_atwd_b_FreqFit_C","",cal_atwd_b_FreqFit_C_);
  AddParameter("Calib_hvGainFit_slope","",cal_hvGainFit_slope_);
  AddParameter("Calib_hvGainFit_intercept","",cal_hvGainFit_intercept_);
  AddParameter("Calib_atwdBinCalibFit_slope","",cal_atwdBinCalibFit_slope_);
  AddParameter("Calib_atwdBinCalibFit_intercept","",cal_atwdBinCalibFit_intercept_);

}

I3MCSourceServiceFactory::
~I3MCSourceServiceFactory(){}

void I3MCSourceServiceFactory::Configure()
{
  GetParameter("Threshold",ic_threshold_);
  GetParameter("TimeWindow",ic_timeWindow_);
  GetParameter("ConfigID",ic_configID_);
  GetParameter("IceTop_Threshold",it_threshold_);
  GetParameter("IceTop_TimeWindow",it_timeWindow_);
  GetParameter("IceTop_ConfigID",it_configID_);
  GetParameter("CalServiceName",calServiceName_);
  GetParameter("StatusServiceName",statusServiceName_);
  GetParameter("GeoServiceName",geoServiceName_);

  //DetectorStatus Parameters
  GetParameter("DetStat_StartYear",ds_startYear_);
  GetParameter("DetStat_StartDAQTime",ds_startDAQTime_);
  GetParameter("DetStat_EndYear",ds_endYear_);
  GetParameter("DetStat_EndDAQTime",ds_endDAQTime_);
  GetParameter("DetStat_IceTopLCWindowPre",ds_icetopLCWindowPre_);
  GetParameter("DetStat_IceTopLCWindowPost",ds_icetopLCWindowPost_);
  GetParameter("DetStat_IceTopHighGainVoltage",ds_icetopHighGainVoltage_);
  GetParameter("DetStat_IceTopLowGainVoltage",ds_icetopLowGainVoltage_);
  GetParameter("DetStat_InIceLCWindowPre",ds_iniceLCWindowPre_);
  GetParameter("DetStat_InIceLCWindowPost",ds_iniceLCWindowPost_);
  GetParameter("DetStat_LCSpan",ds_lcSpan_);
  GetParameter("DetStat_InIceVoltage",ds_iniceVoltage_);
  GetParameter("DetStat_TriggerMode",ds_triggerMode_);
  GetParameter("DetStat_LCMode",ds_lcMode_);
  GetParameter("DetStat_StatusATWDa",ds_statusATWDa_);
  GetParameter("DetStat_StatusATWDb",ds_statusATWDb_);
  GetParameter("DetStat_StatusFADCInIce",ds_statusFADC_InIce_);
  GetParameter("DetStat_StatusFADCIceTop",ds_statusFADC_IceTop_);
  GetParameter("DetStat_SPEThreshold",ds_speThreshold_);
  GetParameter("DetStat_FEPedestal",ds_fePedestal_);
  GetParameter("DetStat_DACTriggerBias0",ds_dacTriggerBias0_);
  GetParameter("DetStat_DACTriggerBias1",ds_dacTriggerBias1_);
  GetParameter("DetStat_DACFADCRef",ds_dacFADCRef_);

  GetParameter("DetStat_NBinsATWD0InIce",ds_nBinsATWD0_InIce_);
  GetParameter("DetStat_NBinsATWD1InIce",ds_nBinsATWD1_InIce_);
  GetParameter("DetStat_NBinsATWD2InIce",ds_nBinsATWD2_InIce_);
  GetParameter("DetStat_NBinsFADCInIce",ds_nBinsFADC_InIce_);

  GetParameter("DetStat_NBinsATWD0IceTop",ds_nBinsATWD0_IceTop_);
  GetParameter("DetStat_NBinsATWD1IceTop",ds_nBinsATWD1_IceTop_);
  GetParameter("DetStat_NBinsATWD2IceTop",ds_nBinsATWD2_IceTop_);
  GetParameter("DetStat_NBinsFADCIceTop",ds_nBinsFADC_IceTop_);

  //Calibration Parameters
  GetParameter("Calib_StartYear",cal_startYear_);
  GetParameter("Calib_StartDAQTime",cal_startDAQTime_);
  GetParameter("Calib_EndYear",cal_endYear_);
  GetParameter("Calib_EndDAQTime",cal_endDAQTime_);
  GetParameter("Calib_Temperature",cal_temperature_);
  GetParameter("Calib_fadcBaselineFit_slope",cal_fadcBaselineFit_slope_);
  GetParameter("Calib_fadcBaselineFit_intercept",cal_fadcBaselineFit_intercept_);
  GetParameter("Calib_fadcGain",cal_fadcGain_);
  GetParameter("Calib_atwd0Gain",cal_atwd0Gain_);
  GetParameter("Calib_atwd1Gain",cal_atwd1Gain_);
  GetParameter("Calib_atwd2Gain",cal_atwd2Gain_);
  GetParameter("Calib_atwd_a_FreqFit_A",cal_atwd_a_FreqFit_A_);
  GetParameter("Calib_atwd_a_FreqFit_B",cal_atwd_a_FreqFit_B_);
  GetParameter("Calib_atwd_a_FreqFit_C",cal_atwd_a_FreqFit_C_);
  GetParameter("Calib_atwd_b_FreqFit_A",cal_atwd_b_FreqFit_A_);
  GetParameter("Calib_atwd_b_FreqFit_B",cal_atwd_b_FreqFit_B_);
  GetParameter("Calib_atwd_b_FreqFit_C",cal_atwd_b_FreqFit_C_);
  GetParameter("Calib_hvGainFit_slope",cal_hvGainFit_slope_);
  GetParameter("Calib_hvGainFit_intercept",cal_hvGainFit_intercept_);
  GetParameter("Calib_atwdBinCalibFit_slope",cal_atwdBinCalibFit_slope_);
  GetParameter("Calib_atwdBinCalibFit_intercept",cal_atwdBinCalibFit_intercept_);

  ic_trigger_.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, TriggerKey::SIMPLE_MULTIPLICITY, ic_configID_);
  ic_trigStatus_.GetTriggerName().append("simple_multiplicity");
  ic_trigStatus_.GetTriggerSettings().insert(make_pair("threshold", ic_threshold_));
  ic_trigStatus_.GetTriggerSettings().insert(make_pair("timeWindow", static_cast<int>(ic_timeWindow_)));

  it_trigger_.GetTriggerKey() = TriggerKey(TriggerKey::ICE_TOP, TriggerKey::SIMPLE_MULTIPLICITY, it_configID_);
  it_trigStatus_.GetTriggerName().append("simple_multiplicity");
  it_trigStatus_.GetTriggerSettings().insert(make_pair("threshold", it_threshold_));
  it_trigStatus_.GetTriggerSettings().insert(make_pair("timeWindow", static_cast<int>(it_timeWindow_)));
}

bool I3MCSourceServiceFactory::InstallService(I3Context& services)
{

  I3GeometryServicePtr geo_service = context_.Get<I3GeometryServicePtr>(geoServiceName_);
  if(!geo_service) log_fatal("Couldn't find the geometry service.");

  if(!status_){
    I3DetectorStatusServicePtr old_status = context_.Get<I3DetectorStatusServicePtr>();
    cerr<<"old_status: "<<old_status<<endl;
    status_ = 
      shared_ptr<I3MCDetectorStatusService>
      (new I3MCDetectorStatusService(geo_service,old_status));
    log_debug("Made new I3MCDetectorStatusService.");

    status_->InsertTriggerStatus(ic_trigger_, ic_trigStatus_);
    status_->InsertTriggerStatus(it_trigger_, it_trigStatus_);

    //Configure the new detector status service

    status_->SetStartYear(ds_startYear_);
    status_->SetStartDAQTime(ds_startDAQTime_);
    status_->SetEndYear(ds_endYear_);
    status_->SetEndDAQTime(ds_endDAQTime_);
    
    status_->SetIceTopLCWindowPre(ds_icetopLCWindowPre_);
    status_->SetIceTopLCWindowPost(ds_icetopLCWindowPost_);
    status_->SetIceTopHighGainVoltage(ds_icetopHighGainVoltage_);
    status_->SetIceTopLowGainVoltage(ds_icetopLowGainVoltage_);
    status_->SetInIceLCWindowPre(ds_iniceLCWindowPre_);
    status_->SetInIceLCWindowPost(ds_iniceLCWindowPost_);
    status_->SetLCSpan(ds_lcSpan_);
    status_->SetInIceVoltage(ds_iniceVoltage_);
    status_->SetTriggerMode(ds_triggerMode_);
    status_->SetLCMode(ds_lcMode_);
    status_->SetStatusATWDa(ds_statusATWDa_);
    status_->SetStatusATWDb(ds_statusATWDb_);
    status_->SetStatusFADCInIce(ds_statusFADC_InIce_);
    status_->SetStatusFADCIceTop(ds_statusFADC_IceTop_);
    status_->SetSPEThreshold(ds_speThreshold_);
    status_->SetFEPedestal(ds_fePedestal_);
    status_->SetDACTriggerBias0(ds_dacTriggerBias0_);
    status_->SetDACTriggerBias1(ds_dacTriggerBias1_);
    status_->SetDACFADCRef(ds_dacFADCRef_);
    status_->SetNBinsATWD0InIce(ds_nBinsATWD0_InIce_);
    status_->SetNBinsATWD1InIce(ds_nBinsATWD1_InIce_);
    status_->SetNBinsATWD2InIce(ds_nBinsATWD2_InIce_);
    status_->SetNBinsFADCInIce(ds_nBinsFADC_InIce_);
    status_->SetNBinsATWD0IceTop(ds_nBinsATWD0_IceTop_);
    status_->SetNBinsATWD1IceTop(ds_nBinsATWD1_IceTop_);
    status_->SetNBinsATWD2IceTop(ds_nBinsATWD2_IceTop_);
    status_->SetNBinsFADCIceTop(ds_nBinsFADC_IceTop_);
  }

  if(!calibration_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>();
    calibration_ = 
      shared_ptr<I3MCCalibrationService>
      (new I3MCCalibrationService(geo_service,old_cal));
    log_debug("Made new I3MCCalibrationService.");

    calibration_->SetStartYear(cal_startYear_);
    calibration_->SetStartDAQTime(cal_startDAQTime_);
    calibration_->SetEndYear(cal_endYear_);
    calibration_->SetEndDAQTime(cal_endDAQTime_);

    calibration_->SetTemperature(cal_temperature_);
    calibration_->SetFADCBaselineSlope(cal_fadcBaselineFit_slope_);
    calibration_->SetFADCBaselineIntercept(cal_fadcBaselineFit_intercept_);
    calibration_->SetFADCGain(cal_fadcGain_);
    calibration_->SetATWD0(cal_atwd0Gain_);
    calibration_->SetATWD1(cal_atwd1Gain_);
    calibration_->SetATWD2(cal_atwd2Gain_);
    calibration_->SetATWDaFreqFitA(cal_atwd_a_FreqFit_A_);
    calibration_->SetATWDaFreqFitB(cal_atwd_a_FreqFit_B_);
    calibration_->SetATWDaFreqFitC(cal_atwd_a_FreqFit_C_);
    calibration_->SetATWDbFreqFitA(cal_atwd_b_FreqFit_A_);
    calibration_->SetATWDbFreqFitB(cal_atwd_b_FreqFit_B_);
    calibration_->SetATWDbFreqFitC(cal_atwd_b_FreqFit_C_);
    calibration_->SetHVGainSlope(cal_hvGainFit_slope_);
    calibration_->SetHVGainIntercept(cal_hvGainFit_intercept_);
    calibration_->SetATWDBinCalibSlope(cal_atwdBinCalibFit_slope_);
    calibration_->SetATWDBinCalibIntercept(cal_atwdBinCalibFit_intercept_);
  }

  bool good_calib = services.Put<I3CalibrationService>(calServiceName_,calibration_);
  log_debug("good_calib %d",good_calib);
  bool good_status = services.Put<I3DetectorStatusService>(statusServiceName_,status_);
  log_debug("good_status %d",good_status);

  return (good_calib && good_status);
	  
}

