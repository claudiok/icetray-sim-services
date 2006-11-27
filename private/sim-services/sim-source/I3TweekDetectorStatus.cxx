#include "sim-services/sim-source/I3TweekDetectorStatus.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "phys-services/I3GeometryService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

I3TweekDetectorStatus::I3TweekDetectorStatus(I3DetectorStatusServicePtr s) :						     
  icetopLCWindowPre_(NAN),
  icetopLCWindowPost_(NAN),
  icetopHighGainVoltage_(NAN),
  icetopLowGainVoltage_(NAN),
  iniceLCWindowPre_(NAN),
  iniceLCWindowPost_(NAN),
  lcSpan_(INT_MIN),
  iniceVoltage_(NAN),
  triggerMode_(I3DOMStatus::UnknownTrigMode),
  lcMode_(I3DOMStatus::UnknownLCMode),
  statusATWDa_(I3DOMStatus::Unknown),
  statusATWDb_(I3DOMStatus::Unknown),
  statusFADC_InIce_(I3DOMStatus::Unknown),
  statusFADC_IceTop_(I3DOMStatus::Unknown),
  speThreshold_(NAN),
  fePedestal_(NAN),
  dacTriggerBias0_(INT_MIN),
  dacTriggerBias1_(INT_MIN),
  dacFADCRef_(INT_MIN),
  nBinsATWD0_InIce_(INT_MIN),
  nBinsATWD1_InIce_(INT_MIN),
  nBinsATWD2_InIce_(INT_MIN),
  nBinsFADC_InIce_(INT_MIN),
  nBinsATWD0_IceTop_(INT_MIN),
  nBinsATWD1_IceTop_(INT_MIN),
  nBinsATWD2_IceTop_(INT_MIN),
  nBinsFADC_IceTop_(INT_MIN)
{
  old_status_service_ = s;
}

void I3TweekDetectorStatus::InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus)
{
  if(!triggerStatus_.insert(make_pair(trig.GetTriggerKey(), trigstatus)).second)
    log_fatal("trigger status insertion in detector status failed.");
}

I3DetectorStatusConstPtr
I3TweekDetectorStatus::GetDetectorStatus(I3Time time)
{
  log_debug("GetDetectorStatus");

  if(!status_){
    if(old_status_service_){
      status_ = I3DetectorStatusPtr(new I3DetectorStatus(*(old_status_service_->GetDetectorStatus(time))));
    }else log_fatal("this service does not create detector status objects");
  }

  //changed all inice to om_geo
  map<OMKey,I3DOMStatus>::iterator iter;
  for( iter  = status_->domStatus.begin(); 
       iter != status_->domStatus.end(); 
       iter++ ){

    if (iter->first.GetString()<0 )//skip AMANDA
      continue;

    if(triggerMode_ != I3DOMStatus::UnknownTrigMode)
      iter->second.trigMode = triggerMode_;
    if(lcMode_ != I3DOMStatus::UnknownLCMode)
      iter->second.lcMode = lcMode_;
   
    if(statusATWDa_ != I3DOMStatus::Unknown)
      iter->second.statusATWDa = statusATWDa_;
    if(statusATWDb_ != I3DOMStatus::Unknown)
      iter->second.statusATWDb = statusATWDb_;
    
    if(!isnan(speThreshold_))
      iter->second.speThreshold = speThreshold_;
    if(!isnan(fePedestal_))
      iter->second.fePedestal = fePedestal_;
    
    if(dacTriggerBias0_ != INT_MIN)
      iter->second.dacTriggerBias0 = dacTriggerBias0_;
    if(dacTriggerBias1_ != INT_MIN)
      iter->second.dacTriggerBias1 = dacTriggerBias1_;
    
    if(dacFADCRef_ != INT_MIN)
      iter->second.dacFADCRef = dacFADCRef_;
  
    if ( iter->first.GetOM()>60 )
      {
	if(statusFADC_IceTop_ != I3DOMStatus::Unknown )
	  iter->second.statusFADC = statusFADC_IceTop_;
	if(static_cast<int>(iter->second.nBinsATWD0) != INT_MIN )
	  iter->second.nBinsATWD0 = nBinsATWD0_IceTop_;
	if(static_cast<int>(iter->second.nBinsATWD1) != INT_MIN )
	  iter->second.nBinsATWD1 = nBinsATWD1_IceTop_;
	if(static_cast<int>(iter->second.nBinsATWD2) != INT_MIN )
	  iter->second.nBinsATWD2 = nBinsATWD2_IceTop_;
	if(static_cast<int>(iter->second.nBinsFADC) != INT_MIN )
	  iter->second.nBinsFADC = nBinsFADC_IceTop_;
	
	if(!isnan(icetopLCWindowPre_))
	  iter->second.lcWindowPre = icetopLCWindowPre_;
	if(!isnan(icetopLCWindowPost_))
	  iter->second.lcWindowPost = icetopLCWindowPost_;
	
	if ( iter->first.GetOM() == 61 ||
	     iter->first.GetOM() == 63 )
	  {	
	    if(!isnan(icetopHighGainVoltage_))
	      iter->second.pmtHV = icetopHighGainVoltage_;
	  }
	
	else if ( iter->first.GetOM() == 62 ||
		  iter->first.GetOM() == 64 )
	  {
	    if(!isnan(icetopLowGainVoltage_))
	      iter->second.pmtHV = icetopLowGainVoltage_;
	  }
      }	
    else
      {
	if( statusFADC_InIce_ != I3DOMStatus::Unknown)
	  iter->second.statusFADC = statusFADC_InIce_;
	if( nBinsATWD0_InIce_ != INT_MIN)
	  iter->second.nBinsATWD0 = nBinsATWD0_InIce_;
	if( nBinsATWD1_InIce_ != INT_MIN)
	  iter->second.nBinsATWD1 = nBinsATWD1_InIce_;
	if( nBinsATWD2_InIce_ != INT_MIN)
	  iter->second.nBinsATWD2 = nBinsATWD2_InIce_;
	if( nBinsFADC_InIce_ != INT_MIN)
	      iter->second.nBinsFADC = nBinsFADC_InIce_;
	if( lcSpan_ != INT_MIN)
	  iter->second.lcSpan = lcSpan_;
	
	if(!isnan(iniceLCWindowPre_))
	  iter->second.lcWindowPre = iniceLCWindowPre_;
	if(!isnan(iniceLCWindowPost_))
	  iter->second.lcWindowPost = iniceLCWindowPost_;
	
	if(!isnan(iniceVoltage_))
	  iter->second.pmtHV = iniceVoltage_;
      }
  }
  return status_;
}
