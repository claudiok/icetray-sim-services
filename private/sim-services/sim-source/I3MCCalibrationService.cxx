#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3CalibrationConstPtr
I3MCCalibrationService::GetCalibration(I3Time time){

  log_debug("GetCalibration");

  I3GeometryConstPtr geo = geo_service_->GetGeometry(time);
  const I3OMGeoMap& om_geo = geo->omgeo;

  I3OMGeoMap::const_iterator iter;

  I3CalibrationPtr calibration = I3CalibrationPtr(new I3Calibration);

  I3Time start(2000,0);
  I3Time end(3000,0);

  calibration->startTime = start;
  calibration->endTime = end;
  //changed all inice to om_geo
  for( iter  = om_geo.begin(); iter != om_geo.end(); iter++ ){
    OMKey thiskey = iter->first;

    if (thiskey.GetString() < 0 )
    {
      log_debug("Skipping AMANDA OM");
      continue;
    }//end sanity check

    I3DOMCalibration domCalib;

    LinearFit fadcBaselineFit;
    fadcBaselineFit.slope = 1.248;
    fadcBaselineFit.intercept = -864.32;
    
    domCalib.SetFADCBaselineFit(fadcBaselineFit);

    // Units are actually V/count
    domCalib.SetFADCGain(9.733e-5*I3Units::V);

    domCalib.SetATWDGain(0, -16.0);
    domCalib.SetATWDGain(1, -2.0);
    domCalib.SetATWDGain(2, -0.25);
	
    QuadraticFit qfit0,qfit1;
	
    qfit0.quadFitA = 2.5858788;
    qfit0.quadFitB = 0.013337472;
    qfit0.quadFitC = NAN;
	
    qfit1.quadFitA = 2.3853257;
    qfit1.quadFitB = 0.014224272;
    qfit1.quadFitC = NAN; 

    domCalib.SetATWDFreqFit(0,qfit0);
    domCalib.SetATWDFreqFit(1,qfit1);

    LinearFit hvgainfit;	
    hvgainfit.intercept = -15.1997;
    hvgainfit.slope = 7.0842533;

    domCalib.SetHVGainFit(hvgainfit);

    for( unsigned int channel = 0; channel < 3; ++channel )
      {
	for( unsigned int id = 0; id <= 1; ++id )
	  {
	    for( unsigned int bin = 0; bin < 128; ++bin )
	      {
		LinearFit binfit;
		binfit.slope = -0.002*I3Units::V;
		binfit.intercept = 2.9*I3Units::V;
	   
		domCalib.SetATWDBinCalibFit(id,channel,bin,
					    binfit);
	      }
	  }
      }
    calibration->domCal[thiskey] = domCalib;
  }
  return static_cast<I3CalibrationConstPtr>(calibration);

}


