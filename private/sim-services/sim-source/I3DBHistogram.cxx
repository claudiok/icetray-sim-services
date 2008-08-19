/**
 * class: I3DBHistogram.cxx
 * (c) 2004 IceCube Collaboration
 * Version $Id: I3DBHistogram.cxx,v 1.16 2005/03/17 18:33:43 olivas Exp $
 *
 * Date 08 June 2006
 * @version $Revision: 1.1 $
 * @date $Date: 2005/03/17 18:33:43 $
 * @author Alex Olivas <olivas@icecube.umd.edu>
 *
 */

#include "sim-services/sim-source/I3DBHistogram.h"
#include <interfaces/I3CalibrationService.h>
#include <interfaces/I3DetectorStatusService.h>
#include <interfaces/I3EventService.h>

#include <dataclasses/calibration/I3Calibration.h>
#include <dataclasses/status/I3DetectorStatus.h>
#include <dataclasses/I3Time.h>
#include <dataclasses/I3Units.h>
#include <dataclasses/I3DOMFunctions.h>
#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"
#include "icetray/OMKey.h"

#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"

#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>

#include <boost/foreach.hpp>

I3_MODULE(I3DBHistogram);

void BookDOMCalibHistograms(I3CalibrationConstPtr , std::string );
void BookDOMStatusHistograms(I3DetectorStatusConstPtr , std::string );
void MakeDOMFunctionsPlots(I3CalibrationConstPtr ,I3DetectorStatusConstPtr);

void SetTemperature(TH1D*);
void SetFADCBaselineSlope(TH1D*);
void SetFADCBaselineIntercept(TH1D*);
void SetFADCGain(TH1D*);
void SetATWD0Gain(TH1D*);
void SetATWD1Gain(TH1D*);
void SetATWD2Gain(TH1D*);
void SetATWDaFreqFit_A(TH1D*);
void SetATWDaFreqFit_B(TH1D*);
void SetATWDaFreqFit_C(TH1D*);
void SetATWDbFreqFit_A(TH1D*);
void SetATWDbFreqFit_B(TH1D*);
void SetATWDbFreqFit_C(TH1D*);
void SetHVGainSlope(TH1D*);
void SetHVGainIntercept(TH1D*);
void SetATWD0aBinCalibSlope(TH1D*);
void SetATWD0aBinCalibIntercept(TH1D*);
void SetATWD1aBinCalibSlope(TH1D*);
void SetATWD1aBinCalibIntercept(TH1D*);
void SetATWD2aBinCalibSlope(TH1D*);
void SetATWD2aBinCalibIntercept(TH1D*);
void SetATWD0bBinCalibSlope(TH1D*);
void SetATWD0bBinCalibIntercept(TH1D*);
void SetATWD1bBinCalibSlope(TH1D*);
void SetATWD1bBinCalibIntercept(TH1D*);
void SetATWD2bBinCalibSlope(TH1D*);
void SetATWD2bBinCalibIntercept(TH1D*);

void SetLCSpan(TH1D*);
void SetLCWindowPre(TH1D*);
void SetLCWindowPost(TH1D*);
void SetDeltaCompress(TH1D*);
void SetPMTHVInIce(TH1D*);
void SetPMTHVIceTopHG(TH1D*);
void SetPMTHVIceTopLG(TH1D*);
void SetTriggerMode(TH1D*);
void SetLCMode(TH1D*);
void SetStatusATWDa(TH1D*);
void SetStatusATWDb(TH1D*);
void SetStatusFADC(TH1D*);
void SetSPEThreshold(TH1D*);
void SetMPEThreshold(TH1D*);
void SetFEPedestal(TH1D*);
void SetDACTriggerBias0(TH1D*);
void SetDACTriggerBias1(TH1D*);
void SetFADCRef(TH1D*);
void SetNBinsATWD0(TH1D*);
void SetNBinsATWD1(TH1D*);
void SetNBinsATWD2(TH1D*);
void SetNBinsFADC(TH1D*);

void SetFADCDeltaT(TH1D*);
void SetFrontEndImpedance(TH1D*);
void SetPMTTransitTimeSlope(TH1D*);
void SetPMTTransitTimeIntercept(TH1D*);
void SetATWD0aBaseline(TH1D*);
void SetATWD1aBaseline(TH1D*);
void SetATWD2aBaseline(TH1D*);
void SetATWD0bBaseline(TH1D*);
void SetATWD1bBaseline(TH1D*);
void SetATWD2bBaseline(TH1D*);

void SetATWDaDeltaT(TH1D*);
void SetATWDbDeltaT(TH1D*);
void SetSPEDiscCalibSlope(TH1D*);
void SetSPEDiscCalibIntercept(TH1D*);
void SetMPEDiscCalibSlope(TH1D*);
void SetMPEDiscCalibIntercept(TH1D*);


void FitAndFormatHisto(TH1D* h, 
		       string, 
		       string, 
		       bool);


I3DBHistogram::I3DBHistogram(const I3Context& ctx) : 
  I3Module(ctx)
{ 
  log_debug("Constructor I3DBHistogram");  
  AddOutBox("OutBox");
}

I3DBHistogram::~I3DBHistogram(){}

void I3DBHistogram::Configure()
{
  log_debug("Configuring I3DBHistogram");

  gROOT->SetStyle("Plain");
  gStyle->SetFrameLineWidth(2);
  gStyle->SetOptStat(111011);
  gStyle->SetOptFit(1);
}

void I3DBHistogram::Calibration(I3FramePtr frame)
{
  log_debug("Calibration");

  I3CalibrationConstPtr calib = 
    frame->Get<I3CalibrationConstPtr>();
  
  BookDOMCalibHistograms(calib,"DBDomCalib.root");

  PushFrame(frame,"OutBox");
}

void I3DBHistogram::DetectorStatus(I3FramePtr frame)
{
  log_debug("DetectorStatus");

  I3DetectorStatusConstPtr status = 
    frame->Get<I3DetectorStatusConstPtr>();

  BookDOMStatusHistograms(status,"DBDomStatus.root");

  PushFrame(frame,"OutBox");
}//Physics()

