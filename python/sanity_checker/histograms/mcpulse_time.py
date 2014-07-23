import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    if "I3MCPulseSeriesMap" in frame :
        weight = event_weight(frame)
        return [(pulse.time, weight * pulse.charge) \
                    for omkey, pulseseries in frame["I3MCPulseSeriesMap"] \
                    for pulse in pulseseries ]
    return []

_draw_args = { "bins" : bins(100,10000/I3Units.ns,12000/I3Units.ns),
               "xlabel" : "t(ns)",
               "title" : "I3MCPulse Time",
               "figname" : "mcpulse_time.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

mcpulse_time_h = Histogram(frame_op = _frame_op,
                           draw_args = _draw_args,
                           name = 'mcpulse_time')
