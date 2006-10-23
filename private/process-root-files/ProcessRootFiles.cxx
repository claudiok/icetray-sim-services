#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include "sim-services/sim-source/I3DefaultValues.h"
#include <sstream>
#include <TStyle.h>

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
void SetPMTHV(TH1D*);
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


void ProcessCalibration();
void ProcessDetectorStatus();

int main(){
  gROOT->SetStyle("Plain");
  gStyle->SetFrameLineWidth(2);

  ProcessCalibration();
  ProcessDetectorStatus();

  return 0;
}

void ProcessCalibration(){
  TFile f("DBDomCalib.root");

  TH1D* temp_h = (TH1D*)f.Get("temp");
  SetTemperature(temp_h);

  TH1D* fadc_bl_slope_h = (TH1D*)f.Get("fadc_bl_slope");
  SetFADCBaselineSlope(fadc_bl_slope_h);

  TH1D* fadc_bl_int_h = (TH1D*)f.Get("fadc_bl_int");
  SetFADCBaselineIntercept(fadc_bl_int_h);

  TH1D* fadc_gain_h = (TH1D*)f.Get("fadc_gain");
  SetFADCGain(fadc_gain_h);

  TH1D* atwd0_gain_h = (TH1D*)f.Get("atwd0_gain");
  SetATWD0Gain(atwd0_gain_h);

  TH1D* atwd1_gain_h = (TH1D*)f.Get("atwd1_gain");
  SetATWD1Gain(atwd1_gain_h);

  TH1D* atwd2_gain_h = (TH1D*)f.Get("atwd2_gain");
  SetATWD2Gain(atwd2_gain_h);
		    
  TH1D* quadfitA_0_h = (TH1D*)f.Get("quadfitA_0");
  SetATWDaFreqFit_A(quadfitA_0_h);

  TH1D* quadfitB_0_h = (TH1D*)f.Get("quadfitB_0");
  SetATWDaFreqFit_B(quadfitB_0_h);

  TH1D* quadfitC_0_h = (TH1D*)f.Get("quadfitC_0");
  SetATWDaFreqFit_C(quadfitC_0_h);

  TH1D* quadfitA_1_h = (TH1D*)f.Get("quadfitA_1");
  SetATWDbFreqFit_A(quadfitA_1_h);

  TH1D* quadfitB_1_h = (TH1D*)f.Get("quadfitB_1");
  SetATWDbFreqFit_B(quadfitB_1_h);

  TH1D* quadfitC_1_h = (TH1D*)f.Get("quadfitC_1");
  SetATWDbFreqFit_C(quadfitC_1_h);

  TH1D* hvgain_slope_h = (TH1D*)f.Get("hvgain_slope");
  SetHVGainSlope(hvgain_slope_h);

  TH1D* hvgain_int_h = (TH1D*)f.Get("hvgain_int");
  SetHVGainIntercept(hvgain_int_h );

  TH1D* atwd0_a_bc_slope_h = (TH1D*)f.Get("atwd0_A_bc_slope");
  SetATWD0aBinCalibSlope(atwd0_a_bc_slope_h);

  TH1D* atwd0_a_bc_int_h = (TH1D*)f.Get("atwd0_A_bc_int");
  SetATWD0aBinCalibIntercept(atwd0_a_bc_int_h);

  TH1D* atwd1_a_bc_slope_h = (TH1D*)f.Get("atwd1_A_bc_slope");
  SetATWD1aBinCalibSlope(atwd1_a_bc_slope_h);

  TH1D* atwd1_a_bc_int_h = (TH1D*)f.Get("atwd1_A_bc_int");
  SetATWD1aBinCalibIntercept(atwd1_a_bc_int_h);

  TH1D* atwd2_a_bc_slope_h = (TH1D*)f.Get("atwd2_A_bc_slope");
  SetATWD2aBinCalibSlope(atwd2_a_bc_slope_h);

  TH1D* atwd2_a_bc_int_h = (TH1D*)f.Get("atwd2_A_bc_int");
  SetATWD2aBinCalibIntercept(atwd2_a_bc_int_h);

  TH1D* atwd0_b_bc_slope_h = (TH1D*)f.Get("atwd0_B_bc_slope");
  SetATWD0bBinCalibSlope(atwd0_b_bc_slope_h);

  TH1D* atwd0_b_bc_int_h = (TH1D*)f.Get("atwd0_B_bc_int");
  SetATWD0bBinCalibIntercept(atwd0_b_bc_int_h);

  TH1D* atwd1_b_bc_slope_h = (TH1D*)f.Get("atwd1_B_bc_slope");
  SetATWD1bBinCalibSlope(atwd1_b_bc_slope_h);

  TH1D* atwd1_b_bc_int_h = (TH1D*)f.Get("atwd1_B_bc_int");
  SetATWD1bBinCalibIntercept(atwd1_b_bc_int_h);

  TH1D* atwd2_b_bc_slope_h = (TH1D*)f.Get("atwd2_B_bc_slope");
  SetATWD2bBinCalibSlope(atwd2_b_bc_slope_h);

  TH1D* atwd2_b_bc_int_h = (TH1D*)f.Get("atwd2_B_bc_int");
  SetATWD2bBinCalibIntercept(atwd2_b_bc_int_h);

  TFile g("DBDomCalib-Processed.root","RECREATE");
  g.cd();
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
  g.Close();
}


