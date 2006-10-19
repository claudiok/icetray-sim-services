#ifndef I3MCSOURCESERVICEFACTORY_H
#define I3MCSOURCESERVICEFACTORY_H
/*
 * class: I3MCSourceServiceFactory
 *
 * Version $Id: I3MCSourceServiceFactory.h 11148 2005-10-03 21:55:04Z olivas $
 *
 * Date: 17 Feb 2004
 *
 * (c) IceCube Collaboration
 */

class I3Context;

#include "icetray/I3ServiceFactory.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include "dataclasses/status/I3DOMStatus.h"

class I3MCDetectorStatusService;
class I3MCCalibrationService;

class I3MCSourceServiceFactory
: public I3ServiceFactory
{
 public:

  I3MCSourceServiceFactory(const I3Context& context);

  virtual ~I3MCSourceServiceFactory();

  virtual bool InstallService(I3Context& services);

  virtual void Configure();

 private:

  /**
   *InIce trigger parameters
   */
  int ic_configID_;
  int ic_threshold_;
  double ic_timeWindow_;
  I3Trigger ic_trigger_;
  I3TriggerStatus ic_trigStatus_;

  /**
   *IceTop trigger parameters
   */
  int it_configID_;
  int it_threshold_;
  double it_timeWindow_;
  I3Trigger it_trigger_;
  I3TriggerStatus it_trigStatus_;


  std::string calServiceName_;
  std::string statusServiceName_;
  std::string geoServiceName_;


  shared_ptr<I3MCDetectorStatusService> status_;
  shared_ptr<I3MCCalibrationService> calibration_;

  I3MCSourceServiceFactory
    (const I3MCSourceServiceFactory& rhs); // stop default
  I3MCSourceServiceFactory operator=
    (const I3MCSourceServiceFactory& rhs); // stop default

  /**
   *Detector Status parameters
   */

  /**
   *Start of the valid time range of the detector status
   */
  int32_t ds_startYear_;
  int64_t ds_startDAQTime_;

  /**
   *End of the valid time range of the detector status
   */
  int32_t ds_endYear_;
  int64_t ds_endDAQTime_;

  /**
   * Icetop local coincidence trigger windows
   */
  double ds_icetopLCWindowPre_;
  double ds_icetopLCWindowPost_;

  /**
   * IceTop PMT voltages for high and low gain
   */
  double ds_icetopHighGainVoltage_;
  double ds_icetopLowGainVoltage_;

  /**
   * InIce local coincidence trigger windows
   */
  double ds_iniceLCWindowPre_;
  double ds_iniceLCWindowPost_;

  /**
   * Number of neighbors required to satisfy local coincidence
   */
  int ds_lcSpan_;

  /**
   * InIce PMT voltage
   */
  double ds_iniceVoltage_;

//  I3DOMStatus::TrigMode ds_triggerMode_;
//  I3DOMStatus::LCMode ds_lcMode_;
//  I3DOMStatus::OnOff ds_statusATWDa_;
//  I3DOMStatus::OnOff ds_statusATWDb_;
//  I3DOMStatus::OnOff ds_statusFADC_;

  int ds_triggerMode_;
  int ds_lcMode_;
  int ds_statusATWDa_;
  int ds_statusATWDb_;
  int ds_statusFADC_;

  double ds_speThreshold_;
  double ds_fePedestal_;
  int ds_dacTriggerBias0_;
  int ds_dacTriggerBias1_;
  int ds_dacFADCRef_;

  /**
   *Number of FADC bins
   */
  int ds_nBinsATWD0_;
  int ds_nBinsATWD1_;
  int ds_nBinsATWD2_;
  int ds_nBinsFADC_;

  SET_LOGGER("I3MCSourceServiceFactory");
};

#endif 
