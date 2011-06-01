#include <I3Test.h>

#include "icetray/I3Tray.h"
#include "I3SimSourceTestModule.h"
#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3DBHistogram.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"

#include "trigger-sim/default-values/I3InIceTriggerDefaults.h"
#include "trigger-sim/default-values/I3IceTopTriggerDefaults.h"

#include "sim-services/tweak-sources/I3TweakDOMStatusService.h"
#include "sim-services/tweak-sources/I3TweakCalibrationService.h"
#include "sim-services/tweak-sources/I3TweakTriggerService.h"

#include "icetray/test/ConstructorTest.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>

using boost::algorithm::split;
using boost::algorithm::is_any_of;

using namespace std;

// Test some of Calibrate Module functionality
TEST_GROUP(SimSource);

TEST(clean_construction){
  clean_constructor_test<I3DBHistogram>();
  clean_constructor_test<I3TweakDOMStatusService>();
  clean_constructor_test<I3TweakCalibrationService>();
  clean_constructor_test<I3TweakTriggerService>();
}

TEST(default_config)
{
  I3Tray tray;

  std::string I3_PORTS(getenv("I3_PORTS"));
  std::string gcd_file("/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz");

  tray.AddModule("I3InfiniteSource","sourceme")
    ("Prefix", I3_PORTS + gcd_file);

  tray.AddModule("I3MCEventHeaderGenerator","headerme")
    ("Year", 2007)
    ("DAQTime",314159);
    
  tray.AddModule("I3MetaSynth","muxer");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("DetStat_StartYear",I3DetStatDefaults::START_YEAR)
    ("DetStat_StartDAQTime",I3DetStatDefaults::START_DAQTIME)
    ("DetStat_EndYear",I3DetStatDefaults::END_YEAR)
    ("DetStat_EndDAQTime",I3DetStatDefaults::END_DAQTIME)
    ("DetStat_IceTopLCWindowPre",I3DetStatDefaults::ICETOP_LCWINDOW_PRE)
    ("DetStat_IceTopLCWindowPost",I3DetStatDefaults::ICETOP_LCWINDOW_POST)
    ("DetStat_IceTopHighGainVoltage",I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE)
    ("DetStat_IceTopLowGainVoltage",I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE)
    ("DetStat_InIceLCWindowPre",I3DetStatDefaults::INICE_LCWINDOW_PRE)
    ("DetStat_InIceLCWindowPost",I3DetStatDefaults::INICE_LCWINDOW_POST)
    ("DetStat_InIceLCSpan",I3DetStatDefaults::INICE_LCSPAN)
    ("DetStat_IceTopLCSpan",I3DetStatDefaults::ICETOP_LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_InIceTriggerMode",static_cast<int>(I3DetStatDefaults::INICE_TRIGGER_MODE))
    ("DetStat_IceTopHGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_HG_TRIGGER_MODE))
    ("DetStat_IceTopLGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_LG_TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::INICE_SPE_THRESHOLD)
    ("DetStat_FEPedestal",I3DetStatDefaults::FE_PEDESTAL)
    ("DetStat_DACTriggerBias0",I3DetStatDefaults::DAC_TRIGGER_BIAS0)
    ("DetStat_DACTriggerBias1",I3DetStatDefaults::DAC_TRIGGER_BIAS1)
    ("DetStat_DACFADCRef",I3DetStatDefaults::DAC_FADC_REF)
    ("DetStat_NBinsATWD0InIce",I3DetStatDefaults::NBINS_ATWD0_INICE)
    ("DetStat_NBinsATWD1InIce",I3DetStatDefaults::NBINS_ATWD1_INICE)
    ("DetStat_NBinsATWD2InIce",I3DetStatDefaults::NBINS_ATWD2_INICE)
    ("DetStat_NBinsFADCInIce",I3DetStatDefaults::NBINS_FADC_INICE)
    ("DetStat_NBinsATWD0IceTop",I3DetStatDefaults::NBINS_ATWD0_ICETOP)
    ("DetStat_NBinsATWD1IceTop",I3DetStatDefaults::NBINS_ATWD1_ICETOP)
    ("DetStat_NBinsATWD2IceTop",I3DetStatDefaults::NBINS_ATWD2_ICETOP)
    ("DetStat_NBinsFADCIceTop",I3DetStatDefaults::NBINS_FADC_ICETOP)
    ("Calib_StartYear",I3CalibDefaults::START_YEAR)
    ("Calib_StartDAQTime",I3CalibDefaults::START_DAQTIME)
    ("Calib_EndYear",I3CalibDefaults::END_YEAR)
    ("Calib_EndDAQTime",I3CalibDefaults::END_DAQTIME)
    ("Calib_Temperature",I3CalibDefaults::TEMPERATURE)
    ("Calib_fadcBaselineFit_slope",I3CalibDefaults::FADC_BASELINE_FIT_SLOPE)
    ("Calib_fadcBaselineFit_intercept",I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT)
    ("Calib_fadcGain",I3CalibDefaults::FADC_GAIN)
    ("Calib_atwd0Gain",I3CalibDefaults::ATWD0_GAIN)
    ("Calib_atwd1Gain",I3CalibDefaults::ATWD1_GAIN)
    ("Calib_atwd2Gain",I3CalibDefaults::ATWD2_GAIN)
    ("Calib_atwd_a_FreqFit_A",I3CalibDefaults::ATWD_A_FREQFIT_A)
    ("Calib_atwd_a_FreqFit_B",I3CalibDefaults::ATWD_A_FREQFIT_B)
    ("Calib_atwd_a_FreqFit_C",I3CalibDefaults::ATWD_A_FREQFIT_C)
    ("Calib_atwd_b_FreqFit_A",I3CalibDefaults::ATWD_B_FREQFIT_A)
    ("Calib_atwd_b_FreqFit_B",I3CalibDefaults::ATWD_B_FREQFIT_B)
    ("Calib_atwd_b_FreqFit_C",I3CalibDefaults::ATWD_B_FREQFIT_C)
    ("Calib_hvGainFit_slope",I3CalibDefaults::HV_GAIN_FIT_SLOPE)
    ("Calib_hvGainFit_intercept",I3CalibDefaults::HV_GAIN_FIT_INTERCEPT)
    ("Calib_atwdBinCalibFit_slope",I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE)
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
    ("Calib_ATWDResponseWidth",I3CalibDefaults::ATWD_RESPONSE_WIDTH)
    ("Calib_ATWDADeltaT",I3CalibDefaults::ATWDA_DELTAT)
    ("Calib_ATWDBDeltaT",I3CalibDefaults::ATWDB_DELTAT)
    ("Calib_SPEDiscThreshIntercept",I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_SPEDiscThreshSlope",I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE)
    ("Calib_MPEDiscThreshIntercept",I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_MPEDiscThreshSlope",I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE)
  	 ("Calib_PMTDiscThreshIntercept", I3CalibDefaults::PMT_DISCRIMINATOR_INTERCEPT)
	 ("Calib_PMTDiscThreshSlope", I3CalibDefaults::PMT_DISCRIMINATOR_SLOPE);
  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

TEST(tweaked_config)
{

  double icetopLCWindowPre(200.*I3Units::ns);
  double icetopLCWindowPost(300.*I3Units::ns);
  double icetopHighGainVoltage(1099*I3Units::volt);
  double icetopLowGainVoltage(444*I3Units::volt);
  double iniceLCWindowPre(1234.0*I3Units::ns);
  double iniceLCWindowPost(2143.0*I3Units::ns);
  int icetop_LCSpan(4);
  int inice_LCSpan(8);
  double iniceVoltage(2076*I3Units::volt);
  int iniceTriggerMode(0);
  int icetopHGTriggerMode(1);
  int icetopLGTriggerMode(2);

  int lcMode_inice_first(0);
  int lcMode_inice_bulk(3);
  int lcMode_inice_last(4);
  int lcMode_icetop(2);

  int statusATWDa(1);
  int statusATWDb(0);
  int statusFADCInIce(10);
  int statusFADCIceTop(1);
  double speThreshold(1.618*I3Units::mV);
  double fePedestal(3.14*I3Units::volt);
  int dacTriggerBias0(900);
  int dacTriggerBias1(1000);
  int dacFADCRef(666);
  int nBinsATWD0InIce(256);
  int nBinsATWD1InIce(123);
  int nBinsATWD2InIce(4);
  int nBinsFADCInIce(42);

  int nBinsATWD0IceTop(1024);
  int nBinsATWD1IceTop(8);
  int nBinsATWD2IceTop(666);
  int nBinsFADCIceTop(765);

  double temperature(273.);
  double fadcBaselineFit_slope(1.333);
  double fadcBaselineFit_intercept(-932.6);

  double fadcGain(1.765e-5*I3Units::V);
  double atwd0Gain(-42.0);
  double atwd1Gain(-1.44);
  double atwd2Gain(-0.31459);

  double atwd_a_FreqFit_A(22.098901);
  double atwd_a_FreqFit_B(0.0091022);
  double atwd_a_FreqFit_C(0.445092);

  double atwd_b_FreqFit_A(30.98012);
  double atwd_b_FreqFit_B(1.44534);
  double atwd_b_FreqFit_C(0.1984);

  double hvGainFit_slope(7.0);
  double hvGainFit_intercept(-33.3333);

  double atwdBinCalibFit_slope(-0.0021091*I3Units::V);
  double atwdBinCalibFit_intercept(3.4*I3Units::V);

  double atwd_response_width(0.33333);

  double atwda_deltat(0.298897);
  double atwdb_deltat(0.9879823);

  double spe_disc_thresh_slope(198.987823);
  double spe_disc_thresh_int(19084.30);
  double mpe_disc_thresh_slope(121.2222);
  double mpe_disc_thresh_int(1290.000001);
  double pmt_disc_thresh_slope(100.2222);
  double pmt_disc_thresh_int(200.000001);

  I3Tray tray;

  std::string I3_PORTS(getenv("I3_PORTS"));
  std::string gcd_file("/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz");

  tray.AddModule("I3InfiniteSource","sourceme")
    ("Prefix", I3_PORTS + gcd_file);

  tray.AddModule("I3MCEventHeaderGenerator","headerme")
    ("Year", 2007)
    ("DAQTime",314159);

  tray.AddService("I3TweakDOMStatusService","tweak-status")
    ("IceTopLCWindowPre",icetopLCWindowPre)
    ("IceTopLCWindowPost",icetopLCWindowPost)
    ("IceTopHighGainVoltage",icetopHighGainVoltage)
    ("IceTopLowGainVoltage",icetopLowGainVoltage)
    ("InIceLCWindowPre",iniceLCWindowPre)
    ("InIceLCWindowPost",iniceLCWindowPost)
    ("InIceLCSpan",inice_LCSpan)
    ("IceTopLCSpan",icetop_LCSpan)
    ("InIceVoltage",iniceVoltage)
    ("InIceTriggerMode",iniceTriggerMode)
    ("IceTopHGTriggerMode",icetopHGTriggerMode)
    ("IceTopLGTriggerMode",icetopLGTriggerMode)
    ("LCModeInIceFirstDOM",lcMode_inice_first)
    ("LCModeInIceBulkDOMs",lcMode_inice_bulk)
    ("LCModeInIceLastDOM",lcMode_inice_last)
    ("LCModeIceTopDOMs",lcMode_icetop)
    ("StatusATWDa",statusATWDa)
    ("StatusATWDb",statusATWDb)
    ("StatusFADCInIce",statusFADCInIce)
    ("StatusFADCIceTop",statusFADCIceTop)
    ("SPEThreshold",speThreshold)
    ("FEPedestal",fePedestal)
    ("DACTriggerBias0",dacTriggerBias0)
    ("DACTriggerBias1",dacTriggerBias1)
    ("DACFADCRef",dacFADCRef)
    ("NBinsATWD0InIce",nBinsATWD0InIce)
    ("NBinsATWD1InIce",nBinsATWD1InIce)
    ("NBinsATWD2InIce",nBinsATWD2InIce)
    ("NBinsFADCInIce",nBinsFADCInIce)
    ("NBinsATWD0IceTop",nBinsATWD0IceTop)
    ("NBinsATWD1IceTop",nBinsATWD1IceTop)
    ("NBinsATWD2IceTop",nBinsATWD2IceTop)
    ("NBinsFADCIceTop",nBinsFADCIceTop);

  tray.AddService("I3TweakCalibrationService","tweak-cal")
    ("Temperature",temperature)
    ("FADCBaselineFit_slope",fadcBaselineFit_slope)
    ("FADCBaselineFit_intercept",fadcBaselineFit_intercept)
    ("FADCGain",fadcGain)
    ("ATWD0Gain",atwd0Gain)
    ("ATWD1Gain",atwd1Gain)
    ("ATWD2Gain",atwd2Gain)
    ("ATWD_a_FreqFit_A",atwd_a_FreqFit_A)
    ("ATWD_a_FreqFit_B",atwd_a_FreqFit_B)
    ("ATWD_a_FreqFit_C",atwd_a_FreqFit_C)
    ("ATWD_b_FreqFit_A",atwd_b_FreqFit_A)
    ("ATWD_b_FreqFit_B",atwd_b_FreqFit_B)
    ("ATWD_b_FreqFit_C",atwd_b_FreqFit_C)
    ("HVGainFit_slope",hvGainFit_slope)
    ("HVGainFit_intercept",hvGainFit_intercept)
    ("ATWDBinCalibFit_slope",atwdBinCalibFit_slope)
    ("ATWDBinCalibFit_intercept",atwdBinCalibFit_intercept)
    ("ATWDResponseWidth",atwd_response_width)
    ("ATWDADeltaT",atwda_deltat)
    ("ATWDBDeltaT",atwdb_deltat)
    ("SPEDiscThreshIntercept",spe_disc_thresh_int)
    ("SPEDiscThreshSlope",spe_disc_thresh_slope)
    ("MPEDiscThreshIntercept",mpe_disc_thresh_int)
    ("MPEDiscThreshSlope",mpe_disc_thresh_slope)
    ("PMTDiscThreshIntercept",pmt_disc_thresh_int)
    ("PMTDiscThreshSlope",pmt_disc_thresh_slope);

  tray.AddModule("I3MetaSynth","muxer")
    ("CalibrationService","I3TweakCalibrationService")
    ("DetectorStatusService","I3TweakDOMStatusService");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("DetStat_IceTopLCWindowPre",icetopLCWindowPre)
    ("DetStat_IceTopLCWindowPost",icetopLCWindowPost)
    ("DetStat_IceTopHighGainVoltage",icetopHighGainVoltage)
    ("DetStat_IceTopLowGainVoltage",icetopLowGainVoltage)
    ("DetStat_InIceLCWindowPre",iniceLCWindowPre)
    ("DetStat_InIceLCWindowPost",iniceLCWindowPost)
    ("DetStat_InIceLCSpan",inice_LCSpan)
    ("DetStat_IceTopLCSpan",icetop_LCSpan)
    ("DetStat_InIceVoltage",iniceVoltage)
    ("DetStat_InIceTriggerMode",iniceTriggerMode)
    ("DetStat_IceTopHGTriggerMode",icetopHGTriggerMode)
    ("DetStat_IceTopLGTriggerMode",icetopLGTriggerMode)
    ("DetStat_LCModeInIceFirstDOM",lcMode_inice_first)
    ("DetStat_LCModeInIceBulkDOMs",lcMode_inice_bulk)
    ("DetStat_LCModeInIceLastDOM",lcMode_inice_last)
    ("DetStat_LCModeIceTopDOMs",lcMode_icetop)
    ("DetStat_StatusATWDa",statusATWDa)
    ("DetStat_StatusATWDb",statusATWDb)
    ("DetStat_StatusFADCInIce",statusFADCInIce)
    ("DetStat_StatusFADCIceTop",statusFADCIceTop)
    ("DetStat_SPEThreshold",speThreshold)
    ("DetStat_FEPedestal",fePedestal)
    ("DetStat_DACTriggerBias0",dacTriggerBias0)
    ("DetStat_DACTriggerBias1",dacTriggerBias1)
    ("DetStat_DACFADCRef",dacFADCRef)
    ("DetStat_NBinsATWD0InIce",nBinsATWD0InIce)
    ("DetStat_NBinsATWD1InIce",nBinsATWD1InIce)
    ("DetStat_NBinsATWD2InIce",nBinsATWD2InIce)
    ("DetStat_NBinsFADCInIce",nBinsFADCInIce)
    ("DetStat_NBinsATWD0IceTop",nBinsATWD0IceTop)
    ("DetStat_NBinsATWD1IceTop",nBinsATWD1IceTop)
    ("DetStat_NBinsATWD2IceTop",nBinsATWD2IceTop)
    ("DetStat_NBinsFADCIceTop",nBinsFADCIceTop)
    ("Calib_Temperature",temperature)
    ("Calib_fadcBaselineFit_slope",fadcBaselineFit_slope)
    ("Calib_fadcBaselineFit_intercept",fadcBaselineFit_intercept)
    ("Calib_fadcGain",fadcGain)
    ("Calib_atwd0Gain",atwd0Gain)
    ("Calib_atwd1Gain",atwd1Gain)
    ("Calib_atwd2Gain",atwd2Gain)
    ("Calib_atwd_a_FreqFit_A",atwd_a_FreqFit_A)
    ("Calib_atwd_a_FreqFit_B",atwd_a_FreqFit_B)
    ("Calib_atwd_a_FreqFit_C",atwd_a_FreqFit_C)
    ("Calib_atwd_b_FreqFit_A",atwd_b_FreqFit_A)
    ("Calib_atwd_b_FreqFit_B",atwd_b_FreqFit_B)
    ("Calib_atwd_b_FreqFit_C",atwd_b_FreqFit_C)
    ("Calib_hvGainFit_slope",hvGainFit_slope)
    ("Calib_hvGainFit_intercept",hvGainFit_intercept)
    ("Calib_atwdBinCalibFit_slope",atwdBinCalibFit_slope)
    ("Calib_atwdBinCalibFit_intercept",atwdBinCalibFit_intercept)
    ("Calib_ATWDResponseWidth",atwd_response_width)
    ("Calib_ATWDADeltaT",atwda_deltat)
    ("Calib_ATWDBDeltaT",atwdb_deltat)
    ("Calib_SPEDiscThreshIntercept",spe_disc_thresh_int)
    ("Calib_SPEDiscThreshSlope",spe_disc_thresh_slope)
    ("Calib_MPEDiscThreshIntercept",mpe_disc_thresh_int)
    ("Calib_MPEDiscThreshSlope",mpe_disc_thresh_slope)
    ("Calib_PMTDiscThreshIntercept",pmt_disc_thresh_int)
    ("Calib_PMTDiscThreshSlope",pmt_disc_thresh_slope);

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

TEST(modify_with_extreme_prejudice)
{

  double icetopLCWindowPre(200.*I3Units::ns);
  double icetopLCWindowPost(300.*I3Units::ns);
  double icetopHighGainVoltage(1099*I3Units::volt);
  double icetopLowGainVoltage(444*I3Units::volt);
  double iniceLCWindowPre(1234.0*I3Units::ns);
  double iniceLCWindowPost(2143.0*I3Units::ns);
  int icetop_LCSpan(4);
  int inice_LCSpan(8);
  double iniceVoltage(2076*I3Units::volt);
  int iniceTriggerMode(0);
  int icetopHGTriggerMode(1);
  int icetopLGTriggerMode(2);

  int lcMode_inice_first(0);
  int lcMode_inice_bulk(3);
  int lcMode_inice_last(4);
  int lcMode_icetop(2);

  int statusATWDa(1);
  int statusATWDb(0);
  int statusFADCInIce(10);
  int statusFADCIceTop(1);
  double speThreshold(1.618*I3Units::mV);
  double fePedestal(3.14*I3Units::volt);
  int dacTriggerBias0(900);
  int dacTriggerBias1(1000);
  int dacFADCRef(666);
  int nBinsATWD0InIce(256);
  int nBinsATWD1InIce(123);
  int nBinsATWD2InIce(4);
  int nBinsFADCInIce(42);

  int nBinsATWD0IceTop(1024);
  int nBinsATWD1IceTop(8);
  int nBinsATWD2IceTop(666);
  int nBinsFADCIceTop(765);

  double temperature(273.);
  double fadcBaselineFit_slope(1.333);
  double fadcBaselineFit_intercept(-932.6);

  double fadcGain(1.765e-5*I3Units::V);
  double atwd0Gain(-42.0);
  double atwd1Gain(-1.44);
  double atwd2Gain(-0.31459);

  double atwd_a_FreqFit_A(22.098901);
  double atwd_a_FreqFit_B(0.0091022);
  double atwd_a_FreqFit_C(0.445092);

  double atwd_b_FreqFit_A(30.98012);
  double atwd_b_FreqFit_B(1.44534);
  double atwd_b_FreqFit_C(0.1984);

  double hvGainFit_slope(7.0);
  double hvGainFit_intercept(-33.3333);

  double atwdBinCalibFit_slope(-0.0021091*I3Units::V);
  double atwdBinCalibFit_intercept(3.4*I3Units::V);

  double atwd_response_width(0.33333);

  double atwda_deltat(0.298897);
  double atwdb_deltat(0.9879823);

  double spe_disc_thresh_slope(198.987823);
  double spe_disc_thresh_int(19084.30);
  double mpe_disc_thresh_slope(121.2222);
  double mpe_disc_thresh_int(1290.000001);
  double pmt_disc_thresh_slope(100.2222);
  double pmt_disc_thresh_int(200.000001);

  I3Tray tray;

  tray.AddModule("I3InfiniteSource","sourceme");

  tray.AddService("I3EmptyStreamsFactory","streams");

//  tray.AddModule("I3MCEventHeaderGenerator","headerme")
//    ("Year", 2037)
//    ("DAQTime",314159);

  tray.AddService("I3TweakDOMStatusService","tweak-status")
    ("IceTopLCWindowPre",icetopLCWindowPre)
    ("IceTopLCWindowPost",icetopLCWindowPost)
    ("IceTopHighGainVoltage",icetopHighGainVoltage)
    ("IceTopLowGainVoltage",icetopLowGainVoltage)
    ("InIceLCWindowPre",iniceLCWindowPre)
    ("InIceLCWindowPost",iniceLCWindowPost)
    ("InIceLCSpan",inice_LCSpan)
    ("IceTopLCSpan",icetop_LCSpan)
    ("InIceVoltage",iniceVoltage)
    ("InIceTriggerMode",iniceTriggerMode)
    ("IceTopHGTriggerMode",icetopHGTriggerMode)
    ("IceTopLGTriggerMode",icetopLGTriggerMode)
    ("LCModeInIceFirstDOM",lcMode_inice_first)
    ("LCModeInIceBulkDOMs",lcMode_inice_bulk)
    ("LCModeInIceLastDOM",lcMode_inice_last)
    ("LCModeIceTopDOMs",lcMode_icetop)
    ("StatusATWDa",statusATWDa)
    ("StatusATWDb",statusATWDb)
    ("StatusFADCInIce",statusFADCInIce)
    ("StatusFADCIceTop",statusFADCIceTop)
    ("SPEThreshold",speThreshold)
    ("FEPedestal",fePedestal)
    ("DACTriggerBias0",dacTriggerBias0)
    ("DACTriggerBias1",dacTriggerBias1)
    ("DACFADCRef",dacFADCRef)
    ("NBinsATWD0InIce",nBinsATWD0InIce)
    ("NBinsATWD1InIce",nBinsATWD1InIce)
    ("NBinsATWD2InIce",nBinsATWD2InIce)
    ("NBinsFADCInIce",nBinsFADCInIce)
    ("NBinsATWD0IceTop",nBinsATWD0IceTop)
    ("NBinsATWD1IceTop",nBinsATWD1IceTop)
    ("NBinsATWD2IceTop",nBinsATWD2IceTop)
    ("NBinsFADCIceTop",nBinsFADCIceTop);

  tray.AddService("I3TweakCalibrationService","tweak-cal")
    ("Temperature",temperature)
    ("FADCBaselineFit_slope",fadcBaselineFit_slope)
    ("FADCBaselineFit_intercept",fadcBaselineFit_intercept)
    ("FADCGain",fadcGain)
    ("ATWD0Gain",atwd0Gain)
    ("ATWD1Gain",atwd1Gain)
    ("ATWD2Gain",atwd2Gain)
    ("ATWD_a_FreqFit_A",atwd_a_FreqFit_A)
    ("ATWD_a_FreqFit_B",atwd_a_FreqFit_B)
    ("ATWD_a_FreqFit_C",atwd_a_FreqFit_C)
    ("ATWD_b_FreqFit_A",atwd_b_FreqFit_A)
    ("ATWD_b_FreqFit_B",atwd_b_FreqFit_B)
    ("ATWD_b_FreqFit_C",atwd_b_FreqFit_C)
    ("HVGainFit_slope",hvGainFit_slope)
    ("HVGainFit_intercept",hvGainFit_intercept)
    ("ATWDBinCalibFit_slope",atwdBinCalibFit_slope)
    ("ATWDBinCalibFit_intercept",atwdBinCalibFit_intercept)
    ("ATWDResponseWidth",atwd_response_width)
    ("ATWDADeltaT",atwda_deltat)
    ("ATWDBDeltaT",atwdb_deltat)
    ("SPEDiscThreshIntercept",spe_disc_thresh_int)
    ("SPEDiscThreshSlope",spe_disc_thresh_slope)
    ("MPEDiscThreshIntercept",mpe_disc_thresh_int)
    ("MPEDiscThreshSlope",mpe_disc_thresh_slope)
    ("PMTDiscThreshIntercept",pmt_disc_thresh_int)
    ("PMTDiscThreshSlope",pmt_disc_thresh_slope);

  tray.AddService("I3MCSourceServiceFactory","backtodefault")
    ("OldCalServiceName","I3TweakCalibrationService")
    ("OldStatusServiceName","I3TweakDOMStatusService")
    ("CalServiceName","BackToDefaultCal")
    ("StatusServiceName","BackToDefaultStat")
    ("ModifyWithExtremePrejudice",true);

  tray.AddModule("I3MetaSynth","muxer")
    ("CalibrationService","BackToDefaultCal")
    ("DetectorStatusService","BackToDefaultStat");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("DetStat_StartYear",I3DetStatDefaults::START_YEAR)
    ("DetStat_StartDAQTime",I3DetStatDefaults::START_DAQTIME)
    ("DetStat_EndYear",I3DetStatDefaults::END_YEAR)
    ("DetStat_EndDAQTime",I3DetStatDefaults::END_DAQTIME)
    ("DetStat_IceTopLCWindowPre",I3DetStatDefaults::ICETOP_LCWINDOW_PRE)
    ("DetStat_IceTopLCWindowPost",I3DetStatDefaults::ICETOP_LCWINDOW_POST)
    ("DetStat_IceTopHighGainVoltage",I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE)
    ("DetStat_IceTopLowGainVoltage",I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE)
    ("DetStat_InIceLCWindowPre",I3DetStatDefaults::INICE_LCWINDOW_PRE)
    ("DetStat_InIceLCWindowPost",I3DetStatDefaults::INICE_LCWINDOW_POST)
    ("DetStat_InIceLCSpan",I3DetStatDefaults::INICE_LCSPAN)
    ("DetStat_IceTopLCSpan",I3DetStatDefaults::ICETOP_LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_InIceTriggerMode",static_cast<int>(I3DetStatDefaults::INICE_TRIGGER_MODE))
    ("DetStat_IceTopHGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_HG_TRIGGER_MODE))
    ("DetStat_IceTopLGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_LG_TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::INICE_SPE_THRESHOLD)
    ("DetStat_FEPedestal",I3DetStatDefaults::FE_PEDESTAL)
    ("DetStat_DACTriggerBias0",I3DetStatDefaults::DAC_TRIGGER_BIAS0)
    ("DetStat_DACTriggerBias1",I3DetStatDefaults::DAC_TRIGGER_BIAS1)
    ("DetStat_DACFADCRef",I3DetStatDefaults::DAC_FADC_REF)
    ("DetStat_NBinsATWD0InIce",I3DetStatDefaults::NBINS_ATWD0_INICE)
    ("DetStat_NBinsATWD1InIce",I3DetStatDefaults::NBINS_ATWD1_INICE)
    ("DetStat_NBinsATWD2InIce",I3DetStatDefaults::NBINS_ATWD2_INICE)
    ("DetStat_NBinsFADCInIce",I3DetStatDefaults::NBINS_FADC_INICE)
    ("DetStat_NBinsATWD0IceTop",I3DetStatDefaults::NBINS_ATWD0_ICETOP)
    ("DetStat_NBinsATWD1IceTop",I3DetStatDefaults::NBINS_ATWD1_ICETOP)
    ("DetStat_NBinsATWD2IceTop",I3DetStatDefaults::NBINS_ATWD2_ICETOP)
    ("DetStat_NBinsFADCIceTop",I3DetStatDefaults::NBINS_FADC_ICETOP)
    ("Calib_StartYear",I3CalibDefaults::START_YEAR)
    ("Calib_StartDAQTime",I3CalibDefaults::START_DAQTIME)
    ("Calib_EndYear",I3CalibDefaults::END_YEAR)
    ("Calib_EndDAQTime",I3CalibDefaults::END_DAQTIME)
    ("Calib_Temperature",I3CalibDefaults::TEMPERATURE)
    ("Calib_fadcBaselineFit_slope",I3CalibDefaults::FADC_BASELINE_FIT_SLOPE)
    ("Calib_fadcBaselineFit_intercept",I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT)
    ("Calib_fadcGain",I3CalibDefaults::FADC_GAIN)
    ("Calib_atwd0Gain",I3CalibDefaults::ATWD0_GAIN)
    ("Calib_atwd1Gain",I3CalibDefaults::ATWD1_GAIN)
    ("Calib_atwd2Gain",I3CalibDefaults::ATWD2_GAIN)
    ("Calib_atwd_a_FreqFit_A",I3CalibDefaults::ATWD_A_FREQFIT_A)
    ("Calib_atwd_a_FreqFit_B",I3CalibDefaults::ATWD_A_FREQFIT_B)
    ("Calib_atwd_a_FreqFit_C",I3CalibDefaults::ATWD_A_FREQFIT_C)
    ("Calib_atwd_b_FreqFit_A",I3CalibDefaults::ATWD_B_FREQFIT_A)
    ("Calib_atwd_b_FreqFit_B",I3CalibDefaults::ATWD_B_FREQFIT_B)
    ("Calib_atwd_b_FreqFit_C",I3CalibDefaults::ATWD_B_FREQFIT_C)
    ("Calib_hvGainFit_slope",I3CalibDefaults::HV_GAIN_FIT_SLOPE)
    ("Calib_hvGainFit_intercept",I3CalibDefaults::HV_GAIN_FIT_INTERCEPT)
    ("Calib_atwdBinCalibFit_slope",I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE)
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
    ("Calib_ATWDResponseWidth",I3CalibDefaults::ATWD_RESPONSE_WIDTH)
    ("Calib_ATWDADeltaT",I3CalibDefaults::ATWDA_DELTAT)
    ("Calib_ATWDBDeltaT",I3CalibDefaults::ATWDB_DELTAT)
    ("Calib_SPEDiscThreshIntercept",I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_SPEDiscThreshSlope",I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE)
    ("Calib_MPEDiscThreshIntercept",I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_MPEDiscThreshSlope",I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE)
    ("Calib_PMTDiscThreshIntercept", I3CalibDefaults::PMT_DISCRIMINATOR_INTERCEPT)
    ("Calib_PMTDiscThreshSlope", I3CalibDefaults::PMT_DISCRIMINATOR_SLOPE);

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}


TEST(tweaked_extended_config)
{

  double icetopLCWindowPre(200.*I3Units::ns);
  double icetopLCWindowPost(300.*I3Units::ns);
  double icetopHighGainVoltage(1099*I3Units::volt);
  double icetopLowGainVoltage(444*I3Units::volt);
  double iniceLCWindowPre(1234.0*I3Units::ns);
  double iniceLCWindowPost(2143.0*I3Units::ns);
  int inice_LCSpan(6);
  int icetop_LCSpan(42);
  double iniceVoltage(2076*I3Units::volt);
  int iniceTriggerMode(0);
  int icetopHGTriggerMode(1);
  int icetopLGTriggerMode(2);

  int lcMode_inice_first(0);
  int lcMode_inice_bulk(3);
  int lcMode_inice_last(4);
  int lcMode_icetop(2);

  int statusATWDa(1);
  int statusATWDb(0);
  int statusFADCInIce(10);
  int statusFADCIceTop(1);
  double speThreshold(1.618*I3Units::mV);
  double fePedestal(3.14*I3Units::volt);
  int dacTriggerBias0(900);
  int dacTriggerBias1(1000);
  int dacFADCRef(666);
  int nBinsATWD0InIce(256);
  int nBinsATWD1InIce(123);
  int nBinsATWD2InIce(4);
  int nBinsFADCInIce(42);

  int nBinsATWD0IceTop(1024);
  int nBinsATWD1IceTop(8);
  int nBinsATWD2IceTop(666);
  int nBinsFADCIceTop(765);

  double temperature(273.);
  double fadcBaselineFit_slope(1.333);
  double fadcBaselineFit_intercept(-932.6);

  double fadcGain(1.765e-5*I3Units::V);
  double atwd0Gain(-42.0);
  double atwd1Gain(-1.44);
  double atwd2Gain(-0.31459);

  double atwd_a_FreqFit_A(22.098901);
  double atwd_a_FreqFit_B(0.0091022);
  double atwd_a_FreqFit_C(0.445092);

  double atwd_b_FreqFit_A(30.98012);
  double atwd_b_FreqFit_B(1.44534);
  double atwd_b_FreqFit_C(0.1984);

  double hvGainFit_slope(7.0);
  double hvGainFit_intercept(-33.3333);

  double atwdBinCalibFit_slope(-0.0021091*I3Units::V);
  double atwdBinCalibFit_intercept(3.4*I3Units::V);

  double atwd_response_width(0.33333);

  double atwda_deltat(0.298897);
  double atwdb_deltat(0.9879823);

  double spe_disc_thresh_slope(198.987823);
  double spe_disc_thresh_int(19084.30);
  double mpe_disc_thresh_slope(121.2222);
  double mpe_disc_thresh_int(1290.000001);
  double pmt_disc_thresh_slope(200.0);
  double pmt_disc_thresh_int(100.0);

  I3Tray tray;

  std::string I3_PORTS(getenv("I3_PORTS"));
  std::string gcd_file("/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz");

  tray.AddModule("I3InfiniteSource","sourceme")
    ("Prefix", I3_PORTS + gcd_file);

  tray.AddModule("I3MCEventHeaderGenerator","headerme")
    ("Year", 2007)
    ("DAQTime",314159);

  std::string stringsToUse("21,29,30,38,39,40,49,50,59,58,67,66,74,73,65,72,78,48,57,47,46,56,63,64,55,71,70,76,77,75,69,60,68,61,62,52,44,53,54,45");
  std::string stationsToUse(stringsToUse);
    
  tray.AddService("I3GeometrySelectorServiceFactory","geo-selector")
    ("StringsToUse",stringsToUse)
    ("StationsToUse",stationsToUse)
    ("GeoSelectorName","IC40-Geo");

  //this should install default values for 40 strings
  tray.AddService("I3MCSourceServiceFactory","ic40")
    ("GeoServiceName","IC40-Geo")
    ("CalServiceName","IC40-CalService")
    ("StatusServiceName","IC40-StatService");
    
  tray.AddService("I3TweakDOMStatusService","tweak-status")
    ("OldServiceName","IC40-StatService")
    ("IceTopLCWindowPre",icetopLCWindowPre)
    ("IceTopLCWindowPost",icetopLCWindowPost)
    ("IceTopHighGainVoltage",icetopHighGainVoltage)
    ("IceTopLowGainVoltage",icetopLowGainVoltage)
    ("InIceLCWindowPre",iniceLCWindowPre)
    ("InIceLCWindowPost",iniceLCWindowPost)
    ("InIceLCSpan",inice_LCSpan)
    ("IceTopLCSpan",icetop_LCSpan)
    ("InIceVoltage",iniceVoltage)
    ("InIceTriggerMode",iniceTriggerMode)
    ("IceTopHGTriggerMode",icetopHGTriggerMode)
    ("IceTopLGTriggerMode",icetopLGTriggerMode)
    ("LCModeInIceFirstDOM",lcMode_inice_first)
    ("LCModeInIceBulkDOMs",lcMode_inice_bulk)
    ("LCModeInIceLastDOM",lcMode_inice_last)
    ("LCModeIceTopDOMs",lcMode_icetop)
    ("StatusATWDa",statusATWDa)
    ("StatusATWDb",statusATWDb)
    ("StatusFADCInIce",statusFADCInIce)
    ("StatusFADCIceTop",statusFADCIceTop)
    ("SPEThreshold",speThreshold)
    ("FEPedestal",fePedestal)
    ("DACTriggerBias0",dacTriggerBias0)
    ("DACTriggerBias1",dacTriggerBias1)
    ("DACFADCRef",dacFADCRef)
    ("NBinsATWD0InIce",nBinsATWD0InIce)
    ("NBinsATWD1InIce",nBinsATWD1InIce)
    ("NBinsATWD2InIce",nBinsATWD2InIce)
    ("NBinsFADCInIce",nBinsFADCInIce)
    ("NBinsATWD0IceTop",nBinsATWD0IceTop)
    ("NBinsATWD1IceTop",nBinsATWD1IceTop)
    ("NBinsATWD2IceTop",nBinsATWD2IceTop)
    ("NBinsFADCIceTop",nBinsFADCIceTop);

  tray.AddService("I3TweakCalibrationService","tweak-cal")
    ("OldServiceName","IC40-CalService")
    ("Temperature",temperature)
    ("FADCBaselineFit_slope",fadcBaselineFit_slope)
    ("FADCBaselineFit_intercept",fadcBaselineFit_intercept)
    ("FADCGain",fadcGain)
    ("ATWD0Gain",atwd0Gain)
    ("ATWD1Gain",atwd1Gain)
    ("ATWD2Gain",atwd2Gain)
    ("ATWD_a_FreqFit_A",atwd_a_FreqFit_A)
    ("ATWD_a_FreqFit_B",atwd_a_FreqFit_B)
    ("ATWD_a_FreqFit_C",atwd_a_FreqFit_C)
    ("ATWD_b_FreqFit_A",atwd_b_FreqFit_A)
    ("ATWD_b_FreqFit_B",atwd_b_FreqFit_B)
    ("ATWD_b_FreqFit_C",atwd_b_FreqFit_C)
    ("HVGainFit_slope",hvGainFit_slope)
    ("HVGainFit_intercept",hvGainFit_intercept)
    ("ATWDBinCalibFit_slope",atwdBinCalibFit_slope)
    ("ATWDBinCalibFit_intercept",atwdBinCalibFit_intercept)
    ("ATWDResponseWidth",atwd_response_width)
    ("ATWDADeltaT",atwda_deltat)
    ("ATWDBDeltaT",atwdb_deltat)
    ("SPEDiscThreshIntercept",spe_disc_thresh_slope)
    ("SPEDiscThreshSlope",spe_disc_thresh_int)
    ("MPEDiscThreshIntercept",mpe_disc_thresh_slope)
    ("MPEDiscThreshSlope",mpe_disc_thresh_int)
    ("PMTDiscThreshIntercept",pmt_disc_thresh_slope)
    ("PMTDiscThreshSlope",pmt_disc_thresh_int);

  std::string stringsToUse_80("1:80");
  std::string stationsToUse_80("1:80");
    
  tray.AddService("I3GeometrySelectorServiceFactory","ic80")
    ("StringsToUse",stringsToUse_80)
    ("StationsToUse",stationsToUse_80)
    ("GeoSelectorName","IC80-Geo");


  std::vector<std::string> doNotModifyStrings;
  split(doNotModifyStrings,stringsToUse,is_any_of(","));

  std::vector<int> doNotModifyStrings_Int;
  BOOST_FOREACH(std::string& s,doNotModifyStrings)
    doNotModifyStrings_Int.push_back(atoi(s.c_str()));

  std::vector<std::string> doNotModifyStations;
  split(doNotModifyStations,stationsToUse,is_any_of(","));

  std::vector<int> doNotModifyStations_Int;
  BOOST_FOREACH(std::string& s,doNotModifyStations)
    doNotModifyStations_Int.push_back(atoi(s.c_str()));

  tray.AddService("I3MCSourceServiceFactory","mcsource")
    ("GeoServiceName","IC80-Geo")
    ("OldCalServiceName","I3TweakCalibrationService")
    ("OldStatusServiceName","I3TweakDOMStatusService")
    ("DoNotModifyStrings",doNotModifyStrings_Int)
    ("DoNotModifyStations",doNotModifyStations_Int);
  
  tray.AddModule("I3MetaSynth","muxer")
    ("GeometryService","IC80-Geo");
  
  tray.AddModule("I3SimSourceTestModule","test_module_tweaks")
    ("StringsToUse",stringsToUse)
    ("StationsToUse",stationsToUse)
    ("DetStat_IceTopLCWindowPre",icetopLCWindowPre)
    ("DetStat_IceTopLCWindowPost",icetopLCWindowPost)
    ("DetStat_IceTopHighGainVoltage",icetopHighGainVoltage)
    ("DetStat_IceTopLowGainVoltage",icetopLowGainVoltage)
    ("DetStat_InIceLCWindowPre",iniceLCWindowPre)
    ("DetStat_InIceLCWindowPost",iniceLCWindowPost)
    ("DetStat_InIceLCSpan",inice_LCSpan)
    ("DetStat_IceTopLCSpan",icetop_LCSpan)
    ("DetStat_InIceVoltage",iniceVoltage)
    ("DetStat_InIceTriggerMode",iniceTriggerMode)
    ("DetStat_IceTopHGTriggerMode",icetopHGTriggerMode)
    ("DetStat_IceTopLGTriggerMode",icetopLGTriggerMode)
    ("DetStat_LCModeInIceFirstDOM",lcMode_inice_first)
    ("DetStat_LCModeInIceBulkDOMs",lcMode_inice_bulk)
    ("DetStat_LCModeInIceLastDOM",lcMode_inice_last)
    ("DetStat_LCModeIceTopDOMs",lcMode_icetop)
    ("DetStat_StatusATWDa",statusATWDa)
    ("DetStat_StatusATWDb",statusATWDb)
    ("DetStat_StatusFADCInIce",statusFADCInIce)
    ("DetStat_StatusFADCIceTop",statusFADCIceTop)
    ("DetStat_SPEThreshold",speThreshold)
    ("DetStat_FEPedestal",fePedestal)
    ("DetStat_DACTriggerBias0",dacTriggerBias0)
    ("DetStat_DACTriggerBias1",dacTriggerBias1)
    ("DetStat_DACFADCRef",dacFADCRef)
    ("DetStat_NBinsATWD0InIce",nBinsATWD0InIce)
    ("DetStat_NBinsATWD1InIce",nBinsATWD1InIce)
    ("DetStat_NBinsATWD2InIce",nBinsATWD2InIce)
    ("DetStat_NBinsFADCInIce",nBinsFADCInIce)
    ("DetStat_NBinsATWD0IceTop",nBinsATWD0IceTop)
    ("DetStat_NBinsATWD1IceTop",nBinsATWD1IceTop)
    ("DetStat_NBinsATWD2IceTop",nBinsATWD2IceTop)
    ("DetStat_NBinsFADCIceTop",nBinsFADCIceTop)
    ("Calib_Temperature",temperature)
    ("Calib_fadcBaselineFit_slope",fadcBaselineFit_slope)
    ("Calib_fadcBaselineFit_intercept",fadcBaselineFit_intercept)
    ("Calib_fadcGain",fadcGain)
    ("Calib_atwd0Gain",atwd0Gain)
    ("Calib_atwd1Gain",atwd1Gain)
    ("Calib_atwd2Gain",atwd2Gain)
    ("Calib_atwd_a_FreqFit_A",atwd_a_FreqFit_A)
    ("Calib_atwd_a_FreqFit_B",atwd_a_FreqFit_B)
    ("Calib_atwd_a_FreqFit_C",atwd_a_FreqFit_C)
    ("Calib_atwd_b_FreqFit_A",atwd_b_FreqFit_A)
    ("Calib_atwd_b_FreqFit_B",atwd_b_FreqFit_B)
    ("Calib_atwd_b_FreqFit_C",atwd_b_FreqFit_C)
    ("Calib_hvGainFit_slope",hvGainFit_slope)
    ("Calib_hvGainFit_intercept",hvGainFit_intercept)
    ("Calib_atwdBinCalibFit_slope",atwdBinCalibFit_slope)
    ("Calib_atwdBinCalibFit_intercept",atwdBinCalibFit_intercept)
    ("Calib_ATWDResponseWidth",atwd_response_width)
    ("Calib_ATWDADeltaT",atwda_deltat)
    ("Calib_ATWDBDeltaT",atwdb_deltat)
    ("Calib_SPEDiscThreshIntercept",spe_disc_thresh_slope)
    ("Calib_SPEDiscThreshSlope",spe_disc_thresh_int)
    ("Calib_MPEDiscThreshIntercept",mpe_disc_thresh_slope)
    ("Calib_MPEDiscThreshSlope",mpe_disc_thresh_int)
    ("Calib_PMTDiscThreshIntercept",pmt_disc_thresh_slope)
    ("Calib_PMTDiscThreshSlope",pmt_disc_thresh_int);
  
  //tests the rest
  tray.AddModule("I3SimSourceTestModule","test_module_defaults")
    ("StringsToExclude",stringsToUse) //exclude the original 40 strings from this test and test the rest
    ("StationsToExclude",stationsToUse)//exclude the original 40 stations from this test and test the rest
    ("DetStat_IceTopLCWindowPre",I3DetStatDefaults::ICETOP_LCWINDOW_PRE)
    ("DetStat_IceTopLCWindowPost",I3DetStatDefaults::ICETOP_LCWINDOW_POST)
    ("DetStat_IceTopHighGainVoltage",I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE)
    ("DetStat_IceTopLowGainVoltage",I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE)
    ("DetStat_InIceLCWindowPre",I3DetStatDefaults::INICE_LCWINDOW_PRE)
    ("DetStat_InIceLCWindowPost",I3DetStatDefaults::INICE_LCWINDOW_POST)
    ("DetStat_InIceLCSpan",I3DetStatDefaults::INICE_LCSPAN)
    ("DetStat_IceTopLCSpan",I3DetStatDefaults::ICETOP_LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_InIceTriggerMode",static_cast<int>(I3DetStatDefaults::INICE_TRIGGER_MODE))
    ("DetStat_IceTopHGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_HG_TRIGGER_MODE))
    ("DetStat_IceTopLGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_LG_TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::INICE_SPE_THRESHOLD)
    ("DetStat_FEPedestal",I3DetStatDefaults::FE_PEDESTAL)
    ("DetStat_DACTriggerBias0",I3DetStatDefaults::DAC_TRIGGER_BIAS0)
    ("DetStat_DACTriggerBias1",I3DetStatDefaults::DAC_TRIGGER_BIAS1)
    ("DetStat_DACFADCRef",I3DetStatDefaults::DAC_FADC_REF)
    ("DetStat_NBinsATWD0InIce",I3DetStatDefaults::NBINS_ATWD0_INICE)
    ("DetStat_NBinsATWD1InIce",I3DetStatDefaults::NBINS_ATWD1_INICE)
    ("DetStat_NBinsATWD2InIce",I3DetStatDefaults::NBINS_ATWD2_INICE)
    ("DetStat_NBinsFADCInIce",I3DetStatDefaults::NBINS_FADC_INICE)
    ("DetStat_NBinsATWD0IceTop",I3DetStatDefaults::NBINS_ATWD0_ICETOP)
    ("DetStat_NBinsATWD1IceTop",I3DetStatDefaults::NBINS_ATWD1_ICETOP)
    ("DetStat_NBinsATWD2IceTop",I3DetStatDefaults::NBINS_ATWD2_ICETOP)
    ("DetStat_NBinsFADCIceTop",I3DetStatDefaults::NBINS_FADC_ICETOP)
    ("Calib_Temperature",I3CalibDefaults::TEMPERATURE)
    ("Calib_fadcBaselineFit_slope",I3CalibDefaults::FADC_BASELINE_FIT_SLOPE)
    ("Calib_fadcBaselineFit_intercept",I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT)
    ("Calib_fadcGain",I3CalibDefaults::FADC_GAIN)
    ("Calib_atwd0Gain",I3CalibDefaults::ATWD0_GAIN)
    ("Calib_atwd1Gain",I3CalibDefaults::ATWD1_GAIN)
    ("Calib_atwd2Gain",I3CalibDefaults::ATWD2_GAIN)
    ("Calib_atwd_a_FreqFit_A",I3CalibDefaults::ATWD_A_FREQFIT_A)
    ("Calib_atwd_a_FreqFit_B",I3CalibDefaults::ATWD_A_FREQFIT_B)
    ("Calib_atwd_a_FreqFit_C",I3CalibDefaults::ATWD_A_FREQFIT_C)
    ("Calib_atwd_b_FreqFit_A",I3CalibDefaults::ATWD_B_FREQFIT_A)
    ("Calib_atwd_b_FreqFit_B",I3CalibDefaults::ATWD_B_FREQFIT_B)
    ("Calib_atwd_b_FreqFit_C",I3CalibDefaults::ATWD_B_FREQFIT_C)
    ("Calib_hvGainFit_slope",I3CalibDefaults::HV_GAIN_FIT_SLOPE)
    ("Calib_hvGainFit_intercept",I3CalibDefaults::HV_GAIN_FIT_INTERCEPT)
    ("Calib_atwdBinCalibFit_slope",I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE)
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
    ("Calib_ATWDResponseWidth",I3CalibDefaults::ATWD_RESPONSE_WIDTH)
    ("Calib_ATWDADeltaT",I3CalibDefaults::ATWDA_DELTAT)
    ("Calib_ATWDBDeltaT",I3CalibDefaults::ATWDB_DELTAT)
    ("Calib_SPEDiscThreshIntercept",I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_SPEDiscThreshSlope",I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE)
    ("Calib_MPEDiscThreshIntercept",I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_MPEDiscThreshSlope",I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE);
//  	 ("Calib_PMTDiscThreshIntercept", I3CalibDefaults::PMT_DISCRIMINATOR_INTERCEPT)
//	 ("Calib_PMTDiscThreshSlope", I3CalibDefaults::PMT_DISCRIMINATOR_SLOPE);
  
  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();
  
}

TEST(do_no_harm)
{
  I3Tray tray;

  std::string I3_PORTS(getenv("I3_PORTS"));
  std::string gcd_file("/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz");

  tray.AddModule("I3InfiniteSource","sourceme")
    ("Prefix", I3_PORTS + gcd_file);

  tray.AddModule("I3MCEventHeaderGenerator","headerme")
    ("Year", 2007)
    ("DAQTime",314159);

  tray.AddService("I3TweakDOMStatusService","tweak-status");
  tray.AddService("I3TweakCalibrationService","tweak-cal");
    
  tray.AddModule("I3MetaSynth","muxer")
    ("CalibrationService","I3TweakCalibrationService")
    ("DetectorStatusService","I3TweakDOMStatusService");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("StringsToUse","1:80")
    ("StationsToUse","1:80")
    ("DetStat_StartYear",I3DetStatDefaults::START_YEAR)
    ("DetStat_StartDAQTime",I3DetStatDefaults::START_DAQTIME)
    ("DetStat_EndYear",I3DetStatDefaults::END_YEAR)
    ("DetStat_EndDAQTime",I3DetStatDefaults::END_DAQTIME)
    ("DetStat_IceTopLCWindowPre",I3DetStatDefaults::ICETOP_LCWINDOW_PRE)
    ("DetStat_IceTopLCWindowPost",I3DetStatDefaults::ICETOP_LCWINDOW_POST)
    ("DetStat_IceTopHighGainVoltage",I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE)
    ("DetStat_IceTopLowGainVoltage",I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE)
    ("DetStat_InIceLCWindowPre",I3DetStatDefaults::INICE_LCWINDOW_PRE)
    ("DetStat_InIceLCWindowPost",I3DetStatDefaults::INICE_LCWINDOW_POST)
    ("DetStat_InIceLCSpan",I3DetStatDefaults::INICE_LCSPAN)
    ("DetStat_IceTopLCSpan",I3DetStatDefaults::ICETOP_LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_InIceTriggerMode",static_cast<int>(I3DetStatDefaults::INICE_TRIGGER_MODE))
    ("DetStat_IceTopHGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_HG_TRIGGER_MODE))
    ("DetStat_IceTopLGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_LG_TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::INICE_SPE_THRESHOLD)
    ("DetStat_FEPedestal",I3DetStatDefaults::FE_PEDESTAL)
    ("DetStat_DACTriggerBias0",I3DetStatDefaults::DAC_TRIGGER_BIAS0)
    ("DetStat_DACTriggerBias1",I3DetStatDefaults::DAC_TRIGGER_BIAS1)
    ("DetStat_DACFADCRef",I3DetStatDefaults::DAC_FADC_REF)
    ("DetStat_NBinsATWD0InIce",I3DetStatDefaults::NBINS_ATWD0_INICE)
    ("DetStat_NBinsATWD1InIce",I3DetStatDefaults::NBINS_ATWD1_INICE)
    ("DetStat_NBinsATWD2InIce",I3DetStatDefaults::NBINS_ATWD2_INICE)
    ("DetStat_NBinsFADCInIce",I3DetStatDefaults::NBINS_FADC_INICE)
    ("DetStat_NBinsATWD0IceTop",I3DetStatDefaults::NBINS_ATWD0_ICETOP)
    ("DetStat_NBinsATWD1IceTop",I3DetStatDefaults::NBINS_ATWD1_ICETOP)
    ("DetStat_NBinsATWD2IceTop",I3DetStatDefaults::NBINS_ATWD2_ICETOP)
    ("DetStat_NBinsFADCIceTop",I3DetStatDefaults::NBINS_FADC_ICETOP)
    ("Calib_StartYear",I3CalibDefaults::START_YEAR)
    ("Calib_StartDAQTime",I3CalibDefaults::START_DAQTIME)
    ("Calib_EndYear",I3CalibDefaults::END_YEAR)
    ("Calib_EndDAQTime",I3CalibDefaults::END_DAQTIME)
    ("Calib_Temperature",I3CalibDefaults::TEMPERATURE)
    ("Calib_fadcBaselineFit_slope",I3CalibDefaults::FADC_BASELINE_FIT_SLOPE)
    ("Calib_fadcBaselineFit_intercept",I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT)
    ("Calib_fadcGain",I3CalibDefaults::FADC_GAIN)
    ("Calib_atwd0Gain",I3CalibDefaults::ATWD0_GAIN)
    ("Calib_atwd1Gain",I3CalibDefaults::ATWD1_GAIN)
    ("Calib_atwd2Gain",I3CalibDefaults::ATWD2_GAIN)
    ("Calib_atwd_a_FreqFit_A",I3CalibDefaults::ATWD_A_FREQFIT_A)
    ("Calib_atwd_a_FreqFit_B",I3CalibDefaults::ATWD_A_FREQFIT_B)
    ("Calib_atwd_a_FreqFit_C",I3CalibDefaults::ATWD_A_FREQFIT_C)
    ("Calib_atwd_b_FreqFit_A",I3CalibDefaults::ATWD_B_FREQFIT_A)
    ("Calib_atwd_b_FreqFit_B",I3CalibDefaults::ATWD_B_FREQFIT_B)
    ("Calib_atwd_b_FreqFit_C",I3CalibDefaults::ATWD_B_FREQFIT_C)
    ("Calib_hvGainFit_slope",I3CalibDefaults::HV_GAIN_FIT_SLOPE)
    ("Calib_hvGainFit_intercept",I3CalibDefaults::HV_GAIN_FIT_INTERCEPT)
    ("Calib_atwdBinCalibFit_slope",I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE)
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
    ("Calib_ATWDResponseWidth",I3CalibDefaults::ATWD_RESPONSE_WIDTH)
    ("Calib_ATWDADeltaT",I3CalibDefaults::ATWDA_DELTAT)
    ("Calib_ATWDBDeltaT",I3CalibDefaults::ATWDB_DELTAT)
    ("Calib_SPEDiscThreshIntercept",I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_SPEDiscThreshSlope",I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE)
    ("Calib_MPEDiscThreshIntercept",I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_MPEDiscThreshSlope",I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE)
  	 ("Calib_PMTDiscThreshIntercept", I3CalibDefaults::PMT_DISCRIMINATOR_INTERCEPT)
	 ("Calib_PMTDiscThreshSlope", I3CalibDefaults::PMT_DISCRIMINATOR_SLOPE);


  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

TEST(do_no_harm_MJD)
{
  I3Tray tray;

  std::string I3_PORTS(getenv("I3_PORTS"));
  std::string gcd_file("/test-data/sim/GeoCalibDetectorStatus_IC59.55040.i3.gz");

  tray.AddModule("I3InfiniteSource","sourceme")
    ("Prefix", I3_PORTS + gcd_file);

  tray.AddModule("I3MCEventHeaderGenerator","headerme")
    ("Year", 2007)
    ("DAQTime",314159);

  tray.AddService("I3TweakDOMStatusService","tweak-status");
  tray.AddService("I3TweakCalibrationService","tweak-cal");
    
  tray.AddModule("I3MetaSynth","muxer")
    ("CalibrationService","I3TweakCalibrationService")
    ("DetectorStatusService","I3TweakDOMStatusService");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("StringsToUse","1:80")
    ("StationsToUse","1:80")
    ("DetStat_StartYear",I3DetStatDefaults::START_YEAR)
    ("DetStat_StartDAQTime",I3DetStatDefaults::START_DAQTIME)
    ("DetStat_EndYear",I3DetStatDefaults::END_YEAR)
    ("DetStat_EndDAQTime",I3DetStatDefaults::END_DAQTIME)
    ("DetStat_IceTopLCWindowPre",I3DetStatDefaults::ICETOP_LCWINDOW_PRE)
    ("DetStat_IceTopLCWindowPost",I3DetStatDefaults::ICETOP_LCWINDOW_POST)
    ("DetStat_IceTopHighGainVoltage",I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE)
    ("DetStat_IceTopLowGainVoltage",I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE)
    ("DetStat_InIceLCWindowPre",I3DetStatDefaults::INICE_LCWINDOW_PRE)
    ("DetStat_InIceLCWindowPost",I3DetStatDefaults::INICE_LCWINDOW_POST)
    ("DetStat_InIceLCSpan",I3DetStatDefaults::INICE_LCSPAN)
    ("DetStat_IceTopLCSpan",I3DetStatDefaults::ICETOP_LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_InIceTriggerMode",static_cast<int>(I3DetStatDefaults::INICE_TRIGGER_MODE))
    ("DetStat_IceTopHGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_HG_TRIGGER_MODE))
    ("DetStat_IceTopLGTriggerMode",static_cast<int>(I3DetStatDefaults::ICETOP_LG_TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::INICE_SPE_THRESHOLD)
    ("DetStat_FEPedestal",I3DetStatDefaults::FE_PEDESTAL)
    ("DetStat_DACTriggerBias0",I3DetStatDefaults::DAC_TRIGGER_BIAS0)
    ("DetStat_DACTriggerBias1",I3DetStatDefaults::DAC_TRIGGER_BIAS1)
    ("DetStat_DACFADCRef",I3DetStatDefaults::DAC_FADC_REF)
    ("DetStat_NBinsATWD0InIce",I3DetStatDefaults::NBINS_ATWD0_INICE)
    ("DetStat_NBinsATWD1InIce",I3DetStatDefaults::NBINS_ATWD1_INICE)
    ("DetStat_NBinsATWD2InIce",I3DetStatDefaults::NBINS_ATWD2_INICE)
    ("DetStat_NBinsFADCInIce",I3DetStatDefaults::NBINS_FADC_INICE)
    ("DetStat_NBinsATWD0IceTop",I3DetStatDefaults::NBINS_ATWD0_ICETOP)
    ("DetStat_NBinsATWD1IceTop",I3DetStatDefaults::NBINS_ATWD1_ICETOP)
    ("DetStat_NBinsATWD2IceTop",I3DetStatDefaults::NBINS_ATWD2_ICETOP)
    ("DetStat_NBinsFADCIceTop",I3DetStatDefaults::NBINS_FADC_ICETOP)
    ("Calib_StartYear",I3CalibDefaults::START_YEAR)
    ("Calib_StartDAQTime",I3CalibDefaults::START_DAQTIME)
    ("Calib_EndYear",I3CalibDefaults::END_YEAR)
    ("Calib_EndDAQTime",I3CalibDefaults::END_DAQTIME)
    ("Calib_Temperature",I3CalibDefaults::TEMPERATURE)
    ("Calib_fadcBaselineFit_slope",I3CalibDefaults::FADC_BASELINE_FIT_SLOPE)
    ("Calib_fadcBaselineFit_intercept",I3CalibDefaults::FADC_BASSLINE_FIT_INTERCEPT)
    ("Calib_fadcGain",I3CalibDefaults::FADC_GAIN)
    ("Calib_atwd0Gain",I3CalibDefaults::ATWD0_GAIN)
    ("Calib_atwd1Gain",I3CalibDefaults::ATWD1_GAIN)
    ("Calib_atwd2Gain",I3CalibDefaults::ATWD2_GAIN)
    ("Calib_atwd_a_FreqFit_A",I3CalibDefaults::ATWD_A_FREQFIT_A)
    ("Calib_atwd_a_FreqFit_B",I3CalibDefaults::ATWD_A_FREQFIT_B)
    ("Calib_atwd_a_FreqFit_C",I3CalibDefaults::ATWD_A_FREQFIT_C)
    ("Calib_atwd_b_FreqFit_A",I3CalibDefaults::ATWD_B_FREQFIT_A)
    ("Calib_atwd_b_FreqFit_B",I3CalibDefaults::ATWD_B_FREQFIT_B)
    ("Calib_atwd_b_FreqFit_C",I3CalibDefaults::ATWD_B_FREQFIT_C)
    ("Calib_hvGainFit_slope",I3CalibDefaults::HV_GAIN_FIT_SLOPE)
    ("Calib_hvGainFit_intercept",I3CalibDefaults::HV_GAIN_FIT_INTERCEPT)
    ("Calib_atwdBinCalibFit_slope",I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE)
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT)
    ("Calib_ATWDResponseWidth",I3CalibDefaults::ATWD_RESPONSE_WIDTH)
    ("Calib_ATWDADeltaT",I3CalibDefaults::ATWDA_DELTAT)
    ("Calib_ATWDBDeltaT",I3CalibDefaults::ATWDB_DELTAT)
    ("Calib_SPEDiscThreshIntercept",I3CalibDefaults::SPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_SPEDiscThreshSlope",I3CalibDefaults::SPE_DISCRIMINATOR_SLOPE)
    ("Calib_MPEDiscThreshIntercept",I3CalibDefaults::MPE_DISCRIMINATOR_INTERCEPT)
    ("Calib_MPEDiscThreshSlope",I3CalibDefaults::MPE_DISCRIMINATOR_SLOPE)
  	 ("Calib_PMTDiscThreshIntercept", I3CalibDefaults::PMT_DISCRIMINATOR_INTERCEPT)
	 ("Calib_PMTDiscThreshSlope", I3CalibDefaults::PMT_DISCRIMINATOR_SLOPE);


  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

