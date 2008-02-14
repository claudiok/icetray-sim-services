#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3MCDetectorStatusService.h"
#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"
#include "sim-services/sim-source/default-values/I3InIceTriggerDefaults.h"
#include "sim-services/sim-source/default-values/I3IceTopTriggerDefaults.h"
#include "sim-services/sim-source/default-values/I3TWRDefaults.h"

I3_SERVICE_FACTORY(I3MCSourceServiceFactory);

I3MCSourceServiceFactory::
I3MCSourceServiceFactory(const I3Context& context) : 
  I3ServiceFactory(context),
  installCalibration_(true),
  installDetectorStatus_(true),
  installInIceTriggers_(true),
  installIceTopTriggers_(true),
  installTWRTriggers_(true)
{
  oldCalServiceName_ = I3DefaultName<I3CalibrationService>::value();
  oldStatusServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  calServiceName_ = I3DefaultName<I3CalibrationService>::value();
  statusServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  geoServiceName_ = I3DefaultName<I3GeometryService>::value();

  AddParameter("CalServiceName","Name of calibration service to install",calServiceName_);
  AddParameter("StatusServiceName","Name of detector status service to install",statusServiceName_);
  AddParameter("OldCalServiceName","Name of calibration service to retrieve",oldCalServiceName_);
  AddParameter("OldStatusServiceName","Name of detector status service to retrieve",oldStatusServiceName_);
  AddParameter("GeoServiceName","Name of geometry service",geoServiceName_);
  AddParameter("InstallCalibration","Install Calibration",installCalibration_);
  AddParameter("InstallDetectorStatus","Install DetectorStatus",installDetectorStatus_);
  AddParameter("InstallInIceTriggers","Install InIce Triggers",installInIceTriggers_);
  AddParameter("InstallIceTopTriggers","Install IceTop Triggers",installIceTopTriggers_);
  AddParameter("InstallTWRTriggers","Install TWR Triggers",installTWRTriggers_);
  AddParameter("DoNotModifyStrings","Do not modify these strings",skipStrings_);
  AddParameter("DoNotModifyStations","Do not modify these stations",skipStations_);
}

I3MCSourceServiceFactory::
~I3MCSourceServiceFactory(){}