void ProcessDetectorStatus(){
  TFile f("DBDomStatus.root");

  TH1D* lcspan_h = (TH1D*)f.Get("lcspan");
  SetLCSpan(lcspan_h);

  TH1D* lcWindowPre_h = (TH1D*)f.Get("lcWindowPre");
  SetLCWindowPre(lcWindowPre_h);

  TH1D* lcWindowPost_h = (TH1D*)f.Get("lcWindowPost");
  SetLCWindowPost(lcWindowPost_h);

  TH1D* pmtHV_h = (TH1D*)f.Get("pmtHV"); 
  SetPMTHV(pmtHV_h);

  TH1D* trigMode_h = (TH1D*)f.Get("trigMode");
  SetTriggerMode(trigMode_h);

  TH1D* lcMode_h = (TH1D*)f.Get("lcMode");
  SetLCMode(lcMode_h);

  TH1D* statusATWDa_h = (TH1D*)f.Get("statusATWDa");
  SetStatusATWDa(statusATWDa_h);

  TH1D* statusATWDb_h = (TH1D*)f.Get("statusATWDb");
  SetStatusATWDb(statusATWDb_h);

  TH1D* statusFADC_h = (TH1D*)f.Get("statusFADC");
  SetStatusFADC(statusFADC_h);

  TH1D* speThreshold_h = (TH1D*)f.Get("speThreshold");
  SetSPEThreshold(speThreshold_h);

  TH1D* fePedestal_h = (TH1D*)f.Get("fePedestal");
  SetFEPedestal(fePedestal_h);

  TH1D* dacTriggerBias0_h = (TH1D*)f.Get("dacTriggerBias0");
  SetDACTriggerBias0(dacTriggerBias0_h);

  TH1D* dacTriggerBias1_h = (TH1D*)f.Get("dacTriggerBias1");
  SetDACTriggerBias1(dacTriggerBias1_h);

  TH1D* dacFADCRef_h = (TH1D*)f.Get("dacFADCRef");
  SetFADCRef(dacFADCRef_h);

  TH1D* nBinsATWD0_h = (TH1D*)f.Get("nBinsATWD0");
  SetNBinsATWD0(nBinsATWD0_h);

  TH1D* nBinsATWD1_h = (TH1D*)f.Get("nBinsATWD1");
  SetNBinsATWD1(nBinsATWD1_h);

  TH1D* nBinsATWD2_h = (TH1D*)f.Get("nBinsATWD2");
  SetNBinsATWD2(nBinsATWD2_h);

  TH1D* nBinsFADC_h = (TH1D*)f.Get("nBinsFADC");
  SetNBinsFADC(nBinsFADC_h);


  TFile g("DBDomStatus-Processed.root","RECREATE");
  g.cd();

  lcspan_h->Write();
  lcWindowPre_h->Write();
  lcWindowPost_h->Write();
  pmtHV_h->Write();
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
}

void DrawHisto(TH1D* h, string filename, string defVal){

  TPaveText* t = new TPaveText(0.55,0.66,0.8,0.7,"NDC");
  t->AddText(defVal.c_str());
  t->SetTextColor(2);
  t->SetFillColor(0);
  t->SetBorderSize(1);

  TCanvas c;
  h->Draw();
  t->Draw();
  c.SaveAs(filename.c_str());
}

void SetTemperature(TH1D* h){

  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::TEMPERATURE
       <<" K";
  h->SetXTitle("T(K)");
  DrawHisto(h,"DOMTemperature.gif",defVal.str());
};

void SetFADCBaselineSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_BASELINE_FIT_SLOPE
       <<" ADC ticks/DAC-10-Units";
  h->SetXTitle("ADC ticks/DAC-10-Units");
  DrawHisto(h,"SetFADCBaselineSlope.gif",defVal.str());
};

void SetFADCBaselineIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT
       <<" ADC ticks";
  h->SetXTitle("ADC ticks");
  DrawHisto(h,"SetFADCBaselineIntercept.gif",defVal.str());
};

void SetFADCGain(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::FADC_GAIN
       <<" V/tick";
  h->SetXTitle("FADC Gain(V/tick)");
  DrawHisto(h,"FADCGain.gif",defVal.str());
};

void SetATWD0Gain(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD0_GAIN
       <<" ";
  //h->SetXTitle("Units ???");
  DrawHisto(h,"ATWD0Gain.gif",defVal.str());
};

void SetATWD1Gain(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD1_GAIN
       <<" ";
  //h->SetXTitle("Units???");
  DrawHisto(h,"ATWD1Gain.gif",defVal.str());
};

void SetATWD2Gain(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD2_GAIN
       <<" ";
  //h->SetXTitle("Units");
  DrawHisto(h,"ATWD2Gain.gif",defVal.str());
};

void SetATWDaFreqFit_A(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_A
       <<" MHz";
  h->SetXTitle("MHz");
  DrawHisto(h,"ATWDaFreqFit_A.gif",defVal.str());
};

void SetATWDaFreqFit_B(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_B
       <<" ";
  //h->SetXTitle("");
  DrawHisto(h,"ATWDaFreqFit_B.gif",defVal.str());
};

void SetATWDaFreqFit_C(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_A_FREQFIT_C
       <<"";
  //h->SetXTitle("");
  DrawHisto(h,"ATWDaFreqFit_C.gif",defVal.str());
};

void SetATWDbFreqFit_A(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_A
       <<" MHz";
  h->SetXTitle("MHz");
  DrawHisto(h,"ATWDbFreqFit_A.gif",defVal.str());
};

void SetATWDbFreqFit_B(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_B
       <<"";
  //h->SetXTitle("");
  DrawHisto(h,"ATWDbFreqFit_B.gif",defVal.str());
};

void SetATWDbFreqFit_C(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_B_FREQFIT_C
       <<"";
  //h->SetXTitle("");
  DrawHisto(h,"ATWDbFreqFit_C.gif",defVal.str());
};

void SetHVGainSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::HV_GAIN_FIT_SLOPE
       <<" ";
  h->SetXTitle("Slope");
  DrawHisto(h,"HVGainFitSlope.gif",defVal.str());
};

void SetHVGainIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::HV_GAIN_FIT_INTERCEPT
       <<" ";
  h->SetXTitle("Intercept");
  DrawHisto(h,"HVGainFitIntercept.gif",defVal.str());
};

void SetATWD0aBinCalibSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE
       <<" V";
  h->SetXTitle("Slope(V)");
  DrawHisto(h,"ATWD0aBinCalibSlope.gif",defVal.str());
};

void SetATWD0aBinCalibIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT
       <<" V";
  h->SetXTitle("Intercept(V)");
  DrawHisto(h,"ATWD0aBinCalibIntercept.gif",defVal.str());
};

void SetATWD1aBinCalibSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE
       <<" V";
  h->SetXTitle("Slope(V)");
  DrawHisto(h,"ATWD1aBinCalibSlope.gif",defVal.str());
};

void SetATWD1aBinCalibIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT
       <<" V";
  h->SetXTitle("Intercept(V)");
  DrawHisto(h,"ATWD1aBinCalibIntercept.gif",defVal.str());
};

void SetATWD2aBinCalibSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE
       <<" V";
  h->SetXTitle("Slope(V)");
  DrawHisto(h,"ATWD2aBinCalibSlope.gif",defVal.str());
};

void SetATWD2aBinCalibIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT
       <<" V";
  h->SetXTitle("Intercept(V)");
  DrawHisto(h,"ATWD2aBinCalibIntercept.gif",defVal.str());
};


void SetATWD0bBinCalibSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE
       <<" V";
  h->SetXTitle("Slope(V)");
  DrawHisto(h,"ATWD0bBinCalibSlope.gif",defVal.str());
};

void SetATWD0bBinCalibIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT
       <<" V";
  h->SetXTitle("Intercept(V)");
  DrawHisto(h,"ATWD0bBinCalibIntercept.gif",defVal.str());
};

void SetATWD1bBinCalibSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE
       <<" V";
  h->SetXTitle("Slope(V)");
  DrawHisto(h,"ATWD1bBinCalibSlope.gif",defVal.str());
};

void SetATWD1bBinCalibIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT
       <<" V";
  h->SetXTitle("Intercept(V)");
  DrawHisto(h,"ATWD1bBinCalibIntercept.gif",defVal.str());
};

