#!/usr/bin/env python

# This test was added to address ticket #381
# http://code.icecube.wisc.edu/projects/icecube/ticket/381
#
# The main difference between the current version and
# previous versions is the loading of the BadDomList
# library with the python module import ( equivalent to
# icetray.load("libBadDomList") ).  Most scripts
# really only want to get at the static bad DOM list.
#
# The first thing I discovered is the problem goes away
# if you switch the import order or import *any* library
# that's linked to gsl before loading the BadDomList
# library.  By the way, the error occurs in sprng and
# is not reproducible with other rngs.
#
# I thought I had isolated the problem to the mysql lib
# (loaded through the link to I3Db ) which lead to this project :
#   http://code.icecube.wisc.edu/svn/sandbox/olivas/load_mysql
# I was able to reproduce the error with this project.
# Then something changed and all tests pass.

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

