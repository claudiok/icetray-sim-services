#include "sim-services/sim-source/I3MCDetectorStatusService.h"
#include "simclasses/I3MCTWRParams.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "phys-services/I3GeometryService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "phys-services/geo-selector/GeoSelUtils.h"

//In this file you'll find the default values
//for the detector status in the I3DetStatDefaults namespace
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"

I3MCDetectorStatusService::I3MCDetectorStatusService(I3GeometryServicePtr g,
						     I3DetectorStatusServicePtr s):
  startYear_(I3DetStatDefaults::START_YEAR),
  startDAQTime_(I3DetStatDefaults::START_DAQTIME),
  endYear_(I3DetStatDefaults::END_YEAR),
  endDAQTime_(I3DetStatDefaults::END_DAQTIME),
  icetopLCWindowPre_(I3DetStatDefaults::ICETOP_LCWINDOW_PRE),
  icetopLCWindowPost_(I3DetStatDefaults::ICETOP_LCWINDOW_POST),
  icetopHighGainVoltage_(I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE),
  icetopLowGainVoltage_(I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE),
  iniceLCWindowPre_(I3DetStatDefaults::INICE_LCWINDOW_PRE),
  iniceLCWindowPost_(I3DetStatDefaults::INICE_LCWINDOW_POST),
  lcSpan_(I3DetStatDefaults::LCSPAN),
  iniceVoltage_(I3DetStatDefaults::INICE_VOLTAGE),
  triggerMode_(I3DetStatDefaults::TRIGGER_MODE),
  lcMode_inice_first_(I3DetStatDefaults::LCMODE_INICE_FIRST),
  lcMode_inice_bulk_(I3DetStatDefaults::LCMODE_INICE_BULK),
  lcMode_inice_last_(I3DetStatDefaults::LCMODE_INICE_LAST),
  lcMode_icetop_(I3DetStatDefaults::LCMODE_ICETOP),
  statusATWDa_(I3DetStatDefaults::STATUS_ATWDa),
  statusATWDb_(I3DetStatDefaults::STATUS_ATWDb),
  statusFADC_InIce_(I3DetStatDefaults::STATUS_FADC_INICE),
  statusFADC_IceTop_(I3DetStatDefaults::STATUS_FADC_ICETOP),
  speThreshold_(I3DetStatDefaults::SPE_THRESHOLD),
  fePedestal_(I3DetStatDefaults::FE_PEDESTAL),
  dacTriggerBias0_(I3DetStatDefaults::DAC_TRIGGER_BIAS0),
  dacTriggerBias1_(I3DetStatDefaults::DAC_TRIGGER_BIAS1),
  dacFADCRef_(I3DetStatDefaults::DAC_FADC_REF),
  nBinsATWD0_InIce_(I3DetStatDefaults::NBINS_ATWD0_INICE),
  nBinsATWD1_InIce_(I3DetStatDefaults::NBINS_ATWD1_INICE),
  nBinsATWD2_InIce_(I3DetStatDefaults::NBINS_ATWD2_INICE),
  nBinsFADC_InIce_(I3DetStatDefaults::NBINS_FADC_INICE),
  nBinsATWD0_IceTop_(I3DetStatDefaults::NBINS_ATWD0_ICETOP),
  nBinsATWD1_IceTop_(I3DetStatDefaults::NBINS_ATWD1_ICETOP),
  nBinsATWD2_IceTop_(I3DetStatDefaults::NBINS_ATWD2_ICETOP),
  nBinsFADC_IceTop_(I3DetStatDefaults::NBINS_FADC_ICETOP)
{
  geo_service_ = g;
  old_status_service_ = s;
}

void I3MCDetectorStatusService::InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus)
{
  log_trace("inserting trigger %s",trigstatus.GetTriggerName().c_str());
  if(!triggerStatus_.insert(make_pair(trig.GetTriggerKey(), trigstatus)).second)
    log_fatal("trigger status insertion in detector status failed.");
}

I3DetectorStatusConstPtr
I3MCDetectorStatusService::GetDetectorStatus(I3Time time)
{
  log_debug("GetDetectorStatus");

  I3GeometryConstPtr geo = geo_service_->GetGeometry(time);
  const I3OMGeoMap& om_geo = geo->omgeo;

  if(!status_){
    if(old_status_service_){
      log_trace("...copying old detector status...");
      status_ = I3DetectorStatusPtr(new I3DetectorStatus(*(old_status_service_->GetDetectorStatus(time))));

      map<TriggerKey, I3TriggerStatus>::iterator i = triggerStatus_.begin();
      for( ;i != triggerStatus_.end(); i++)
	status_->triggerStatus.insert(*i);

    }else{
      log_trace("...making new detector status...");
      status_ = I3DetectorStatusPtr(new I3DetectorStatus);
      I3Time start(startYear_,startDAQTime_);
      I3Time end(endYear_,endDAQTime_);
      
      status_->startTime = start;
      status_->endTime = end;
      status_->triggerStatus = triggerStatus_;
    }
  }

  SetDOMStatus(status_,om_geo);

  log_trace("size of I3DOMStatus Map = %zu",status_->domStatus.size());

  return status_;
}

