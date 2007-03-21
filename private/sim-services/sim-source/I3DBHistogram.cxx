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
#include <phys-services/I3CalibrationService.h>
#include <phys-services/I3DetectorStatusService.h>
#include <phys-services/I3EventService.h>
#include <dataclasses/I3Time.h>
#include <dataclasses/I3Units.h>
#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"

#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TStyle.h>

I3_MODULE(I3DBHistogram);

void BookDOMCalibHistograms(I3CalibrationConstPtr , std::string );
void BookDOMStatusHistograms(I3DetectorStatusConstPtr , std::string );

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
void SetPMTHVInIce(TH1D*);
void SetPMTHVIceTopHG(TH1D*);
void SetPMTHVIceTopLG(TH1D*);
void SetTriggerMode(TH1D*);
void SetLCMode(TH1D*);
void SetStatusATWDa(TH1D*);
void SetStatusATWDb(TH1D*);
void SetStatusFADC(TH1D*);
void SetSPEThreshold(TH1D*);
void SetFEPedestal(TH1D*);
void SetDACTriggerBias0(TH1D*);
void SetDACTriggerBias1(TH1D*);
void SetFADCRef(TH1D*);
void SetNBinsATWD0(TH1D*);
void SetNBinsATWD1(TH1D*);
void SetNBinsATWD2(TH1D*);
void SetNBinsFADC(TH1D*);

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

void I3DBHistogram::Physics(I3FramePtr frame)
{
  log_debug("Physics");

  I3CalibrationConstPtr calib = 
    frame->Get<I3CalibrationConstPtr>();
  
  BookDOMCalibHistograms(calib,"DBDomCalib.root");

  I3DetectorStatusConstPtr status = 
    frame->Get<I3DetectorStatusConstPtr>();

  BookDOMStatusHistograms(status,"DBDomStatus.root");

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
  TH1D* pmtHV_icetopLG_h = new TH1D("pmtHV_icetopLG","PMT High Voltage - IceTop Low Gain",50,400,900.);
  TH1D* trigMode_h = new TH1D("trigMode","Trigger Mode",6,-1.5,4.5);
  TH1D* lcMode_h = new TH1D("lcMode","Local Coincidence Mode",6,-1.5,4.5);
  TH1D* statusATWDa_h = new TH1D("statusATWDa","Status ATWDa",3,-1.5,1.5);
  TH1D* statusATWDb_h = new TH1D("statusATWDb","Status ATWDb",3,-1.5,1.5);
  TH1D* statusFADC_h = new TH1D("statusFADC","Status FADC",3,-1.5,1.5);

  TH1D* speThreshold_h = new TH1D("speThreshold","SPE Threshold",100,1.,2.);//I3Units::mV
  TH1D* fePedestal_h = new TH1D("fePedestal","FE Pedestal",100,2.5,2.7);//I3Units::V

  TH1D* dacTriggerBias0_h = new TH1D("dacTriggerBias0","DAC Trigger Bias 0",100,0,1000);
  TH1D* dacTriggerBias1_h = new TH1D("dacTriggerBias1","DAC Trigger Bias 1",100,0,1000);
  TH1D* dacFADCRef_h = new TH1D("dacFADCRef","DAC FADC Ref",100,0,1000);

  TH1D* nBinsATWD0_h = new TH1D("nBinsATWD0","Number of bins ATWD0",257,0,256);
  TH1D* nBinsATWD1_h = new TH1D("nBinsATWD1","Number of bins ATWD1",257,0,256);
  TH1D* nBinsATWD2_h = new TH1D("nBinsATWD2","Number of bins ATWD2",257,0,256);
  TH1D* nBinsFADC_h = new TH1D("nBinsFADC","Number of bins FADC",257,0,256);

  map<OMKey, I3DOMStatus>::const_iterator stat_iter;

  for(stat_iter = status->domStatus.begin();
      stat_iter != status->domStatus.end(); 
      stat_iter++){

    lcspan_h->Fill(stat_iter->second.lcSpan);
    lcWindowPre_h->Fill(stat_iter->second.lcWindowPre);
    lcWindowPost_h->Fill(stat_iter->second.lcWindowPost);

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
    speThreshold_h->Fill(stat_iter->second.speThreshold/I3Units::mV);
    fePedestal_h->Fill(stat_iter->second.fePedestal/I3Units::V);
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
  pmtHV_inice_h->Write();
  pmtHV_icetopHG_h->Write();
  pmtHV_icetopLG_h->Write();
  trigMode_h->Write();
  lcMode_h->Write();
  statusATWDa_h->Write();
  statusATWDb_h->Write();
  statusFADC_h->Write();
  speThreshold_h->Write();
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
  SetPMTHVInIce(pmtHV_inice_h);
  SetPMTHVIceTopHG(pmtHV_icetopHG_h);
  SetPMTHVIceTopLG(pmtHV_icetopLG_h);
  SetTriggerMode(trigMode_h);
  SetLCMode(lcMode_h);
  SetStatusATWDa(statusATWDa_h);
  SetStatusATWDb(statusATWDb_h);
  SetStatusFADC(statusFADC_h);
  SetSPEThreshold(speThreshold_h);
  SetFEPedestal(fePedestal_h);
  SetDACTriggerBias0(dacTriggerBias0_h);
  SetDACTriggerBias1(dacTriggerBias1_h);
  SetFADCRef(dacFADCRef_h);
  SetNBinsATWD0(nBinsATWD0_h);
  SetNBinsATWD1(nBinsATWD1_h);
  SetNBinsATWD2(nBinsATWD2_h);
  SetNBinsFADC(nBinsFADC_h);

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

  const string I3_WORK(getenv("I3_WORK"));
  string plot_path(I3_WORK + "/sim-services/resources/plots/");
  c.SaveAs((plot_path+filename).c_str());
}


void SetTemperature(TH1D* h){

  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::TEMPERATURE
       <<" K";
  h->SetXTitle("T(K)");
  FitAndFormatHisto(h,"DOMTemperature.gif",defVal.str());
};

void SetFADCBaselineSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_BASELINE_FIT_SLOPE
       <<" ADC ticks/DAC-10-Units";
  h->SetXTitle("ADC ticks/DAC-10-Units");
  FitAndFormatHisto(h,"SetFADCBaselineSlope.gif",defVal.str(),true);
};

void SetFADCBaselineIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT
       <<" ADC ticks";
  h->SetXTitle("ADC ticks");
  FitAndFormatHisto(h,"SetFADCBaselineIntercept.gif",defVal.str(),true);
};

void SetFADCGain(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_GAIN/I3Units::V
       <<" V/tick";
  h->SetXTitle("FADC Gain(V/tick)");
  FitAndFormatHisto(h,"FADCGain.gif",defVal.str(),true);
};

void SetATWD0Gain(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD0_GAIN
       <<" ";
  //h->SetXTitle("Units ???");
  FitAndFormatHisto(h,"ATWD0Gain.gif",defVal.str(),true);
};

void SetATWD1Gain(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD1_GAIN
       <<" ";
  //h->SetXTitle("Units???");
  FitAndFormatHisto(h,"ATWD1Gain.gif",defVal.str(),true);
};

void SetATWD2Gain(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD2_GAIN
       <<" ";
  //h->SetXTitle("Units");
  FitAndFormatHisto(h,"ATWD2Gain.gif",defVal.str(),true);
};

void SetATWDaFreqFit_A(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_A
       <<" MHz";
  h->SetXTitle("MHz");
  FitAndFormatHisto(h,"ATWDaFreqFit_A.gif",defVal.str(),true);
};

void SetATWDaFreqFit_B(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_B
       <<" ";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"ATWDaFreqFit_B.gif",defVal.str(),true);
};

void SetATWDaFreqFit_C(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_C
       <<"";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"ATWDaFreqFit_C.gif",defVal.str(),true);
};

void SetATWDbFreqFit_A(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_A
       <<" MHz";
  h->SetXTitle("MHz");
  FitAndFormatHisto(h,"ATWDbFreqFit_A.gif",defVal.str(),true);
};

void SetATWDbFreqFit_B(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_B
       <<"";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"ATWDbFreqFit_B.gif",defVal.str(),true);
};

void SetATWDbFreqFit_C(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_C
       <<"";
  //h->SetXTitle("");
  FitAndFormatHisto(h,"ATWDbFreqFit_C.gif",defVal.str(),true);
};

void SetHVGainSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::HV_GAIN_FIT_SLOPE
       <<" ";
  h->SetXTitle("Slope");
  FitAndFormatHisto(h,"HVGainFitSlope.gif",defVal.str(),true);
};

void SetHVGainIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<I3CalibDefaults::HV_GAIN_FIT_INTERCEPT
       <<" ";
  h->SetXTitle("Intercept");
  FitAndFormatHisto(h,"HVGainFitIntercept.gif",defVal.str(),true);
};

void SetATWD0aBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
 FitAndFormatHisto(h,"ATWD0aBinCalibSlope.gif",defVal.str(),true);
};

void SetATWD0aBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"ATWD0aBinCalibIntercept.gif",defVal.str(),true);
};

void SetATWD1aBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"ATWD1aBinCalibSlope.gif",defVal.str(),true);
};

