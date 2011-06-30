/**
 * class: I3DBTestModule.cxx
 * (c) 2004 IceCube Collaboration
 * Version $Id: I3DBTestModule.cxx,v 1.16 2005/03/17 18:33:43 olivas Exp $
 *
 * Date 08 June 2006
 * @version $Revision: 1.1 $
 * @date $Date: 2005/03/17 18:33:43 $
 * @author Alex Olivas <olivas@icecube.umd.edu>
 *
 */
#include <I3Test.h>
#include "I3DBTestModule.h"
#include <interfaces/I3CalibrationService.h>
#include <interfaces/I3DetectorStatusService.h>
#include <interfaces/I3EventService.h>
#include <dataclasses/I3Time.h>
#include <icetray/I3Units.h>
#include <dataclasses/calibration/I3DOMCalibration.h>
#include <dataclasses/calibration/I3Calibration.h>
#include <dataclasses/status/I3DOMStatus.h>
#include <dataclasses/status/I3DetectorStatus.h>
#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"

#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>

using namespace std;

I3_MODULE(I3DBTestModule);

void TestCalibration(I3CalibrationConstPtr);
void TestDOMStatus(I3DetectorStatusConstPtr);

void TestGaussian(TH1D* h, 
		  double);

I3DBTestModule::I3DBTestModule(const I3Context& ctx) : 
  I3Module(ctx)
{ 
  log_debug("Constructor I3DBTestModule");  
  AddOutBox("OutBox");
}

I3DBTestModule::~I3DBTestModule(){}

void I3DBTestModule::Configure()
{
  log_debug("Configuring I3DBTestModule");
}

void I3DBTestModule::DAQ(I3FramePtr frame)
{
  log_debug("DAQ");

  I3CalibrationConstPtr calib = 
    frame->Get<I3CalibrationConstPtr>();
  
  TestCalibration(calib);

  I3DetectorStatusConstPtr status = 
    frame->Get<I3DetectorStatusConstPtr>();

  TestDOMStatus(status);

  PushFrame(frame,"OutBox");
}//DAQ()


