/**
 * class: I3TweakTriggerTestModule.cxx
 * (c) 2004 IceCube Collaboration
 * Version $Id: I3TweakTriggerTestModule.cxx,v 1.16 2005/03/17 18:33:43 olivas Exp $
 *
 * Date 08 June 2006
 * @version $Revision: 1.1 $
 * @date $Date: 2005/03/17 18:33:43 $
 * @author Alex Olivas <olivas@icecube.umd.edu>
 *
 */

#include <I3Test.h>
#include "I3TweakTriggerTestModule.h"

#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

#include <dataclasses/status/I3DetectorStatus.h>
#include <boost/foreach.hpp>

I3_MODULE(I3TweakTriggerTestModule);

typedef std::map<int, vector<double> > readout_config_t;

I3TweakTriggerTestModule::I3TweakTriggerTestModule(const I3Context& ctx) : 
  I3Module(ctx)
{ 
  log_debug("Constructor I3TweakTriggerTestModule");  

  AddParameter("SourceID","Trigger Source ID",key_source_);
  AddParameter("TypeID","Trigger Type ID",key_type_);
  AddParameter("ConfigID","Trigger Config ID",key_configID_);
  AddParameter("TriggerName","Trigger name",trig_name_);
  AddParameter("ValueNameList","List of name of values",setting_name_list_);
  AddParameter("ValueList","List of values",setting_value_list_);
  AddParameter("ReadoutConfigMap","Map of readout windows",readout_config_map_);

  AddOutBox("OutBox");
}

I3TweakTriggerTestModule::~I3TweakTriggerTestModule(){}

void I3TweakTriggerTestModule::Configure()
{
  log_debug("Configuring I3TweakTriggerTestModule");

  GetParameter("SourceID",key_source_);
  GetParameter("TypeID",key_type_);
  GetParameter("ConfigID",key_configID_);
  GetParameter("TriggerName",trig_name_);
  GetParameter("ValueNameList",setting_name_list_);
  GetParameter("ValueList",setting_value_list_);
  GetParameter("ReadoutConfigMap",readout_config_map_);

}

void I3TweakTriggerTestModule::Physics(I3FramePtr frame)
{
  log_debug("Physics");

  const I3DetectorStatus& det_stat = frame->Get<I3DetectorStatus>();

  TriggerKey key;
  key.SetSource(static_cast<TriggerKey::SourceID>(key_source_));
  key.SetType(static_cast<TriggerKey::TypeID>(key_type_));
  key.SetConfigID(key_configID_);

  //make sure the trigger status is found in the map
  ENSURE(det_stat.triggerStatus.count(key) == 1);

  std::map<TriggerKey, I3TriggerStatus>::const_iterator t_stat_iter 
    = det_stat.triggerStatus.find(key);

  ENSURE(t_stat_iter->second.GetTriggerName() == trig_name_);

  const std::map<std::string, int>& setting_map = 
    t_stat_iter->second.GetTriggerSettings();

  int index(0);
  BOOST_FOREACH(string& key, setting_name_list_){
    ENSURE(setting_map.count(key) == 1);
    ENSURE(setting_map.find(key)->second == setting_value_list_[index]);
    index++;
  }

  const std::map<I3TriggerStatus::Subdetector, I3TriggerReadoutConfig>& 
    readout_configs = t_stat_iter->second.GetReadoutSettings();
      
  index = 0;
  BOOST_FOREACH(readout_config_t::value_type& v_pair, readout_config_map_){
    I3TriggerStatus::Subdetector sd_key = 
      static_cast<I3TriggerStatus::Subdetector>(v_pair.first);

    ENSURE(readout_configs.count(sd_key) == 1);

    const I3TriggerReadoutConfig& ro_config = 
      readout_configs.find(sd_key)->second;
    
    ENSURE(ro_config.readoutTimeMinus == v_pair.second[0]);
    ENSURE(ro_config.readoutTimePlus == v_pair.second[1]);
    ENSURE(ro_config.readoutTimeOffset == v_pair.second[2]);

  }


  PushFrame(frame,"OutBox");
}//Physics()
 
