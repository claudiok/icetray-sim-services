#include "sim-services/sim-source/I3MCCalibrationService.h"
#include <interfaces/I3GeometryService.h>
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/geometry/I3Geometry.h"
#include "icetray/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "phys-services/geo-selector/GeoSelUtils.h"
#include <fstream>

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
  atwdBinCalibFit_intercept_(I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT),
  atwd_response_width_(I3CalibDefaults::ATWD_RESPONSE_WIDTH),
  fadc_response_width_(I3CalibDefaults::FADC_RESPONSE_WIDTH),
  tauparam_P0_(I3CalibDefaults::TAU_P0_POST_2006),
  tauparam_P1_(I3CalibDefaults::TAU_P1_POST_2006),
  tauparam_P2_(I3CalibDefaults::TAU_P2_POST_2006),
  tauparam_P3_(I3CalibDefaults::TAU_P3_POST_2006),
  tauparam_P4_(I3CalibDefaults::TAU_P4_POST_2006),
  tauparam_P5_(I3CalibDefaults::TAU_P5_POST_2006),
  tauparam_TauFrac_(I3CalibDefaults::TAU_FRACTION_POST_2006),
  fadcDeltaT_(I3CalibDefaults::FADC_DELTA_T),
  frontendImpedance_(I3CalibDefaults::FRONT_END_IMPEDANCE),
  pmtTransitTimeSlope_(I3CalibDefaults::PMT_TRANSIT_TIME_SLOPE),
  pmtTransitTimeIntercept_(I3CalibDefaults::PMT_TRANSIT_TIME_INTERCEPT),
  domcalVersion_(I3CalibDefaults::DOMCAL_VERSION),
  atwda0_baseline_(I3CalibDefaults::ATWDA0_BASELINE),
  atwda1_baseline_(I3CalibDefaults::ATWDA1_BASELINE),
  atwda2_baseline_(I3CalibDefaults::ATWDA2_BASELINE),
  atwdb0_baseline_(I3CalibDefaults::ATWDB0_BASELINE),
  atwdb1_baseline_(I3CalibDefaults::ATWDB1_BASELINE),
  atwdb2_baseline_(I3CalibDefaults::ATWDB2_BASELINE),
  atwda_deltat_(I3CalibDefaults::ATWDA_DELTAT),
  atwdb_deltat_(I3CalibDefaults::ATWDB_DELTAT),
  spe_disc_thresh_slope_(I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE),
  spe_disc_thresh_intercept_(I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT),
  mpe_disc_thresh_slope_(I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE),
  mpe_disc_thresh_intercept_(I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT),
  pmt_disc_thresh_slope_(I3CalibDefaults::PMT_DISCRIMINATOR_SLOPE),
  pmt_disc_thresh_intercept_(I3CalibDefaults::PMT_DISCRIMINATOR_INTERCEPT),
  inice_relative_efficiencies_(I3CalibDefaults::INICE_RELATIVE_EFFICIENCY),
  deepcore_relative_efficiencies_(I3CalibDefaults::DEEPCORE_RELATIVE_EFFICIENCY),
  inice_dom_noise_rates_(I3CalibDefaults::INICE_NOISE_RATE),
  deepcore_dom_noise_rates_(I3CalibDefaults::DEEPCORE_NOISE_RATE)
{
  geo_service_ = g;
  cal_service_ = c;
}