void I3MCSourceServiceFactory::Configure()
{
  GetParameter("OldCalServiceName",oldCalServiceName_);
  GetParameter("OldStatusServiceName",oldStatusServiceName_);
  GetParameter("CalServiceName",calServiceName_);
  GetParameter("StatusServiceName",statusServiceName_);
  GetParameter("GeoServiceName",geoServiceName_);
  GetParameter("InstallCalibration",installCalibration_);
  GetParameter("InstallDetectorStatus",installDetectorStatus_);
  GetParameter("InstallInIceTriggers",installInIceTriggers_);
  GetParameter("InstallIceTopTriggers",installIceTopTriggers_);
  GetParameter("InstallTWRTriggers",installTWRTriggers_);
  GetParameter("DoNotModifyStrings",skipStrings_);
  GetParameter("DoNotModifyStations",skipStations_);
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

    log_trace("number of triggers before insertion = %zu",statusService_->GetTriggerStatusSize());
    //Fill it with triggers
    FillTriggers(statusService_);
    log_trace("number of triggers after insertion = %zu",statusService_->GetTriggerStatusSize());

    //Configure with default parameters
    Configure(statusService_);    

    statusService_->SetSkipStrings(skipStrings_);
    statusService_->SetSkipStations(skipStations_);

    //Pass the TWR Params map to the detector status
    I3MCTWRParamsMapPtr twrParams = context_.Get<I3MCTWRParamsMapPtr>("I3MCTWRParamsMap");
    if(twrParams){
      statusService_->SetMCTWRParamsMap(twrParams);
    }else{
      log_debug("Couldn't find TWR Params Map.  Assuming IceCube only mode.");
    }
  }

  if(!calibrationService_){
    I3CalibrationServicePtr old_cal = context_.Get<I3CalibrationServicePtr>(oldCalServiceName_);
    calibrationService_ = 
      shared_ptr<I3MCCalibrationService>
      (new I3MCCalibrationService(geo_service,old_cal));
    log_debug("Made new I3MCCalibrationService.");

    calibrationService_->SetSkipStrings(skipStrings_);
    calibrationService_->SetSkipStations(skipStations_);
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

void I3MCSourceServiceFactory::FillTriggers(I3MCDetectorStatusServicePtr s){

  if(installInIceTriggers_){
    I3Trigger t_smt;
    t_smt.GetTriggerKey() = TriggerKey(I3InIceTriggerDefaults::SOURCE_ID, 
				       I3InIceTriggerDefaults::TYPE_ID, 
				       I3InIceTriggerDefaults::CONFIG_ID);

    I3TriggerStatus ts_smt;
    ts_smt.GetTriggerName().append("simple_multiplicity");
    ts_smt.GetTriggerSettings().insert(make_pair("threshold", 
						 I3InIceTriggerDefaults::THRESHOLD));
    ts_smt.GetTriggerSettings().insert(make_pair("timeWindow", 
						 static_cast<int>(I3InIceTriggerDefaults::TIME_WINDOW)));

    I3Trigger t_str;
    t_str.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, 
				       TriggerKey::STRING, 
				       666);

    I3TriggerStatus ts_str;
    ts_str.GetTriggerName().append("simple_multiplicity");
    ts_str.GetTriggerSettings().insert(make_pair("threshold", 5));
    ts_str.GetTriggerSettings().insert(make_pair("triggerWindow", 1000));
    ts_str.GetTriggerSettings().insert(make_pair("max_span", 7));
    ts_str.GetTriggerSettings().insert(make_pair("veto_depth", 3));

    s->InsertTriggerStatus(t_smt, ts_smt);
    s->InsertTriggerStatus(t_str, ts_str);

  }    

  if(installIceTopTriggers_){
    I3Trigger t;
    t.GetTriggerKey() = TriggerKey(I3IceTopTriggerDefaults::SOURCE_ID,
				   I3IceTopTriggerDefaults::TYPE_ID,
				   I3IceTopTriggerDefaults::CONFIG_ID);
    I3TriggerStatus ts;
    ts.GetTriggerName().append("simple_multiplicity");
    ts.GetTriggerSettings().insert(make_pair("threshold", 
					     I3IceTopTriggerDefaults::THRESHOLD));
    ts.GetTriggerSettings().insert(make_pair("timeWindow", 
					     static_cast<int>(I3IceTopTriggerDefaults::TIME_WINDOW)));
    s->InsertTriggerStatus(t, ts);
  }

  if(installTWRTriggers_){

    /**
     *Simple Multiplicity Trigger
     */
    I3Trigger smt_trig;
    smt_trig.GetTriggerKey() = TriggerKey(TriggerKey::AMANDA_TWR_DAQ, TriggerKey::SIMPLE_MULTIPLICITY);

    I3TriggerStatus smt_stat;
    smt_stat.GetTriggerName().append("multiplicity");
    smt_stat.GetTriggerSettings().insert(make_pair("dmadd_pretrig_mult",
						   I3TWRDefaults::DMADD_PRETRIG_MULT));
    smt_stat.GetTriggerSettings().insert(make_pair("dmadd_mult",
						  I3TWRDefaults::DMADD_MULT ));
    smt_stat.GetTriggerSettings().insert(make_pair("dmadd_window",
						   I3TWRDefaults::DMADD_WINDOW));
    smt_stat.GetTriggerSettings().insert(make_pair("swamp_op_maxamp",
						   I3TWRDefaults::SWAMP_OP_MAXAMP));
    smt_stat.GetTriggerSettings().insert(make_pair("swamp_el_maxamp",
						   I3TWRDefaults::SWAMP_EL_MAXAMP));
    smt_stat.GetTriggerSettings().insert(make_pair("twr_op_bins_before",
						   I3TWRDefaults::TWR_OP_BINS_BEFORE));
    smt_stat.GetTriggerSettings().insert(make_pair("twr_op_bins_after",
						   I3TWRDefaults::TWR_OP_BINS_AFTER));
    smt_stat.GetTriggerSettings().insert(make_pair("twr_op_thresh_end",
						   I3TWRDefaults::TWR_OP_THRESH_END));
    smt_stat.GetTriggerSettings().insert(make_pair("twr_el_bins_before",
						   I3TWRDefaults::TWR_EL_BINS_BEFORE));
    smt_stat.GetTriggerSettings().insert(make_pair("twr_el_bins_after",
						   I3TWRDefaults::TWR_EL_BINS_AFTER));
    smt_stat.GetTriggerSettings().insert(make_pair("twr_el_thresh_end_frac",
						   I3TWRDefaults::TWR_EL_THRESH_END_FRAC));
    s->InsertTriggerStatus(smt_trig,smt_stat );

    /**
     *String Trigger
     */
    I3Trigger str_trig;
    str_trig.GetTriggerKey() = TriggerKey(TriggerKey::AMANDA_TWR_DAQ, TriggerKey::STRING);

    I3TriggerStatus str_stat;
    str_stat.GetTriggerName().append("string");
    str_stat.GetTriggerSettings().insert(make_pair("string_1_4_nmod",
						   I3TWRDefaults::STRING_1_4_NMOD));
    str_stat.GetTriggerSettings().insert(make_pair("string_1_4_mult",
						   I3TWRDefaults::STRING_1_4_MULT));
    str_stat.GetTriggerSettings().insert(make_pair("string_5_19_nmod",
						   I3TWRDefaults::STRING_5_19_NMOD));
    str_stat.GetTriggerSettings().insert(make_pair("string_5_19_mult",
						   I3TWRDefaults::STRING_5_19_MULT));
    s->InsertTriggerStatus(str_trig,str_stat );
    
    /**
     *Fragment Multiplicity Trigger
     */
    I3Trigger frag_trig;
    frag_trig.GetTriggerKey() = TriggerKey(TriggerKey::AMANDA_TWR_DAQ, TriggerKey::FRAGMENT_MULTIPLICITY);

    I3TriggerStatus frag_stat;
    frag_stat.GetTriggerName().append("fragment");
    frag_stat.GetTriggerSettings().insert(make_pair("twr_soft_fragmult",
						    I3TWRDefaults::TWR_SOFT_FRAGMULT));
    frag_stat.GetTriggerSettings().insert(make_pair("twr_soft_window",
						    I3TWRDefaults::TWR_SOFT_WINDOW));
    s->InsertTriggerStatus(frag_trig, frag_stat);

    /**
     *Volume Trigger
     */    
    I3Trigger vol_trig;
    vol_trig.GetTriggerKey() = TriggerKey(TriggerKey::AMANDA_TWR_DAQ, TriggerKey::VOLUME);

    I3TriggerStatus vol_stat;
    vol_stat.GetTriggerName().append("volume");
    vol_stat.GetTriggerSettings().insert(make_pair("twr_soft_n1hit",
						   I3TWRDefaults::TWR_SOFT_N1HIT));
    vol_stat.GetTriggerSettings().insert(make_pair("twr_soft_npairs",
						   I3TWRDefaults::TWR_SOFT_NPAIRS));
    vol_stat.GetTriggerSettings().insert(make_pair("twr_soft_window",
						   I3TWRDefaults::TWR_SOFT_WINDOW));
    vol_stat.GetTriggerSettings().insert(make_pair("twr_soft_vol_startmult",
						   I3TWRDefaults::TWR_SOFT_VOL_STARTMULT));
    vol_stat.GetTriggerSettings().insert(make_pair("twr_soft_vol_stopmult",
						   I3TWRDefaults::TWR_SOFT_VOL_STOPMULT));
    s->InsertTriggerStatus(vol_trig, vol_stat);

  }
}

