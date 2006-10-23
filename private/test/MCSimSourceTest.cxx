#include <I3Test.h>

#include "icetray/I3Tray.h"
#include "sim-services/sim-source/I3SimSourceTestModule.h"
#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "dataclasses/I3Units.h"
#include "sim-services/sim-source/I3DefaultValues.h"

using namespace std;

// Test some of Calibrate Module functionality
TEST_GROUP(SimSource);

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
    
  tray.AddService("I3MCSourceServiceFactory","mcsource")
    ("Calib_atwd_a_FreqFit_C",0.)
    ("Calib_atwd_b_FreqFit_C",0.);

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
    ("DetStat_LCMode",static_cast<int>(I3DetStatDefaults::LCMODE))
    ("DetStat_StatusATWDa",static_cast<int>(I3DetStatDefaults::STATUS_ATWDa))
    ("DetStat_StatusATWDb",static_cast<int>(I3DetStatDefaults::STATUS_ATWDb))
    ("DetStat_StatusFADC",static_cast<int>(I3DetStatDefaults::STATUS_FADC))
    ("DetStat_SPEThreshold",I3DetStatDefaults::SPE_THRESHOLD)
    ("DetStat_FEPedestal",I3DetStatDefaults::FE_PEDESTAL)
    ("DetStat_DACTriggerBias0",I3DetStatDefaults::DAC_TRIGGER_BIAS0)
    ("DetStat_DACTriggerBias1",I3DetStatDefaults::DAC_TRIGGER_BIAS1)
    ("DetStat_DACFADCRef",I3DetStatDefaults::DAC_FADC_REF)
    ("DetStat_NBinsATWD0",I3DetStatDefaults::NBINS_ATWD0)
    ("DetStat_NBinsATWD1",I3DetStatDefaults::NBINS_ATWD1)
    ("DetStat_NBinsATWD2",I3DetStatDefaults::NBINS_ATWD2)
    ("DetStat_NBinsFADC",I3DetStatDefaults::NBINS_FADC)
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
    //("Calib_atwd_a_FreqFit_C",I3CalibDefaults::ATWD_A_FREQFIT_C)
    ("Calib_atwd_a_FreqFit_C",0.)
    ("Calib_atwd_b_FreqFit_A",I3CalibDefaults::ATWD_B_FREQFIT_A)
    ("Calib_atwd_b_FreqFit_B",I3CalibDefaults::ATWD_B_FREQFIT_B)
    //("Calib_atwd_b_FreqFit_C",I3CalibDefaults::ATWD_B_FREQFIT_C)
    ("Calib_atwd_b_FreqFit_C",0.)
    ("Calib_hvGainFit_slope",I3CalibDefaults::HV_GAIN_FIT_SLOPE)
    ("Calib_hvGainFit_intercept",I3CalibDefaults::HV_GAIN_FIT_INTERCEPT)
    ("Calib_atwdBinCalibFit_slope",I3CalibDefaults::ATWD_BINCALIB_FIT_SLOPE)
    ("Calib_atwdBinCalibFit_intercept",I3CalibDefaults::ATWD_BINCALIB_FIT_INTERCEPT);

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}


