import numpy
from icecube import dataclasses
from ..bases.histogram import Histogram
from .utils import bins, event_weight

from icecube.dataclasses.trigger_hierarchy_recipes import n_triggers

def _frame_op(frame):
    if "I3TriggerHierarchy" in frame \
           and "InIceRawData" in frame :
        weight = event_weight(frame)
        th = frame["I3TriggerHierarchy"]
        if n_triggers(th, sourceID = dataclasses.I3Trigger.IN_ICE,
                      typeID = dataclasses.I3Trigger.STRING) > 0 :
            return (len(frame["InIceRawData"]), weight)
        else:
            return (-1,1)
    return (-1,1)
    
_draw_args = { "bins" : bins(200,0,200),
               "xlabel" : "NChan",
               "title" : "N Channels (DOM Launch) - STRING",
               "figname" : "n_dom_chan_str.png",
               "ylabel" : r"$\Gamma(\rm{Hz})$",
               "log" : True }

n_dom_chan_str_h = Histogram(frame_op = _frame_op,
                             draw_args = _draw_args,
                             name = 'n_dom_chan_str')
