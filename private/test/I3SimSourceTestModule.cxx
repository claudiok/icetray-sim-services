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
#include "I3SimSourceTestModule.h"

#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

#include <interfaces/I3CalibrationService.h>
#include <interfaces/I3DetectorStatusService.h>
#include <phys-services/geo-selector/GeoSelUtils.h>

#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"

I3_MODULE(I3SimSourceTestModule);

I3SimSourceTestModule::I3SimSourceTestModule(const I3Context& ctx) : 
  I3Module(ctx),
  stringsToUse_("1:80"),
  stationsToUse_("1:80")
{ 
  log_debug("Constructor I3SimSourceTestModule");  

  AddParameter("StringsToUse","List of string in original geometry",stringsToUse_);
  AddParameter("StationsToUse","List of stations in original geometry",stationsToUse_);

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
  AddParameter("DetStat_InIceLCSpan","",ds_iniceLCSpan_);
  AddParameter("DetStat_IceTopLCSpan","",ds_icetopLCSpan_);
  AddParameter("DetStat_InIceVoltage","",ds_iniceVoltage_);
  AddParameter("DetStat_TriggerMode","",ds_triggerMode_);
  AddParameter("DetStat_LCModeInIceFirstDOM","",ds_lcMode_inice_first_);
  AddParameter("DetStat_LCModeInIceBulkDOMs","",ds_lcMode_inice_bulk_);
  AddParameter("DetStat_LCModeInIceLastDOM","",ds_lcMode_inice_last_);
  AddParameter("DetStat_LCModeIceTopDOMs","",ds_lcMode_icetop_);
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
  AddParameter("DetStat_NBinsFADCInIce","Number of FADC bins",ds_nBinsFADC_InIce_);
  AddParameter("DetStat_NBinsATWD0IceTop","",ds_nBinsATWD0_IceTop_);
  AddParameter("DetStat_NBinsATWD1IceTop","",ds_nBinsATWD1_IceTop_);
  AddParameter("DetStat_NBinsATWD2IceTop","",ds_nBinsATWD2_IceTop_);
  AddParameter("DetStat_NBinsFADCIceTop","Number of FADC bins",ds_nBinsFADC_IceTop_);

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
  AddParameter("Calib_ATWDResponseWidth","",cal_atwd_response_width_);

  AddOutBox("OutBox");
}

I3SimSourceTestModule::~I3SimSourceTestModule(){}

void I3SimSourceTestModule::Configure()
{
  log_debug("Configuring I3SimSourceTestModule");

  GetParameter("StringsToUse",stringsToUse_);
  GetParameter("StationsToUse",stationsToUse_);

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
  GetParameter("DetStat_InIceLCSpan",ds_iniceLCSpan_);
  GetParameter("DetStat_IceTopLCSpan",ds_icetopLCSpan_);
  GetParameter("DetStat_InIceVoltage",ds_iniceVoltage_);
  GetParameter("DetStat_TriggerMode",ds_triggerMode_);
  GetParameter("DetStat_LCModeInIceFirstDOM",ds_lcMode_inice_first_);
  GetParameter("DetStat_LCModeInIceBulkDOMs",ds_lcMode_inice_bulk_);
  GetParameter("DetStat_LCModeInIceLastDOM",ds_lcMode_inice_last_);
  GetParameter("DetStat_LCModeIceTopDOMs",ds_lcMode_icetop_);
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
  GetParameter("Calib_ATWDResponseWidth",cal_atwd_response_width_);
}