void I3DBHistogram::Physics(I3FramePtr frame)
{
  log_debug("Physics");

  I3CalibrationConstPtr calib = 
    frame->Get<I3CalibrationConstPtr>();
  
  I3DetectorStatusConstPtr status = 
    frame->Get<I3DetectorStatusConstPtr>();

  MakeDOMFunctionsPlots(calib,status);

  PushFrame(frame,"OutBox");
}//Physics()


void BookDOMCalibHistograms(I3CalibrationConstPtr calib, 
			    std::string rootFileName){

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

  TH1D* fadc_delta_t_h = new TH1D("fadc_delta_t_h",
			"FADC #Delta T",
			100,-120*I3Units::ns,-80*I3Units::ns);

  TH1D* frontend_impedance_h = new TH1D("frontend_impedance_h",
			"Front End Impededance",
			100,40.,50.);

  TH1D* pmt_transit_time_slope_h = new TH1D("pmt_transit_time_slope_h",
			"PMT Transit Time - slope",
			100,1500,2500);

  TH1D* pmt_transit_time_int_h = new TH1D("pmt_transit_time_intercept_h",
			"PMT Transit Time - intercept",
			100,75,100);

  double bl_min(-3.);
  double bl_max(7.);

  TH1D* atwd0_a_bl_h = new TH1D("atwd0_A_baseline",
			  "ATWD_A Channel 0 Baseline",
			  100,bl_min,bl_max);

  TH1D* atwd1_a_bl_h = new TH1D("atwd1_A_baseline",
			  "ATWD_A Channel 1 Baseline",
			  100,bl_min,bl_max);

  TH1D* atwd2_a_bl_h = new TH1D("atwd2_A_baseline",
			  "ATWD_A Channel 2 Baseline",
			  100,bl_min,bl_max);

  TH1D* atwd0_b_bl_h = new TH1D("atwd0_B_baseline",
			  "ATWD_B Channel 0 Baseline",
			  100,bl_min,bl_max);

  TH1D* atwd1_b_bl_h = new TH1D("atwd1_B_baseline",
			  "ATWD_B Channel 1 Baseline",
			  100,bl_min,bl_max);

  TH1D* atwd2_b_bl_h = new TH1D("atwd2_B_baseline",
			  "ATWD_B Channel 2 Baseline",
			  100,bl_min,bl_max);

  TH1D* atwda_deltat_h = new TH1D("atwda_deltat","ATWD A #Delta t",10,-0.1,0.1);
  TH1D* atwdb_deltat_h = new TH1D("atwdb_deltat","ATWD B #Delta t",20,-2.5,2.5);

  TH1D* spe_disc_calib_slope_h = new TH1D("spe_disc_calib_slope",
					  "SPE Discriminator Slope",
					  20,0.01,0.02);

  TH1D* spe_disc_calib_int_h = new TH1D("spe_disc_calib_int",
					"SPE Discriminator Intercept",
					20,-9,-4);

  TH1D* mpe_disc_calib_slope_h = new TH1D("mpe_disc_calib_slope",
					  "MPE Discriminator Slope",
					  20,0.1,0.18);
  TH1D* mpe_disc_calib_int_h = new TH1D("mpe_disc_calib_int",
					"MPE Discriminator Intercept",
					20,-90,-60);


  map<OMKey, I3DOMCalibration>::const_iterator cal_iter;

  for(cal_iter = calib->domCal.begin();
      cal_iter != calib->domCal.end(); 
      cal_iter++){

    atwda_deltat_h->Fill(cal_iter->second.GetATWDDeltaT(0));
    atwdb_deltat_h->Fill(cal_iter->second.GetATWDDeltaT(1));
    spe_disc_calib_slope_h->Fill(cal_iter->second.GetSPEDiscCalib().slope);
    spe_disc_calib_int_h->Fill(cal_iter->second.GetSPEDiscCalib().intercept);
    mpe_disc_calib_slope_h->Fill(cal_iter->second.GetMPEDiscCalib().slope);
    mpe_disc_calib_int_h->Fill(cal_iter->second.GetMPEDiscCalib().intercept);

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

    fadc_delta_t_h->Fill(cal_iter->second.GetFADCDeltaT()/I3Units::ns);
    cerr<<"FADC DeltaT = "<<cal_iter->second.GetFADCDeltaT()/I3Units::ns<<" ns"<<endl;
    frontend_impedance_h->Fill(cal_iter->second.GetFrontEndImpedance()/I3Units::ohm);

    pmt_transit_time_slope_h->Fill(cal_iter->second.GetTransitTime().slope);
    pmt_transit_time_int_h->Fill(cal_iter->second.GetTransitTime().intercept);
    
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

      atwd0_a_bl_h->Fill(cal_iter->second.GetATWDBaseline(0,0,bin)/I3Units::mV);
      atwd1_a_bl_h->Fill(cal_iter->second.GetATWDBaseline(0,1,bin)/I3Units::mV);
      atwd2_a_bl_h->Fill(cal_iter->second.GetATWDBaseline(0,2,bin)/I3Units::mV);

      atwd0_b_bl_h->Fill(cal_iter->second.GetATWDBaseline(1,0,bin)/I3Units::mV);
      atwd1_b_bl_h->Fill(cal_iter->second.GetATWDBaseline(1,1,bin)/I3Units::mV);
      atwd2_b_bl_h->Fill(cal_iter->second.GetATWDBaseline(1,2,bin)/I3Units::mV);
    }
  }


 TFile f(rootFileName.c_str(),"RECREATE");
  f.cd();
  temp_h->Write();
  fadc_bl_slope_h->Write();
  fadc_bl_int_h->Write();
  fadc_gain_h->Write();
  atwd0_gain_h->Write();
  atwd1_gain_h->Write();
  atwd2_gain_h->Write();
  quadfitA_0_h->Write();
  quadfitB_0_h->Write();
  quadfitC_0_h->Write();
  quadfitA_1_h->Write();
  quadfitB_1_h->Write();
  quadfitC_1_h->Write();

  hvgain_slope_h->Write();
  hvgain_int_h->Write();

  atwd0_a_bc_slope_h->Write();
  atwd1_a_bc_slope_h->Write();
  atwd2_a_bc_slope_h->Write();
  
  atwd0_b_bc_slope_h->Write();
  atwd1_b_bc_slope_h->Write();
  atwd2_b_bc_slope_h->Write();
  
  atwd0_a_bc_int_h->Write();
  atwd1_a_bc_int_h->Write();
  atwd2_a_bc_int_h->Write();
  
  atwd0_b_bc_int_h->Write();
  atwd1_b_bc_int_h->Write();
  atwd2_b_bc_int_h->Write();

  atwda_deltat_h->Write();
  atwdb_deltat_h->Write();
  spe_disc_calib_slope_h->Write();
  spe_disc_calib_int_h->Write();
  mpe_disc_calib_slope_h->Write();
  mpe_disc_calib_int_h->Write();
  
  f.Close();

  SetTemperature(temp_h);
  SetFADCBaselineSlope(fadc_bl_slope_h);
  SetFADCBaselineIntercept(fadc_bl_int_h);
  SetFADCGain(fadc_gain_h);
  SetATWD0Gain(atwd0_gain_h);
  SetATWD1Gain(atwd1_gain_h);
  SetATWD2Gain(atwd2_gain_h);
  SetATWDaFreqFit_A(quadfitA_0_h);
  SetATWDaFreqFit_B(quadfitB_0_h);
  SetATWDaFreqFit_C(quadfitC_0_h);
  SetATWDbFreqFit_A(quadfitA_1_h);
  SetATWDbFreqFit_B(quadfitB_1_h);
  SetATWDbFreqFit_C(quadfitC_1_h);
  SetHVGainSlope(hvgain_slope_h);
  SetHVGainIntercept(hvgain_int_h );
  SetATWD0aBinCalibSlope(atwd0_a_bc_slope_h);
  SetATWD0aBinCalibIntercept(atwd0_a_bc_int_h);
  SetATWD1aBinCalibSlope(atwd1_a_bc_slope_h);
  SetATWD1aBinCalibIntercept(atwd1_a_bc_int_h);
  SetATWD2aBinCalibSlope(atwd2_a_bc_slope_h);
  SetATWD2aBinCalibIntercept(atwd2_a_bc_int_h);
  SetATWD0bBinCalibSlope(atwd0_b_bc_slope_h);
  SetATWD0bBinCalibIntercept(atwd0_b_bc_int_h);
  SetATWD1bBinCalibSlope(atwd1_b_bc_slope_h);
  SetATWD1bBinCalibIntercept(atwd1_b_bc_int_h);
  SetATWD2bBinCalibSlope(atwd2_b_bc_slope_h);
  SetATWD2bBinCalibIntercept(atwd2_b_bc_int_h);

  SetFADCDeltaT(fadc_delta_t_h);
  SetFrontEndImpedance(frontend_impedance_h);
  SetPMTTransitTimeSlope(pmt_transit_time_slope_h);
  SetPMTTransitTimeIntercept(pmt_transit_time_int_h);
  SetATWD0aBaseline(atwd0_a_bl_h);
  SetATWD1aBaseline(atwd1_a_bl_h);
  SetATWD2aBaseline(atwd2_a_bl_h);
  SetATWD0bBaseline(atwd0_b_bl_h);
  SetATWD1bBaseline(atwd1_b_bl_h);
  SetATWD2bBaseline(atwd2_b_bl_h);

  SetATWDaDeltaT(atwda_deltat_h);
  SetATWDbDeltaT(atwdb_deltat_h);
  SetSPEDiscCalibSlope(spe_disc_calib_slope_h);
  SetSPEDiscCalibIntercept(spe_disc_calib_int_h);
  SetMPEDiscCalibSlope(mpe_disc_calib_slope_h);
  SetMPEDiscCalibIntercept(mpe_disc_calib_int_h);
} 