void SetATWD2bBinCalibSlope(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE
       <<" V";
  h->SetXTitle("Slope(V)");
  DrawHisto(h,"ATWD2bBinCalibSlope.gif",defVal.str());
};

void SetATWD2bBinCalibIntercept(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT
       <<" V";
  h->SetXTitle("Intercept(V)");
  DrawHisto(h,"ATWD2bBinCalibIntercept.gif",defVal.str());
};

void SetLCSpan(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::LCSPAN
       <<" ";
  h->SetXTitle("Span");
  DrawHisto(h,"LCSpan.gif",defVal.str());
}
void SetLCWindowPre(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"IceTop Default = "
	<<I3DetStatDefaults::ICETOP_LCWINDOW_PRE
	<<" "
	<<"InIce Default = "
	<<I3DetStatDefaults::INICE_LCWINDOW_PRE;
  h->SetXTitle("Window(ns)");
  DrawHisto(h,"LCWindowPre.gif",defVal.str());

}

void SetLCWindowPost(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"IceTop Default = "
	<<I3DetStatDefaults::ICETOP_LCWINDOW_POST
	<<" "
	<<"InIce Default = "
	<<I3DetStatDefaults::INICE_LCWINDOW_POST;
  h->SetXTitle("Window(ns)");
  DrawHisto(h,"LCWindowPost.gif",defVal.str());

}

void SetPMTHV(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default IceTop HG "
	<<I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE
	<<" "
	<<"Default IceTop LG "
	<<I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE
	<<" "
	<<"Default InIce "
	<<I3DetStatDefaults::INICE_VOLTAGE
	<<" ";
  h->SetXTitle("PMT Voltage(V)");
  DrawHisto(h,"PMTVoltage.gif",defVal.str());

}
void SetTriggerMode(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::TRIGGER_MODE)
       <<" ";
  h->SetXTitle("Mode");
  DrawHisto(h,"TriggerMode.gif",defVal.str());

}
void SetLCMode(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::LCMODE)
	<<" ";
  h->SetXTitle("LC Mode");
  DrawHisto(h,"LCMode.gif",defVal.str());

}
void SetStatusATWDa(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_ATWDa)
	<<" ";
  h->SetXTitle("ATWDa Status");
  DrawHisto(h,"ATWDaStatus.gif",defVal.str());

}
void SetStatusATWDb(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_ATWDb)
	<<" ";
  h->SetXTitle("ATWDb Status ");
  DrawHisto(h,"ATWDbStatus.gif",defVal.str());

}
void SetStatusFADC(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
	<<static_cast<int>(I3DetStatDefaults::STATUS_FADC)
	<<" ";
  h->SetXTitle("FADC Status");
  DrawHisto(h,"FADCStatus.gif",defVal.str());

}

void SetSPEThreshold(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::SPE_THRESHOLD
       <<" mV";
  h->SetXTitle("threshold(mV)");
  DrawHisto(h,"SPEThreshold.gif",defVal.str());

}
void SetFEPedestal(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::FE_PEDESTAL
       <<" ";
  h->SetXTitle("pedestal(V)");
  DrawHisto(h,"fePedestal.gif",defVal.str());

}
void SetDACTriggerBias0(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_TRIGGER_BIAS0
       <<" ";
  h->SetXTitle(" ");
  DrawHisto(h,"DACTriggBias0.gif",defVal.str());

}
void SetDACTriggerBias1(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_TRIGGER_BIAS1
       <<" ";
  h->SetXTitle(" ");
  DrawHisto(h,"DACTriggBias1.gif",defVal.str());

}
void SetFADCRef(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::DAC_FADC_REF
       <<" ";
  h->SetXTitle(" ");
  DrawHisto(h,"FADCRef.gif",defVal.str());

}
void SetNBinsATWD0(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::NBINS_ATWD0
       <<" bins";
  h->SetXTitle("N Bins");
  DrawHisto(h,"NBinsATWD0.gif",defVal.str());

}
void SetNBinsATWD1(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::NBINS_ATWD1
       <<" bins";
  h->SetXTitle("N Bins");
  DrawHisto(h,"NBinsATWD1.gif",defVal.str());

}
void SetNBinsATWD2(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::NBINS_ATWD2
       <<" bins";
  h->SetXTitle("N Bins");
  DrawHisto(h,"NBinsATWD2.gif",defVal.str());

}
void SetNBinsFADC(TH1D* h){
  gStyle->SetOptStat(111101);
  std::stringstream defVal;
  defVal<<"Default = "
       <<I3DetStatDefaults::NBINS_FADC
       <<" bins";
  h->SetXTitle("N Bins");
  DrawHisto(h,"NBinsFADC.gif",defVal.str());

}
