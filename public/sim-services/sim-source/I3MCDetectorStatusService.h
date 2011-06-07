/**
 * Definition of I3MCDetectorStatusFiller class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3MCDetectorStatusService.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3MCDetectorStatusService.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3MCDETECTORSTATUSSERVICE_H
#define I3MCDETECTORSTATUSSERVICE_H

#include "interfaces/I3DetectorStatusService.h"
#include "interfaces/I3GeometryService.h"
#include "simclasses/I3MCTWRParams.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"
#include <dataclasses/status/I3DOMStatus.h>
#include <dataclasses/status/I3DetectorStatus.h>
#include <dataclasses/geometry/I3Geometry.h>

/**
 *
 * @brief This service allows you to fill the detector status data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3MCDetectorStatusService : public I3DetectorStatusService
{
public:

  I3MCDetectorStatusService(I3GeometryServicePtr,I3DetectorStatusServicePtr);

  virtual I3DetectorStatusConstPtr GetDetectorStatus(I3Time time);
  void InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus);

  size_t GetTriggerStatusSize(){ return triggerStatus_.size(); }; 
  size_t GetDOMStatusSize(){ return status_->domStatus.size(); }; 

  virtual ~I3MCDetectorStatusService(){};

  SET_LOGGER("I3MCDetectorStatusService");


  void SetSkipStrings(vector<int>& v){ skipStrings_ = v;};
  void SetSkipStations(vector<int>& v){ skipStations_ = v;};
  void ModifyWithExtremePrejudice(bool b){ modifyWithExtremePrejudice_ = b; };

 private:
  I3MCDetectorStatusService();
  I3GeometryServicePtr geo_service_;
  I3DetectorStatusServicePtr old_status_service_;

  void SetDOMStatus(I3DetectorStatusPtr&, const I3OMGeoMap&);

  shared_ptr<I3DetectorStatus> status_;
  map<TriggerKey, I3TriggerStatus> triggerStatus_;

  /**
   *Configuration parameters for detector status
   */

  /**
   *Start of the valid time range of the detector status
   */
  int32_t startYear_;
  int64_t startDAQTime_;

  /**
   *End of the valid time range of the detector status
   */
  int32_t endYear_;
  int64_t endDAQTime_;

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
  int inice_LCSpan_;
  int icetop_LCSpan_;

  /**
   * InIce PMT voltage
   */
  double iniceVoltage_;

  I3DOMStatus::TrigMode iniceTriggerMode_;
  I3DOMStatus::TrigMode icetopHGTriggerMode_;
  I3DOMStatus::TrigMode icetopLGTriggerMode_;

  I3DOMStatus::LCMode lcMode_inice_first_;
  I3DOMStatus::LCMode lcMode_inice_bulk_;
  I3DOMStatus::LCMode lcMode_inice_last_;
  I3DOMStatus::LCMode lcMode_icetop_;
  I3DOMStatus::OnOff statusATWDa_;
  I3DOMStatus::OnOff statusATWDb_;

  I3DOMStatus::OnOff statusFADC_InIce_;
  I3DOMStatus::OnOff statusFADC_IceTop_;

  double iniceSPEThreshold_;
  double iniceMPEThreshold_;
  double icetopSPEThreshold_;
  double icetopMPEThreshold_;
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

  double twrOpBinSize_;
  double twrElBinSize_;
  unsigned twrBaseline_;

  I3DOMStatus::OnOff deltaCompression_;
  I3DOMStatus::DOMGain domGainType_;
  bool slcActive_;

  bool modifyWithExtremePrejudice_;
  /**
   * Don't modify these strings/stations
   */
  vector<int> skipStrings_;
  vector<int> skipStations_;
};

I3_POINTER_TYPEDEFS(I3MCDetectorStatusService);
#endif
