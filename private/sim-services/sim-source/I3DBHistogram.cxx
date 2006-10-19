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

#include <TFile.h>
#include <TH1D.h>

I3_MODULE(I3DBHistogram);

void BookDOMCalibHistograms(I3CalibrationConstPtr , std::string );
void BookDOMStatusHistograms(I3DetectorStatusConstPtr , std::string );

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

  cout<<"calib->startTime.GetUTCYear(): "<<calib->startTime.GetUTCYear()<<endl;
  cout<<"calib->startTime.GetUTCDaqTime(): "<<calib->startTime.GetUTCDaqTime()<<endl;
  cout<<"calib->endTime.GetUTCYear(): "<<calib->endTime.GetUTCYear()<<endl;
  cout<<"calib->endTime.GetUTCDaqTime(): "<<calib->endTime.GetUTCDaqTime()<<endl;
  cout<<"calib->domCal.size(): "<<calib->domCal.size()<<endl;
  cout<<"calib->amandaCal.size(): "<<calib->amandaCal.size()<<endl;
  cout<<"calib->tankCal.size(): "<<calib->tankCal.size()<<endl;

  TH1D temp_h("temp","Temperature(K)",50,233,283);
  TH1D fadc_bl_slope_h("fadc_bl_slope","FADC Baseline Slope",10,1.2,1.3);
  TH1D fadc_bl_int_h("fadc_bl_int","FADC Baseline Intercept",70,-900,-830);

  TH1D fadc_gain_h("fadc_gain","FADC Gain(V)",
		   100,0.00006*I3Units::V,0.00012*I3Units::V);

  TH1D atwd0_gain_h("atwd0_gain","ATWD Channel 0 Gain(V)",
		    100,-20,-10);
  TH1D atwd1_gain_h("atwd1_gain","ATWD Channel 1 Gain(V)",
		    100,-3,0);
  TH1D atwd2_gain_h("atwd2_gain","ATWD Channel 2 Gain(V)",
		    100,-1,0);

  TH1D quadfitA_0_h("quadfitA_0","Quadratic Fit A - 0",100,15,40);
  TH1D quadfitB_0_h("quadfitB_0","Quadratic Fit B - 0",100,0,1);
  TH1D quadfitC_0_h("quadfitC_0","Quadratic Fit C - 0",100,0,1);

  TH1D quadfitA_1_h("quadfitA_1","Quadratic Fit A - 1",100,15,40);
  TH1D quadfitB_1_h("quadfitB_1","Quadratic Fit B - 1",100,0,1);
  TH1D quadfitC_1_h("quadfitC_1","Quadratic Fit C - 1",100,0,1);

  TH1D hvgain_slope_h("hvgain_slope","HV Gain Fit Slope",100,0,10);
  TH1D hvgain_int_h("hvgain_int","HV Gain Fit Intercept",100,-30,0);

  TH1D atwd0_a_bc_slope_h("atwd0_A_bc_slope",
			  "ATWD_A Channel 0 BinCalib Fit Slope",
			  100,-0.001*I3Units::V,-0.003*I3Units::V);
  TH1D atwd0_a_bc_int_h("atwd0_A_bc_int",
			"ATWD_A Channel 0 BinCalib Fit Intercept",
			100,2.5*I3Units::V,3.5*I3Units::V);

  TH1D atwd1_a_bc_slope_h("atwd1_A_bc_slope",
			  "ATWD_A Channel 1 BinCalib Fit Slope",
			  100,-0.001*I3Units::V,-0.003*I3Units::V);
  TH1D atwd1_a_bc_int_h("atwd1_A_bc_int",
			"ATWD_A Channel 1 BinCalib Fit Intercept",
			100,2.5*I3Units::V,3.5*I3Units::V);

  TH1D atwd2_a_bc_slope_h("atwd2_A_bc_slope",
			  "ATWD_A Channel 2 BinCalib Fit Slope",
			  100,-0.001*I3Units::V,-0.003*I3Units::V);
  TH1D atwd2_a_bc_int_h("atwd2_A_bc_int",
			"ATWD_A Channel 2 BinCalib Fit Intercept",
			100,2.5*I3Units::V,3.5*I3Units::V);

  TH1D atwd0_b_bc_slope_h("atwd0_B_bc_slope",
			  "ATWD_B Channel 0 BinCalib Fit Slope",
			  100,-0.001*I3Units::V,-0.003*I3Units::V);
  TH1D atwd0_b_bc_int_h("atwd0_B_bc_int",
			"ATWD_B Channel 0 BinCalib Fit Intercept",
			100,2.5*I3Units::V,3.5*I3Units::V);

  TH1D atwd1_b_bc_slope_h("atwd1_B_bc_slope",
			  "ATWD_B Channel 1 BinCalib Fit Slope",
			  100,-0.001*I3Units::V,-0.003*I3Units::V);
  TH1D atwd1_b_bc_int_h("atwd1_B_bc_int",
			"ATWD_B Channel 1 BinCalib Fit Intercept",
			100,2.5*I3Units::V,3.5*I3Units::V);

  TH1D atwd2_b_bc_slope_h("atwd2_B_bc_slope",
			  "ATWD_B Channel 2 BinCalib Fit Slope",
			  100,-0.001*I3Units::V,-0.003*I3Units::V);
  TH1D atwd2_b_bc_int_h("atwd2_B_bc_int",
			"ATWD_B Channel 2 BinCalib Fit Intercept",
			100,2.5*I3Units::V,3.5*I3Units::V);


  map<OMKey, I3DOMCalibration>::const_iterator cal_iter;

  cal_iter = calib->domCal.begin();

  cout<<"cal_iter->second.GetTransitTime().slope: "
      <<cal_iter->second.GetTransitTime().slope<<endl;
  cout<<"cal_iter->second.GetTransitTime().intercept: "
      <<cal_iter->second.GetTransitTime().intercept<<endl;
  cout<<"cal_iter->second.GetFADCDeltaT(): "
      <<cal_iter->second.GetFADCDeltaT()<<endl;
  cout<<"cal_iter->second.GetFrontEndImpedance(): "
      <<cal_iter->second.GetFrontEndImpedance()<<endl;
  cout<<"cal_iter->second.GetDOMCalVersion()"
      <<cal_iter->second.GetDOMCalVersion()<<endl;

  for(cal_iter = calib->domCal.begin();
      cal_iter != calib->domCal.end(); 
      cal_iter++){

    temp_h.Fill(cal_iter->second.GetTemperature());

    fadc_bl_slope_h.Fill(cal_iter->second.GetFADCBaselineFit().slope);
    fadc_bl_int_h.Fill(cal_iter->second.GetFADCBaselineFit().intercept);
    fadc_gain_h.Fill(cal_iter->second.GetFADCGain());
    atwd0_gain_h.Fill(cal_iter->second.GetATWDGain(0));
    atwd1_gain_h.Fill(cal_iter->second.GetATWDGain(1));
    atwd2_gain_h.Fill(cal_iter->second.GetATWDGain(2));

    quadfitA_0_h.Fill(cal_iter->second.GetATWDFreqFit(0).quadFitA);
    quadfitB_0_h.Fill(cal_iter->second.GetATWDFreqFit(0).quadFitB);
    quadfitC_0_h.Fill(cal_iter->second.GetATWDFreqFit(0).quadFitC);

    quadfitA_1_h.Fill(cal_iter->second.GetATWDFreqFit(1).quadFitA);
    quadfitB_1_h.Fill(cal_iter->second.GetATWDFreqFit(1).quadFitB);
    quadfitC_1_h.Fill(cal_iter->second.GetATWDFreqFit(1).quadFitC);

    hvgain_slope_h.Fill(cal_iter->second.GetHVGainFit().slope);
    hvgain_int_h.Fill(cal_iter->second.GetHVGainFit().intercept);

    for( unsigned int bin = 0; bin < 128; ++bin ){
      atwd0_a_bc_slope_h.Fill(cal_iter->second.GetATWDBinCalibFit(0,0,bin).slope);
      atwd1_a_bc_slope_h.Fill(cal_iter->second.GetATWDBinCalibFit(0,1,bin).slope);
      atwd2_a_bc_slope_h.Fill(cal_iter->second.GetATWDBinCalibFit(0,2,bin).slope);

      atwd0_b_bc_slope_h.Fill(cal_iter->second.GetATWDBinCalibFit(1,0,bin).slope);
      atwd1_b_bc_slope_h.Fill(cal_iter->second.GetATWDBinCalibFit(1,1,bin).slope);
      atwd2_b_bc_slope_h.Fill(cal_iter->second.GetATWDBinCalibFit(1,2,bin).slope);

      atwd0_a_bc_int_h.Fill(cal_iter->second.GetATWDBinCalibFit(0,0,bin).intercept);
      atwd1_a_bc_int_h.Fill(cal_iter->second.GetATWDBinCalibFit(0,1,bin).intercept);
      atwd2_a_bc_int_h.Fill(cal_iter->second.GetATWDBinCalibFit(0,2,bin).intercept);

      atwd0_b_bc_int_h.Fill(cal_iter->second.GetATWDBinCalibFit(1,0,bin).intercept);
      atwd1_b_bc_int_h.Fill(cal_iter->second.GetATWDBinCalibFit(1,1,bin).intercept);
      atwd2_b_bc_int_h.Fill(cal_iter->second.GetATWDBinCalibFit(1,2,bin).intercept);
    }
  }


 TFile f(rootFileName.c_str(),"RECREATE");
  f.cd();
  temp_h.Write();
  fadc_bl_slope_h.Write();
  fadc_bl_int_h.Write();
  fadc_gain_h.Write();
  atwd0_gain_h.Write();
  atwd1_gain_h.Write();
  atwd2_gain_h.Write();
  quadfitA_0_h.Write();
  quadfitB_0_h.Write();
  quadfitC_0_h.Write();
  quadfitA_1_h.Write();
  quadfitB_1_h.Write();
  quadfitC_1_h.Write();

  atwd0_a_bc_slope_h.Write();
  atwd1_a_bc_slope_h.Write();
  atwd2_a_bc_slope_h.Write();
  
  atwd0_b_bc_slope_h.Write();
  atwd1_b_bc_slope_h.Write();
  atwd2_b_bc_slope_h.Write();
  
  atwd0_a_bc_int_h.Write();
  atwd1_a_bc_int_h.Write();
  atwd2_a_bc_int_h.Write();
  
  atwd0_b_bc_int_h.Write();
  atwd1_b_bc_int_h.Write();
  atwd2_b_bc_int_h.Write();
  
  f.Close();
} 

