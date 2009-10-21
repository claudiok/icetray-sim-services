#include <I3Test.h>

#include "icetray/I3Tray.h"
#include "I3SimSourceTestModule.h"
#include "sim-services/sim-source/I3MCSourceServiceFactory.h"
#include "sim-services/sim-source/I3DBHistogram.h"
#include "sim-services/time-generator/I3MCTimeGeneratorServiceFactory.h"
#include "dataclasses/I3Units.h"
#include "sim-services/tweak-sources/I3TweakTriggerService.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>

using boost::assign::list_of;
using boost::assign::map_list_of;
using boost::algorithm::split;
using boost::algorithm::is_any_of;

// Test some of Calibrate Module functionality
TEST_GROUP(TweakTrigger);

TEST(tweaked_config)
{
  I3Tray tray;

  std::string I3_BUILD = getenv("I3_BUILD");
  std::string amageofile = I3_BUILD + "/phys-services/resources/amanda.geo";
  std::string icecubegeofile = I3_BUILD + "/phys-services/resources/icecube.geo";

  tray.AddService("I3MCTimeGeneratorServiceFactory","time-gen")    
    ("Year",2007)
    ("DAQTime",314159);
    
  tray.AddService("I3TextFileGeometryServiceFactory","geometry")
    ("AmandaGeoFile",amageofile)
    ("IceCubeGeoFile",icecubegeofile);
    
  tray.AddService("I3MCSourceServiceFactory","mcsource");

  vector<string> name_list = list_of("threshold")("timeWindow");
  vector<int> value_list = list_of(10)(5000);

  vector<double> ro = list_of(3000.)(3000.)(0.);
  map<int, vector<double> > readout_windows = 
    map_list_of(0, ro);

  I3TriggerStatus ts;

  tray.AddService("I3TweakTriggerService","tweak-trigg")
    ("TweakedServiceName","I3TweakTriggerService")
    ("SourceID",0)// IN_ICE
    ("TypeID",0) //SIMPLE_MULTIPLICITY
    ("ConfigID",1006) //config for 2009
    ("TriggerName","inice_trig")
    ("ValueNameList",name_list)
    ("ValueList",value_list)
    //("ReadoutConfigMap",readout_windows);
    ("I3TriggerStatus",ts);


  tray.AddModule("I3Muxer","muxer")
    ("DetectorStatusService","I3TweakTriggerService");

  tray.AddModule("I3TweakTriggerTestModule","test_module")
    ("SourceID",0)// IN_ICE
    ("TypeID",0) //SIMPLE_MULTIPLICITY
    ("ConfigID",1006) //config for 2009
    ("TriggerName","inice_trig")
    ("ValueNameList",name_list)
    ("ValueList",value_list)
    //("ReadoutConfigMap",readout_windows)
    ;

  tray.AddModule("TrashCan","trash");
  
  tray.Execute(4);
  tray.Finish();

}