TEST(custom_config)
{

  int32_t ds_startYear(2005);
  int64_t ds_startDAQTime(240258490029587286LL);
  int32_t ds_endYear(2010);
  int64_t ds_endDAQTime(236184998900773063LL);
  double icetopLCWindowPre(200.*I3Units::ns);
  double icetopLCWindowPost(300.*I3Units::ns);
  double icetopHighGainVoltage(1099*I3Units::volt);
  double icetopLowGainVoltage(444*I3Units::volt);
  double iniceLCWindowPre(1234.0*I3Units::ns);
  double iniceLCWindowPost(2143.0*I3Units::ns);
  int lcSpan(2);
  double iniceVoltage(2076*I3Units::volt);
  int triggerMode(0);
  int lcMode(2);
  int statusATWDa(1);
  int statusATWDb(0);
  int statusFADC(-1);
  double speThreshold(1.618*I3Units::mV);
  double fePedestal(3.14*I3Units::volt);
  int dacTriggerBias0(900);
  int dacTriggerBias1(1000);
  int dacFADCRef(666);
  int nBinsATWD0(256);
  int nBinsATWD1(123);
  int nBinsATWD2(4);
  int nBinsFADC(42);

  int32_t cal_startYear(2006);
  int64_t cal_startDAQTime(240258491946728273LL);
  int32_t cal_endYear(2009);
  int64_t cal_endDAQTime(220390192849823984LL);

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
    
  tray.AddService("I3MCSourceServiceFactory","mcsource")
    ("DetStat_StartYear",ds_startYear)
    ("DetStat_StartDAQTime",ds_startDAQTime)
    ("DetStat_EndYear",ds_endYear)
    ("DetStat_EndDAQTime",ds_endDAQTime)
    ("DetStat_IceTopLCWindowPre",icetopLCWindowPre)
    ("DetStat_IceTopLCWindowPost",icetopLCWindowPost)
    ("DetStat_IceTopHighGainVoltage",icetopHighGainVoltage)
    ("DetStat_IceTopLowGainVoltage",icetopLowGainVoltage)
    ("DetStat_InIceLCWindowPre",iniceLCWindowPre)
    ("DetStat_InIceLCWindowPost",iniceLCWindowPost)
    ("DetStat_LCSpan",lcSpan)
    ("DetStat_InIceVoltage",iniceVoltage)
    ("DetStat_TriggerMode",triggerMode)
    ("DetStat_LCMode",lcMode)
    ("DetStat_StatusATWDa",statusATWDa)
    ("DetStat_StatusATWDb",statusATWDb)
    ("DetStat_StatusFADC",statusFADC)
    ("DetStat_SPEThreshold",speThreshold)
    ("DetStat_FEPedestal",fePedestal)
    ("DetStat_DACTriggerBias0",dacTriggerBias0)
    ("DetStat_DACTriggerBias1",dacTriggerBias1)
    ("DetStat_DACFADCRef",dacFADCRef)
    ("DetStat_NBinsATWD0",nBinsATWD0)
    ("DetStat_NBinsATWD1",nBinsATWD1)
    ("DetStat_NBinsATWD2",nBinsATWD2)
    ("DetStat_NBinsFADC",nBinsFADC)
    ("Calib_StartYear",cal_startYear)
    ("Calib_StartDAQTime",cal_startDAQTime)
    ("Calib_EndYear",cal_endYear)
    ("Calib_EndDAQTime",cal_endDAQTime)
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


  tray.AddModule("I3Muxer","muxer");

  tray.AddModule("I3SimSourceTestModule","test_module")
    ("DetStat_StartYear",ds_startYear)
    ("DetStat_StartDAQTime",ds_startDAQTime)
    ("DetStat_EndYear",ds_endYear)
    ("DetStat_EndDAQTime",ds_endDAQTime)
    ("DetStat_IceTopLCWindowPre",icetopLCWindowPre)
    ("DetStat_IceTopLCWindowPost",icetopLCWindowPost)
    ("DetStat_IceTopHighGainVoltage",icetopHighGainVoltage)
    ("DetStat_IceTopLowGainVoltage",icetopLowGainVoltage)
    ("DetStat_InIceLCWindowPre",iniceLCWindowPre)
    ("DetStat_InIceLCWindowPost",iniceLCWindowPost)
    ("DetStat_LCSpan",lcSpan)
    ("DetStat_InIceVoltage",iniceVoltage)
    ("DetStat_TriggerMode",triggerMode)
    ("DetStat_LCMode",lcMode)
    ("DetStat_StatusATWDa",statusATWDa)
    ("DetStat_StatusATWDb",statusATWDb)
    ("DetStat_StatusFADC",statusFADC)
    ("DetStat_SPEThreshold",speThreshold)
    ("DetStat_FEPedestal",fePedestal)
    ("DetStat_DACTriggerBias0",dacTriggerBias0)
    ("DetStat_DACTriggerBias1",dacTriggerBias1)
    ("DetStat_DACFADCRef",dacFADCRef)
    ("DetStat_NBinsATWD0",nBinsATWD0)
    ("DetStat_NBinsATWD1",nBinsATWD1)
    ("DetStat_NBinsATWD2",nBinsATWD2)
    ("DetStat_NBinsFADC",nBinsFADC)
    ("Calib_StartYear",cal_startYear)
    ("Calib_StartDAQTime",cal_startDAQTime)
    ("Calib_EndYear",cal_endYear)
    ("Calib_EndDAQTime",cal_endDAQTime)
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
