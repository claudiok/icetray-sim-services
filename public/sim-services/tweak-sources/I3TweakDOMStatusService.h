/**
 * Definition of I3TweakDOMStatus class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3TweakDOMStatus.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3TweakDOMStatus.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3TWEAKDOMSTATUSSERVICE_H
#define I3TWEAKDOMSTATUSSERVICE_H

#include "sim-services/tweak-sources/I3TweakDOMStatus.h"
#include "icetray/I3ServiceFactory.h"
#include <icetray/I3DefaultName.h>
/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3TweakDOMStatusService : public I3ServiceFactory
{
public:

  I3TweakDOMStatusService(const I3Context& context);

  virtual ~I3TweakDOMStatusService(){};

  virtual bool InstallService(I3Context& services);

  virtual void Configure();

  SET_LOGGER("I3TweakDOMStatusService");

 private:
  I3TweakDOMStatusService();
  I3TweakDOMStatusService
    (const I3TweakDOMStatusService& rhs); // stop default
  I3TweakDOMStatusService operator=
    (const I3TweakDOMStatusService& rhs); // stop default

  I3TweakDOMStatusPtr status_service_;

  std::string oldServiceName_;
  std::string tweakedServiceName_;

  /**
   * Icetop local coincidence trigger windows
   */
  double icetopLCWindowPre_;
  double icetopLCWindowPost_;

  /**
   * IceTop PMT voltages for high and low gain
   */
  double icetopHighGainVoltage_;
  double icetopLowGainVoltage_;

  /**
   * InIce local coincidence trigger windows
   */
  double iniceLCWindowPre_;
  double iniceLCWindowPost_;

  /**
   * Number of neighbors required to satisfy local coincidence
   */
  int lcSpan_;

  /**
   * InIce PMT voltage
   */
  double iniceVoltage_;

  int triggerMode_;
  int lcMode_inice_first_;
  int lcMode_inice_bulk_;
  int lcMode_inice_last_;
  int lcMode_icetop_;
  int statusATWDa_;
  int statusATWDb_;

  int statusFADC_InIce_;
  int statusFADC_IceTop_;

  double speThreshold_;
  double fePedestal_;
  int dacTriggerBias0_;
  int dacTriggerBias1_;
  int dacFADCRef_;

  /**
   *Number of FADC bins
   */
  int nBinsATWD0_InIce_;
  int nBinsATWD1_InIce_;
  int nBinsATWD2_InIce_;
  int nBinsFADC_InIce_;

  int nBinsATWD0_IceTop_;
  int nBinsATWD1_IceTop_;
  int nBinsATWD2_IceTop_;
  int nBinsFADC_IceTop_;

};
I3_POINTER_TYPEDEFS(I3TweakDOMStatusService);
I3_DEFAULT_NAME(I3TweakDOMStatusService);
#endif