void I3MCDetectorStatusService::SetDOMStatus(I3DetectorStatusPtr status, const I3OMGeoMap& omgeo){

  I3DOMStatus domStatus;

  domStatus.trigMode = triggerMode_;
  
  domStatus.statusATWDa = statusATWDa_;
  domStatus.statusATWDb = statusATWDb_;
  
  domStatus.speThreshold = speThreshold_;
  domStatus.fePedestal = fePedestal_;
  
  domStatus.dacTriggerBias0 = dacTriggerBias0_;
  domStatus.dacTriggerBias1 = dacTriggerBias1_;
  
  domStatus.dacFADCRef = dacFADCRef_;
  
  I3OMGeoMap::const_iterator iter;
  //changed all inice to omgeo
  unsigned nSkipped(0);
  unsigned nCreated(0);
  unsigned nAMANDA(0);
  log_trace("omgeo.size() = %zu",omgeo.size());
  for( iter  = omgeo.begin(); iter != omgeo.end(); iter++ ){
      OMKey thiskey = iter->first;
      I3OMGeo::OMType type = iter->second.omtype;
      
      if(status->domStatus.find(thiskey) != status->domStatus.end() ||
	 (geo_sel_utils::exists(thiskey.GetString(),skipStrings_) && 
	  type == I3OMGeo::IceCube) ||
	 (geo_sel_utils::exists(thiskey.GetString(),skipStations_) && 
	  type == I3OMGeo::IceTop) ){
	nSkipped++;
	continue;
      }	

      if (type != I3OMGeo :: AMANDA){
	//Don't do AMANDA OMs
	if ( type == I3OMGeo::IceTop ){
	  domStatus.lcMode = lcMode_icetop_;
	  domStatus.statusFADC = statusFADC_IceTop_;
	  domStatus.nBinsATWD0 = nBinsATWD0_IceTop_;
	  domStatus.nBinsATWD1 = nBinsATWD1_IceTop_;
	  domStatus.nBinsATWD2 = nBinsATWD2_IceTop_;
	  domStatus.nBinsFADC = nBinsFADC_IceTop_;
	  
	  domStatus.lcWindowPre = icetopLCWindowPre_;
	  domStatus.lcWindowPost = icetopLCWindowPost_;
	  
	  if ( thiskey.GetOM() == 61 ||
	       thiskey.GetOM() == 63 )
	    {	
	      domStatus.pmtHV = icetopHighGainVoltage_;
	    }
	  
	  else if ( thiskey.GetOM() == 62 ||
		    thiskey.GetOM() == 64 )
	    {
	      domStatus.pmtHV = icetopLowGainVoltage_;
	    }
	}else{
	  /**
	   * the DOMs have different LC settings depending on where they are
	   */
	  //take a peak at the neighboring DOMs
	  I3OMGeoMap::const_iterator next_omgeo(iter); next_omgeo++;
	  I3OMGeoMap::const_iterator prev_omgeo(iter); prev_omgeo--;

	  if(iter == omgeo.begin() ||
	     prev_omgeo->first.GetString() != iter->first.GetString()){
	    //then we're at the top of a string
	    domStatus.lcMode = lcMode_inice_first_;
	  }else if(next_omgeo->second.omtype == I3OMGeo::IceTop ||
		   next_omgeo->first.GetString() != iter->first.GetString()){
	    //then we're at the bottom of a string
	    domStatus.lcMode = lcMode_inice_last_;
	  }
	  else domStatus.lcMode = lcMode_inice_bulk_;

	  domStatus.statusFADC = statusFADC_InIce_;
	  domStatus.nBinsATWD0 = nBinsATWD0_InIce_;
	  domStatus.nBinsATWD1 = nBinsATWD1_InIce_;
	  domStatus.nBinsATWD2 = nBinsATWD2_InIce_;
	  domStatus.nBinsFADC = nBinsFADC_InIce_;
	  domStatus.lcSpan = lcSpan_;
	  
	  domStatus.lcWindowPre = iniceLCWindowPre_;
	  domStatus.lcWindowPost = iniceLCWindowPost_;
	  
	  domStatus.pmtHV = iniceVoltage_;
	}
	status->domStatus[thiskey] = domStatus;
	log_trace("creating record for DOM %s",thiskey.str().c_str());
	nCreated++;
      }else{
	log_trace("skipping AMANDA OM %s",thiskey.str().c_str());
	nAMANDA++;
      }
  }
  log_debug("nSkipped: %d nCreated: %d ",nSkipped,nCreated);
  log_debug("status->domStatus.size() = %zu",status->domStatus.size());
  log_debug("nAMANDA: %d", nAMANDA);
  if(nSkipped + nCreated  != status->domStatus.size())
    log_fatal("missing status records!!!");
}

void I3MCDetectorStatusService::SetAOMStatus(I3DetectorStatusPtr status, 
					     const I3OMGeoMap& omgeo){

  TWRAOMStatus aomStatus;

  aomStatus.SetBinSize(twrBinSize_);
  aomStatus.SetBaseline(twrBaseline_);//

  I3OMGeoMap::const_iterator iter;
  for( iter  = omgeo.begin(); iter != omgeo.end(); iter++ ){
      OMKey omKey = iter->first;
      I3OMGeo::OMType type = iter->second.omtype;
      
      if (type != I3OMGeo :: AMANDA){
	continue;//Only do AMANDA OMs
      }else{
	I3MCTWRParamsMap::iterator iter = twrParamsMap_->find(omKey);
	if(iter != twrParamsMap_->end()){
	  aomStatus.SetStopDelay(iter->second.stop_delay);
	  aomStatus.SetThreshold(iter->second.TWR_thresh);
	  //if(iter->second.optical) t = TWRAOMStatus::OPTICAL
	  //else t = TWRAOMStatus::ELECTRICAL;
	  TWRAOMStatus::CableType t = (iter->second.optical 
				       ? TWRAOMStatus::OPTICAL :
				       TWRAOMStatus::ELECTRICAL);
	  aomStatus.SetCableType(t);
	  status->aomStatus[omKey] = aomStatus;
	}else{

	}
      }
  }

}
