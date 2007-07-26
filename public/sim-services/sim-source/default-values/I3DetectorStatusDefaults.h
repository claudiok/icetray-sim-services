/**
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3DetectorStatusDefaults.h 6917 2005-04-26 02:56:37Z olivas $
 * 
 * @file I3DetectorStatusDefaults.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3DETECTORSTATUSDEFAULTS_H
#define I3DETECTORSTATUSDEFAULTS_H

#include "dataclasses/I3Units.h"
#include "dataclasses/status/I3DOMStatus.h"

namespace I3DetStatDefaults{
  /**
   *Start of the valid time range of the detector status
   */
  const int32_t START_YEAR = 2000;
  const int64_t START_DAQTIME = 0;

  /**
   *End of the valid time range of the detector status
   */
  const int32_t END_YEAR = 3000;
  const int64_t END_DAQTIME = 0;

  /**
   * Icetop local coincidence trigger windows
   */
  const double ICETOP_LCWINDOW_PRE = 125.0*I3Units::ns;
  const double ICETOP_LCWINDOW_POST = 125.0*I3Units::ns;

  /**
   * IceTop PMT voltages for high and low gain
   */
  const double ICETOP_HIGHGAIN_VOLTAGE = 1278*I3Units::volt;
  const double ICETOP_LOWGAIN_VOLTAGE = 945*I3Units::volt;

  /**
   * InIce local coincidence trigger windows
   */
  const double INICE_LCWINDOW_PRE = 1000.0*I3Units::ns;  //OLD: -800.0*I3Units::mV
  const double INICE_LCWINDOW_POST = 1000.0*I3Units::ns; //OLD: 800.0*I3Units::mV
                                                         // Yes, it was mV...Jesus.

  /**
   * Number of neighbors required to satisfy local coincidence
   */
  const int LCSPAN = 2; //CHANGED...used to be 1 in 2006

  /**
   * InIce PMT voltage
   */
  const double INICE_VOLTAGE = 1276*I3Units::volt; //NO CHANGE

  const I3DOMStatus::TrigMode TRIGGER_MODE = I3DOMStatus::SPE; //NO CHANGE
  const I3DOMStatus::OnOff STATUS_ATWDa = I3DOMStatus::On; //NO CHANGE
  const I3DOMStatus::OnOff STATUS_ATWDb = I3DOMStatus::On; //NO CHANGE
  const I3DOMStatus::OnOff STATUS_FADC_INICE = I3DOMStatus::On; //NO CHANGE
  const I3DOMStatus::OnOff STATUS_FADC_ICETOP = I3DOMStatus::Off; //NO CHANGE

  const I3DOMStatus::LCMode LCMODE_INICE_BULK = I3DOMStatus::UpOrDown;
  const I3DOMStatus::LCMode LCMODE_INICE_FIRST = I3DOMStatus::Down;
  const I3DOMStatus::LCMode LCMODE_INICE_LAST = I3DOMStatus::Up; 
  const I3DOMStatus::LCMode LCMODE_ICETOP = I3DOMStatus::Down;

  const double SPE_THRESHOLD = 1.422*I3Units::mV; //OLD: 4.0*I3Units::mV
  const double FE_PEDESTAL = 2.6*I3Units::volt;   //NO CHANGE
  const int DAC_TRIGGER_BIAS0 = 850; //NO CHANGE
  const int DAC_TRIGGER_BIAS1 = 850; //NO CHANGE
  const int DAC_FADC_REF = 800; //NO CHANGE

  /**
   *Number of bins
   */
  const unsigned NBINS_ATWD0_INICE = 0; //NO CHANGE
  const unsigned NBINS_ATWD1_INICE = 0; //OLD: 128
  const unsigned NBINS_ATWD2_INICE = 0; //OLD: 128
  const unsigned NBINS_FADC_INICE = 0;  //OLD: 256

  const unsigned NBINS_ATWD0_ICETOP = 0; //NO CHANGE
  const unsigned NBINS_ATWD1_ICETOP = 0; //OLD: 128
  const unsigned NBINS_ATWD2_ICETOP = 0; //OLD: 128
  const unsigned NBINS_FADC_ICETOP = 0;  //OLD: 256

  const I3DOMStatus::OnOff DELTA_COMPRESSION = I3DOMStatus::On;

}

#endif
