#include "sim-services/sim-source/I3MCDetectorStatusService.h"
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

I3MCDetectorStatusService::I3MCDetectorStatusService(I3GeometryServicePtr g) :
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
  lcMode_(I3DetStatDefaults::LCMODE),
  statusATWDa_(I3DetStatDefaults::STATUS_ATWDa),
  statusATWDb_(I3DetStatDefaults::STATUS_ATWDb),
  statusFADC_(I3DetStatDefaults::STATUS_FADC),
  speThreshold_(I3DetStatDefaults::SPE_THRESHOLD),
  fePedestal_(I3DetStatDefaults::FE_PEDESTAL),
  dacTriggerBias0_(I3DetStatDefaults::DAC_TRIGGER_BIAS0),
  dacTriggerBias1_(I3DetStatDefaults::DAC_TRIGGER_BIAS1),
  dacFADCRef_(I3DetStatDefaults::DAC_FADC_REF),
  nBinsATWD0_(I3DetStatDefaults::NBINS_ATWD0),
  nBinsATWD1_(I3DetStatDefaults::NBINS_ATWD1),
  nBinsATWD2_(I3DetStatDefaults::NBINS_ATWD2),
  nBinsFADC_(I3DetStatDefaults::NBINS_FADC)
{
  geo_service_ = g;
}

void I3MCDetectorStatusService::InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus)
{
  if (!status_)
    status_ = I3DetectorStatusPtr(new I3DetectorStatus);
  if(!status_->triggerStatus.insert(make_pair(trig.GetTriggerKey(), trigstatus)).second)
    log_fatal("trigger status insertion in detector status failed.");
}

I3DetectorStatusConstPtr
I3MCDetectorStatusService::GetDetectorStatus(I3Time time)
{
  log_debug("GetDetectorStatus");

  I3GeometryConstPtr geo = geo_service_->GetGeometry(time);
  const I3OMGeoMap& om_geo = geo->omgeo;

  if (!status_)
    status_ = I3DetectorStatusPtr(new I3DetectorStatus);

  I3OMGeoMap::const_iterator iter;

  I3Time start(startYear_,startDAQTime_);
  I3Time end(endYear_,endDAQTime_);

  status_->startTime = start;
  status_->endTime = end;

  //changed all inice to om_geo
  for( iter  = om_geo.begin(); iter != om_geo.end(); iter++ ){
      OMKey thiskey = iter->first;
      I3OMGeo::OMType type = iter->second.omtype;
      
      I3DOMStatus domStatus;

      if ( type == I3OMGeo::IceTop )
	{
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
      }	
      else
      {
	domStatus.lcSpan = lcSpan_;

	domStatus.lcWindowPre = iniceLCWindowPre_;
	domStatus.lcWindowPost = iniceLCWindowPost_;

	domStatus.pmtHV = iniceVoltage_;
      }

      domStatus.trigMode = triggerMode_;
      domStatus.lcMode = lcMode_;

      domStatus.statusATWDa = statusATWDa_;
      domStatus.statusATWDb = statusATWDb_;
      domStatus.statusFADC = statusFADC_;
	
      domStatus.speThreshold = speThreshold_;
      domStatus.fePedestal = fePedestal_;

      domStatus.dacTriggerBias0 = dacTriggerBias0_;
      domStatus.dacTriggerBias1 = dacTriggerBias1_;

      domStatus.dacFADCRef = dacFADCRef_;
    
      domStatus.nBinsATWD0 = nBinsATWD0_;
      domStatus.nBinsATWD1 = nBinsATWD1_;
      domStatus.nBinsATWD2 = nBinsATWD2_;

      domStatus.nBinsFADC = nBinsFADC_;

      status_->domStatus[thiskey] = domStatus;
    }
  return status_;
}
