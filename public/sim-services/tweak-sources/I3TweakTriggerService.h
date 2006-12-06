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

#ifndef I3TWEAKTRIGGERSERVICE_H
#define I3TWEAKTRIGGERSERVICE_H

#include "sim-services/tweak-sources/I3TweakTrigger.h"
#include "icetray/I3ServiceFactory.h"
#include <icetray/I3DefaultName.h>
/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3TweakTriggerService : public I3ServiceFactory
{
public:

  I3TweakTriggerService(const I3Context& context);

  virtual ~I3TweakTriggerService(){};

  virtual bool InstallService(I3Context& services);

  virtual void Configure();

  SET_LOGGER("I3TweakTriggerService");

 private:
  I3TweakTriggerService();
  I3TweakTriggerService
    (const I3TweakTriggerService& rhs); // stop default
  I3TweakTriggerService operator=
    (const I3TweakTriggerService& rhs); // stop default

  I3TweakTriggerPtr status_service_;

  std::string oldServiceName_;
  std::string tweakedServiceName_;

  int key_source_;
  int key_type_;
  int key_configID_;
  std::string trig_name_;
  std::vector<std::string> setting_name_list_;
  std::vector<int> setting_value_list_;

};
I3_POINTER_TYPEDEFS(I3TweakTriggerService);
I3_DEFAULT_NAME(I3TweakTriggerService);

#endif
