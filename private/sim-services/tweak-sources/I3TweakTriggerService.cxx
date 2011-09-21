#include "sim-services/tweak-sources/I3TweakTriggerService.h"
#include "dataclasses/status/I3DetectorStatus.h"
#include "icetray/I3Units.h"
#include "icetray/I3TrayHeaders.h"

I3_SERVICE_FACTORY(I3TweakTriggerService);

I3TweakTriggerService::I3TweakTriggerService(const I3Context& context) :
  I3ServiceFactory(context),
  key_source_(INT_MIN),
  key_type_(INT_MIN),
  key_configID_(INT_MIN),
  trig_name_(""),
  add_new_trigger_(false)
{
  oldServiceName_ = I3DefaultName<I3DetectorStatusService>::value();
  tweakedServiceName_ = I3DefaultName<I3TweakTriggerService>::value();
  AddParameter("OldServiceName","Name of service to tweak",oldServiceName_);
  AddParameter("TweakedServiceName","Name of tweaked service",tweakedServiceName_);
  AddParameter("SourceID","Trigger Source ID",key_source_);
  AddParameter("TypeID","Trigger Type ID",key_type_);
  AddParameter("ConfigID","Trigger Config ID",key_configID_);
  AddParameter("TriggerName","Trigger name",trig_name_);
  AddParameter("ValueNameList","List of name of values",setting_name_list_);
  AddParameter("ValueList","List of values",setting_value_list_);
  AddParameter("ReadoutConfigMap","Map of readout windows",readout_config_map_);
  AddParameter("AddNewTrigger","Adding a new trigger",add_new_trigger_);
}


void I3TweakTriggerService::Configure()
{  
  GetParameter("OldServiceName",oldServiceName_);
  GetParameter("TweakedServiceName",tweakedServiceName_);
  GetParameter("SourceID",key_source_);
  GetParameter("TypeID",key_type_);
  GetParameter("ConfigID",key_configID_);
  
  if(key_source_ == INT_MIN) log_fatal("You must specify a SourceID");
  if(key_type_ == INT_MIN) log_fatal("You must specify a TypeID");
  if(key_configID_ == INT_MIN) log_fatal("You must specify a ConfigID");

  GetParameter("TriggerName",trig_name_);
  GetParameter("ValueNameList",setting_name_list_);
  GetParameter("ValueList",setting_value_list_);
  GetParameter("ReadoutConfigMap",readout_config_map_);
  GetParameter("AddNewTrigger",add_new_trigger_);

  if(setting_name_list_.size() != setting_value_list_.size() )
    log_fatal("The name list and the value list must be the same size.");

  if(trig_name_.size() ==0 &&
     setting_name_list_.size()==0 &&
     setting_value_list_.size()==0)
    log_warn("Neither trigger name nor ValuesList set.  This service won't change anything.");
}

bool I3TweakTriggerService::InstallService(I3Context& services)
{
  if(!status_service_){
    I3DetectorStatusServicePtr old_status = context_.Get<I3DetectorStatusServicePtr>(oldServiceName_);
    status_service_ = 
      I3TweakTriggerPtr
      (new I3TweakTrigger(old_status));
    log_debug("Made new I3TweakTriggerService.");
    //Configure the detector status
    status_service_->SetSourceID(static_cast<TriggerKey::SourceID>(key_source_));
    status_service_->SetTypeID(static_cast<TriggerKey::TypeID>(key_type_));
    status_service_->SetConfigID(key_configID_);
    status_service_->SetTriggerName(trig_name_);

    std::vector<std::pair<std::string,int> > setting_list;
    std::vector<std::string>::iterator i;
    std::vector<int>::iterator j;
    for(i=setting_name_list_.begin(), j=setting_value_list_.begin();
	i!=setting_name_list_.end(), j!=setting_value_list_.end();
	i++, j++){
      std::pair<std::string,int> p(*i,*j);
      setting_list.push_back(p);      
    }
    status_service_->SetValues(setting_list);
    status_service_->SetReadoutWindowConfig(readout_config_map_);
    status_service_->AddNewTrigger(add_new_trigger_);
  }

  bool good_status = services.Put<I3DetectorStatusService>(tweakedServiceName_,status_service_);
  log_debug("good_status %d",good_status);

  return good_status;
}



