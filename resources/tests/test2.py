#!/usr/bin/env python

from I3Tray import *
from icecube.BadDomList import bad_dom_list_static

load("libphys-services")

tray = I3Tray()

seed = 314159
tray.AddService("I3SPRNGRandomServiceFactory","random")(
    ("Seed",seed),
    ("NStreams",2),
    ("StreamNum",1))


tray.AddModule("Dump", "dump")

tray.Execute(1)

tray.Finish()