void I3MCSourceServiceFactory::Configure(I3MCDetectorStatusServicePtr s){
  s->SetStartYear(I3DetStatDefaults::START_YEAR);
  s->SetStartDAQTime(I3DetStatDefaults::START_DAQTIME);
  s->SetEndYear(I3DetStatDefaults::END_YEAR);
  s->SetEndDAQTime(I3DetStatDefaults::END_DAQTIME);
  
  s->SetIceTopLCWindowPre(I3DetStatDefaults::ICETOP_LCWINDOW_PRE);
  s->SetIceTopLCWindowPost(I3DetStatDefaults::ICETOP_LCWINDOW_POST);
  s->SetIceTopHighGainVoltage(I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE);
  s->SetIceTopLowGainVoltage(I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE);
  s->SetInIceLCWindowPre(I3DetStatDefaults::INICE_LCWINDOW_PRE);
  s->SetInIceLCWindowPost(I3DetStatDefaults::INICE_LCWINDOW_POST);
  s->SetInIceLCSpan(I3DetStatDefaults::INICE_LCSPAN);
  s->SetIceTopLCSpan(I3DetStatDefaults::ICETOP_LCSPAN);
  s->SetInIceVoltage(I3DetStatDefaults::INICE_VOLTAGE);
  s->SetTriggerMode(I3DetStatDefaults::TRIGGER_MODE);

  s->SetLCModeInIceFirstDOM(I3DetStatDefaults::LCMODE_INICE_FIRST);
  s->SetLCModeInIceBulkDOMs(I3DetStatDefaults::LCMODE_INICE_BULK);
  s->SetLCModeInIceLastDOM(I3DetStatDefaults::LCMODE_INICE_LAST);
  s->SetLCModeIceTopDOMs(I3DetStatDefaults::LCMODE_ICETOP);

  s->SetStatusATWDa(I3DetStatDefaults::STATUS_ATWDa);
  s->SetStatusATWDb(I3DetStatDefaults::STATUS_ATWDb);
  s->SetStatusFADCInIce(I3DetStatDefaults::STATUS_FADC_INICE);
  s->SetStatusFADCIceTop(I3DetStatDefaults::STATUS_FADC_ICETOP);
  s->SetSPEThreshold(I3DetStatDefaults::SPE_THRESHOLD);
  s->SetFEPedestal(I3DetStatDefaults::FE_PEDESTAL);
  s->SetDACTriggerBias0(I3DetStatDefaults::DAC_TRIGGER_BIAS0);
  s->SetDACTriggerBias1(I3DetStatDefaults::DAC_TRIGGER_BIAS1);
  s->SetDACFADCRef(I3DetStatDefaults::DAC_FADC_REF);
  s->SetNBinsATWD0InIce(I3DetStatDefaults::NBINS_ATWD0_INICE);
  s->SetNBinsATWD1InIce(I3DetStatDefaults::NBINS_ATWD1_INICE);
  s->SetNBinsATWD2InIce(I3DetStatDefaults::NBINS_ATWD2_INICE);
  s->SetNBinsFADCInIce(I3DetStatDefaults::NBINS_FADC_INICE);
  s->SetNBinsATWD0IceTop(I3DetStatDefaults::NBINS_ATWD0_ICETOP);
  s->SetNBinsATWD1IceTop(I3DetStatDefaults::NBINS_ATWD1_ICETOP);
  s->SetNBinsATWD2IceTop(I3DetStatDefaults::NBINS_ATWD2_ICETOP);
  s->SetNBinsFADCIceTop(I3DetStatDefaults::NBINS_FADC_ICETOP);
  s->SetDeltaCompression(I3DetStatDefaults::DELTA_COMPRESSION);
}

