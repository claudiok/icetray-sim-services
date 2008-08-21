#include "sim-services/tweak-sources/I3TweakDOMStatus.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "interfaces/I3GeometryService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

I3TweakDOMStatus::I3TweakDOMStatus(I3DetectorStatusServicePtr s) :						     
  icetopLCWindowPre_(NAN),
  icetopLCWindowPost_(NAN),
  icetopHighGainVoltage_(NAN),
  icetopLowGainVoltage_(NAN),
  iniceLCWindowPre_(NAN),
  iniceLCWindowPost_(NAN),
  inice_LCSpan_(INT_MIN),
  icetop_LCSpan_(INT_MIN),
  iniceVoltage_(NAN),
  iniceTriggerMode_(I3DOMStatus::UnknownTrigMode),
  icetopHGTriggerMode_(I3DOMStatus::UnknownTrigMode),
  icetopLGTriggerMode_(I3DOMStatus::UnknownTrigMode),
  lcMode_inice_first_(I3DOMStatus::UnknownLCMode),
  lcMode_inice_bulk_(I3DOMStatus::UnknownLCMode),
  lcMode_inice_last_(I3DOMStatus::UnknownLCMode),
  lcMode_icetop_(I3DOMStatus::UnknownLCMode),
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

I3DetectorStatusConstPtr
I3TweakDOMStatus::GetDetectorStatus(I3Time time)
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

	if( icetop_LCSpan_ != INT_MIN)
	  iter->second.lcSpan = icetop_LCSpan_;

	if(lcMode_icetop_ != I3DOMStatus::UnknownLCMode)
	  iter->second.lcMode = lcMode_icetop_;

	if(statusFADC_IceTop_ != I3DOMStatus::Unknown )
	  iter->second.statusFADC = statusFADC_IceTop_;
	if(nBinsATWD0_IceTop_ != INT_MIN )
	  iter->second.nBinsATWD0 = nBinsATWD0_IceTop_;
	if(nBinsATWD1_IceTop_ != INT_MIN )
	  iter->second.nBinsATWD1 = nBinsATWD1_IceTop_;
	if(nBinsATWD2_IceTop_ != INT_MIN )
	  iter->second.nBinsATWD2 = nBinsATWD2_IceTop_;
	if(nBinsFADC_IceTop_ != INT_MIN )
	  iter->second.nBinsFADC = nBinsFADC_IceTop_;
	
	if(!isnan(icetopLCWindowPre_))
	  iter->second.lcWindowPre = icetopLCWindowPre_;
	if(!isnan(icetopLCWindowPost_))
	  iter->second.lcWindowPost = icetopLCWindowPost_;
	
	if ( iter->second.domGainType == I3DOMStatus::High)
	  {	
	    if(!isnan(icetopHighGainVoltage_))
	      iter->second.pmtHV = icetopHighGainVoltage_;
	    if(icetopHGTriggerMode_ != I3DOMStatus::UnknownTrigMode)
	      iter->second.trigMode = icetopHGTriggerMode_;
	  }
	
	else if ( iter->second.domGainType == I3DOMStatus::Low)
	  {
	    if(!isnan(icetopLowGainVoltage_))
	      iter->second.pmtHV = icetopLowGainVoltage_;
	    if(icetopLGTriggerMode_ != I3DOMStatus::UnknownTrigMode)
	      iter->second.trigMode = icetopLGTriggerMode_;
	  }
      }	
    else
      {
	if(iter->first.GetOM() == 1 && 
	   lcMode_inice_first_ != I3DOMStatus::UnknownLCMode)
	  iter->second.lcMode = lcMode_inice_first_;
	else if(iter->first.GetOM() == 60 && 
		lcMode_inice_last_ != I3DOMStatus::UnknownLCMode)
	  iter->second.lcMode = lcMode_inice_last_;
	else if(lcMode_inice_bulk_ != I3DOMStatus::UnknownLCMode)	  
	  iter->second.lcMode = lcMode_inice_bulk_;

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
	if( inice_LCSpan_ != INT_MIN)
	  iter->second.lcSpan = inice_LCSpan_;
	
	if(!isnan(iniceLCWindowPre_))
	  iter->second.lcWindowPre = iniceLCWindowPre_;
	if(!isnan(iniceLCWindowPost_))
	  iter->second.lcWindowPost = iniceLCWindowPost_;
	
	if(!isnan(iniceVoltage_))
	  iter->second.pmtHV = iniceVoltage_;

	if(iniceTriggerMode_ != I3DOMStatus::UnknownTrigMode)
	  iter->second.trigMode = iniceTriggerMode_;
   
      }
  }
  return status_;
}
