#include "sim-services/sim-source/I3MCCalibrationService.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"

I3MCCalibrationService::I3MCCalibrationService(I3GeometryServicePtr g,
					       I3CalibrationServicePtr c) :
  startYear_(I3CalibDefaults::START_YEAR),
  startDAQTime_(I3CalibDefaults::START_DAQTIME),
  endYear_(I3CalibDefaults::END_YEAR),
  endDAQTime_(I3CalibDefaults::END_DAQTIME),
  temperature_(I3CalibDefaults::TEMPERATURE),
  fadcBaselineFit_slope_(I3CalibDefaults::FADC_BASELINE_FIT_SLOPE),
  fadcBaselineFit_intercept_(I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT),
  fadcGain_(I3CalibDefaults::FADC_GAIN),
  atwd0Gain_(I3CalibDefaults::ATWD0_GAIN),
  atwd1Gain_(I3CalibDefaults::ATWD1_GAIN),
  atwd2Gain_(I3CalibDefaults::ATWD2_GAIN),
  atwd_a_FreqFit_A_(I3CalibDefaults::ATWD_A_FREQFIT_A),
  atwd_a_FreqFit_B_(I3CalibDefaults::ATWD_A_FREQFIT_B),
  atwd_a_FreqFit_C_(I3CalibDefaults::ATWD_A_FREQFIT_C),
  atwd_b_FreqFit_A_(I3CalibDefaults::ATWD_B_FREQFIT_A),
  atwd_b_FreqFit_B_(I3CalibDefaults::ATWD_B_FREQFIT_B),
  atwd_b_FreqFit_C_(I3CalibDefaults::ATWD_B_FREQFIT_C),
  hvGainFit_slope_(I3CalibDefaults::HV_GAIN_FIT_SLOPE),
  hvGainFit_intercept_(I3CalibDefaults::HV_GAIN_FIT_INTERCEPT),
  atwdBinCalibFit_slope_(I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE),
  atwdBinCalibFit_intercept_(I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
{
  geo_service_ = g;
  cal_service_ = c;
}


I3CalibrationConstPtr
I3MCCalibrationService::GetCalibration(I3Time time){

  log_debug("GetCalibration");

  I3GeometryConstPtr geo = geo_service_->GetGeometry(time);
  const I3OMGeoMap& om_geo = geo->omgeo;

  I3OMGeoMap::const_iterator iter;

  I3CalibrationPtr calibration;
  if(cal_service_){
    calibration = 
      I3CalibrationPtr(new I3Calibration(*(cal_service_->GetCalibration(time))));
  }else{
    calibration = I3CalibrationPtr(new I3Calibration);
    I3Time start(startYear_,startDAQTime_);
    I3Time end(endYear_,endDAQTime_);
    
    calibration->startTime = start;
    calibration->endTime = end;
  }

  //changed all inice to om_geo

  I3DOMCalibration domCalib;

  domCalib.SetTemperature(temperature_);
  
  LinearFit fadcBaselineFit;
  fadcBaselineFit.slope = fadcBaselineFit_slope_;
  fadcBaselineFit.intercept = fadcBaselineFit_intercept_;
  
  domCalib.SetFADCBaselineFit(fadcBaselineFit);
  
  // Units are actually V/count
  domCalib.SetFADCGain(fadcGain_);
  
  domCalib.SetATWDGain(0, atwd0Gain_);
  domCalib.SetATWDGain(1, atwd1Gain_);
  domCalib.SetATWDGain(2, atwd2Gain_);
  
  QuadraticFit qfit0;
  qfit0.quadFitA = atwd_a_FreqFit_A_;
  qfit0.quadFitB = atwd_a_FreqFit_B_;
  qfit0.quadFitC = atwd_a_FreqFit_C_;
  
  QuadraticFit qfit1;
  qfit1.quadFitA = atwd_b_FreqFit_A_;
  qfit1.quadFitB = atwd_b_FreqFit_B_;
  qfit1.quadFitC = atwd_b_FreqFit_C_;
  
  domCalib.SetATWDFreqFit(0,qfit0);
  domCalib.SetATWDFreqFit(1,qfit1);
  
  LinearFit hvgainfit;	
  hvgainfit.slope = hvGainFit_slope_;
  hvgainfit.intercept = hvGainFit_intercept_;
  
  domCalib.SetHVGainFit(hvgainfit);
  
  LinearFit binfit;
  binfit.slope = atwdBinCalibFit_slope_;
  binfit.intercept = atwdBinCalibFit_intercept_;

  for( unsigned int channel = 0; channel < 3; ++channel )
      for( unsigned int id = 0; id <= 1; ++id )
	  for( unsigned int bin = 0; bin < 128; ++bin )	      
	      domCalib.SetATWDBinCalibFit(id,channel,bin,binfit);

  for( iter  = om_geo.begin(); iter != om_geo.end(); iter++ )
  {
    if (iter->second.omtype == I3OMGeo :: AMANDA)
      continue;

    //Only add a default if an object does not already exist
    if(calibration->domCal.find(iter->first) ==
       calibration->domCal.end())
      calibration->domCal[iter->first] = domCalib;
  }
  
  return calibration;

}