void BookDOMStatusHistograms(I3DetectorStatusConstPtr status, 
			    std::string rootFileName){

  TH1D* lcspan_h = new TH1D("lcspan","Local Coincidence Span",4,-0.5,3.5);
  TH1D* lcWindowPre_h = new TH1D("lcWindowPre","Local Coincidence Window Pre",
		    200,0, 2000.0*I3Units::ns);
  TH1D* lcWindowPost_h = new TH1D("lcWindowPost","Local Coincidence Window Post",
		    200,0, 2000.0*I3Units::ns);
  TH1D* pmtHV_inice_h = new TH1D("pmtHV_inice","PMT High Voltage - InIce",60,1000., 1600.);
  TH1D* pmtHV_icetopHG_h = new TH1D("pmtHV_icetopHG","PMT High Voltage - IceTop High Gain",60,1000., 1600.);
  TH1D* pmtHV_icetopLG_h = new TH1D("pmtHV_icetopLG","PMT High Voltage - IceTop Low Gain",50,600,900.);
  TH1D* trigMode_h = new TH1D("trigMode","Trigger Mode",6,-1.5,4.5);
  TH1D* lcMode_h = new TH1D("lcMode","Local Coincidence Mode",6,-1.5,4.5);
  TH1D* statusATWDa_h = new TH1D("statusATWDa","Status ATWDa",3,-1.5,1.5);
  TH1D* statusATWDb_h = new TH1D("statusATWDb","Status ATWDb",3,-1.5,1.5);
  TH1D* statusFADC_h = new TH1D("statusFADC","Status FADC",3,-1.5,1.5);

  TH1D* speThreshold_h = new TH1D("speThreshold","SPE Threshold",100,500.,800);//I3Units::mV changed to DAC units
  TH1D* mpeThreshold_h = new TH1D("mpeThreshold","MPE Threshold",100,500.,800);//I3Units::mV changed to DAC units
  TH1D* fePedestal_h = new TH1D("fePedestal","FE Pedestal",100,2100,2200);

  TH1D* dacTriggerBias0_h = new TH1D("dacTriggerBias0","DAC Trigger Bias 0",100,700,1000);
  TH1D* dacTriggerBias1_h = new TH1D("dacTriggerBias1","DAC Trigger Bias 1",100,700,1000);
  TH1D* dacFADCRef_h = new TH1D("dacFADCRef","DAC FADC Ref",100,700,900);

  TH1D* nBinsATWD0_h = new TH1D("nBinsATWD0","Number of bins ATWD0",257,0,256);
  TH1D* nBinsATWD1_h = new TH1D("nBinsATWD1","Number of bins ATWD1",257,0,256);
  TH1D* nBinsATWD2_h = new TH1D("nBinsATWD2","Number of bins ATWD2",257,0,256);
  TH1D* nBinsFADC_h = new TH1D("nBinsFADC","Number of bins FADC",257,0,256);

  TH1D* deltaCompress_h = new TH1D("deltaCompress","Delta Compression",3,-1.5,1.5);

  map<OMKey, I3DOMStatus>::const_iterator stat_iter;

  for(stat_iter = status->domStatus.begin();
      stat_iter != status->domStatus.end(); 
      stat_iter++){

    lcspan_h->Fill(stat_iter->second.lcSpan);
    lcWindowPre_h->Fill(stat_iter->second.lcWindowPre);
    lcWindowPost_h->Fill(stat_iter->second.lcWindowPost);
    deltaCompress_h->Fill(stat_iter->second.deltaCompress);

    if(stat_iter->first.GetOM() <=60){
      pmtHV_inice_h->Fill(stat_iter->second.pmtHV/I3Units::volt);
    }else{
      if((stat_iter->first.GetOM() == 61) || 
	 (stat_iter->first.GetOM() == 63))
	pmtHV_icetopHG_h->Fill(stat_iter->second.pmtHV/I3Units::volt);
      if((stat_iter->first.GetOM() == 62) || 
	 (stat_iter->first.GetOM() == 64))
      pmtHV_icetopLG_h->Fill(stat_iter->second.pmtHV/I3Units::volt);
    }

    trigMode_h->Fill(stat_iter->second.trigMode);
    lcMode_h->Fill(stat_iter->second.lcMode);
    statusATWDa_h->Fill(stat_iter->second.statusATWDa);
    statusATWDb_h->Fill(stat_iter->second.statusATWDb);
    statusFADC_h->Fill(stat_iter->second.statusFADC);
    speThreshold_h->Fill(stat_iter->second.speThreshold);
    mpeThreshold_h->Fill(stat_iter->second.mpeThreshold);
    fePedestal_h->Fill(stat_iter->second.fePedestal);
    dacTriggerBias0_h->Fill(stat_iter->second.dacTriggerBias0);
    dacTriggerBias1_h->Fill(stat_iter->second.dacTriggerBias1);
    dacFADCRef_h->Fill(stat_iter->second.dacFADCRef);
    nBinsATWD0_h->Fill(stat_iter->second.nBinsATWD0);
    nBinsATWD1_h->Fill(stat_iter->second.nBinsATWD1);
    nBinsATWD2_h->Fill(stat_iter->second.nBinsATWD2);
    nBinsFADC_h->Fill(stat_iter->second.nBinsFADC);
  }

  TFile g(rootFileName.c_str(),"RECREATE");
  g.cd();

  lcspan_h->Write();
  lcWindowPre_h->Write();
  lcWindowPost_h->Write();
  deltaCompress_h->Write();
  pmtHV_inice_h->Write();
  pmtHV_icetopHG_h->Write();
  pmtHV_icetopLG_h->Write();
  trigMode_h->Write();
  lcMode_h->Write();
  statusATWDa_h->Write();
  statusATWDb_h->Write();
  statusFADC_h->Write();
  speThreshold_h->Write();
  mpeThreshold_h->Write();
  fePedestal_h->Write();
  dacTriggerBias0_h->Write();
  dacTriggerBias1_h->Write();
  dacFADCRef_h->Write();
  nBinsATWD0_h->Write();
  nBinsATWD1_h->Write();
  nBinsATWD2_h->Write();
  nBinsFADC_h->Write();

  g.Close();

  SetLCSpan(lcspan_h);
  SetLCWindowPre(lcWindowPre_h);
  SetLCWindowPost(lcWindowPost_h);
  SetDeltaCompress(deltaCompress_h);
  SetPMTHVInIce(pmtHV_inice_h);
  SetPMTHVIceTopHG(pmtHV_icetopHG_h);
  SetPMTHVIceTopLG(pmtHV_icetopLG_h);
  SetTriggerMode(trigMode_h);
  SetLCMode(lcMode_h);
  SetStatusATWDa(statusATWDa_h);
  SetStatusATWDb(statusATWDb_h);
  SetStatusFADC(statusFADC_h);
  SetSPEThreshold(speThreshold_h);
  SetMPEThreshold(mpeThreshold_h);
  SetFEPedestal(fePedestal_h);
  SetDACTriggerBias0(dacTriggerBias0_h);
  SetDACTriggerBias1(dacTriggerBias1_h);
  SetFADCRef(dacFADCRef_h);
  SetNBinsATWD0(nBinsATWD0_h);
  SetNBinsATWD1(nBinsATWD1_h);
  SetNBinsATWD2(nBinsATWD2_h);
  SetNBinsFADC(nBinsFADC_h);

}

