#include "sim-services/tweak-sources/I3TweakCalibration.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3TweakCalibration::I3TweakCalibration(I3CalibrationServicePtr c) :
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
  atwdBinCalibFit_intercept_(NAN),
  atwd_response_width_(NAN),
  fadc_response_width_(NAN),
  tauparam_P0_(NAN),
  tauparam_P1_(NAN),
  tauparam_P2_(NAN),
  tauparam_P3_(NAN),
  tauparam_P4_(NAN),
  tauparam_P5_(NAN),
  tauparam_TauFrac_(NAN),
  fadcDeltaT_(NAN),
  frontendImpedance_(NAN),
  pmtTransitTimeSlope_(NAN),
  pmtTransitTimeIntercept_(NAN),
  domcalVersion_(""),
  atwda0_baseline_(NAN),
  atwda1_baseline_(NAN),
  atwda2_baseline_(NAN),
  atwdb0_baseline_(NAN),
  atwdb1_baseline_(NAN),
  atwdb2_baseline_(NAN)
{
  cal_service_ = c;
}


I3CalibrationConstPtr
I3TweakCalibration::GetCalibration(I3Time time){

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
      LinearFit hvgainfit(iter->second.GetHVGainFit());	
      if(!isnan(hvGainFit_slope_))
	hvgainfit.slope = hvGainFit_slope_;
      if(!isnan(hvGainFit_intercept_))
	hvgainfit.intercept = hvGainFit_intercept_;
      iter->second.SetHVGainFit(hvgainfit);    
    }

    if(!isnan(atwd_response_width_))
      iter->second.SetATWDResponseWidth(atwd_response_width_);

    if(!isnan(fadc_response_width_))
      iter->second.SetFADCResponseWidth(fadc_response_width_);
   
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


    if(!isnan(tauparam_P0_) ||
       !isnan(tauparam_P1_) ||
       !isnan(tauparam_P2_) ||
       !isnan(tauparam_P3_) ||
       !isnan(tauparam_P4_) ||
       !isnan(tauparam_P5_) ||
       !isnan(tauparam_TauFrac_)){
      
      TauParam p(iter->second.GetTauParameters());
      if(!isnan(tauparam_P0_))
	p.P0 = tauparam_P0_;
      if(!isnan(tauparam_P1_))
	p.P1 = tauparam_P1_;	    
      if(!isnan(tauparam_P2_))
	p.P2 = tauparam_P2_;	
      if(!isnan(tauparam_P3_))
	p.P3 = tauparam_P3_;	
      if(!isnan(tauparam_P4_))
	p.P4 = tauparam_P4_;	
      if(!isnan(tauparam_P5_))
	p.P5 = tauparam_P5_;	 
      if(!isnan(tauparam_TauFrac_))
	p.TauFrac = tauparam_TauFrac_;	

      iter->second.SetTauParameters(p);
    }


    if(!isnan(fadcDeltaT_))
      iter->second.SetFADCDeltaT(fadcDeltaT_);
    if(!isnan(frontendImpedance_))
      iter->second.SetFrontEndImpedance(frontendImpedance_);

    if(!isnan(pmtTransitTimeSlope_) ||
       !isnan(pmtTransitTimeIntercept_)){
      LinearFit pmtTransitTime = iter->second.GetTransitTime();	
      if(!isnan(pmtTransitTimeSlope_))
	pmtTransitTime.slope = pmtTransitTimeSlope_;
      if(!isnan(pmtTransitTimeIntercept_))
	pmtTransitTime.intercept = pmtTransitTimeIntercept_;
      iter->second.SetTransitTime(pmtTransitTime);    
    }

    if(domcalVersion_.size())
      iter->second.SetDOMCalVersion(domcalVersion_);

    if(!isnan(atwda0_baseline_))
      for( unsigned int bin = 0; bin < 128; ++bin )
	iter->second.SetATWDBaseline(0,0,bin,atwda0_baseline_);
    if(!isnan(atwda1_baseline_))
      for( unsigned int bin = 0; bin < 128; ++bin )
	iter->second.SetATWDBaseline(0,0,bin,atwda1_baseline_);
    if(!isnan(atwda2_baseline_))
      for( unsigned int bin = 0; bin < 128; ++bin )
	iter->second.SetATWDBaseline(0,0,bin,atwda2_baseline_);
    if(!isnan(atwdb0_baseline_))
      for( unsigned int bin = 0; bin < 128; ++bin )
	iter->second.SetATWDBaseline(0,0,bin,atwdb0_baseline_);
    if(!isnan(atwdb1_baseline_))
      for( unsigned int bin = 0; bin < 128; ++bin )
	iter->second.SetATWDBaseline(0,0,bin,atwdb1_baseline_);
    if(!isnan(atwdb2_baseline_))
      for( unsigned int bin = 0; bin < 128; ++bin )
	iter->second.SetATWDBaseline(0,0,bin,atwdb2_baseline_);

  }
  
  return calibration;

}


