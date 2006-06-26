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
  configID_(0),
  threshold_(8),
  timeWindow_(5.*I3Units::microsecond)
{
  calServiceName_ = I3DefaultName<I3CalibrationService>::value();
  statusServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  geoServiceName_ = I3DefaultName<I3GeometryService>::value();
  AddParameter("Threshold","Multiplicity Trigger Threshold",threshold_);
  AddParameter("TimeWindow","Multiplicity Trigger Time Window",timeWindow_);
  AddParameter("ConfigID","Trigger Status Config ID",configID_);
  AddParameter("CalServiceName","Name of calibration service to install",calServiceName_);
  AddParameter("StatusServiceName","Name of detector status service to install",statusServiceName_);
  AddParameter("GeoServiceName","Name of geometry service",geoServiceName_);
}

I3MCSourceServiceFactory::
~I3MCSourceServiceFactory(){}

void I3MCSourceServiceFactory::Configure()
{
  GetParameter("Threshold",threshold_);
  GetParameter("TimeWindow",timeWindow_);
  GetParameter("ConfigID",configID_);
  GetParameter("CalServiceName",calServiceName_);
  GetParameter("StatusServiceName",statusServiceName_);
  GetParameter("GeoServiceName",geoServiceName_);

  Trigger.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, TriggerKey::SIMPLE_MULTIPLICITY, configID_);

  TrigStatus.GetTriggerName().append("simple_multiplicity");
  TrigStatus.GetTriggerSettings().insert(make_pair("threshold", threshold_));
  TrigStatus.GetTriggerSettings().insert(make_pair("timeWindow", static_cast<int>(timeWindow_)));
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

    status_->SetTriggerStatus(Trigger, TrigStatus);
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