void MakeDOMFunctionsPlots(I3CalibrationConstPtr calib, 
		     I3DetectorStatusConstPtr status){

  TH1D* pmtgain_h = new TH1D("pmtgain","PMT Gain",100,5.5,7.5);
  TH1D* atwda_sampling_rate_h = new TH1D("atwda_sampling_rate",
					 "ATWDa Sampling Rate",100,270,310);
  TH1D* atwdb_sampling_rate_h = new TH1D("atwdb_sampling_rate",
					 "ATWDb Sampling Rate",100,270,310);

  atwda_sampling_rate_h->SetXTitle("rate(MHz)");
  atwdb_sampling_rate_h->SetXTitle("rate(MHz)");

  TH1D* spemean_h = new TH1D("spemean","SPE Mean",100,5.5,7.5);

  TH1D* fadcbaseline_h = new TH1D("fadcbaseline","FADC Baseline",50,110,160);
  fadcbaseline_h->SetXTitle("Baseline(mV)");

  TH1D* ttime_h = new TH1D("ttime","Transit Time",60,130,180);
  ttime_h->SetXTitle("t(ns)");

  TH1D* ic_spethresh_h = new TH1D("ic_spethresh","InIce SPE Discriminator Threshold",60,0,5);
  TH1D* ic_mpethresh_h = new TH1D("ic_mpethresh","InIce MPE Discriminator Threshold",60,120,140);

  TH1D* lg_it_spethresh_h = new TH1D("lg_it_spethresh","Low Gain IceTop SPE Discriminator Threshold",60,0,20);
  TH1D* lg_it_mpethresh_h = new TH1D("lg_it_mpethresh","Low Gain IceTop MPE Discriminator Threshold",60,20,150);

  TH1D* hg_it_spethresh_h = new TH1D("hg_it_spethresh","High Gain IceTop SPE Discriminator Threshold",100,0,70);
  TH1D* hg_it_mpethresh_h = new TH1D("hg_it_mpethresh","High Gain IceTop MPE Discriminator Threshold",60,20,150);

  ic_spethresh_h->SetXTitle("thrs(mV)");
  ic_mpethresh_h->SetXTitle("thrs(mV)");
  lg_it_spethresh_h->SetXTitle("thrs(mV)");
  lg_it_mpethresh_h->SetXTitle("thrs(mV)");
  hg_it_spethresh_h->SetXTitle("thrs(mV)");
  hg_it_mpethresh_h->SetXTitle("thrs(mV)");

  map<OMKey, I3DOMCalibration>::const_iterator cal_iter;

  for(cal_iter = calib->domCal.begin();
      cal_iter != calib->domCal.end(); 
      cal_iter++){

    if(status->domStatus.find(cal_iter->first) != 
       status->domStatus.end()){
      const I3DOMStatus& domstat = status->domStatus.find(cal_iter->first)->second;
      const I3DOMCalibration& domcal = cal_iter->second;

      double gain = PMTGain(domstat,domcal);
      double atwda_sr = ATWDSamplingRate(0,domstat,domcal);
      double atwdb_sr = ATWDSamplingRate(1,domstat,domcal);
      double spemean = SPEMean(domstat,domcal);
      double fadcbaseline = FADCBaseline(domstat,domcal);
      double ttime = TransitTime(domstat,domcal);

      double spethresh = SPEDiscriminatorThreshold(domstat,domcal);
      double mpethresh = MPEDiscriminatorThreshold(domstat,domcal);

      pmtgain_h->Fill(log10(gain));
      atwda_sampling_rate_h->Fill(atwda_sr/I3Units::megahertz);
      atwdb_sampling_rate_h->Fill(atwdb_sr/I3Units::megahertz);
      spemean_h->Fill(log10(spemean/(I3Units::eSI*I3Units::C)));
      fadcbaseline_h->Fill(fadcbaseline);
      ttime_h->Fill(ttime/I3Units::ns);
      if(cal_iter->first.GetOM() <= 60){
	ic_spethresh_h->Fill(spethresh/I3Units::mV);
	ic_mpethresh_h->Fill(mpethresh/I3Units::mV);
      }else{
	if(domstat.domGainType == I3DOMStatus::High){
	  hg_it_spethresh_h->Fill(spethresh/I3Units::mV);
	  hg_it_mpethresh_h->Fill(mpethresh/I3Units::mV);
	}
	if(domstat.domGainType == I3DOMStatus::Low){
	  lg_it_spethresh_h->Fill(spethresh/I3Units::mV);
	  lg_it_mpethresh_h->Fill(mpethresh/I3Units::mV);
	}
      }
    }
  }

  const string I3_BUILD(getenv("I3_BUILD"));
  string plot_path(I3_BUILD + "/sim-services/resources/plots/");

  TCanvas c;
  c.SetLogy(true);
  pmtgain_h->Draw();
  c.SaveAs((plot_path + "dom/PMTGain.png").c_str());

  c.SetLogy(true);
  atwda_sampling_rate_h->Draw();
  c.SaveAs((plot_path + "dom/ATWDaSamplingRate.png").c_str());

  c.SetLogy(true);
  atwdb_sampling_rate_h->Draw();
  c.SaveAs((plot_path + "dom/ATWDbSamplingRate.png").c_str());

  c.SetLogy(true);
  spemean_h->Draw();
  c.SaveAs((plot_path + "dom/SPEMean.png").c_str());

  c.SetLogy(true);
  fadcbaseline_h->Draw();
  c.SaveAs((plot_path + "dom/FADCBasline.png").c_str());

  c.SetLogy(true);
  ttime_h->Draw();
  c.SaveAs((plot_path + "dom/TransitTime.png").c_str());

  c.SetLogy(true);
  ic_spethresh_h->Draw();
  c.SaveAs((plot_path + "dom/InIceSPEDiscThresh.png").c_str());

  c.SetLogy(true);
  ic_mpethresh_h->Draw();
  c.SaveAs((plot_path + "dom/InIceMPEDiscThresh.png").c_str());

  c.SetLogy(true);
  hg_it_spethresh_h->Draw();
  c.SaveAs((plot_path + "dom/HighGainIceTopSPEDiscThresh.png").c_str());

  c.SetLogy(true);
  hg_it_mpethresh_h->Draw();
  c.SaveAs((plot_path + "dom/HighGainIceTopMPEDiscThresh.png").c_str());

  c.SetLogy(true);
  lg_it_spethresh_h->Draw();
  c.SaveAs((plot_path + "dom/LowGainIceTopSPEDiscThresh.png").c_str());

  c.SetLogy(true);
  lg_it_mpethresh_h->Draw();
  c.SaveAs((plot_path + "dom/LowGainIceTopMPEDiscThresh.png").c_str());

} 

