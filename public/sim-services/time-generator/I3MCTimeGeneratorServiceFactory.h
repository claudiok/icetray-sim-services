#ifndef I3MCTIMEGENERATORSERVICEFACTORY_H
#define I3MCTIMEGENERATORSERVICEFACTORY_H

/**
 *
 *@file
 *@brief Definition of I3MCTimeGeneratorServiceFactory 
 *
 * (c) 2005 IceCube Collaboration
 */

#include "icetray/I3ServiceFactory.h"
#include "icetray/I3Tray.h"

class I3Context;
class I3Frame;
class I3MCTimeGeneratorService;

/**
 * I3MCTimeGeneratorServiceFactory:  
 * 
 * Sets the time of the simulated event
 */

class I3MCTimeGeneratorServiceFactory : public I3ServiceFactory
{
 public:

  I3MCTimeGeneratorServiceFactory(const I3Context& context);

  virtual ~I3MCTimeGeneratorServiceFactory();
  virtual bool InstallService(I3Context& services);
  virtual void Configure();

  SET_LOGGER("I3MCTimeGeneratorServiceFactory");

 private:
  
  ///Start time of run period
  int year_;
  int64_t daqTime_;
  string eventServiceName_;

  shared_ptr<I3MCTimeGeneratorService> eventService_;
};

#endif
