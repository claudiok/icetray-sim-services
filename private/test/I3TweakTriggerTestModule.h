#ifndef I3TWEAKTRIGGERTESTMODULE_H
#define I3TWEAKTRIGGERTESTMODULE_H
/**
 * class: I3TweakTriggerTestModule
 *
 * Version $Id: I3TweakTriggerTestModule.h,v 1.11 2005/03/03 00:43:41 olivas Exp $
 *
 * date:  2 July 2004
 *
 * (c) 2004 IceCube Collaboration
 */

#include "icetray/I3Module.h"

/** 
 *@class I3TweakTriggerTestModule I3TweakTriggerTestModule.h 
 */

class I3TweakTriggerTestModule : public I3Module
{
 public:

  I3TweakTriggerTestModule(const I3Context& ctx);
  ~I3TweakTriggerTestModule();
  
  void Configure();
  void Physics(I3FramePtr frame);

 private:
  I3TweakTriggerTestModule();
  I3TweakTriggerTestModule(const I3TweakTriggerTestModule&);
  I3TweakTriggerTestModule& operator=(const I3TweakTriggerTestModule&);

  int key_source_;
  int key_type_;
  int key_configID_;
  std::string trig_name_;
  std::vector<std::string> setting_name_list_;
  std::vector<int> setting_value_list_;
  std::map<int,vector<double> > readout_config_map_;

  SET_LOGGER("I3TweakTriggerTestModule");

};

#endif //I3TWEAKTRIGGERTESTMODULE_H
