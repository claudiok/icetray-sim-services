#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3MCDetectorStatusService.h"
#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "dataclasses/I3Units.h"

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
  ds_nBinsFADC_(50)
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
  AddParameter("DetStat_StatusFADC","",ds_statusFADC_);
  AddParameter("DetStat_SPEThreshold","",ds_speThreshold_);
  AddParameter("DetStat_FEPedestal","",ds_fePedestal_);
  AddParameter("DetStat_DACTriggerBias0","",ds_dacTriggerBias0_);
  AddParameter("DetStat_DACTriggerBias1","",ds_dacTriggerBias1_);
  AddParameter("DetStat_DACFADCRef","",ds_dacFADCRef_);
  AddParameter("DetStat_NBinsATWD0","",ds_nBinsATWD0_);
  AddParameter("DetStat_NBinsATWD1","",ds_nBinsATWD1_);
  AddParameter("DetStat_NBinsATWD2","",ds_nBinsATWD2_);
  AddParameter("DetStat_NBinsFADC","Number of FADC bins",ds_nBinsFADC_);
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
  GetParameter("DetStat_StatusFADC",ds_statusFADC_);
  GetParameter("DetStat_SPEThreshold",ds_speThreshold_);
  GetParameter("DetStat_FEPedestal",ds_fePedestal_);
  GetParameter("DetStat_DACTriggerBias0",ds_dacTriggerBias0_);
  GetParameter("DetStat_DACTriggerBias1",ds_dacTriggerBias1_);
  GetParameter("DetStat_DACFADCRef",ds_dacFADCRef_);
  GetParameter("DetStat_NBinsATWD0",ds_nBinsATWD0_);
  GetParameter("DetStat_NBinsATWD1",ds_nBinsATWD1_);
  GetParameter("DetStat_NBinsATWD2",ds_nBinsATWD2_);
  GetParameter("DetStat_NBinsFADC",ds_nBinsFADC_);

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
    status_ = 
      shared_ptr<I3MCDetectorStatusService>
      (new I3MCDetectorStatusService(geo_service));
    log_debug("Made new I3MCDetectorStatusService.");

    status_->InsertTriggerStatus(ic_trigger_, ic_trigStatus_);
    status_->InsertTriggerStatus(it_trigger_, it_trigStatus_);

    //Configure the new detector status service
    status_->SetNBinsFADC(ds_nBinsFADC_);
  }

  if(!calibration_){
    calibration_ = 
      shared_ptr<I3MCCalibrationService>
      (new I3MCCalibrationService(geo_service));
    log_debug("Made new I3MCCalibrationService.");
  }


  bool good_calib = services.Put<I3CalibrationService>(calServiceName_,calibration_);
  log_debug("good_calib %d",good_calib);
  bool good_status = services.Put<I3DetectorStatusService>(statusServiceName_,status_);
  log_debug("good_status %d",good_status);

  return (good_calib && good_status);
	  
}

