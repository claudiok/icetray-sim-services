#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3MCRawDOMStatusService.h"
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
  it_timeWindow_(2000.*I3Units::ns)
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
      shared_ptr<I3MCRawDOMStatusService>
      (new I3MCRawDOMStatusService(geo_service));
    log_debug("Made new I3MCRawDOMStatusService.");

    status_->InsertTriggerStatus(ic_trigger_, ic_trigStatus_);
    status_->InsertTriggerStatus(it_trigger_, it_trigStatus_);
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

