/**
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3InIceTriggerDefaults.h 6917 2005-04-26 02:56:37Z olivas $
 * 
 * @file I3InIceTriggerDefaults.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3INICETRIGGERDEFAULTS_H
#define I3INICETRIGGERDEFAULTS_H

#include "dataclasses/I3Units.h"

namespace I3InIceTriggerDefaults{

  /**
   *IceCube trigger settings
   */
  const int CONFIG_ID = 0;
  const int THRESHOLD = 8;
  const double TIMEWINDOW = 5.*I3Units::microsecond;
}

#endif
