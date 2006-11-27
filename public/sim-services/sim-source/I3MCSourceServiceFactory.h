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

class I3MCSourceServiceFactory : public I3ServiceFactory
{
 public:

  I3MCSourceServiceFactory(const I3Context& context);

  virtual ~I3MCSourceServiceFactory();

  virtual bool InstallService(I3Context& services);

  virtual void Configure();

 private:

  std::string calServiceName_;
  std::string statusServiceName_;
  std::string geoServiceName_;

  bool installCalibration_;
  bool installDetectorStatus_;

  shared_ptr<I3MCDetectorStatusService> statusService_;
  shared_ptr<I3MCCalibrationService> calibrationService_;

  I3MCSourceServiceFactory
    (const I3MCSourceServiceFactory& rhs); // stop default
  I3MCSourceServiceFactory operator=
    (const I3MCSourceServiceFactory& rhs); // stop default

  I3Trigger ic_trigger_;
  I3TriggerStatus ic_trigStatus_;

  I3Trigger it_trigger_;
  I3TriggerStatus it_trigStatus_;

  SET_LOGGER("I3MCSourceServiceFactory");
};

#endif 