void SetATWD1aBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"ATWD1aBinCalibIntercept.gif",defVal.str(),true);
};

void SetATWD2aBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"ATWD2aBinCalibSlope.gif",defVal.str(),true);
};

void SetATWD2aBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"ATWD2aBinCalibIntercept.gif",defVal.str(),true);
};


void SetATWD0bBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"ATWD0bBinCalibSlope.gif",defVal.str(),true);
};

void SetATWD0bBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"ATWD0bBinCalibIntercept.gif",defVal.str(),true);
};

void SetATWD1bBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"ATWD1bBinCalibSlope.gif",defVal.str(),true);
};

void SetATWD1bBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"ATWD1bBinCalibIntercept.gif",defVal.str(),true);
};

void SetATWD2bBinCalibSlope(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE/I3Units::V
       <<" V";
  h->SetXTitle("Slope(V)");
  FitAndFormatHisto(h,"ATWD2bBinCalibSlope.gif",defVal.str(),true);
};

void SetATWD2bBinCalibIntercept(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT/I3Units::V
       <<" V";
  h->SetXTitle("Intercept(V)");
  FitAndFormatHisto(h,"ATWD2bBinCalibIntercept.gif",defVal.str(),true);
};

void SetLCSpan(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::LCSPAN
       <<" ";
  h->SetXTitle("Span");
  FitAndFormatHisto(h,"LCSpan.gif",defVal.str());
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
  FitAndFormatHisto(h,"LCWindowPre.gif",defVal.str());

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
  FitAndFormatHisto(h,"LCWindowPost.gif",defVal.str());

}

void SetPMTHVInIce(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default "
	<<I3DetStatDefaults::INICE_VOLTAGE/I3Units::volt
	<<"V ";
  h->SetXTitle("PMT Voltage(V)");
  FitAndFormatHisto(h,"PMTVoltageInIce.gif",defVal.str());

}

void SetPMTHVIceTopHG(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default "
	<<I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE/I3Units::volt
	<<"V ";
  h->SetXTitle("PMT Voltage(V)");
  FitAndFormatHisto(h,"PMTVoltageIceTopHG.gif",defVal.str());

}

void SetPMTHVIceTopLG(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default "
	<<I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE/I3Units::volt
	<<"V ";
  h->SetXTitle("PMT Voltage(V)");
  FitAndFormatHisto(h,"PMTVoltageIceTopLG.gif",defVal.str());

}

void SetTriggerMode(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::TRIGGER_MODE)
       <<" ";
  h->SetXTitle("Mode");
  FitAndFormatHisto(h,"TriggerMode.gif",defVal.str());

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
  FitAndFormatHisto(h,"LCMode.gif",defVal.str());

}
void SetStatusATWDa(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_ATWDa)
	<<" ";
  h->SetXTitle("ATWDa Status");
  FitAndFormatHisto(h,"ATWDaStatus.gif",defVal.str());

}
void SetStatusATWDb(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_ATWDb)
	<<" ";
  h->SetXTitle("ATWDb Status ");
  FitAndFormatHisto(h,"ATWDbStatus.gif",defVal.str());

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
  FitAndFormatHisto(h,"FADCStatus.gif",defVal.str());

}

void SetSPEThreshold(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::SPE_THRESHOLD/I3Units::mV
       <<" mV";
  h->SetXTitle("threshold(mV)");
  FitAndFormatHisto(h,"SPEThreshold.gif",defVal.str());

}
void SetFEPedestal(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::FE_PEDESTAL/I3Units::V
       <<"V ";
  h->SetXTitle("pedestal(V)");
  FitAndFormatHisto(h,"fePedestal.gif",defVal.str());

}
void SetDACTriggerBias0(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_TRIGGER_BIAS0
       <<" ";
  h->SetXTitle(" ");
  FitAndFormatHisto(h,"DACTriggBias0.gif",defVal.str());

}
void SetDACTriggerBias1(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_TRIGGER_BIAS1
       <<" ";
  h->SetXTitle(" ");
  FitAndFormatHisto(h,"DACTriggBias1.gif",defVal.str());

}
void SetFADCRef(TH1D* h){
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_FADC_REF
       <<" ";
  h->SetXTitle(" ");
  FitAndFormatHisto(h,"FADCRef.gif",defVal.str());

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
  FitAndFormatHisto(h,"NBinsATWD0.gif",defVal.str());

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
  FitAndFormatHisto(h,"NBinsATWD1.gif",defVal.str());

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
  FitAndFormatHisto(h,"NBinsATWD2.gif",defVal.str());

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
  FitAndFormatHisto(h,"NBinsFADC.gif",defVal.str());

}