I3CalibrationConstPtr
I3MCCalibrationService::GetCalibration(I3Time time){

  log_debug("GetCalibration");

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

  domCalib.SetATWDResponseWidth(atwd_response_width_);
  domCalib.SetFADCResponseWidth(fadc_response_width_);

  TauParam tauparam;
  tauparam.P0 = tauparam_P0_;
  tauparam.P1 = tauparam_P1_;
  tauparam.P2 = tauparam_P2_;
  tauparam.P3 = tauparam_P3_;
  tauparam.P4 = tauparam_P4_;
  tauparam.P5 = tauparam_P5_;
  tauparam.TauFrac = tauparam_TauFrac_;
  domCalib.SetTauParameters(tauparam);

  domCalib.SetFADCDeltaT(fadcDeltaT_);
  domCalib.SetFrontEndImpedance(frontendImpedance_);
  
  LinearFit pmttransit;
  pmttransit.slope = pmtTransitTimeSlope_;
  pmttransit.intercept = pmtTransitTimeIntercept_;
  domCalib.SetTransitTime(pmttransit);
  
  domCalib.SetDOMCalVersion(domcalVersion_);

  for( unsigned int bin = 0; bin < 128; ++bin ){
    domCalib.SetATWDBaseline(0,0,bin,atwda0_baseline_);
    domCalib.SetATWDBaseline(0,1,bin,atwda1_baseline_);
    domCalib.SetATWDBaseline(0,2,bin,atwda2_baseline_);
    domCalib.SetATWDBaseline(1,0,bin,atwdb0_baseline_);
    domCalib.SetATWDBaseline(1,1,bin,atwdb1_baseline_);
    domCalib.SetATWDBaseline(1,2,bin,atwdb2_baseline_);
  }

  for( unsigned int channel = 0; channel < 3; ++channel )
    for( unsigned int id = 0; id <= 1; ++id )
      for( unsigned int bin = 0; bin < 128; ++bin )
	domCalib.SetATWDBinCalibFit(id,channel,bin,binfit);      

  domCalib.SetATWDDeltaT(0,atwda_deltat_);
  domCalib.SetATWDDeltaT(1,atwdb_deltat_);

  LinearFit spe_disc_thresh;
  spe_disc_thresh.slope = spe_disc_thresh_slope_;
  spe_disc_thresh.intercept = spe_disc_thresh_intercept_;
  domCalib.SetSPEDiscCalib(spe_disc_thresh);

  LinearFit mpe_disc_thresh;
  mpe_disc_thresh.slope = mpe_disc_thresh_slope_;
  mpe_disc_thresh.intercept = mpe_disc_thresh_intercept_;
  domCalib.SetMPEDiscCalib(mpe_disc_thresh);

  LinearFit pmt_disc_thresh;
  pmt_disc_thresh.slope = pmt_disc_thresh_slope_;
  pmt_disc_thresh.intercept = pmt_disc_thresh_intercept_;
  domCalib.SetPMTDiscCalib(pmt_disc_thresh);

  I3GeometryConstPtr geo = geo_service_->GetGeometry(time);
  const I3OMGeoMap& om_geo = geo->omgeo;

  I3OMGeoMap::const_iterator iter;
  for( iter  = om_geo.begin(); iter != om_geo.end(); iter++ )
  {
    if (iter->second.omtype == I3OMGeo :: AMANDA)
      continue;

    //set the noise and relative efficiency
    if (iter->first.GetString() > 80 && iter->first.GetOM() <= 60){
      //this is a deepcore string
      domCalib.SetRelativeDomEff(deepcore_relative_efficiencies_);
      domCalib.SetDomNoiseRate(deepcore_dom_noise_rates_);
    }else{
      //this is an InIce/IceTop string
      domCalib.SetRelativeDomEff(inice_relative_efficiencies_);
      domCalib.SetDomNoiseRate(inice_dom_noise_rates_);
    }

    calibration->domCal[iter->first] = domCalib;
    log_trace("creating record for DOM %s",iter->first.str().c_str());
  }
  
  // Creating IceTop VEMCal records
  const I3StationGeoMap& sta_geo = geo->stationgeo;
  I3StationGeoMap::const_iterator sta_iter;
  for(sta_iter=sta_geo.begin(); sta_iter!=sta_geo.end(); ++sta_iter)
  {
    for(unsigned int i=0; i<sta_iter->second.size(); i++)
    {
      const I3TankGeo& tankGeo = sta_iter->second.at(i);
      
      I3VEMCalibration vemCalib;
      vemCalib.hglgCrossOver = 2600.0;
      vemCalib.muPeakWidth   = 20.0;
      vemCalib.corrFactor    = 1.0;
      
      switch(tankGeo.tanktype)
      {
	case I3TankGeo::Zirconium_Lined:
	  vemCalib.pePerVEM = 130.0;
	  break;
        case I3TankGeo::Tyvek_Lined:
	  vemCalib.pePerVEM = 220.0;
	  break;
        default:
	  log_error("Unknown type of Tank %d%s", sta_iter->first, (i<1?"A":"B"));   
	  continue;
      }
	
      std::vector<OMKey>::const_iterator om_iter;
      for(om_iter  = tankGeo.omKeyList_.begin();
	  om_iter != tankGeo.omKeyList_.end();
	  ++om_iter)
      {
	calibration->vemCal[*om_iter] = vemCalib;
	log_trace("creating vemcal record for DOM %s", om_iter->str().c_str());
      }
    }
  }
  
  return calibration;
}


