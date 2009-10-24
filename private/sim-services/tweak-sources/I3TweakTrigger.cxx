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

  /**
   * Form the trigger to pull the information out of the trigger status
   */
  TriggerKey key;
  key.SetSource(key_source_);
  key.SetType(key_type_);
  key.SetConfigID(key_configID_);

  /**
   * Get the trigger status out of the detector status
   */
  map<TriggerKey,I3TriggerStatus>& t_map = detectorStatus_->triggerStatus;
  map<TriggerKey,I3TriggerStatus>::iterator t_iter;
  t_iter = t_map.find(key);
  if(t_iter == t_map.end()) log_fatal("trigger key not found in map");

  /**
   * Set the trigger name to the 'tweaked' trigger name
   */
  t_iter->second.GetTriggerName() = trig_name_;

  /**
   * Loop through the settings and make the required tweaks
   */
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
 
 /**
   * Loop through the readout windows and make the required tweaks
   */
  t_iter->second.GetReadoutSettings() = readout_config_map_;

  return detectorStatus_;
}
