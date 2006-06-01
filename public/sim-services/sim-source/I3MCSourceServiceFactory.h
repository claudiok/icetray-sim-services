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

class I3MCRawDOMStatusService;
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

  int configID_;
  int threshold_;
  int timeWindow_;

  std::string calServiceName_;
  std::string statusServiceName_;
  std::string geoServiceName_;

  I3Trigger Trigger;
  I3TriggerStatus TrigStatus;

  shared_ptr<I3MCRawDOMStatusService> status_;
  shared_ptr<I3MCCalibrationService> calibration_;

  I3MCSourceServiceFactory
    (const I3MCSourceServiceFactory& rhs); // stop default
  I3MCSourceServiceFactory operator=
    (const I3MCSourceServiceFactory& rhs); // stop default


   SET_LOGGER("I3MCSourceServiceFactory");
};

#endif 
