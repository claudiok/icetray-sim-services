/**
 * Definition of I3TweakTrigger class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3TweakTrigger.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3TweakTrigger.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3TWEAKTRIGGER_H
#define I3TWEAKTRIGGER_H

#include "interfaces/I3DetectorStatusService.h"
#include <icetray/I3Logging.h>
#include <dataclasses/TriggerKey.h>

class I3TweakTrigger : public I3DetectorStatusService
{
public:

  I3TweakTrigger(I3DetectorStatusServicePtr c);

  virtual I3DetectorStatusConstPtr GetDetectorStatus(I3Time time);
  virtual ~I3TweakTrigger() { };

  SET_LOGGER("I3TweakTrigger");

  void SetSourceID(TriggerKey::SourceID s){ key_source_ = s; };
  void SetTypeID(TriggerKey::TypeID t){ key_type_ = t; };
  void SetConfigID(int c){ key_configID_ = c; };
  void SetTriggerName(std::string s){ trig_name_ = s;};
  void SetValues(std::vector<std::pair<std::string,int> > v){ setting_list_ = v;};

  void SetReadoutWindowConfig(std::map<int, std::vector<double> > m){ 
    readout_config_map_ = m;
  };

 private:
  I3TweakTrigger();
  I3DetectorStatusPtr detectorStatus_;
  I3DetectorStatusServicePtr oldDSService_;

  TriggerKey::SourceID key_source_;
  TriggerKey::TypeID key_type_;
  int key_configID_;
  std::string trig_name_;
  std::vector<std::pair<std::string,int> > setting_list_;
  std::map<int, std::vector<double> > readout_config_map_;

};

I3_POINTER_TYPEDEFS(I3TweakTrigger);

#endif