void BookDOMStatusHistograms(I3DetectorStatusConstPtr status, 
			    std::string rootFileName){

  cout<<"status->domStatus.size(): "<<status->domStatus.size()<<endl;

  TH1D lcspan_h("lcspan","Local Coincidence Span",4,0,3);
  TH1D lcWindowPre_h("lcWindowPre","Local Coincidence Window Pre",
		    200,0, 2000.0*I3Units::ns);
  TH1D lcWindowPost_h("lcWindowPost","Local Coincidence Window Post",
		    200,0, 2000.0*I3Units::ns);
  TH1D pmtHV_h("pmtHV","PMT High Voltage",
		    80,800.*I3Units::volt, 1600.0*I3Units::volt);
  TH1D trigMode_h("trigMode","Trigger Mode",5,-1,3);
  TH1D lcMode_h("lcMode","Local Coincidence Mode",6,-1,4);
  TH1D statusATWDa_h("statusATWDa","Status ATWDa",3,-1,1);
  TH1D statusATWDb_h("statusATWDb","Status ATWDb",3,-1,1);
  TH1D statusFADC_h("statusFADC","Status FADC",3,-1,1);

  TH1D speThreshold_h("speThreshold","SPE Threshold",100,0,5.*I3Units::mV);
  TH1D fePedestal_h("fePedestal","FE Pedestal",100,2.5*I3Units::volt,2.7*I3Units::volt);

  TH1D dacTriggerBias0_h("dacTriggerBias0","DAC Trigger Bias 0",100,0,1000);
  TH1D dacTriggerBias1_h("dacTriggerBias1","DAC Trigger Bias 1",100,0,1000);
  TH1D dacFADCRef_h("dacFADCRef","DAC FADC Ref",100,0,1000);

  TH1D nBinsATWD0_h("nBinsATWD0","Number of bins ATWD0",257,0,256);
  TH1D nBinsATWD1_h("nBinsATWD1","Number of bins ATWD1",257,0,256);
  TH1D nBinsATWD2_h("nBinsATWD2","Number of bins ATWD2",257,0,256);
  TH1D nBinsFADC_h("nBinsFADC","Number of bins FADC",257,0,256);

  map<OMKey, I3DOMStatus>::const_iterator stat_iter;
  stat_iter = status->domStatus.begin();
  cout<<"stat_iter->second.fePedestal: "
      <<stat_iter->second.fePedestal<<endl;

  for(stat_iter = status->domStatus.begin();
      stat_iter != status->domStatus.end(); 
      stat_iter++){

    if((stat_iter->second.lcWindowPre > 450.*I3Units::ns) &&
       (stat_iter->second.lcWindowPre < 550.*I3Units::ns)){
      cout<<stat_iter->first<<" = "
	  <<stat_iter->second.lcWindowPre/I3Units::ns<<" ns"<<endl;
    }

    lcspan_h.Fill(stat_iter->second.lcSpan);
    lcWindowPre_h.Fill(stat_iter->second.lcWindowPre);
    lcWindowPost_h.Fill(stat_iter->second.lcWindowPost);
    pmtHV_h.Fill(stat_iter->second.pmtHV);
    trigMode_h.Fill(stat_iter->second.trigMode);
    lcMode_h.Fill(stat_iter->second.lcMode);
    statusATWDa_h.Fill(stat_iter->second.statusATWDa);
    statusATWDb_h.Fill(stat_iter->second.statusATWDb);
    statusFADC_h.Fill(stat_iter->second.statusFADC);
    speThreshold_h.Fill(stat_iter->second.speThreshold);
    fePedestal_h.Fill(stat_iter->second.fePedestal);
    dacTriggerBias0_h.Fill(stat_iter->second.dacTriggerBias0);
    dacTriggerBias1_h.Fill(stat_iter->second.dacTriggerBias1);
    dacFADCRef_h.Fill(stat_iter->second.dacFADCRef);
    nBinsATWD0_h.Fill(stat_iter->second.nBinsATWD0);
    nBinsATWD1_h.Fill(stat_iter->second.nBinsATWD1);
    nBinsATWD2_h.Fill(stat_iter->second.nBinsATWD2);
    nBinsFADC_h.Fill(stat_iter->second.nBinsFADC);
  }

  TFile g(rootFileName.c_str(),"RECREATE");
  g.cd();

  lcspan_h.Write();
  lcWindowPre_h.Write();
  lcWindowPost_h.Write();
  pmtHV_h.Write();
  trigMode_h.Write();
  lcMode_h.Write();
  statusATWDa_h.Write();
  statusATWDb_h.Write();
  statusFADC_h.Write();
  speThreshold_h.Write();
  fePedestal_h.Write();
  dacTriggerBias0_h.Write();
  dacTriggerBias1_h.Write();
  dacFADCRef_h.Write();
  nBinsATWD0_h.Write();
  nBinsATWD1_h.Write();
  nBinsATWD2_h.Write();
  nBinsFADC_h.Write();

  g.Close();

}
