#include "sim-services/tweak-sources/I3TweakTrigger.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

I3TweakTrigger::I3TweakTrigger(I3DetectorStatusServicePtr s):
  key_source_(TriggerKey::UNKNOWN_SOURCE),
  key_type_(TriggerKey::UNKNOWN_TYPE),
  key_configID_(INT_MIN),
  trig_name_("")
{
  oldDSService_ = s;
}

I3DetectorStatusConstPtr
I3TweakTrigger::GetDetectorStatus(I3Time time)
{
  log_debug("GetDetectorStatus");

  if(!detectorStatus_){
    if(oldDSService_){
      detectorStatus_ = I3DetectorStatusPtr(new I3DetectorStatus(*(oldDSService_->GetDetectorStatus(time))));
    }else log_fatal("this service does not create detector status objects");
  }

  TriggerKey key;
  key.SetSource(key_source_);
  key.SetType(key_type_);
  key.SetConfigID(key_configID_);

  map<TriggerKey,I3TriggerStatus>& t_map = detectorStatus_->triggerStatus;
  map<TriggerKey,I3TriggerStatus>::iterator t_iter;
  t_iter = t_map.find(key);
  if(t_iter == t_map.end()) log_fatal("trigger key not found in map");

  t_iter->second.GetTriggerName() = trig_name_;

  vector<pair<string,int> >::iterator set_iter;
  for(set_iter = setting_list_.begin();
      set_iter != setting_list_.end();
      set_iter++){
    if(t_iter->second.GetTriggerSettings().find(set_iter->first) !=
       t_iter->second.GetTriggerSettings().end()){
      log_debug("changing the value for %s to %d",
		set_iter->first.c_str(), set_iter->second);
    }else{
      log_debug("adding a new setting %s value = %d",
		set_iter->first.c_str(), set_iter->second);
    }
    t_iter->second.GetTriggerSettings()[set_iter->first] = set_iter->second;
  }

  map<I3TriggerStatus::Subdetector, I3TriggerReadoutConfig> ro_map;
  map<int, vector<double> >::iterator ro_iter;
  for(ro_iter = readout_config_map_.begin();
      ro_iter != readout_config_map_.end();
      ro_iter++){
    I3TriggerReadoutConfig ro_config;
    ro_config.readoutTimeMinus = ro_iter->second[0];
    ro_config.readoutTimePlus = ro_iter->second[1];
    ro_config.readoutTimeOffset = ro_iter->second[2];

    I3TriggerStatus::Subdetector sd = 
      static_cast<I3TriggerStatus::Subdetector>(ro_iter->first);

    ro_map[sd] = ro_config;
  }
  t_iter->second.GetReadoutSettings() = ro_map;

  return detectorStatus_;
}
