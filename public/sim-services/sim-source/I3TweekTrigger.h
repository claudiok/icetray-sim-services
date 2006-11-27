/**
 * Definition of I3TweekTrigger class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3TweekTrigger.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3TweekTrigger.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3TWEEKTRIGGER_H
#define I3TWEEKTRIGGER_H

#include "phys-services/I3DetectorStatusService.h"
#include "phys-services/I3GeometryService.h"

class I3TweekTrigger : public I3DetectorStatusService
{
public:

  I3TweekTrigger(I3DetectorStatusServicePtr c);

  virtual I3DetectorStatusConstPtr GetDetectorStatus(I3Time time);
  virtual ~I3TweekTrigger() { };

  SET_LOGGER("I3TweekTrigger");

  void SetSourceID(TriggerKey::SourceID s){ key_source_ = s; };
  void SetTypeID(TriggerKey::TypeID t){ key_type_ = t; };
  void SetConfigID(int c){ key_configID_ = c; };
  void SetTriggerName(std::string s){ trig_name_ = s;};
  void SetValueName(std::string s){ trig_setting_name_ = s;};
  void SetValue(int v){ trig_setting_ = v;};

 private:
  I3TweekTrigger();
  I3DetectorStatusPtr detectorStatus_;
  I3DetectorStatusServicePtr oldDSService_;

  TriggerKey::SourceID key_source_;
  TriggerKey::TypeID key_type_;
  int key_configID_;
  std::string trig_name_;
  std::string trig_setting_name_;
  int trig_setting_;

};

#endif
