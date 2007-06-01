/**
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3IceTopTriggerDefaults.h 6917 2005-04-26 02:56:37Z olivas $
 * 
 * @file I3IceTopTriggerDefaults.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3ICETOPTRIGGERDEFAULTS_H
#define I3ICETOPTRIGGERDEFAULTS_H

#include "dataclasses/I3Units.h"
#include "dataclasses/TriggerKey.h"

namespace I3IceTopTriggerDefaults{

  /**
   *IceCube trigger settings
   */
  const TriggerKey::SourceID SOURCE_ID = TriggerKey::ICE_TOP;
  const TriggerKey::TypeID TYPE_ID = TriggerKey::SIMPLE_MULTIPLICITY;
  const int CONFIG_ID = 15;
  const int THRESHOLD = 6;
  const double TIME_WINDOW = 2.*I3Units::microsecond;

}

#endif
