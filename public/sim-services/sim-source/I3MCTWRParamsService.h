#ifndef I3MCTWRPARAMSERVICE_H
#define I3MCTWRPARAMSERVICE_H

/*
 * class: I3MCTWRParamsService
 *
 * Version $Id: I3MCTWRParamsService.h 11148 2005-10-03 21:55:04Z olivas $
 *
 * Date: 17 Feb 2004
 *
 * (c) IceCube Collaboration
 */

class I3Context;

#include "icetray/I3ServiceFactory.h"
#include "simclasses/I3MCTWRParams.h"

class I3MCTWRParamsService : public I3ServiceFactory
{
 public:

  I3MCTWRParamsService(const I3Context& context);

  virtual ~I3MCTWRParamsService();

  virtual bool InstallService(I3Context& services);

  virtual void Configure();

 private:

  std::string elecFileName_;
  std::string cidFileName_;
  std::string stopDelayFileName_;
  std::string dmaddThrFileName_;
  std::string cableDelayFileName_;
  std::string relSensFileName_;
  std::string serviceName_;
  shared_ptr<I3MCTWRParamsMap> twrParams_;

  I3MCTWRParamsService
    (const I3MCTWRParamsService& rhs); // stop default
  I3MCTWRParamsService operator=
    (const I3MCTWRParamsService& rhs); // stop default

  SET_LOGGER("I3MCTWRParamsService");
};

#endif 
