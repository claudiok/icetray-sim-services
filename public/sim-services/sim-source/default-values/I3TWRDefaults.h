/**
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3TWRDefaults.h 6917 2005-04-26 02:56:37Z olivas $
 * 
 * @file I3TWRDefaults.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3TWRDEFAULTS_H
#define I3TWRDEFAULTS_H

#include "dataclasses/I3Units.h"

namespace I3TWRDefaults{

  /**
   *TWR trigger settings
   */
  const int STRING_1_4_NMOD = 9;
  const int STRING_1_4_MULT = 6;
  const int STRING_5_19_NMOD = 11;
  const int STRING_5_19_MULT = 7;
  const int DMADD_PRETRIG_MULT = 13;
  const int DMADD_MULT = 18;
  const int DMADD_WINDOW = 2500;
  const int TWR_SOFT_N1HIT = 4;
  const int TWR_SOFT_NPAIRS = 8;
  const int TWR_SOFT_FRAGMULT = 20;
  const int TWR_SOFT_WINDOW = 2500;
  const int TWR_SOFT_VOL_STARTMULT = 9;
  const int TWR_SOFT_VOL_STOPMULT = 8;
  const int TWR_OP_BINS_BEFORE = 3;
  const int TWR_OP_BINS_AFTER = 4;
  const int TWR_OP_THRESH_END = 4;
  const int TWR_EL_BINS_BEFORE = 12;
  const int TWR_EL_BINS_AFTER = 8;
  const int TWR_EL_THRESH_END_FRAC = 10;
  const int SWAMP_OP_MAXAMP = 2500;
  const int SWAMP_EL_MAXAMP = 4070;

}

#endif
