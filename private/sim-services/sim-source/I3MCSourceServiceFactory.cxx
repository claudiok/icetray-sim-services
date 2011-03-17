#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3MCDetectorStatusService.h"
#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"
#include "sim-services/sim-source/default-values/I3TWRDefaults.h"

#include "trigger-sim/default-values/I3InIceTriggerDefaults.h"
#include "trigger-sim/default-values/I3IceTopTriggerDefaults.h"

I3_SERVICE_FACTORY(I3MCSourceServiceFactory);

I3MCSourceServiceFactory::
I3MCSourceServiceFactory(const I3Context& context) : 
  I3ServiceFactory(context),
  installCalibration_(true),
  installDetectorStatus_(true),
  modifyWithExtremePrejudice_(false)
{
  oldCalServiceName_ = I3DefaultName<I3CalibrationService>::value();
  oldStatusServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  calServiceName_ = I3DefaultName<I3CalibrationService>::value();
  statusServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  geoServiceName_ = I3DefaultName<I3GeometryService>::value();

  // these are deprecated
  bool installInIceTriggers(false);
  bool installIceTopTriggers(false);
  bool installTWRTriggers(false);

  AddParameter("CalServiceName","Name of calibration service to install",calServiceName_);
  AddParameter("StatusServiceName","Name of detector status service to install",statusServiceName_);
  AddParameter("OldCalServiceName","Name of calibration service to retrieve",oldCalServiceName_);
  AddParameter("OldStatusServiceName","Name of detector status service to retrieve",oldStatusServiceName_);
  AddParameter("GeoServiceName","Name of geometry service",geoServiceName_);
  AddParameter("InstallCalibration","Install Calibration",installCalibration_);
  AddParameter("InstallDetectorStatus","Install DetectorStatus",installDetectorStatus_);
  AddParameter("InstallInIceTriggers","Install InIce Triggers",installInIceTriggers);
  AddParameter("InstallIceTopTriggers","Install IceTop Triggers",installIceTopTriggers);
  AddParameter("InstallTWRTriggers","Install TWR Triggers",installTWRTriggers);
  AddParameter("DoNotModifyStrings","Do not modify these strings",skipStrings_);
  AddParameter("DoNotModifyStations","Do not modify these stations",skipStations_);
  AddParameter("ModifyWithExtremePrejudice","Overwrite C and D for modules not on the DoNotModify lists",modifyWithExtremePrejudice_);
}

I3MCSourceServiceFactory::
~I3MCSourceServiceFactory(){}

void I3MCSourceServiceFactory::Configure()
{
  bool installInIceTriggers(false);
  bool installIceTopTriggers(false);
  bool installTWRTriggers(false);

  GetParameter("OldCalServiceName",oldCalServiceName_);
  GetParameter("OldStatusServiceName",oldStatusServiceName_);
  GetParameter("CalServiceName",calServiceName_);
  GetParameter("StatusServiceName",statusServiceName_);
  GetParameter("GeoServiceName",geoServiceName_);
  GetParameter("InstallCalibration",installCalibration_);
  GetParameter("InstallDetectorStatus",installDetectorStatus_);
  GetParameter("InstallInIceTriggers",installInIceTriggers);
  GetParameter("InstallIceTopTriggers",installIceTopTriggers);
  GetParameter("InstallTWRTriggers",installTWRTriggers);
  GetParameter("DoNotModifyStrings",skipStrings_);
  GetParameter("DoNotModifyStations",skipStations_);
  GetParameter("ModifyWithExtremePrejudice",modifyWithExtremePrejudice_);

  if (installInIceTriggers || installIceTopTriggers || installTWRTriggers){
    log_error("This feature is deprecated.");
    log_error("I3MCSourceServiceFactory is no longer responsible for installing triggers.");
    log_error("You must start with a valid GCD file from a previous season.");
  }
}

bool I3MCSourceServiceFactory::InstallService(I3Context& services)
{
  log_debug("InstallService");

  I3GeometryServicePtr geo_service = context_.Get<I3GeometryServicePtr>(geoServiceName_);
  if(!geo_service) log_fatal("Couldn't find the geometry service.");

  if(!statusService_){
    I3DetectorStatusServicePtr old_status = context_.Get<I3DetectorStatusServicePtr>(oldStatusServiceName_);
    log_debug("Couldn't find old status (%s).  Going to create one from scratch.",oldStatusServiceName_.c_str());
    statusService_ = 
      shared_ptr<I3MCDetectorStatusService>
      (new I3MCDetectorStatusService(geo_service,old_status));
    log_debug("Made new I3MCDetectorStatusService.");

    statusService_->SetSkipStrings(skipStrings_);
    statusService_->SetSkipStations(skipStations_);
    statusService_->ModifyWithExtremePrejudice(modifyWithExtremePrejudice_);

  }

  if(!calibrationService_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>(oldCalServiceName_);
    calibrationService_ = 
      shared_ptr<I3MCCalibrationService>
      (new I3MCCalibrationService(geo_service,old_cal));
    log_debug("Made new I3MCCalibrationService.");

    calibrationService_->SetSkipStrings(skipStrings_);
    calibrationService_->SetSkipStations(skipStations_);
    calibrationService_->ModifyWithExtremePrejudice(modifyWithExtremePrejudice_);

  }

  bool good_calib(true);
  bool good_status(true);
  if(installCalibration_){
    good_calib = services.Put<I3CalibrationService>(calServiceName_,calibrationService_);
    log_debug("good_calib %d",good_calib);
  }else{
    log_debug("Not installing Calibration");
  }

  if(installDetectorStatus_){
    good_status = services.Put<I3DetectorStatusService>(statusServiceName_,statusService_);
    log_debug("good_status %d",good_status);
    log_debug("%s has %zu triggers.",statusServiceName_.c_str(),statusService_->GetTriggerStatusSize());
  }else{
    log_debug("Not installing Detector Status");
  }

  return (good_calib && good_status);
	  
}

