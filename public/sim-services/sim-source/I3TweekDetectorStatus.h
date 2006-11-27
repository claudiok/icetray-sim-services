/**
 * Definition of I3MCDetectorStatusFiller class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3TweekDetectorStatus.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3TweekDetectorStatus.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author olivas
 */

#ifndef I3TWEEKDETECTORSTATUS_H
#define I3TWEEKDETECTORSTATUS_H

#include "phys-services/I3DetectorStatusService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

/**
 *
 * @brief This service allows you to fill the detector status data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3TweekDetectorStatus : public I3DetectorStatusService
{
public:

  I3TweekDetectorStatus(I3DetectorStatusServicePtr);

  virtual I3DetectorStatusConstPtr GetDetectorStatus(I3Time time);
  void InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus);

  virtual ~I3TweekDetectorStatus(){};

  SET_LOGGER("I3TweekDetectorStatus");

  void SetIceTopLCWindowPre(double p){icetopLCWindowPre_= p;};
  void SetIceTopLCWindowPost(double p){icetopLCWindowPost_= p;};
  void SetIceTopHighGainVoltage(double v){icetopHighGainVoltage_= v;};
  void SetIceTopLowGainVoltage(double v){icetopLowGainVoltage_= v;};
  void SetInIceLCWindowPre(double p){iniceLCWindowPre_= p;};
  void SetInIceLCWindowPost(double p){iniceLCWindowPost_= p;};
  void SetLCSpan(int s){lcSpan_= s;};
  void SetInIceVoltage(double v){iniceVoltage_= v;};
  void SetTriggerMode(int m){triggerMode_= static_cast<I3DOMStatus::TrigMode>(m);};
  void SetLCMode(int m){lcMode_= static_cast<I3DOMStatus::LCMode>(m);};
  void SetStatusATWDa(int o){statusATWDa_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetStatusATWDb(int o){statusATWDb_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetStatusFADCInIce(int o){statusFADC_InIce_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetStatusFADCIceTop(int o){statusFADC_IceTop_= static_cast<I3DOMStatus::OnOff>(o);};
  void SetSPEThreshold(double t){speThreshold_= t;};
  void SetFEPedestal(double t){fePedestal_= t;};
  void SetDACTriggerBias0(int b){dacTriggerBias0_= b;};
  void SetDACTriggerBias1(int b){dacTriggerBias1_= b;};
  void SetDACFADCRef(int r){ dacFADCRef_ = r; };
  void SetNBinsATWD0InIce(int n){ nBinsATWD0_InIce_ = n; };
  void SetNBinsATWD1InIce(int n){ nBinsATWD1_InIce_ = n; };
  void SetNBinsATWD2InIce(int n){ nBinsATWD2_InIce_ = n; };
  void SetNBinsFADCInIce(int n){ nBinsFADC_InIce_ = n; };
  void SetNBinsATWD0IceTop(int n){ nBinsATWD0_IceTop_ = n; };
  void SetNBinsATWD1IceTop(int n){ nBinsATWD1_IceTop_ = n; };
  void SetNBinsATWD2IceTop(int n){ nBinsATWD2_IceTop_ = n; };
  void SetNBinsFADCIceTop(int n){ nBinsFADC_IceTop_ = n; };

 private:
  I3TweekDetectorStatus();
  I3DetectorStatusServicePtr old_status_service_;

  shared_ptr<I3DetectorStatus> status_;
  map<TriggerKey, I3TriggerStatus> triggerStatus_;

  /**
   *Configuration parameters for detector status
   */

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

  I3DOMStatus::TrigMode triggerMode_;
  I3DOMStatus::LCMode lcMode_;
  I3DOMStatus::OnOff statusATWDa_;
  I3DOMStatus::OnOff statusATWDb_;

  I3DOMStatus::OnOff statusFADC_InIce_;
  I3DOMStatus::OnOff statusFADC_IceTop_;

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

#endif
