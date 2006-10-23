/**
 * class: I3SimSourceTestModule.cxx
 * (c) 2004 IceCube Collaboration
 * Version $Id: I3SimSourceTestModule.cxx,v 1.16 2005/03/17 18:33:43 olivas Exp $
 *
 * Date 08 June 2006
 * @version $Revision: 1.1 $
 * @date $Date: 2005/03/17 18:33:43 $
 * @author Alex Olivas <olivas@icecube.umd.edu>
 *
 */

#include <I3Test.h>
#include "sim-services/sim-source/I3SimSourceTestModule.h"

#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

#include <phys-services/I3CalibrationService.h>
#include <phys-services/I3DetectorStatusService.h>


I3_MODULE(I3SimSourceTestModule);

I3SimSourceTestModule::I3SimSourceTestModule(const I3Context& ctx) : 
  I3Module(ctx)
{ 
  log_debug("Constructor I3SimSourceTestModule");  

  AddParameter("Threshold","InIce Multiplicity Trigger Threshold",ic_threshold_);
  AddParameter("TimeWindow","InIce Multiplicity Trigger Time Window",ic_timeWindow_);
  AddParameter("ConfigID","InIce Trigger Status Config ID",ic_configID_);
  AddParameter("IceTop_Threshold","IceTop Multiplicity Trigger Threshold",it_threshold_);
  AddParameter("IceTop_TimeWindow","IceTop Multiplicity Trigger Time Window",it_timeWindow_);
  AddParameter("IceTop_ConfigID","IceTop Trigger Status Config ID",it_configID_);

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

  AddOutBox("OutBox");
}

I3SimSourceTestModule::~I3SimSourceTestModule(){}

void I3SimSourceTestModule::Configure()
{
  log_debug("Configuring I3SimSourceTestModule");

  GetParameter("Threshold",ic_threshold_);
  GetParameter("TimeWindow",ic_timeWindow_);
  GetParameter("ConfigID",ic_configID_);
  GetParameter("IceTop_Threshold",it_threshold_);
  GetParameter("IceTop_TimeWindow",it_timeWindow_);
  GetParameter("IceTop_ConfigID",it_configID_);

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
}

