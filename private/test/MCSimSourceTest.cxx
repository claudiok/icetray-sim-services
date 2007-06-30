#include <I3Test.h>

#include "icetray/I3Tray.h"
#include "I3SimSourceTestModule.h"
#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3DBHistogram.h"
#include "sim-services/time-generator/I3MCTimeGeneratorServiceFactory.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/default-values/I3CalibrationDefaults.h"
#include "sim-services/sim-source/default-values/I3DetectorStatusDefaults.h"
#include "sim-services/sim-source/default-values/I3InIceTriggerDefaults.h"
#include "sim-services/sim-source/default-values/I3IceTopTriggerDefaults.h"
#include "sim-services/tweak-sources/I3TweakDOMStatusService.h"
#include "sim-services/tweak-sources/I3TweakCalibrationService.h"
#include "sim-services/tweak-sources/I3TweakTriggerService.h"

#include "sim-services/sim-source/I3MCTWRParamsService.h"

#include "icetray/test/ConstructorTest.h"

using namespace std;

// Test some of Calibrate Module functionality
TEST_GROUP(SimSource);

TEST(clean_construction){
  clean_constructor_test<I3MCSourceServiceFactory>();
  clean_constructor_test<I3DBHistogram>();
  clean_constructor_test<I3MCTimeGeneratorServiceFactory>();
  clean_constructor_test<I3MCTWRParamsService>();
  clean_constructor_test<I3TweakDOMStatusService>();
  clean_constructor_test<I3TweakCalibrationService>();
  clean_constructor_test<I3TweakTriggerService>();
}