void FitAndFormatHisto(TH1D* h, 
		       string filename, 
		       string defVal, 
		       bool fit=false){

  TPaveText* t;
  if(defVal.size() < 30){
    t = new TPaveText(0.55,0.4,0.8,0.5,"NDC");
  }else if((defVal.size() > 30) && (defVal.size()<60)){
    t = new TPaveText(0.35,0.4,0.8,0.5,"NDC");
  }else{
    t = new TPaveText(0.15,0.4,0.8,0.5,"NDC");
  }

  t->AddText(defVal.c_str());
  t->SetTextColor(2);
  t->SetFillColor(0);
  t->SetBorderSize(1);

  TCanvas c;
  if(fit) h->Fit("gaus");
  h->Draw();
  t->Draw();

  const string I3_BUILD(getenv("I3_BUILD"));
  string plot_path(I3_BUILD + "/sim-services/resources/plots/");
  c.SaveAs((plot_path+filename).c_str());
}


void SetTemperature(TH1D* h){

  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::TEMPERATURE
       <<" K";
  h->SetXTitle("T(K)");
  FitAndFormatHisto(h,"calibration/DOMTemperature.png",defVal.str());
};

void SetFADCBaselineSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_BASELINE_FIT_SLOPE
       <<" ADC ticks/DAC-10-Units";
  h->SetXTitle("ADC ticks/DAC-10-Units");
  FitAndFormatHisto(h,"calibration/SetFADCBaselineSlope.png",defVal.str(),true);
};

void SetFADCBaselineIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT
       <<" ADC ticks";
  h->SetXTitle("ADC ticks");
  FitAndFormatHisto(h,"calibration/SetFADCBaselineIntercept.png",defVal.str(),true);
};

void SetFADCGain(TH1D* h){

  TCanvas c;

  TF1* dg = new TF1("dg","gaus(0) + gaus(3)",0,0.02);
  dg->SetParameter(0,90);
  dg->SetParameter(1,0.077e-3);
  dg->SetParameter(2,0.001e-3);
  dg->SetParameter(3,80);
  dg->SetParameter(4,0.09e-3);
  dg->SetParameter(5,0.0025e-3);
  h->Fit("dg"); 

  const string I3_BUILD(getenv("I3_BUILD"));
  string plot_path(I3_BUILD + "/sim-services/resources/plots/");
  c.SaveAs((plot_path+"calibration/FADCGain.png").c_str());
};

void SetATWD0Gain(TH1D* h){

  TCanvas c;

  TF1* dg = new TF1("dg","gaus(0) + gaus(3)",-20,-10);
  dg->SetParameter(0,120);
  dg->SetParameter(1,-16);
  dg->SetParameter(2,0.5);
  dg->SetParameter(3,60);
  dg->SetParameter(4,-18.2);
  dg->SetParameter(5,0.5);
  h->Fit("dg"); 

  const string I3_BUILD(getenv("I3_BUILD"));
  string plot_path(I3_BUILD + "/sim-services/resources/plots/");
  c.SaveAs((plot_path+"calibration/ATWD0Gain.png").c_str());

};

void SetATWD1Gain(TH1D* h){
  TCanvas c;

  TF1* dg = new TF1("dg","gaus(0) + gaus(3)",-2.4,-1);
  dg->SetParameter(0,60);
  dg->SetParameter(1,-2);
  dg->SetParameter(2,0.1);
  dg->SetParameter(3,100);
  dg->SetParameter(4,-1.7);
  dg->SetParameter(5,0.1);
  h->Fit("dg"); 

  const string I3_BUILD(getenv("I3_BUILD"));
  string plot_path(I3_BUILD + "/sim-services/resources/plots/");
  c.SaveAs((plot_path+"calibration/ATWD1Gain.png").c_str());
};

void SetATWD2Gain(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD2_GAIN
       <<" ";
  //h->SetXTitle("Units");
  //FitAndFormatHisto(h,"calibration/ATWD2Gain.png",defVal.str(),true);
  TCanvas c;

  TF1* dg = new TF1("dg","gaus(0) + gaus(3)",-0.3,-0.15);
  dg->SetParameter(0,250);
  dg->SetParameter(1,-0.24);
  dg->SetParameter(2,0.01);
  dg->SetParameter(3,150);
  dg->SetParameter(4,-0.2);
  dg->SetParameter(5,0.005);
  h->Fit("dg"); 

  const string I3_BUILD(getenv("I3_BUILD"));
  string plot_path(I3_BUILD + "/sim-services/resources/plots/");
  c.SaveAs((plot_path+"calibration/ATWD2Gain.png").c_str());
};

void SetATWDaFreqFit_A(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_A
       <<" MHz";
  h->SetXTitle("MHz");
  FitAndFormatHisto(h,"calibration/ATWDaFreqFit_A.png",defVal.str(),true);
};

void SetATWDaFreqFit_B(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_B
       <<" ";
  //h->SetXTitle("g");
  FitAndFormatHisto(h,"calibration/ATWDaFreqFit_B.png",defVal.str(),true);
};

void SetATWDaFreqFit_C(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_C
       <<"";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"calibration/ATWDaFreqFit_C.png",defVal.str(),true);
};

void SetATWDbFreqFit_A(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_A
       <<" MHz";
  h->SetXTitle("MHz");
  FitAndFormatHisto(h,"calibration/ATWDbFreqFit_A.png",defVal.str(),true);
};

void SetATWDbFreqFit_B(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_B
       <<"";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"calibration/ATWDbFreqFit_B.png",defVal.str(),true);
};

void SetATWDbFreqFit_C(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_C
       <<"";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"calibration/ATWDbFreqFit_C.png",defVal.str(),true);
};

void SetHVGainSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::HV_GAIN_FIT_SLOPE
       <<" ";
  h->SetXTitle("Slope");
  FitAndFormatHisto(h,"calibration/HVGainFitSlope.png",defVal.str(),true);
};

void SetHVGainIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::HV_GAIN_FIT_INTERCEPT
       <<" ";
  h->SetXTitle("Intercept");
  FitAndFormatHisto(h,"calibration/HVGainFitIntercept.png",defVal.str(),true);
};

void SetATWD0aBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
 FitAndFormatHisto(h,"calibration/ATWD0aBinCalibSlope.png",defVal.str(),true);
};