void I3SimSourceTestModule::Physics(I3FramePtr frame)
{
  log_debug("Physics");

  I3CalibrationConstPtr calib = 
    frame->Get<I3CalibrationConstPtr>();
  
  map<OMKey, I3DOMCalibration>::const_iterator cal_iter;
  for(cal_iter = calib->domCal.begin();
      cal_iter != calib->domCal.end(); 
      cal_iter++){

    ENSURE(cal_iter->first.GetString()>0,"There should be no AMANDA OMs.");

    ENSURE(cal_iter->second.GetTemperature() == cal_temperature_);

    ENSURE(cal_iter->second.GetFADCBaselineFit().slope == cal_fadcBaselineFit_slope_);
    ENSURE(cal_iter->second.GetFADCBaselineFit().intercept == cal_fadcBaselineFit_intercept_);
    ENSURE(cal_iter->second.GetFADCGain() == cal_fadcGain_);
    ENSURE(cal_iter->second.GetATWDGain(0) == cal_atwd0Gain_);
    ENSURE(cal_iter->second.GetATWDGain(1) == cal_atwd1Gain_);
    ENSURE(cal_iter->second.GetATWDGain(2) == cal_atwd2Gain_);

    ENSURE(cal_iter->second.GetATWDFreqFit(0).quadFitA == cal_atwd_a_FreqFit_A_);
    ENSURE(cal_iter->second.GetATWDFreqFit(0).quadFitB == cal_atwd_a_FreqFit_B_);
    ENSURE(cal_iter->second.GetATWDFreqFit(0).quadFitC == cal_atwd_a_FreqFit_C_);

    ENSURE(cal_iter->second.GetATWDFreqFit(1).quadFitA == cal_atwd_b_FreqFit_A_);
    ENSURE(cal_iter->second.GetATWDFreqFit(1).quadFitB == cal_atwd_b_FreqFit_B_);
    ENSURE(cal_iter->second.GetATWDFreqFit(1).quadFitC == cal_atwd_b_FreqFit_C_);

    ENSURE(cal_iter->second.GetHVGainFit().slope == cal_hvGainFit_slope_);
    ENSURE(cal_iter->second.GetHVGainFit().intercept == cal_hvGainFit_intercept_);

    for( unsigned int bin = 0; bin < 128; ++bin ){
      ENSURE(cal_iter->second.GetATWDBinCalibFit(0,0,bin).slope == cal_atwdBinCalibFit_slope_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(0,1,bin).slope == cal_atwdBinCalibFit_slope_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(0,2,bin).slope == cal_atwdBinCalibFit_slope_);

      ENSURE(cal_iter->second.GetATWDBinCalibFit(1,0,bin).slope == cal_atwdBinCalibFit_slope_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(1,1,bin).slope == cal_atwdBinCalibFit_slope_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(1,2,bin).slope == cal_atwdBinCalibFit_slope_);

      ENSURE(cal_iter->second.GetATWDBinCalibFit(0,0,bin).intercept == cal_atwdBinCalibFit_intercept_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(0,1,bin).intercept == cal_atwdBinCalibFit_intercept_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(0,2,bin).intercept == cal_atwdBinCalibFit_intercept_);

      ENSURE(cal_iter->second.GetATWDBinCalibFit(1,0,bin).intercept == cal_atwdBinCalibFit_intercept_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(1,1,bin).intercept == cal_atwdBinCalibFit_intercept_);
      ENSURE(cal_iter->second.GetATWDBinCalibFit(1,2,bin).intercept == cal_atwdBinCalibFit_intercept_);
    }
  }


  I3DetectorStatusConstPtr status = 
    frame->Get<I3DetectorStatusConstPtr>();

  map<OMKey, I3DOMStatus>::const_iterator stat_iter;
  for(stat_iter = status->domStatus.begin();
      stat_iter != status->domStatus.end(); 
      stat_iter++){

    ENSURE(stat_iter->first.GetString()>0,"There should be no AMANDA OMs.");

      if ( stat_iter->first.GetOM() > 60 ){
	ENSURE(stat_iter->second.lcWindowPre == ds_icetopLCWindowPre_);
	ENSURE(stat_iter->second.lcWindowPost == ds_icetopLCWindowPost_);
	
	if ( stat_iter->first.GetOM() == 61 || stat_iter->first.GetOM() == 63 ){	
	      ENSURE(stat_iter->second.pmtHV == ds_icetopHighGainVoltage_);
	}else if ( stat_iter->first.GetOM() == 62 || stat_iter->first.GetOM() == 64 ){
	  ENSURE(stat_iter->second.pmtHV == ds_icetopLowGainVoltage_);
	}
      }	else {
	ENSURE(static_cast<int>(stat_iter->second.lcSpan) == ds_lcSpan_);
	ENSURE(stat_iter->second.lcWindowPre == ds_iniceLCWindowPre_);
	ENSURE(stat_iter->second.lcWindowPost == ds_iniceLCWindowPost_);
	ENSURE(stat_iter->second.pmtHV == ds_iniceVoltage_);
      }

    ENSURE(static_cast<int>(stat_iter->second.trigMode) == ds_triggerMode_);
    ENSURE(static_cast<int>(stat_iter->second.lcMode) == ds_lcMode_);
    ENSURE(static_cast<int>(stat_iter->second.statusATWDa) == ds_statusATWDa_);
    ENSURE(static_cast<int>(stat_iter->second.statusATWDb) == ds_statusATWDb_);
    ENSURE(static_cast<int>(stat_iter->second.statusFADC) == ds_statusFADC_);
    ENSURE(stat_iter->second.speThreshold == ds_speThreshold_);
    ENSURE(stat_iter->second.fePedestal == ds_fePedestal_);
    ENSURE(stat_iter->second.dacTriggerBias0 == ds_dacTriggerBias0_);
    ENSURE(stat_iter->second.dacTriggerBias1 == ds_dacTriggerBias1_);
    ENSURE(stat_iter->second.dacFADCRef == ds_dacFADCRef_);
    ENSURE(stat_iter->second.nBinsATWD0 == ds_nBinsATWD0_);
    ENSURE(stat_iter->second.nBinsATWD1 == ds_nBinsATWD1_);
    ENSURE(stat_iter->second.nBinsATWD2 == ds_nBinsATWD2_);
    ENSURE(stat_iter->second.nBinsFADC == ds_nBinsFADC_);
  }

  PushFrame(frame,"OutBox");
}//Physics()
 
