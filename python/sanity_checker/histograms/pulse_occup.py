import numpy
from I3Tray import I3Units
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

def _frame_op(frame):
    if "I3Geometry" in frame \
       and "I3MCPulseSeriesMap" in frame :
        geo = frame["I3Geometry"].omgeo
        pulsemap = frame["I3MCPulseSeriesMap"]
        weight = event_weight(frame)
        return [(geo[omkey].position.z, weight * pulse.charge) \
                for omkey, pulses in pulsemap for pulse in pulses]
    return []

_draw_args = { "bins" : bins(110,-550*I3Units.m,550*I3Units.m),
               "xlabel" : "z(m)",
               "title" : "MC Pulse Occupancy",
               "figname" : "pulse_occup.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

pe_occup_h = Histogram(frame_op = _frame_op,
                       draw_args = _draw_args,
                       name = 'pulse_occup')