void SetATWD0aBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"calibration/ATWD0aBinCalibIntercept.png",defVal.str(),true);
};

void SetATWD1aBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"calibration/ATWD1aBinCalibSlope.png",defVal.str(),true);
};

void SetATWD1aBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"calibration/ATWD1aBinCalibIntercept.png",defVal.str(),true);
};

void SetATWD2aBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"calibration/ATWD2aBinCalibSlope.png",defVal.str(),true);
};

void SetATWD2aBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"calibration/ATWD2aBinCalibIntercept.png",defVal.str(),true);
};


void SetATWD0bBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"calibration/ATWD0bBinCalibSlope.png",defVal.str(),true);
};

void SetATWD0bBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"calibration/ATWD0bBinCalibIntercept.png",defVal.str(),true);
};

void SetATWD1bBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"calibration/ATWD1bBinCalibSlope.png",defVal.str(),true);
};

void SetATWD1bBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"calibration/ATWD1bBinCalibIntercept.png",defVal.str(),true);
};

void SetATWD2bBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"calibration/ATWD2bBinCalibSlope.png",defVal.str(),true);
};

void SetATWD2bBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"calibration/ATWD2bBinCalibIntercept.png",defVal.str(),true);
};

void SetLCSpan(TH1D* h){
  std::stringstream defVal;
  defVal<<"InIce Default = "
	<<I3DetStatDefaults::INICE_LCSPAN<<" "	
	<<"IceTop Default = "
	<<I3DetStatDefaults::ICETOP_LCSPAN
	<<" ";
  h->SetXTitle("Span");
  FitAndFormatHisto(h,"detstat/LCSpan.png",defVal.str());
}
void SetLCWindowPre(TH1D* h){
  std::stringstream defVal;
  defVal<<"IceTop Default = "
	<<I3DetStatDefaults::ICETOP_LCWINDOW_PRE
	<<"ns "
	<<"InIce Default = "
	<<I3DetStatDefaults::INICE_LCWINDOW_PRE
	<<"ns ";
  h->SetXTitle("Window(ns)");
  FitAndFormatHisto(h,"detstat/LCWindowPre.png",defVal.str());

}

void SetLCWindowPost(TH1D* h){
  std::stringstream defVal;
  defVal<<"IceTop Default = "
	<<I3DetStatDefaults::ICETOP_LCWINDOW_POST
	<<"ns "
	<<"InIce Default = "
	<<I3DetStatDefaults::INICE_LCWINDOW_POST
	<<"ns ";
  h->SetXTitle("Window(ns)");
  FitAndFormatHisto(h,"detstat/LCWindowPost.png",defVal.str());

}

void SetDeltaCompress(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3DetStatDefaults::DELTA_COMPRESSION;
  h->SetXTitle("OnOff");
  FitAndFormatHisto(h,"detstat/DeltaCompression.png",defVal.str());

}

void SetPMTHVInIce(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default "
	<<I3DetStatDefaults::INICE_VOLTAGE/I3Units::volt
	<<"V ";
  h->SetXTitle("PMT Voltage(V)");
  FitAndFormatHisto(h,"detstat/PMTVoltageInIce.png",defVal.str(),true);

}

void SetPMTHVIceTopHG(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default "
	<<I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE/I3Units::volt
	<<"V ";
  h->SetXTitle("PMT Voltage(V)");
  FitAndFormatHisto(h,"detstat/PMTVoltageIceTopHG.png",defVal.str(),true);

}

void SetPMTHVIceTopLG(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default "
	<<I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE/I3Units::volt
	<<"V ";
  h->SetXTitle("PMT Voltage(V)");
  FitAndFormatHisto(h,"detstat/PMTVoltageIceTopLG.png",defVal.str(),true);

}

void SetTriggerMode(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::INICE_TRIGGER_MODE)
       <<" ";
  h->SetXTitle("Mode");
  FitAndFormatHisto(h,"detstat/TriggerMode.png",defVal.str());

}
void SetLCMode(TH1D* h){
  std::stringstream defVal;
  defVal<<" First InIce Default = "
	<<static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST)<<" "
	<<" Bulk InIce Default = "
	<<static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK)<<" "
	<<" Last InIce Default = "
	<<static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST)<<" "
	<<" IceTop InIce Default = "
	<<static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP)<<" ";       
  h->SetXTitle("LC Mode");
  FitAndFormatHisto(h,"detstat/LCMode.png",defVal.str());

}
void SetStatusATWDa(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_ATWDa)
	<<" ";
  h->SetXTitle("ATWDa Status");
  FitAndFormatHisto(h,"detstat/ATWDaStatus.png",defVal.str());

}
void SetStatusATWDb(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_ATWDb)
	<<" ";
  h->SetXTitle("ATWDb Status ");
  FitAndFormatHisto(h,"detstat/ATWDbStatus.png",defVal.str());

}
void SetStatusFADC(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<" InIce = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE)
	<<"  IceTop = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP)
	<<" ";
  h->SetXTitle("FADC Status");
  FitAndFormatHisto(h,"detstat/FADCStatus.png",defVal.str());

}

void SetSPEThreshold(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3DetStatDefaults::INICE_SPE_THRESHOLD;
  h->SetXTitle("threshold(DAC)");
  FitAndFormatHisto(h,"detstat/SPEThreshold.png",defVal.str(),true);

}

