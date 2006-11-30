#include "sim-services/sim-source/I3TweekCalibration.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3TweekCalibration::I3TweekCalibration(I3CalibrationServicePtr c) :
  temperature_(NAN),
  fadcBaselineFit_slope_(NAN),
  fadcBaselineFit_intercept_(NAN),
  fadcGain_(NAN),
  atwd0Gain_(NAN),
  atwd1Gain_(NAN),
  atwd2Gain_(NAN),
  atwd_a_FreqFit_A_(NAN),
  atwd_a_FreqFit_B_(NAN),
  atwd_a_FreqFit_C_(NAN),
  atwd_b_FreqFit_A_(NAN),
  atwd_b_FreqFit_B_(NAN),
  atwd_b_FreqFit_C_(NAN),
  hvGainFit_slope_(NAN),
  hvGainFit_intercept_(NAN),
  atwdBinCalibFit_slope_(NAN),
  atwdBinCalibFit_intercept_(NAN)
{
  cal_service_ = c;
}


I3CalibrationConstPtr
I3TweekCalibration::GetCalibration(I3Time time){

  log_debug("GetCalibration");

  I3CalibrationPtr calibration;
  if(cal_service_){
    calibration = 
      I3CalibrationPtr(new I3Calibration(*(cal_service_->GetCalibration(time))));
  }else{
    log_fatal("This service does not create calibration objects.");
  }

  //changed all inice to om_geo
  map<OMKey,I3DOMCalibration>::iterator iter;
  for( iter  = calibration->domCal.begin(); 
       iter != calibration->domCal.end(); 
       iter++ ){

    if (iter->first.GetString() < 0) //skip AMANDA
      continue;

    if(!isnan(temperature_))
      iter->second.SetTemperature(temperature_);
    
    if(!isnan(fadcBaselineFit_slope_) ||
       !isnan(fadcBaselineFit_intercept_)){
      LinearFit fadcBaselineFit(iter->second.GetFADCBaselineFit());
      if(!isnan(fadcBaselineFit_slope_))
	fadcBaselineFit.slope = fadcBaselineFit_slope_;
      if(!isnan(fadcBaselineFit_intercept_))
	fadcBaselineFit.intercept = fadcBaselineFit_intercept_;
      
      iter->second.SetFADCBaselineFit(fadcBaselineFit);
    }
    
    // Units are actually V/count
    if(!isnan(fadcGain_))
      iter->second.SetFADCGain(fadcGain_);
    
    if(!isnan(atwd0Gain_))
      iter->second.SetATWDGain(0, atwd0Gain_);
    if(!isnan(atwd1Gain_))
      iter->second.SetATWDGain(1, atwd1Gain_);
    if(!isnan(atwd2Gain_))
      iter->second.SetATWDGain(2, atwd2Gain_);

    if(!isnan(atwd_a_FreqFit_A_) ||
       !isnan(atwd_a_FreqFit_B_) ||
       !isnan(atwd_a_FreqFit_C_)){    
      QuadraticFit qfit0;
      if(!isnan(atwd_a_FreqFit_A_))
	qfit0.quadFitA = atwd_a_FreqFit_A_;
      if(!isnan(atwd_a_FreqFit_B_))
	qfit0.quadFitB = atwd_a_FreqFit_B_;
      if(!isnan(atwd_a_FreqFit_C_))
	qfit0.quadFitC = atwd_a_FreqFit_C_; 
      iter->second.SetATWDFreqFit(0,qfit0);
    }

    if(!isnan(atwd_b_FreqFit_A_) ||
       !isnan(atwd_b_FreqFit_B_) ||
       !isnan(atwd_b_FreqFit_C_)){    
      QuadraticFit qfit1;
      if(!isnan(atwd_b_FreqFit_A_))
	qfit1.quadFitA = atwd_b_FreqFit_A_;
      if(!isnan(atwd_b_FreqFit_B_))
	qfit1.quadFitB = atwd_b_FreqFit_B_;
      if(!isnan(atwd_b_FreqFit_C_))
	qfit1.quadFitC = atwd_b_FreqFit_C_;
    iter->second.SetATWDFreqFit(1,qfit1);
    }


    
    if(!isnan(hvGainFit_slope_) ||
       !isnan(hvGainFit_intercept_)){
      LinearFit hvgainfit;	
      if(!isnan(hvGainFit_slope_))
	hvgainfit.slope = hvGainFit_slope_;
      if(!isnan(hvGainFit_intercept_))
	hvgainfit.intercept = hvGainFit_intercept_;
      iter->second.SetHVGainFit(hvgainfit);    
    }
   
    if(!isnan(atwdBinCalibFit_slope_) || 
       !isnan(atwdBinCalibFit_intercept_)){
      for( unsigned int channel = 0; channel < 3; ++channel )
	for( unsigned int id = 0; id <= 1; ++id )
	  for( unsigned int bin = 0; bin < 128; ++bin ){
	    LinearFit binfit(iter->second.GetATWDBinCalibFit(id,channel,bin));
	    if(!isnan(atwdBinCalibFit_slope_))
	      binfit.slope = atwdBinCalibFit_slope_;
	    if(!isnan(atwdBinCalibFit_intercept_))
	      binfit.intercept = atwdBinCalibFit_intercept_;
	    iter->second.SetATWDBinCalibFit(id,channel,bin,binfit);
	  }
    }
  }
  
  return calibration;

}