void I3MCSourceServiceFactory::Configure(I3MCCalibrationServicePtr c){
  c->SetStartYear(I3CalibDefaults::START_YEAR);
  c->SetStartDAQTime(I3CalibDefaults::START_DAQTIME);
  c->SetEndYear(I3CalibDefaults::END_YEAR);
  c->SetEndDAQTime(I3CalibDefaults::END_DAQTIME);
  
  c->SetTemperature(I3CalibDefaults::TEMPERATURE);
  c->SetFADCBaselineSlope(I3CalibDefaults::FADC_BASELINE_FIT_SLOPE);
  c->SetFADCBaselineIntercept(I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT);
  c->SetFADCGain(I3CalibDefaults::FADC_GAIN);
  c->SetATWD0(I3CalibDefaults::ATWD0_GAIN);
  c->SetATWD1(I3CalibDefaults::ATWD1_GAIN);
  c->SetATWD2(I3CalibDefaults::ATWD2_GAIN);
  c->SetATWDaFreqFitA(I3CalibDefaults::ATWD_A_FREQFIT_A);
  c->SetATWDaFreqFitB(I3CalibDefaults::ATWD_A_FREQFIT_B);
  c->SetATWDaFreqFitC(I3CalibDefaults::ATWD_A_FREQFIT_C);
  c->SetATWDbFreqFitA(I3CalibDefaults::ATWD_B_FREQFIT_A);
  c->SetATWDbFreqFitB(I3CalibDefaults::ATWD_B_FREQFIT_B);
  c->SetATWDbFreqFitC(I3CalibDefaults::ATWD_B_FREQFIT_C);
  c->SetHVGainSlope(I3CalibDefaults::HV_GAIN_FIT_SLOPE);
  c->SetHVGainIntercept(I3CalibDefaults::HV_GAIN_FIT_INTERCEPT);
  c->SetATWDBinCalibSlope(I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  c->SetATWDBinCalibIntercept(I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  c->SetATWDResponseWidth(I3CalibDefaults::ATWD_RESPONSE_WIDTH);
}