TEST(default_config)
{
  I3Tray tray;

  std::string I3_WORK = getenv("I3_WORK");
  std::string amageofile = I3_WORK + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_WORK + "/phys-services/resources/icecube.geo";

  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("Year",2007)
    ("DAQTime",314159);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);
    
  tray.AddService("I3MCSourceServiceFactory","mcsource");

  tray.AddModule("I3Muxer","muxer");

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
    ("DetStat_LCSpan",I3DetStatDefaults::LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_TriggerMode",static_cast<int>(I3DetStatDefaults::TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::SPE_THRESHOLD)
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
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);

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
  int lcSpan(2);
  double iniceVoltage(2076*I3Units::volt);
  int triggerMode(0);

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

  I3Tray tray;

  std::string I3_WORK = getenv("I3_WORK");
  std::string amageofile = I3_WORK + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_WORK + "/phys-services/resources/icecube.geo";

  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("Year",2007)
    ("DAQTime",314159);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);
    
  tray.AddService("I3MCSourceServiceFactory","mcsource");

  tray.AddService("I3TweakDOMStatusService","tweak-status")
    ("IceTopLCWindowPre",icetopLCWindowPre)
    ("IceTopLCWindowPost",icetopLCWindowPost)
    ("IceTopHighGainVoltage",icetopHighGainVoltage)
    ("IceTopLowGainVoltage",icetopLowGainVoltage)
    ("InIceLCWindowPre",iniceLCWindowPre)
    ("InIceLCWindowPost",iniceLCWindowPost)
    ("LCSpan",lcSpan)
    ("InIceVoltage",iniceVoltage)
    ("TriggerMode",triggerMode)
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
    ("ATWDBinCalibFit_intercept",atwdBinCalibFit_intercept);

  tray.AddModule("I3Muxer","muxer")
    ("CalibrationService","I3TweakCalibrationService")
    ("DetectorStatusService","I3TweakDOMStatusService");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("DetStat_IceTopLCWindowPre",icetopLCWindowPre)
    ("DetStat_IceTopLCWindowPost",icetopLCWindowPost)
    ("DetStat_IceTopHighGainVoltage",icetopHighGainVoltage)
    ("DetStat_IceTopLowGainVoltage",icetopLowGainVoltage)
    ("DetStat_InIceLCWindowPre",iniceLCWindowPre)
    ("DetStat_InIceLCWindowPost",iniceLCWindowPost)
    ("DetStat_LCSpan",lcSpan)
    ("DetStat_InIceVoltage",iniceVoltage)
    ("DetStat_TriggerMode",triggerMode)
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
    ("Calib_atwdBinCalibFit_intercept",atwdBinCalibFit_intercept);

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
  int lcSpan(2);
  double iniceVoltage(2076*I3Units::volt);
  int triggerMode(0);

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

  I3Tray tray;

  std::string I3_WORK = getenv("I3_WORK");
  std::string amageofile = I3_WORK + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_WORK + "/phys-services/resources/icecube.geo";
  
  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("Year",2007)
    ("DAQTime",314159);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);

  string stringsToUse("21,29,30,38,39,40,49,50,59");
  string stationsToUse("21,29,30,38,39,40,47,48,49,50,57,58,59,66,67,74");
    
  tray.AddService("I3GeometrySelectorServiceFactory","geo-selector")
    ("StringsToUse",stringsToUse)
    ("StationsToUse",stationsToUse)
    ("GeoSelectorName","I3GeometrySelectorService");

  //this should install default values for 9 strings
  tray.AddService("I3MCSourceServiceFactory","ice9")
    ("GeoServiceName","I3GeometrySelectorService")
    ("CalServiceName","IC9-CalService")
    ("StatusServiceName","IC9-StatService");
    
  tray.AddService("I3TweakDOMStatusService","tweak-status")
    ("OldServiceName","IC9-StatService")
    ("IceTopLCWindowPre",icetopLCWindowPre)
    ("IceTopLCWindowPost",icetopLCWindowPost)
    ("IceTopHighGainVoltage",icetopHighGainVoltage)
    ("IceTopLowGainVoltage",icetopLowGainVoltage)
    ("InIceLCWindowPre",iniceLCWindowPre)
    ("InIceLCWindowPost",iniceLCWindowPost)
    ("LCSpan",lcSpan)
    ("InIceVoltage",iniceVoltage)
    ("TriggerMode",triggerMode)
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
    ("OldServiceName","IC9-CalService")
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
    ("ATWDBinCalibFit_intercept",atwdBinCalibFit_intercept);

  string stringsToUse_23("21,29,30,38,39,40,49,50,59,58,67,66,74,73,65,72,78,48,57,47,46,56,64");
  string stationsToUse_23("21,29,30,38,39,40,47,48,49,50,57,58,59,66,67,74,46,56,65,73,72,78,77,71,64,55");
    
  tray.AddService("I3GeometrySelectorServiceFactory","23-string")
    ("StringsToUse",stringsToUse_23)
    ("StationsToUse",stationsToUse_23)
    ("GeoSelectorName","23string");

  tray.AddService("I3MCSourceServiceFactory","mcsource")
    ("GeoServiceName","23string")
    ("OldCalServiceName","I3TweakCalibrationService")
    ("OldStatusServiceName","I3TweakDOMStatusService");
  
  tray.AddModule("I3Muxer","muxer")
    ("GeometryService","23string");
  
  tray.AddModule("I3SimSourceTestModule","test_module_tweaks")
    ("StringsToUse",stringsToUse)
    ("StationsToUse",stationsToUse)
    ("DetStat_IceTopLCWindowPre",icetopLCWindowPre)
    ("DetStat_IceTopLCWindowPost",icetopLCWindowPost)
    ("DetStat_IceTopHighGainVoltage",icetopHighGainVoltage)
    ("DetStat_IceTopLowGainVoltage",icetopLowGainVoltage)
    ("DetStat_InIceLCWindowPre",iniceLCWindowPre)
    ("DetStat_InIceLCWindowPost",iniceLCWindowPost)
    ("DetStat_LCSpan",lcSpan)
    ("DetStat_InIceVoltage",iniceVoltage)
    ("DetStat_TriggerMode",triggerMode)

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
    ("Calib_atwdBinCalibFit_intercept",atwdBinCalibFit_intercept);
  
  //tests the rest
  tray.AddModule("I3SimSourceTestModule","test_module_defaults")
    ("StringsToUse","58,67,66,74,73,65,72,78,48,57,47,46,56,64")
    ("StationsToUse","46,56,65,73,72,78,77,71,64,55")
    ("DetStat_IceTopLCWindowPre",I3DetStatDefaults::ICETOP_LCWINDOW_PRE)
    ("DetStat_IceTopLCWindowPost",I3DetStatDefaults::ICETOP_LCWINDOW_POST)
    ("DetStat_IceTopHighGainVoltage",I3DetStatDefaults::ICETOP_HIGHGAIN_VOLTAGE)
    ("DetStat_IceTopLowGainVoltage",I3DetStatDefaults::ICETOP_LOWGAIN_VOLTAGE)
    ("DetStat_InIceLCWindowPre",I3DetStatDefaults::INICE_LCWINDOW_PRE)
    ("DetStat_InIceLCWindowPost",I3DetStatDefaults::INICE_LCWINDOW_POST)
    ("DetStat_LCSpan",I3DetStatDefaults::LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_TriggerMode",static_cast<int>(I3DetStatDefaults::TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::SPE_THRESHOLD)
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
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);
  
  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();
  
}

