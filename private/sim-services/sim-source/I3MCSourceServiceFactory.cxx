#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3MCDetectorStatusService.h"
#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"
#include "sim-services/sim-source/default-values/I3InIceTriggerDefaults.h"
#include "sim-services/sim-source/default-values/I3IceTopTriggerDefaults.h"

I3_SERVICE_FACTORY(I3MCSourceServiceFactory);

I3MCSourceServiceFactory::
I3MCSourceServiceFactory(const I3Context& context) : 
  I3ServiceFactory(context)
{
  calServiceName_ = "";
  statusServiceName_ = "";
  geoServiceName_ = I3DefaultName<I3GeometryService>::value();

  AddParameter("CalServiceName","Name of calibration service to install",calServiceName_);
  AddParameter("StatusServiceName","Name of detector status service to install",statusServiceName_);
  AddParameter("GeoServiceName","Name of geometry service",geoServiceName_);
}

I3MCSourceServiceFactory::
~I3MCSourceServiceFactory(){}

void I3MCSourceServiceFactory::Configure()
{
  GetParameter("CalServiceName",calServiceName_);
  GetParameter("StatusServiceName",statusServiceName_);
  GetParameter("GeoServiceName",geoServiceName_);
}

bool I3MCSourceServiceFactory::InstallService(I3Context& services)
{

  I3GeometryServicePtr geo_service = context_.Get<I3GeometryServicePtr>(geoServiceName_);
  if(!geo_service) log_fatal("Couldn't find the geometry service.");

  if(!statusService_){
    I3DetectorStatusServicePtr old_status = context_.Get<I3DetectorStatusServicePtr>();
    statusService_ = 
      shared_ptr<I3MCDetectorStatusService>
      (new I3MCDetectorStatusService(geo_service,old_status));
    log_debug("Made new I3MCDetectorStatusService.");

    I3Trigger ic_trigger;
    ic_trigger.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, 
					    TriggerKey::SIMPLE_MULTIPLICITY, 
					    I3InIceTriggerDefaults::CONFIG_ID);
    I3TriggerStatus ic_trigStatus;
    ic_trigStatus.GetTriggerName().append("simple_multiplicity");
    ic_trigStatus.GetTriggerSettings().insert(make_pair("threshold", 
							I3InIceTriggerDefaults::THRESHOLD));
    ic_trigStatus.GetTriggerSettings().insert(make_pair("timeWindow", static_cast<int>(I3InIceTriggerDefaults::TIME_WINDOW)));

    I3Trigger it_trigger;
    it_trigger.GetTriggerKey() = TriggerKey(TriggerKey::ICE_TOP, 
					    TriggerKey::SIMPLE_MULTIPLICITY, 
					    I3IceTopTriggerDefaults::CONFIG_ID);
    I3TriggerStatus it_trigStatus;
    it_trigStatus.GetTriggerName().append("simple_multiplicity");
    it_trigStatus.GetTriggerSettings().insert(make_pair("threshold", I3IceTopTriggerDefaults::THRESHOLD));
    it_trigStatus.GetTriggerSettings().insert(make_pair("timeWindow", static_cast<int>(I3IceTopTriggerDefaults::TIME_WINDOW)));
    
    statusService_->InsertTriggerStatus(ic_trigger, ic_trigStatus);
    statusService_->InsertTriggerStatus(it_trigger, it_trigStatus);

    //Configure the new detector status service

    statusService_->SetStartYear(I3DetStatDefaults::START_YEAR);
    statusService_->SetStartDAQTime(I3DetStatDefaults::START_DAQTIME);
    statusService_->SetEndYear(I3DetStatDefaults::END_YEAR);
    statusService_->SetEndDAQTime(I3DetStatDefaults::END_DAQTIME);
    
    statusService_->SetIceTopLCWindowPre(I3DetStatDefaults::ICETOP_LCWINDOW_PRE);
    statusService_->SetIceTopLCWindowPost(I3DetStatDefaults::ICETOP_LCWINDOW_POST);
    statusService_->SetIceTopHighGainVoltage(I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE);
    statusService_->SetIceTopLowGainVoltage(I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE);
    statusService_->SetInIceLCWindowPre(I3DetStatDefaults::INICE_LCWINDOW_PRE);
    statusService_->SetInIceLCWindowPost(I3DetStatDefaults::INICE_LCWINDOW_POST);
    statusService_->SetLCSpan(I3DetStatDefaults::LCSPAN);
    statusService_->SetInIceVoltage(I3DetStatDefaults::INICE_VOLTAGE);
    statusService_->SetTriggerMode(I3DetStatDefaults::TRIGGER_MODE);
    statusService_->SetLCMode(I3DetStatDefaults::LCMODE);
    statusService_->SetStatusATWDa(I3DetStatDefaults::STATUS_ATWDa);
    statusService_->SetStatusATWDb(I3DetStatDefaults::STATUS_ATWDb);
    statusService_->SetStatusFADCInIce(I3DetStatDefaults::STATUS_FADC_INICE);
    statusService_->SetStatusFADCIceTop(I3DetStatDefaults::STATUS_FADC_ICETOP);
    statusService_->SetSPEThreshold(I3DetStatDefaults::SPE_THRESHOLD);
    statusService_->SetFEPedestal(I3DetStatDefaults::FE_PEDESTAL);
    statusService_->SetDACTriggerBias0(I3DetStatDefaults::DAC_TRIGGER_BIAS0);
    statusService_->SetDACTriggerBias1(I3DetStatDefaults::DAC_TRIGGER_BIAS1);
    statusService_->SetDACFADCRef(I3DetStatDefaults::DAC_FADC_REF);
    statusService_->SetNBinsATWD0InIce(I3DetStatDefaults::NBINS_ATWD0_INICE);
    statusService_->SetNBinsATWD1InIce(I3DetStatDefaults::NBINS_ATWD1_INICE);
    statusService_->SetNBinsATWD2InIce(I3DetStatDefaults::NBINS_ATWD2_INICE);
    statusService_->SetNBinsFADCInIce(I3DetStatDefaults::NBINS_FADC_INICE);
    statusService_->SetNBinsATWD0IceTop(I3DetStatDefaults::NBINS_ATWD0_ICETOP);
    statusService_->SetNBinsATWD1IceTop(I3DetStatDefaults::NBINS_ATWD1_ICETOP);
    statusService_->SetNBinsATWD2IceTop(I3DetStatDefaults::NBINS_ATWD2_ICETOP);
    statusService_->SetNBinsFADCIceTop(I3DetStatDefaults::NBINS_FADC_ICETOP);
  }

  if(!calibrationService_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>();
    calibrationService_ = 
      shared_ptr<I3MCCalibrationService>
      (new I3MCCalibrationService(geo_service,old_cal));
    log_debug("Made new I3MCCalibrationService.");

    calibrationService_->SetStartYear(I3CalibDefaults::START_YEAR);
    calibrationService_->SetStartDAQTime(I3CalibDefaults::START_DAQTIME);
    calibrationService_->SetEndYear(I3CalibDefaults::END_YEAR);
    calibrationService_->SetEndDAQTime(I3CalibDefaults::END_DAQTIME);

    calibrationService_->SetTemperature(I3CalibDefaults::TEMPERATURE);
    calibrationService_->SetFADCBaselineSlope(I3CalibDefaults::FADC_BASELINE_FIT_SLOPE);
    calibrationService_->SetFADCBaselineIntercept(I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT);
    calibrationService_->SetFADCGain(I3CalibDefaults::FADC_GAIN);
    calibrationService_->SetATWD0(I3CalibDefaults::ATWD0_GAIN);
    calibrationService_->SetATWD1(I3CalibDefaults::ATWD1_GAIN);
    calibrationService_->SetATWD2(I3CalibDefaults::ATWD2_GAIN);
    calibrationService_->SetATWDaFreqFitA(I3CalibDefaults::ATWD_A_FREQFIT_A);
    calibrationService_->SetATWDaFreqFitB(I3CalibDefaults::ATWD_A_FREQFIT_B);
    calibrationService_->SetATWDaFreqFitC(I3CalibDefaults::ATWD_A_FREQFIT_C);
    calibrationService_->SetATWDbFreqFitA(I3CalibDefaults::ATWD_B_FREQFIT_A);
    calibrationService_->SetATWDbFreqFitB(I3CalibDefaults::ATWD_B_FREQFIT_B);
    calibrationService_->SetATWDbFreqFitC(I3CalibDefaults::ATWD_B_FREQFIT_C);
    calibrationService_->SetHVGainSlope(I3CalibDefaults::HV_GAIN_FIT_SLOPE);
    calibrationService_->SetHVGainIntercept(I3CalibDefaults::HV_GAIN_FIT_INTERCEPT);
    calibrationService_->SetATWDBinCalibSlope(I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
    calibrationService_->SetATWDBinCalibIntercept(I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  }

  bool good_calib(true);
  bool good_status(true);
  if(calServiceName_.size()>0){
    //Only install a new service if the use has specified a name
    good_calib = services.Put<I3CalibrationService>(calServiceName_,calibrationService_);
    log_debug("good_calib %d",good_calib);
  }
  if(statusServiceName_.size()>0){
    good_status = services.Put<I3DetectorStatusService>(statusServiceName_,statusService_);
    log_debug("good_status %d",good_status);
  }

  return (good_calib && good_status);
	  
}

