#include "sim-services/sim-source/I3TweekTrigger.h"
#include "dataclasses/status/I3DOMStatus.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3TrayHeaders.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

I3TweekTrigger::I3TweekTrigger(I3DetectorStatusServicePtr s):
  key_source_(TriggerKey::UNKNOWN_SOURCE),
  key_type_(TriggerKey::UNKNOWN_TYPE),
  key_configID_(INT_MIN),
  trig_name_(""),
  trig_setting_name_(""),
  trig_setting_(INT_MIN)
{
  oldDSService_ = s;
}

I3DetectorStatusConstPtr
I3TweekTrigger::GetDetectorStatus(I3Time time)
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

  t_iter->second.GetTriggerSettings()[trig_name_] = trig_setting_;

  return detectorStatus_;
}
