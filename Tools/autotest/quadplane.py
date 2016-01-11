# fly ArduPlane QuadPlane in SITL

import util, pexpect, sys, time, math, shutil, os
from common import *
from pymavlink import mavutil
import random

# get location of scripts
testdir=os.path.dirname(os.path.realpath(__file__))


#HOME_LOCATION='-27.274439,151.290064,343,8.7'
HOME_LOCATION='-35.362938,149.165085,585,354'
WIND="0,180,0.2" # speed,direction,variance

homeloc = None

def fly_mission(mavproxy, mav, filename, height_accuracy=-1):
    '''fly a mission from a file'''
    print("Flying mission %s" % filename)
    mavproxy.send('wp load %s\n' % filename)
    mavproxy.expect('Flight plan received')
    mavproxy.send('wp list\n')
    mavproxy.expect('Requesting [0-9]+ waypoints')
    mavproxy.send('mode AUTO\n')
    wait_mode(mav, 'AUTO')
    if not wait_waypoint(mav, 1, 12, max_dist=60):
        return False
    mavproxy.expect('DISARMED')
    print("Mission OK")
    return True


def fly_QuadPlane(viewerip=None, map=False):
    '''fly QuadPlane in SIL

    you can pass viewerip as an IP address to optionally send fg and
    mavproxy packets too for local viewing of the flight in real time
    '''
    global homeloc

    options = '--sitl=127.0.0.1:5501 --out=127.0.0.1:19550 --streamrate=10'
    if viewerip:
        options += " --out=%s:14550" % viewerip
    if map:
        options += ' --map'

    sil = util.start_SIL('ArduPlane', model='quadplane', wipe=True, home=HOME_LOCATION, speedup=10,
                         defaults_file=os.path.join(testdir, 'quadplane.parm'))
    mavproxy = util.start_MAVProxy_SIL('QuadPlane', options=options)
    mavproxy.expect('Telemetry log: (\S+)')
    logfile = mavproxy.match.group(1)
    print("LOGFILE %s" % logfile)

    buildlog = util.reltopdir("../buildlogs/QuadPlane-test.tlog")
    print("buildlog=%s" % buildlog)
    if os.path.exists(buildlog):
        os.unlink(buildlog)
    try:
        os.link(logfile, buildlog)
    except Exception:
        pass

    util.expect_setup_callback(mavproxy, expect_callback)

    mavproxy.expect('Received [0-9]+ parameters')

    expect_list_clear()
    expect_list_extend([sil, mavproxy])

    print("Started simulator")

    # get a mavlink connection going
    try:
        mav = mavutil.mavlink_connection('127.0.0.1:19550', robust_parsing=True)
    except Exception, msg:
        print("Failed to start mavlink connection on 127.0.0.1:19550" % msg)
        raise
    mav.message_hooks.append(message_hook)
    mav.idle_hooks.append(idle_hook)

    failed = False
    e = 'None'
    try:
        print("Waiting for a heartbeat with mavlink protocol %s" % mav.WIRE_PROTOCOL_VERSION)
        mav.wait_heartbeat()
        print("Waiting for GPS fix")
        mav.recv_match(condition='VFR_HUD.alt>10', blocking=True)
        mav.wait_gps_fix()
        while mav.location().alt < 10:
            mav.wait_gps_fix()
        homeloc = mav.location()
        print("Home location: %s" % homeloc)

        # wait for EKF to settle
        wait_seconds(mav, 15)

        mavproxy.send('arm throttle\n')
        mavproxy.expect('ARMED')
        
        if not fly_mission(mavproxy, mav, os.path.join(testdir, "ArduPlane-Missions/CMAC-VTOL-ccw.txt")):
            print("Failed mission")
            failed = True
        if not log_download(mavproxy, mav, util.reltopdir("../buildlogs/QuadPlane-log.bin")):
            print("Failed log download")
            failed = True
    except pexpect.TIMEOUT, e:
        print("Failed with timeout")
        failed = True

    mav.close()
    util.pexpect_close(mavproxy)
    util.pexpect_close(sil)

    if os.path.exists('QuadPlane-valgrind.log'):
        os.chmod('QuadPlane-valgrind.log', 0644)
        shutil.copy("QuadPlane-valgrind.log", util.reltopdir("../buildlogs/QuadPlane-valgrind.log"))

    if failed:
        print("FAILED: %s" % e)
        return False
    return True