void TestCalibration(I3CalibrationConstPtr calib){

  TH1D* temp_h = new TH1D("temp","DOM Temperature",50,233,283);
  TH1D* fadc_bl_slope_h = new TH1D("fadc_bl_slope","FADC Baseline Slope",50,1.2,1.3);
  TH1D* fadc_bl_int_h = new TH1D("fadc_bl_int","FADC Baseline Intercept",50,-950,-800);

  TH1D* fadc_gain_h = new TH1D("fadc_gain","FADC Gain(V)",
		   100,0.00006,0.00012);//Units are Volts

  TH1D* atwd0_gain_h = new TH1D("atwd0_gain","ATWD Channel 0 Gain(V)",
		    100,-20,-10);
  TH1D* atwd1_gain_h = new TH1D("atwd1_gain","ATWD Channel 1 Gain(V)",
		    100,-2.5,-1);
  TH1D* atwd2_gain_h = new TH1D("atwd2_gain","ATWD Channel 2 Gain(V)",
		    100,-0.3,-0.05);

  TH1D* quadfitA_0_h = new TH1D("quadfitA_0","Quadratic Fit A - 0",30,15,45);
  TH1D* quadfitB_0_h = new TH1D("quadfitB_0","Quadratic Fit B - 0",40,0.2,0.4);
  TH1D* quadfitC_0_h = new TH1D("quadfitC_0","Quadratic Fit C - 0",100,-3.4e-5,-2.4e-5);

  TH1D* quadfitA_1_h = new TH1D("quadfitA_1","Quadratic Fit A - 1",30,15,45);
  TH1D* quadfitB_1_h = new TH1D("quadfitB_1","Quadratic Fit B - 1",40,0.2,0.4);
  TH1D* quadfitC_1_h = new TH1D("quadfitC_1","Quadratic Fit C - 1",100,-3.4e-5,-2.4e-5);

  TH1D* hvgain_slope_h = new TH1D("hvgain_slope","HV Gain Fit Slope",30,6,9);
  TH1D* hvgain_int_h = new TH1D("hvgain_int","HV Gain Fit Intercept",20,-20,-10);

  double slope_min = -0.0025;//I3Units::V
  double slope_max = -0.0015;//I3Units::V

  double int_min = 2.7;//I3Units::V
  double int_max = 3.0;//I3Units::V

  TH1D* atwd0_a_bc_slope_h = new TH1D("atwd0_A_bc_slope",
			  "ATWD_A Channel 0 BinCalib Fit Slope",
			  100,slope_min,slope_max);
  TH1D* atwd0_a_bc_int_h = new TH1D("atwd0_A_bc_int",
			"ATWD_A Channel 0 BinCalib Fit Intercept",
			100,int_min,int_max);

  TH1D* atwd1_a_bc_slope_h = new TH1D("atwd1_A_bc_slope",
			  "ATWD_A Channel 1 BinCalib Fit Slope",
			  100,slope_min,slope_max);
  TH1D* atwd1_a_bc_int_h = new TH1D("atwd1_A_bc_int",
			"ATWD_A Channel 1 BinCalib Fit Intercept",
			100,int_min,int_max);

  TH1D* atwd2_a_bc_slope_h = new TH1D("atwd2_A_bc_slope",
			  "ATWD_A Channel 2 BinCalib Fit Slope",
			  100,slope_min,slope_max);
  TH1D* atwd2_a_bc_int_h = new TH1D("atwd2_A_bc_int",
			"ATWD_A Channel 2 BinCalib Fit Intercept",
			100,int_min,int_max);

  TH1D* atwd0_b_bc_slope_h = new TH1D("atwd0_B_bc_slope",
			  "ATWD_B Channel 0 BinCalib Fit Slope",
			  100,slope_min,slope_max);
  TH1D* atwd0_b_bc_int_h = new TH1D("atwd0_B_bc_int",
			"ATWD_B Channel 0 BinCalib Fit Intercept",
			100,int_min,int_max);

  TH1D* atwd1_b_bc_slope_h = new TH1D("atwd1_B_bc_slope",
			  "ATWD_B Channel 1 BinCalib Fit Slope",
			  100,slope_min,slope_max);
  TH1D* atwd1_b_bc_int_h = new TH1D("atwd1_B_bc_int",
			"ATWD_B Channel 1 BinCalib Fit Intercept",
			100,int_min,int_max);

  TH1D* atwd2_b_bc_slope_h = new TH1D("atwd2_B_bc_slope",
			  "ATWD_B Channel 2 BinCalib Fit Slope",
			  100,slope_min,slope_max);
  TH1D* atwd2_b_bc_int_h = new TH1D("atwd2_B_bc_int",
			"ATWD_B Channel 2 BinCalib Fit Intercept",
			100,int_min,int_max);


  map<OMKey, I3DOMCalibration>::const_iterator cal_iter;

  for(cal_iter = calib->domCal.begin();
      cal_iter != calib->domCal.end(); 
      cal_iter++){

    temp_h->Fill(cal_iter->second.GetTemperature());

    fadc_bl_slope_h->Fill(cal_iter->second.GetFADCBaselineFit().slope);
    fadc_bl_int_h->Fill(cal_iter->second.GetFADCBaselineFit().intercept);
    fadc_gain_h->Fill(cal_iter->second.GetFADCGain()/I3Units::V);
    atwd0_gain_h->Fill(cal_iter->second.GetATWDGain(0));
    atwd1_gain_h->Fill(cal_iter->second.GetATWDGain(1));
    atwd2_gain_h->Fill(cal_iter->second.GetATWDGain(2));

    quadfitA_0_h->Fill(cal_iter->second.GetATWDFreqFit(0).quadFitA);
    quadfitB_0_h->Fill(cal_iter->second.GetATWDFreqFit(0).quadFitB);
    quadfitC_0_h->Fill(cal_iter->second.GetATWDFreqFit(0).quadFitC);

    quadfitA_1_h->Fill(cal_iter->second.GetATWDFreqFit(1).quadFitA);
    quadfitB_1_h->Fill(cal_iter->second.GetATWDFreqFit(1).quadFitB);
    quadfitC_1_h->Fill(cal_iter->second.GetATWDFreqFit(1).quadFitC);

    hvgain_slope_h->Fill(cal_iter->second.GetHVGainFit().slope);
    hvgain_int_h->Fill(cal_iter->second.GetHVGainFit().intercept);

    for( unsigned int bin = 0; bin < 128; ++bin ){
      atwd0_a_bc_slope_h->Fill(cal_iter->second.GetATWDBinCalibFit(0,0,bin).slope/I3Units::V);
      atwd1_a_bc_slope_h->Fill(cal_iter->second.GetATWDBinCalibFit(0,1,bin).slope/I3Units::V);
      atwd2_a_bc_slope_h->Fill(cal_iter->second.GetATWDBinCalibFit(0,2,bin).slope/I3Units::V);

      atwd0_b_bc_slope_h->Fill(cal_iter->second.GetATWDBinCalibFit(1,0,bin).slope/I3Units::V);
      atwd1_b_bc_slope_h->Fill(cal_iter->second.GetATWDBinCalibFit(1,1,bin).slope/I3Units::V);
      atwd2_b_bc_slope_h->Fill(cal_iter->second.GetATWDBinCalibFit(1,2,bin).slope/I3Units::V);

      atwd0_a_bc_int_h->Fill(cal_iter->second.GetATWDBinCalibFit(0,0,bin).intercept/I3Units::V);
      atwd1_a_bc_int_h->Fill(cal_iter->second.GetATWDBinCalibFit(0,1,bin).intercept/I3Units::V);
      atwd2_a_bc_int_h->Fill(cal_iter->second.GetATWDBinCalibFit(0,2,bin).intercept/I3Units::V);

      atwd0_b_bc_int_h->Fill(cal_iter->second.GetATWDBinCalibFit(1,0,bin).intercept/I3Units::V);
      atwd1_b_bc_int_h->Fill(cal_iter->second.GetATWDBinCalibFit(1,1,bin).intercept/I3Units::V);
      atwd2_b_bc_int_h->Fill(cal_iter->second.GetATWDBinCalibFit(1,2,bin).intercept/I3Units::V);
    }
  }


  TestGaussian(temp_h,I3CalibDefaults::TEMPERATURE);
  TestGaussian(fadc_bl_slope_h,I3CalibDefaults::FADC_BASELINE_FIT_SLOPE);
  TestGaussian(fadc_bl_int_h,I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT);
  TestGaussian(fadc_gain_h,I3CalibDefaults::FADC_GAIN);
  TestGaussian(atwd0_gain_h,I3CalibDefaults::ATWD0_GAIN);
  TestGaussian(atwd1_gain_h,I3CalibDefaults::ATWD1_GAIN);
  TestGaussian(atwd2_gain_h,I3CalibDefaults::ATWD2_GAIN);
  TestGaussian(quadfitA_0_h,I3CalibDefaults::ATWD_A_FREQFIT_A);
  TestGaussian(quadfitB_0_h,I3CalibDefaults::ATWD_A_FREQFIT_B);
  TestGaussian(quadfitC_0_h,I3CalibDefaults::ATWD_A_FREQFIT_C);
  TestGaussian(quadfitA_1_h,I3CalibDefaults::ATWD_B_FREQFIT_A);
  TestGaussian(quadfitB_1_h,I3CalibDefaults::ATWD_B_FREQFIT_B);
  TestGaussian(quadfitC_1_h,I3CalibDefaults::ATWD_B_FREQFIT_C);
  TestGaussian(hvgain_slope_h,I3CalibDefaults::HV_GAIN_FIT_SLOPE);
  TestGaussian(hvgain_int_h ,I3CalibDefaults::HV_GAIN_FIT_INTERCEPT);
  TestGaussian(atwd0_a_bc_slope_h,I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  TestGaussian(atwd0_a_bc_int_h,I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  TestGaussian(atwd1_a_bc_slope_h,I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  TestGaussian(atwd1_a_bc_int_h,I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  TestGaussian(atwd2_a_bc_slope_h,I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  TestGaussian(atwd2_a_bc_int_h,I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  TestGaussian(atwd0_b_bc_slope_h,I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  TestGaussian(atwd0_b_bc_int_h,I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  TestGaussian(atwd1_b_bc_slope_h,I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  TestGaussian(atwd1_b_bc_int_h,I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  TestGaussian(atwd2_b_bc_slope_h,I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE);
  TestGaussian(atwd2_b_bc_int_h,I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);

} 

void TestDOMStatus(I3DetectorStatusConstPtr status){

  const double TOLERANCE(0.000001);

  TH1D* pmtHV_inice_h = new TH1D("pmtHV_inice","PMT High Voltage - InIce",60,1000., 1600.);
  TH1D* pmtHV_icetopHG_h = new TH1D("pmtHV_icetopHG","PMT High Voltage - IceTop High Gain",60,1000., 1600.);
  TH1D* pmtHV_icetopLG_h = new TH1D("pmtHV_icetopLG","PMT High Voltage - IceTop Low Gain",50,400,900.);
  TH1D* iniceSPEThreshold_h = new TH1D("iniceSPEThreshold","InIce SPE Threshold",100,500,700);
  TH1D* iniceMPEThreshold_h = new TH1D("iniceMPEThreshold","InIce MPE Threshold",100,500,700);
  TH1D* icetopSPEThreshold_h = new TH1D("icetopSPEThreshold","IceTop SPE Threshold",100,500,700);
  TH1D* icetopMPEThreshold_h = new TH1D("icetopMPEThreshold","IceTop MPE Threshold",100,500,700);
  TH1D* fePedestal_h = new TH1D("fePedestal","FE Pedestal",100,2000,2200);

  map<OMKey, I3DOMStatus>::const_iterator stat_iter;

  for(stat_iter = status->domStatus.begin();
      stat_iter != status->domStatus.end(); 
      stat_iter++){

    if(stat_iter->first.GetOM() <=60){
      pmtHV_inice_h->Fill(stat_iter->second.pmtHV/I3Units::volt);

      ENSURE_DISTANCE(stat_iter->second.lcWindowPre, I3DetStatDefaults::INICE_LCWINDOW_PRE, TOLERANCE);
      ENSURE_DISTANCE(stat_iter->second.lcWindowPost, I3DetStatDefaults::INICE_LCWINDOW_POST, TOLERANCE);
      ENSURE(stat_iter->second.statusFADC == I3DetStatDefaults::STATUS_FADC_INICE);

      ENSURE(stat_iter->second.nBinsATWD0 == I3DetStatDefaults::NBINS_ATWD0_INICE);
      ENSURE(stat_iter->second.nBinsATWD1 == I3DetStatDefaults::NBINS_ATWD1_INICE);
      ENSURE(stat_iter->second.nBinsATWD2 == I3DetStatDefaults::NBINS_ATWD2_INICE);
      ENSURE(stat_iter->second.nBinsFADC == I3DetStatDefaults::NBINS_FADC_INICE);
      ENSURE(stat_iter->second.trigMode == I3DetStatDefaults::INICE_TRIGGER_MODE);

      iniceSPEThreshold_h->Fill(stat_iter->second.speThreshold);
      iniceMPEThreshold_h->Fill(stat_iter->second.mpeThreshold);

      if(stat_iter->first.GetOM() == 1) 
	ENSURE(stat_iter->second.lcMode == I3DetStatDefaults::LCMODE_INICE_FIRST);
      else if(stat_iter->first.GetOM() == 60) 
	ENSURE(stat_iter->second.lcMode == I3DetStatDefaults::LCMODE_INICE_LAST);
      else ENSURE(stat_iter->second.lcMode == I3DetStatDefaults::LCMODE_INICE_BULK);
      
    }else{
      ENSURE(stat_iter->second.lcMode == I3DetStatDefaults::LCMODE_ICETOP);

      ENSURE_DISTANCE(stat_iter->second.lcWindowPre,I3DetStatDefaults::ICETOP_LCWINDOW_PRE , TOLERANCE);
      ENSURE_DISTANCE(stat_iter->second.lcWindowPost, I3DetStatDefaults::ICETOP_LCWINDOW_POST, TOLERANCE);
      ENSURE(stat_iter->second.statusFADC == I3DetStatDefaults::STATUS_FADC_ICETOP);

      ENSURE(stat_iter->second.nBinsATWD0 == I3DetStatDefaults::NBINS_ATWD0_ICETOP);
      ENSURE(stat_iter->second.nBinsATWD1 == I3DetStatDefaults::NBINS_ATWD1_ICETOP);
      ENSURE(stat_iter->second.nBinsATWD2 == I3DetStatDefaults::NBINS_ATWD2_ICETOP);
      ENSURE(stat_iter->second.nBinsFADC == I3DetStatDefaults::NBINS_FADC_ICETOP);

      if(stat_iter->second.domGainType == I3DOMStatus::High){
	pmtHV_icetopHG_h->Fill(stat_iter->second.pmtHV/I3Units::volt);
	icetopMPEThreshold_h->Fill(stat_iter->second.mpeThreshold);
	ENSURE(stat_iter->second.trigMode == I3DetStatDefaults::ICETOP_HG_TRIGGER_MODE);
      }
      if(stat_iter->second.domGainType == I3DOMStatus::High){
	pmtHV_icetopLG_h->Fill(stat_iter->second.pmtHV/I3Units::volt);
	icetopSPEThreshold_h->Fill(stat_iter->second.speThreshold);
	ENSURE(stat_iter->second.trigMode == I3DetStatDefaults::ICETOP_LG_TRIGGER_MODE);
      }
    }

    ENSURE(stat_iter->second.statusATWDa == I3DetStatDefaults::STATUS_ATWDa);
    ENSURE(stat_iter->second.statusATWDb == I3DetStatDefaults::STATUS_ATWDb);

    ENSURE(stat_iter->second.dacTriggerBias0 == I3DetStatDefaults::DAC_TRIGGER_BIAS0);
    ENSURE(stat_iter->second.dacTriggerBias1 == I3DetStatDefaults::DAC_TRIGGER_BIAS1);
    ENSURE(stat_iter->second.dacFADCRef == I3DetStatDefaults::DAC_FADC_REF);

    fePedestal_h->Fill(stat_iter->second.fePedestal);
  }

  TestGaussian(pmtHV_inice_h,I3DetStatDefaults::INICE_VOLTAGE);
  TestGaussian(pmtHV_icetopHG_h,I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE);
  TestGaussian(pmtHV_icetopLG_h,I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE);

  TestGaussian(iniceSPEThreshold_h,I3DetStatDefaults::INICE_SPE_THRESHOLD);
  TestGaussian(iniceMPEThreshold_h,I3DetStatDefaults::INICE_MPE_THRESHOLD);
  TestGaussian(icetopSPEThreshold_h,I3DetStatDefaults::ICETOP_SPE_THRESHOLD);
  TestGaussian(icetopMPEThreshold_h,I3DetStatDefaults::ICETOP_MPE_THRESHOLD);
  TestGaussian(fePedestal_h,I3DetStatDefaults::FE_PEDESTAL);

}

void TestGaussian(TH1D* h, 
	  double defVal){

  TCanvas c;//i make a canvas here
  //to supress a stupid ROOT message.  no joke.
  h->Fit("gaus","Q");
  TF1* f = h->GetFunction("gaus");
  
  double mean = f->GetParameter(1);
  double sigma = f->GetParError(1);

  ENSURE_DISTANCE(mean,defVal,5.*sigma);
}
