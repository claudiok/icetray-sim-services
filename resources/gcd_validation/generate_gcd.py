#!/usr/bin/env python
from I3Tray import I3Tray, I3Units

from icecube import dataclasses as dc

import os
import sys
from os.path import expandvars

season_to_MJD = { "2014" : 56784,\
                  "2013" : 56429,\
                  "2012" : 56063,\
                  "2011" : 55697,\
                  "IC79" : 55380,\
                  "IC59" : 55000,\
                  "IC40" : 54649,\
                  }

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-s","--season",
                  dest="SEASON", default="2014" ,
                  help="Season to generate %s" % season_to_MJD.keys())

parser.add_option("-l","--logfile",
                  dest="LOGFILE", default="./gcd_logfile" ,
                  help="Name of logfile.")

(options, args) = parser.parse_args()

print("Please be patient.  This will take several minutes.")

logfile = open(options.LOGFILE, "w")

from icecube import icetray
icetray.logging.rotating_files(options.LOGFILE)

import subprocess
SCRIPT_PATH = expandvars("$I3_BUILD/sim-services/resources/gcd_validation/details/")

if options.SEASON not in season_to_MJD.keys() :
    msg = "ERROR : Season %s is not supported.  Please choose from the following : \n%s" \
    % (options.SEASON, season_to_MJD.keys())
    print(msg)
    f = open(options.LOGFILE, "a")
    f.write(msg)
    sys.exit(1)
    
MJD = season_to_MJD[options.SEASON]
###
# First get the G, C, and D frames from the DB and generate a raw GCD file.
# This also includes the Bad DOM list.
###
print("Pulling GCD information from the database...")
script = SCRIPT_PATH + "generate_gcd_snapshot.py"
cmd = [script, "-m" , str(MJD), "-l", options.LOGFILE, "-s", options.SEASON]
print cmd
err_code = subprocess.call(cmd)
print("...done pulling GCD file. %s" % "SUCCESS" if err_code == 0 else "FAILURE")
if err_code != 0 : sys.exit(err_code)

###
# Now correct the GCD file that was just generated
###
in_filename = "GeoCalibDetectorStatus_"+options.SEASON+"."+str(MJD)+"_candidate.i3.gz"
out_filename = "GeoCalibDetectorStatus_"+options.SEASON+"."+str(MJD)+"_corrected.i3.gz"
print("Correcting the GCD file...")
script = SCRIPT_PATH + "correct_GCD.py"
err_code = subprocess.call([script,
                    "-i" , in_filename, 
                    "-o" , out_filename, 
                    "-l", options.LOGFILE])
print("...done correcting GCD file. %s" \
      % "SUCCESS" if err_code == 0 else "FAILURE")
if err_code != 0 : sys.exit(err_code)

###
# Now test it.
###
print("Testing the GCD file...")
script = SCRIPT_PATH + "test_GCD.py"
err_code = subprocess.call([script,
                    "-i" , out_filename])
print("...done. %s" % "SUCCESS" if err_code == 0 else "FAILURE")
if err_code != 0 : sys.exit(err_code)

###
# And stress test it.
###
print("Generating a stress-test sample...")
script = SCRIPT_PATH + "generate_IC86_stress_test_samples.py"
err_code = subprocess.call([script,
                    "-g", out_filename,
                    "-f", "stress_test_samples.i3.gz"])
print("...done. %s" % "SUCCESS" if err_code == 0 else "FAILURE")
if err_code != 0 : sys.exit(err_code)

###
# And finally validate it.
###
print("Validating the stress-test sample...")
script = SCRIPT_PATH + "validate_stress_test_samples.py"
err_code = subprocess.call([script,
                    "-i", "stress_test_samples.i3.gz"])
print("...done. %s" % "SUCCESS" if err_code == 0 else "FAILURE")
if err_code != 0 : sys.exit(err_code)


cmd = ["svn","info",expandvars("$I3_SRC")]
svn_info = subprocess.check_output(cmd)
logfile.write(svn_info)

import datetime
timestamp = str(datetime.datetime.now())
logfile.write("%s\n" % timestamp)

cmd = ["md5sum", out_filename]
check_sum = subprocess.check_output(cmd)

