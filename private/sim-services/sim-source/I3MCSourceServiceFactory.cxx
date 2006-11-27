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

  if(!status_){
    I3DetectorStatusServicePtr old_status = context_.Get<I3DetectorStatusServicePtr>();
    status_ = 
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
					    I3IceTopTriggerDefaults::CONFIG_ID)
    I3TriggerStatus it_trigStatus;
    it_trigStatus.GetTriggerName().append("simple_multiplicity");
    it_trigStatus.GetTriggerSettings().insert(make_pair("threshold", I3IceTopTriggerDefaults::THRESHOLD));
    it_trigStatus.GetTriggerSettings().insert(make_pair("timeWindow", static_cast<int>(I3IceTopTriggerDefaults::TIME_WINDOW)));
    
    status_->InsertTriggerStatus(ic_trigger, ic_trigStatus);
    status_->InsertTriggerStatus(it_trigger, it_trigStatus);

    //Configure the new detector status service

    status_->SetStartYear(I3DetStatDefaults::START_YEAR);
    status_->SetStartDAQTime(I3DetStatDefaults::START_DAQTIME);
    status_->SetEndYear(I3DetStatDefaults::END_YEAR);
    status_->SetEndDAQTime(I3DetStatDefaults::END_DAQTIME);
    
    status_->SetIceTopLCWindowPre(I3DetStatDefaults::ICETOP_LCWINDOW_PRE);
    status_->SetIceTopLCWindowPost(I3DetStatDefaults::ICETOP_LCWINDOW_POST);
    status_->SetIceTopHighGainVoltage(I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE);
    status_->SetIceTopLowGainVoltage(I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE);
    status_->SetInIceLCWindowPre(I3DetStatDefaults::INICE_LCWINDOW_PRE);
    status_->SetInIceLCWindowPost(I3DetStatDefaults::INICE_LCWINDOW_POST);
    status_->SetLCSpan(I3DetStatDefaults::LCSPAN);
    status_->SetInIceVoltage(I3DetStatDefaults::INICE_VOLTAGE);
    status_->SetTriggerMode(I3DetStatDefaults::TRIGGER_MODE);
    status_->SetLCMode(I3DetStatDefaults::LCMODE);
    status_->SetStatusATWDa(I3DetStatDefaults::STATUS_ATWDa);
    status_->SetStatusATWDb(I3DetStatDefaults::STATUS_ATWDb);
    status_->SetStatusFADCInIce(I3DetStatDefaults::STATUS_FADC_INICE);
    status_->SetStatusFADCIceTop(I3DetStatDefaults::STATUS_FADC_ICETOP);
    status_->SetSPEThreshold(I3DetStatDefaults::SPE_THRESHOLD);
    status_->SetFEPedestal(I3DetStatDefaults::FE_PEDESTAL);
    status_->SetDACTriggerBias0(I3DetStatDefaults::DAC_TRIGGER_BIAS0);
    status_->SetDACTriggerBias1(I3DetStatDefaults::DAC_TRIGGER_BIAS1);
    status_->SetDACFADCRef(I3DetStatDefaults::DAC_FADC_REF);
    status_->SetNBinsATWD0InIce(I3DetStatDefaults::NBINS_ATWD0_INICE);
    status_->SetNBinsATWD1InIce(I3DetStatDefaults::NBINS_ATWD1_INICE);
    status_->SetNBinsATWD2InIce(I3DetStatDefaults::NBINS_ATWD2_INICE);
    status_->SetNBinsFADCInIce(I3DetStatDefaults::NBINS_FADC_INICE);
    status_->SetNBinsATWD0IceTop(I3DetStatDefaults::NBINS_ATWD0_ICETOP);
    status_->SetNBinsATWD1IceTop(I3DetStatDefaults::NBINS_ATWD1_ICETOP);
    status_->SetNBinsATWD2IceTop(I3DetStatDefaults::NBINS_ATWD2_ICETOP);
    status_->SetNBinsFADCIceTop(I3DetStatDefaults::NBINS_FADC_ICETOP);
  }

  if(!calibration_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>();
    calibration_ = 
      shared_ptr<I3MCCalibrationService>
      (new I3MCCalibrationService(geo_service,old_cal));
    log_debug("Made new I3MCCalibrationService.");

    calibration_->SetStartYear(I3CalibDefaults::START_YEAR);
    calibration_->SetStartDAQTime(I3CalibDefaults::START_DAQTIME);
    calibration_->SetEndYear(I3CalibDefaults::END_YEAR);
    calibration_->SetEndDAQTime(I3CalibDefaults::END_DAQTIME);

    calibration_->SetTemperature(I3CalibDefaults::TEMPERATURE);
    calibration_->SetFADCBaselineSlope(I3CalibDefaults::FADC_BASELINE_FIT_SLOPE);
    calibration_->SetFADCBaselineIntercept(I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT);
    calibration_->SetFADCGain(I3CalibDefaults::FADC_GAIN);
    calibration_->SetATWD0(I3CalibDefaults::ATWD0_GAIN);
    calibration_->SetATWD1(I3CalibDefaults::ATWD1_GAIN);
    calibration_->SetATWD2(I3CalibDefaults::ATWD2_GAIN);
    calibration_->SetATWDaFreqFitA(I3CalibDefaults::ATWD_A_FREQFIT_A);
    calibration_->SetATWDaFreqFitB(I3CalibDefaults::ATWD_A_FREQFIT_B);
    calibration_->SetATWDaFreqFitC(I3CalibDefaults::ATWD_A_FREQFIT_C);
    calibration_->SetATWDbFreqFitA(I3CalibDefaults::ATWD_B_FREQFIT_A);
    calibration_->SetATWDbFreqFitB(I3CalibDefaults::ATWD_B_FREQFIT_B);
    calibration_->SetATWDbFreqFitC(I3CalibDefaults::ATWD_B_FREQFIT_C);
    calibration_->SetHVGainSlope(I3CalibDefaults::HV_GAIN_FIT_SLOPE);
    calibration_->SetHVGainIntercept(I3CalibDefaults::HV_GAIN_FIT_INTERCEPT);
    calibration_->SetATWDBinCalibSlope(I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
    calibration_->SetATWDBinCalibIntercept(I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  }

  bool good_calib(true);
  bool good_status(true);
  if(calServiceName_.size()>0){
    //Only install a new service if the use has specified a name
    good_calib = services.Put<I3CalibrationService>(calServiceName_,calibration_);
    log_debug("good_calib %d",good_calib);
  }
  if(statusServiceName_.size()>0){
    good_status = services.Put<I3DetectorStatusService>(statusServiceName_,status_);
    log_debug("good_status %d",good_status);
  }

  return (good_calib && good_status);
	  
}