void SetMPEThreshold(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3DetStatDefaults::INICE_MPE_THRESHOLD;
  h->SetXTitle("threshold(DAC)");
  FitAndFormatHisto(h,"detstat/MPEThreshold.png",defVal.str(),true);

}
void SetFEPedestal(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3DetStatDefaults::FE_PEDESTAL;
  h->SetXTitle("pedestal(DAC)");
  FitAndFormatHisto(h,"detstat/fePedestal.png",defVal.str());

}
void SetDACTriggerBias0(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_TRIGGER_BIAS0
       <<" ";
  h->SetXTitle(" ");
  FitAndFormatHisto(h,"detstat/DACTriggBias0.png",defVal.str());

}
void SetDACTriggerBias1(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_TRIGGER_BIAS1
       <<" ";
  h->SetXTitle(" ");
  FitAndFormatHisto(h,"detstat/DACTriggBias1.png",defVal.str());

}
void SetFADCRef(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_FADC_REF
       <<" ";
  h->SetXTitle(" ");
  FitAndFormatHisto(h,"detstat/FADCRef.png",defVal.str());

}
void SetNBinsATWD0(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<" InIce = "
	<<I3DetStatDefaults::NBINS_ATWD0_INICE
	<<" bins"
	<<"   IceTop = "
	<<I3DetStatDefaults::NBINS_ATWD0_ICETOP
	<<" bins";
  h->SetXTitle("N Bins");
  FitAndFormatHisto(h,"detstat/NBinsATWD0.png",defVal.str());

}
void SetNBinsATWD1(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<" InIce = "
	<<I3DetStatDefaults::NBINS_ATWD1_INICE
	<<" bins"
	<<"   IceTop = "
	<<I3DetStatDefaults::NBINS_ATWD1_ICETOP
	<<" bins";
  h->SetXTitle("N Bins");
  FitAndFormatHisto(h,"detstat/NBinsATWD1.png",defVal.str());

}
void SetNBinsATWD2(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<" InIce = "
	<<I3DetStatDefaults::NBINS_ATWD2_INICE
	<<" bins"
	<<"   IceTop = "
	<<I3DetStatDefaults::NBINS_ATWD2_ICETOP
	<<" bins";
  h->SetXTitle("N Bins");
  FitAndFormatHisto(h,"detstat/NBinsATWD2.png",defVal.str());

}
void SetNBinsFADC(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<" InIce = "
	<<I3DetStatDefaults::NBINS_FADC_INICE
	<<" bins"
	<<"   IceTop = "
	<<I3DetStatDefaults::NBINS_FADC_ICETOP
	<<" bins";
  h->SetXTitle("N Bins");
  FitAndFormatHisto(h,"detstat/NBinsFADC.png",defVal.str());

}

void SetFADCDeltaT(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::FADC_DELTA_T/I3Units::ns
	<<" ";
  h->SetXTitle("t(ns)");
  FitAndFormatHisto(h,"calibration/FADCDeltaT.png",defVal.str(),true);
}

void SetFrontEndImpedance(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::FRONT_END_IMPEDANCE/I3Units::ohm
	<<" ";
  h->SetXTitle("");
  FitAndFormatHisto(h,"calibration/FrontEndImpedance.png",defVal.str());
}

void SetPMTTransitTimeSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
    <<I3CalibDefaults::PMT_TRANSIT_TIME_SLOPE
	<<" ";
  h->SetXTitle("slope(ns/sqrt(V))");
  FitAndFormatHisto(h,"calibration/PMTTransitTimeSlope.png",defVal.str(),true);
}

void SetPMTTransitTimeIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::PMT_TRANSIT_TIME_INTERCEPT/I3Units::ns
	<<" ";
  h->SetXTitle("intercept(ns)");
  FitAndFormatHisto(h,"calibration/PMTTransitTimeIntercept.png",defVal.str(),true);
}

void SetATWD0aBaseline(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDA0_BASELINE/I3Units::mV
	<<" mV";
  h->SetXTitle("Baseline(mV)");

  FitAndFormatHisto(h,"calibration/ATWD0aBaseline.png",defVal.str(),true);
}

void SetATWD1aBaseline(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDA1_BASELINE/I3Units::mV
	<<" mV";
  h->SetXTitle("Baseline(mV)");

  FitAndFormatHisto(h,"calibration/ATWD1aBaseline.png",defVal.str(),true);
}

void SetATWD2aBaseline(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDA2_BASELINE/I3Units::mV
	<<" mV";
  h->SetXTitle("Baseline(mV)");

  FitAndFormatHisto(h,"calibration/ATWD2aBaseline.png",defVal.str(),true);
}

void SetATWD0bBaseline(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDB0_BASELINE/I3Units::mV
	<<" mV";
  h->SetXTitle("Baseline(mV)");

  FitAndFormatHisto(h,"calibration/ATWD0bBaseline.png",defVal.str(),true);
}

void SetATWD1bBaseline(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDB1_BASELINE/I3Units::mV
	<<" mV";
  h->SetXTitle("Baseline(mV)");

  FitAndFormatHisto(h,"calibration/ATWD1bBaseline.png",defVal.str(),true);
}

void SetATWD2bBaseline(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDB2_BASELINE/I3Units::mV
	<<" mV";
  h->SetXTitle("Baseline(mV)");

  FitAndFormatHisto(h,"calibration/ATWD2bBaseline.png",defVal.str(),true);
}

void SetATWDaDeltaT(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDA_DELTAT/I3Units::ns
       <<" ns";
  h->SetXTitle("#Delta t(ns)");
  FitAndFormatHisto(h,"calibration/ATWDaDeltaT.png",defVal.str());
};

void SetATWDbDeltaT(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::ATWDB_DELTAT/I3Units::ns
	<<" ns";
  h->SetXTitle("#Delta t(ns)");
  FitAndFormatHisto(h,"calibration/ATWDbDeltaT.png",defVal.str(),true);
};

void SetSPEDiscCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE
	<<" ??";
  h->SetXTitle("Slope()");
  FitAndFormatHisto(h,"calibration/SPEDiscCalibSlope.png",defVal.str(),true);
};

void SetSPEDiscCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT
	<<" ??";
  h->SetXTitle("Intercept()");
  FitAndFormatHisto(h,"calibration/SPEDiscCalibIntercept.png",defVal.str(),true);
};

void SetMPEDiscCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE
	<<" ??";
  h->SetXTitle("Slope()");
  FitAndFormatHisto(h,"calibration/MPEDiscCalibSlope.png",defVal.str(),true);
};

void SetMPEDiscCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT
	<<" ??";
  h->SetXTitle("Intercept()");
  FitAndFormatHisto(h,"calibration/MPEDiscCalibIntercept.png",defVal.str(),true);
};