void I3SimSourceTestModule::Physics(I3FramePtr frame)
{
  log_debug("Physics");

  vector<int> goodStrings = geo_sel_utils::make_good_strings(stringsToUse_, "");
  vector<int> goodStations = geo_sel_utils::make_good_strings(stationsToUse_, "");

  const double DISTANCE = 0.000001;

  I3CalibrationConstPtr calib = 
    frame->Get<I3CalibrationConstPtr>();
  
  cerr<<endl;

  vector<int>::iterator i = goodStrings.begin();
  cerr<<"goodStrings = ";
  for( ; i != goodStrings.end(); i++) cerr<<*i<<" ";
  cerr<<endl;

  i = goodStations.begin();
  cerr<<"goodStations = ";
  for( ; i != goodStations.end(); i++) cerr<<*i<<" ";
  cerr<<endl;
  map<OMKey, I3DOMCalibration>::const_iterator cal_iter;
  for(cal_iter = calib->domCal.begin();
      cal_iter != calib->domCal.end(); 
      cal_iter++){

    if(!((geo_sel_utils::exists(cal_iter->first.GetString(),goodStrings) &&
	  (cal_iter->first.GetOM() <= 60)) ||
	 (geo_sel_utils::exists(cal_iter->first.GetString(),goodStations) &&
	  (cal_iter->first.GetOM() > 60))))
      continue;

    ENSURE(cal_iter->first.GetString()>0,"There should be no AMANDA OMs.");

    ENSURE_DISTANCE(cal_iter->second.GetTemperature(), cal_temperature_, DISTANCE);

    ENSURE_DISTANCE(cal_iter->second.GetFADCBaselineFit().slope, cal_fadcBaselineFit_slope_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetFADCBaselineFit().intercept, cal_fadcBaselineFit_intercept_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetFADCGain(), cal_fadcGain_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDGain(0), cal_atwd0Gain_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDGain(1), cal_atwd1Gain_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDGain(2), cal_atwd2Gain_, DISTANCE);

    ENSURE_DISTANCE(cal_iter->second.GetATWDFreqFit(0).quadFitA, cal_atwd_a_FreqFit_A_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDFreqFit(0).quadFitB, cal_atwd_a_FreqFit_B_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDFreqFit(0).quadFitC, cal_atwd_a_FreqFit_C_, DISTANCE);

    ENSURE_DISTANCE(cal_iter->second.GetATWDFreqFit(1).quadFitA, cal_atwd_b_FreqFit_A_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDFreqFit(1).quadFitB, cal_atwd_b_FreqFit_B_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetATWDFreqFit(1).quadFitC, cal_atwd_b_FreqFit_C_, DISTANCE);

    ENSURE_DISTANCE(cal_iter->second.GetHVGainFit().slope, cal_hvGainFit_slope_, DISTANCE);
    ENSURE_DISTANCE(cal_iter->second.GetHVGainFit().intercept, cal_hvGainFit_intercept_, DISTANCE);

    ENSURE_DISTANCE(cal_iter->second.GetATWDResponseWidth(), cal_atwd_response_width_, DISTANCE);

    for( unsigned int bin = 0; bin < 128; ++bin ){
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(0,0,bin).slope, cal_atwdBinCalibFit_slope_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(0,1,bin).slope, cal_atwdBinCalibFit_slope_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(0,2,bin).slope, cal_atwdBinCalibFit_slope_, DISTANCE);

      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(1,0,bin).slope, cal_atwdBinCalibFit_slope_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(1,1,bin).slope, cal_atwdBinCalibFit_slope_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(1,2,bin).slope, cal_atwdBinCalibFit_slope_, DISTANCE);

      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(0,0,bin).intercept, cal_atwdBinCalibFit_intercept_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(0,1,bin).intercept, cal_atwdBinCalibFit_intercept_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(0,2,bin).intercept, cal_atwdBinCalibFit_intercept_, DISTANCE);

      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(1,0,bin).intercept, cal_atwdBinCalibFit_intercept_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(1,1,bin).intercept, cal_atwdBinCalibFit_intercept_, DISTANCE);
      ENSURE_DISTANCE(cal_iter->second.GetATWDBinCalibFit(1,2,bin).intercept, cal_atwdBinCalibFit_intercept_, DISTANCE);
    }
  }


  I3DetectorStatusConstPtr status = 
    frame->Get<I3DetectorStatusConstPtr>();

  map<OMKey, I3DOMStatus>::const_iterator stat_iter;
  for(stat_iter = status->domStatus.begin();
      stat_iter != status->domStatus.end(); 
      stat_iter++){

    if(!((geo_sel_utils::exists(stat_iter->first.GetString(),goodStrings) &&
	  (stat_iter->first.GetOM() <= 60)) ||
	 (geo_sel_utils::exists(stat_iter->first.GetString(),goodStations) &&
	  (stat_iter->first.GetOM() > 60))))
      continue;

    ENSURE(stat_iter->first.GetString()>0,"There should be no AMANDA OMs.");

      if ( stat_iter->first.GetOM() > 60 ){
	ENSURE(static_cast<int>(stat_iter->second.lcSpan) == ds_icetopLCSpan_);
	ENSURE_DISTANCE(stat_iter->second.lcWindowPre, ds_icetopLCWindowPre_, DISTANCE);
	ENSURE_DISTANCE(stat_iter->second.lcWindowPost, ds_icetopLCWindowPost_, DISTANCE);
	ENSURE(static_cast<int>(stat_iter->second.statusFADC) == ds_statusFADC_IceTop_);

	ENSURE(static_cast<int>(stat_iter->second.lcMode) == ds_lcMode_icetop_);
	
	ENSURE(stat_iter->second.nBinsATWD0 == ds_nBinsATWD0_IceTop_);
	ENSURE(stat_iter->second.nBinsATWD1 == ds_nBinsATWD1_IceTop_);
	ENSURE(stat_iter->second.nBinsATWD2 == ds_nBinsATWD2_IceTop_);
	ENSURE(stat_iter->second.nBinsFADC == ds_nBinsFADC_IceTop_);

	if ( stat_iter->first.GetOM() == 61 || stat_iter->first.GetOM() == 63 ){	
	      ENSURE_DISTANCE(stat_iter->second.pmtHV, ds_icetopHighGainVoltage_, DISTANCE);
	}else if ( stat_iter->first.GetOM() == 62 || stat_iter->first.GetOM() == 64 ){
	  ENSURE_DISTANCE(stat_iter->second.pmtHV, ds_icetopLowGainVoltage_, DISTANCE);
	}
      }	else {
	ENSURE(static_cast<int>(stat_iter->second.lcSpan) == ds_iniceLCSpan_);
	ENSURE_DISTANCE(stat_iter->second.lcWindowPre, ds_iniceLCWindowPre_, DISTANCE);
	ENSURE_DISTANCE(stat_iter->second.lcWindowPost, ds_iniceLCWindowPost_, DISTANCE);
	ENSURE_DISTANCE(stat_iter->second.pmtHV, ds_iniceVoltage_, DISTANCE);
	ENSURE(static_cast<int>(stat_iter->second.statusFADC) == ds_statusFADC_InIce_);

	ENSURE(stat_iter->second.nBinsATWD0 == ds_nBinsATWD0_InIce_);
	ENSURE(stat_iter->second.nBinsATWD1 == ds_nBinsATWD1_InIce_);
	ENSURE(stat_iter->second.nBinsATWD2 == ds_nBinsATWD2_InIce_);
	ENSURE(stat_iter->second.nBinsFADC == ds_nBinsFADC_InIce_);

	if(stat_iter->first.GetOM() == 1) 
	  ENSURE(stat_iter->second.lcMode == ds_lcMode_inice_first_);
	else if(stat_iter->first.GetOM() == 60) 
	  ENSURE(stat_iter->second.lcMode == ds_lcMode_inice_last_);
	else ENSURE(stat_iter->second.lcMode == ds_lcMode_inice_bulk_);
      }

    ENSURE(static_cast<int>(stat_iter->second.trigMode) == ds_triggerMode_);
    ENSURE(static_cast<int>(stat_iter->second.statusATWDa) == ds_statusATWDa_);
    ENSURE(static_cast<int>(stat_iter->second.statusATWDb) == ds_statusATWDb_);
    ENSURE_DISTANCE(stat_iter->second.speThreshold, ds_speThreshold_, DISTANCE);
    ENSURE_DISTANCE(stat_iter->second.fePedestal, ds_fePedestal_, DISTANCE);
    ENSURE(stat_iter->second.dacTriggerBias0 == ds_dacTriggerBias0_);
    ENSURE(stat_iter->second.dacTriggerBias1 == ds_dacTriggerBias1_);
    ENSURE(stat_iter->second.dacFADCRef == ds_dacFADCRef_);
  }

  PushFrame(frame,"OutBox");
}//Physics()
 
