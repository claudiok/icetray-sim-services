import numpy
from I3Tray import I3Units

def bins(nbins, min, max) :
    db = float(max-min)/float(nbins)
    return numpy.arange(min,max,db)

def data_livetime(frame) :
    if "CorsikaWeightMap" in frame :
        weightmap = frame["CorsikaWeightMap"]
        return weightmap["TimeScale"]/(weightmap["Weight"] * weightmap["DiplopiaWeight"])
    
    if "I3MCWeightDict" in frame :
        weightmap = frame["I3MCWeightDict"]
        return weightmap["NEvents"]/weightmap["OneWeight"]

    return 1.*I3Units.second