TEST(do_no_harm)
{
  I3Tray tray;

  std::string I3_WORK = getenv("I3_WORK");
  std::string amageofile = I3_WORK + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_WORK + "/phys-services/resources/icecube.geo";

  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("Year",2007)
    ("DAQTime",314159);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);

  tray.AddService("I3MCSourceServiceFactory","mcsource");

  tray.AddService("I3TweakDOMStatusService","tweak-status");
  tray.AddService("I3TweakCalibrationService","tweak-cal");
    
  tray.AddModule("I3Muxer","muxer")
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
    ("DetStat_LCSpan",I3DetStatDefaults::LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_TriggerMode",static_cast<int>(I3DetStatDefaults::TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::SPE_THRESHOLD)
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
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

TEST(do_no_harm_MJD)
{
  I3Tray tray;

  std::string I3_WORK = getenv("I3_WORK");
  std::string amageofile = I3_WORK + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_WORK + "/phys-services/resources/icecube.geo";

  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("MJD",54269);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);

  tray.AddService("I3MCSourceServiceFactory","mcsource");

  tray.AddService("I3TweakDOMStatusService","tweak-status");
  tray.AddService("I3TweakCalibrationService","tweak-cal");
    
  tray.AddModule("I3Muxer","muxer")
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
    ("DetStat_LCSpan",I3DetStatDefaults::LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_TriggerMode",static_cast<int>(I3DetStatDefaults::TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::SPE_THRESHOLD)
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
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

TEST(do_no_harm_skip)
{
  I3Tray tray;

  std::string I3_WORK = getenv("I3_WORK");
  std::string amageofile = I3_WORK + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_WORK + "/phys-services/resources/icecube.geo";

  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("MJD",54269);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);

  vector<int> skipStrings;
  skipStrings.push_back(1);
  skipStrings.push_back(2);
  skipStrings.push_back(3);

  vector<int> skipStations;
  skipStations.push_back(4);
  skipStations.push_back(5);
  skipStations.push_back(6);


  tray.AddService("I3MCSourceServiceFactory","mcsource")
    ("SkipStrings",skipStrings)
    ("SkipStations",skipStations);

  tray.AddService("I3TweakDOMStatusService","tweak-status");
  tray.AddService("I3TweakCalibrationService","tweak-cal");
    
  tray.AddModule("I3Muxer","muxer")
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
    ("DetStat_LCSpan",I3DetStatDefaults::LCSPAN)
    ("DetStat_InIceVoltage",I3DetStatDefaults::INICE_VOLTAGE)
    ("DetStat_TriggerMode",static_cast<int>(I3DetStatDefaults::TRIGGER_MODE))
    ("DetStat_LCModeInIceFirstDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_FIRST))
    ("DetStat_LCModeInIceBulkDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_BULK))
    ("DetStat_LCModeInIceLastDOM",static_cast<int>(I3DetStatDefaults::LCMODE_INICE_LAST))
    ("DetStat_LCModeIceTopDOMs",static_cast<int>(I3DetStatDefaults::LCMODE_ICETOP))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADCInIce",static_cast<int>(I3DetStatDefaults::STATUS_FADC_INICE))
    ("DetStat_StatusFADCIceTop",static_cast<int>(I3DetStatDefaults::STATUS_FADC_ICETOP))
    ("DetStat_SPEThreshold",I3DetStatDefaults::SPE_THRESHOLD)
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
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}
