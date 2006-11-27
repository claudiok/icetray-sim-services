#include "sim-services/sim-source/I3TweekDetectorStatus.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "phys-services/I3GeometryService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

//In this file you'll find the default values
//for the detector status in the I3DetStatDefaults namespace
#include "sim-services/sim-source/I3DefaultValues.h"

I3TweekDetectorStatus::I3TweekDetectorStatus(I3DetectorStatusServicePtr s) :						     
  icetopLCWindowPre_(NAN),
  icetopLCWindowPost_(NAN),
  icetopHighGainVoltage_(NAN),
  icetopLowGainVoltage_(NAN),
  iniceLCWindowPre_(NAN),
  iniceLCWindowPost_(NAN),
  lcSpan_(INT_MIN),
  iniceVoltage_(NAN),
  triggerMode_(INT_MIN),
  lcMode_(INT_MIN),
  statusATWDa_(INT_MIN),
  statusATWDb_(INT_MIN),
  statusFADC_InIce_(INT_MIN),
  statusFADC_IceTop_(INT_MIN),
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

    if (iter->second.omtype == I3OMGeo :: AMANDA)
      continue;

    if(trigMode_ != iter->trigMode)
      iter->trigMode = triggerMode_;
    if(lcMode_ != iter->lcMode)
      iter->lcMode = lcMode_;
   
    if(statusATWDa_ != iter->statusATWDa)
      iter->statusATWDa = statusATWDa_;
    if(statusATWDb_ != iter->statusATWDb)
      iter->statusATWDb = statusATWDb_;
    
    if(!isnan)(speThreshold_)
      iter->speThreshold = speThreshold_;
    if(!isnan(fePedestal_))
      iter->fePedestal = fePedestal_;
    
    if(dacTriggerBias0_ != INT_MIN)
      iter->dacTriggerBias0 = dacTriggerBias0_;
    if(dacTriggerBias1_ != INT_MIN)
      iter->dacTriggerBias1 = dacTriggerBias1_;
    
    if(dacFADCRef_ != INT_MIN)
      iter->dacFADCRef = dacFADCRef_;
  
    if (type != I3OMGeo :: AMANDA){
	//Don't do AMANDA OMs
	if ( type == I3OMGeo::IceTop )
	  {
	    if(statusFADC_IceTop_ != INT_MIN )
	      iter->statusFADC = statusFADC_IceTop_;
	    if(iter->nBinsATWD0 != INT_MIN )
	      iter->nBinsATWD0 = nBinsATWD0_IceTop_;
	    if(iter->nBinsATWD1 != INT_MIN )
	      iter->nBinsATWD1 = nBinsATWD1_IceTop_;
	    if(iter->nBinsATWD2 != INT_MIN )
	      iter->nBinsATWD2 = nBinsATWD2_IceTop_;
	    if(iter->nBinsFADC != INT_MIN )
	      iter->nBinsFADC = nBinsFADC_IceTop_;

	    if(!isnan(icetopLCWindowPre_))
	      iter->lcWindowPre = icetopLCWindowPre_;
	    if(!isnan(icetopLCWindowPost_))
	      iter->lcWindowPost = icetopLCWindowPost_;
	    
	    if ( thiskey.GetOM() == 61 ||
		 thiskey.GetOM() == 63 )
	      {	
		if(!isnan(icetopHighGainVoltage_))
		  iter->pmtHV = icetopHighGainVoltage_;
	      }
	    
	    else if ( thiskey.GetOM() == 62 ||
		      thiskey.GetOM() == 64 )
	      {
		if(!isnan(icetopLowGainVoltage_))
		  iter->pmtHV = icetopLowGainVoltage_;
	      }
	  }	
	else
	  {
	    if( statusFADC_InIce_ != INT_MIN)
	      iter->statusFADC = statusFADC_InIce_;
	    if( nBinsATWD0_InIce_ != INT_MIN)
	      iter->nBinsATWD0 = nBinsATWD0_InIce_;
	    if( nBinsATWD1_InIce_ != INT_MIN)
	      iter->nBinsATWD1 = nBinsATWD1_InIce_;
	    if( nBinsATWD2_InIce_ != INT_MIN)
	      iter->nBinsATWD2 = nBinsATWD2_InIce_;
	    if( nBinsFADC_InIce_ != INT_MIN)
	      iter->nBinsFADC = nBinsFADC_InIce_;
	    if( lcSpan_ != INT_MIN)
	      iter->lcSpan = lcSpan_;

	    if(!isnan(iniceLCWindowPre_))
	      iter->lcWindowPre = iniceLCWindowPre_;
	    if(!isnan(iniceLCWindowPost_))
	      iter->lcWindowPost = iniceLCWindowPost_;
	    
	    if(!isnan(iniceVoltage_))
	      iter->pmtHV = iniceVoltage_;
	  }
      }
  }
  return status_;
}
