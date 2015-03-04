#!/usr/bin/env python
import argparse
import daemon
import httplib
import json
import logging
import os
import serial
import sys
import time

###############################################################################
# Config Options
###############################################################################
LOG_LEVEL = logging.ERROR

CONFIG_METRIC = False

API_HOST = "127.0.0.1"
API_PORT = 8000
API_URL = "/api/data/"
API_TOKEN = "1afb098c68f2d7466c178b60b78c6b6443a88d36"
API_CONTROLLERID = "430868768a457feedd2e625c827cfa486570de86"


###############################################################################
# MySensors Message API
###############################################################################
# Message types
C_PRESENTATION      = 0
C_SET               = 1
C_REQ               = 2
C_INTERNAL          = 3
C_STREAM            = 4

# Presentation sub-types
S_DOOR              = 0
S_MOTION            = 1
S_SMOKE             = 2
S_LIGHT             = 3
S_DIMMER            = 4
S_COVER             = 5
S_TEMP              = 6
S_HUM               = 7
S_BARO              = 8
S_WIND              = 9
S_RAIN              = 10
S_UV                = 11
S_WEIGHT            = 12
S_POWER             = 13
S_HEATER            = 14
S_DISTANCE          = 15
S_LIGHT_LEVEL       = 16
S_ARDUINO_NODE      = 17
S_ARDUINO_RELAY     = 18
S_LOCK              = 19
S_IR                = 20
S_WATER             = 21
S_AIR_QUALITY       = 22
S_CUSTOM            = 23
S_DUST              = 24
S_SCENE_CONTROLLER  = 25

# SET/REQ sub-types
V_TEMP              = 0
V_HUM               = 1
V_LIGHT             = 2
V_DIMMER            = 3
V_PRESSURE          = 4
V_FORECAST          = 5
V_RAIN              = 6
V_RAINRATE          = 7
V_WIND              = 8
V_GUST              = 9
V_DIRECTION         = 10
V_UV                = 11
V_WEIGHT            = 12
V_DISTANCE          = 13
V_IMPEDANCE         = 14
V_ARMED             = 15
V_TRIPPED           = 16
V_WATT              = 17
V_KWH               = 18
V_SCENE_ON          = 19
V_SCENE_OFF         = 20
V_HEATER            = 21
V_HEATER_SW         = 22
V_LIGHT_LEVEL       = 23
V_VAR1              = 24
V_VAR2              = 25
V_VAR3              = 26
V_VAR4              = 27
V_VAR4              = 28
V_UP                = 29
V_DOWN              = 30
V_STOP              = 31
V_IR_SEND           = 32
V_IR_RECEIVE        = 33
V_FLOW              = 34
V_VOLUME            = 35
V_LOCK_STATUS       = 36
V_DUST_LEVEL        = 37
V_VOLTAGE           = 38
V_CURRENT           = 39

# Internal sub-types
I_BATTERY_LEVEL     = 0
I_TIME              = 1
I_VERSION           = 2
I_ID_REQUEST        = 3
I_ID_RESPONSE       = 4
I_INCLUSION_MODE    = 5
I_CONFIG            = 6
I_FIND_PARENT       = 7
I_FIND_PARENT_RESPONSE = 8
I_LOG_MESSAGE       = 9
I_CHILDREN          = 10
I_SKETCH_NAME       = 11
I_SKETCH_VERSION    = 12
I_REBOOT            = 13
I_GATEWAY_READY     = 14


LOG = logging.getLogger("bespin_adapter")


def upload_data(nodeid, sensorid, message_type, sub_type, payload):
    body = {"controller_id": API_CONTROLLERID,
            "node_id": int(nodeid),
            "sensor_id": int(sensorid),
            "payload": payload}
    headers = {"Content-type": "application/json",
               "Accept": "application/json",
               "Authorization": "Token %s" % API_TOKEN}
    conn = httplib.HTTPConnection(API_HOST, API_PORT)
    conn.request("POST", API_URL, json.dumps(body), headers)
    response = conn.getresponse()
    print response.status, response.reason
    data = response.read()
    print data
    conn.close()


###############################################################################
# Presentation Command Handlers
###############################################################################
def command_present_node(nodeid, sensorid, payload):
    """
    A new node has connected to the sensor network.
    """
    print "Node online: %s" % nodeid

def command_present_temp(nodeid, sensorid, payload):
    """
    A node is broadcasting it has a temperature sensor configured.
    """
    print "Node[%s] has temperature sensor with id: %s" % (nodeid, sensorid)

def command_present_hum(nodeid, sensorid, payload):
    """
    A node is broadcasting it has a humidity sensor configured.
    """
    print "Node[%s] has humidity sensor with id: %s" % (nodeid, sensorid)

###############################################################################
# Req/Set Command Handlers
###############################################################################
def command_set_temp(nodeid, sensorid, payload):
    upload_data(nodeid, sensorid, C_SET, V_TEMP, payload)
    print "NODE[%s] Temperature: %sF" % (nodeid, payload)

def command_set_hum(nodeid, sensorid, payload):
    upload_data(nodeid, sensorid, C_SET, V_HUM, payload)
    print "NODE[%s] Humidity: %s%%" % (nodeid, payload)

###############################################################################
# Internal Command Handlers
###############################################################################
def command_internal_time(nodeid, sensorid, payload):
    """
    Node is requesting system time.
    """
    timestamp = int(time.time())
    return encode_command(nodeid, sensorid, C_INTERNAL, 0, I_TIME, data)

def command_internal_config(nodeid, sensorid, payload):
    """
    Node is requesting config
    """
    data = "I"
    if CONFIG_METRIC:
        data = "M"
    return encode_command(nodeid, sensorid, C_INTERNAL, 0, I_CONFIG, data)

HANDLERS = {}
def register_action(msgtype, subtype, func):
    global HANDLERS
    if msgtype not in HANDLERS:
        HANDLERS[msgtype] = {}
    if subtype not in HANDLERS[msgtype]:
        HANDLERS[msgtype][subtype] = []
    HANDLERS[msgtype][subtype].append(func)

register_action(C_PRESENTATION, S_ARDUINO_NODE, command_present_node)
register_action(C_PRESENTATION, S_TEMP, command_present_temp)
register_action(C_PRESENTATION, S_HUM, command_present_hum)

register_action(C_SET, V_TEMP, command_set_temp)
register_action(C_SET, V_HUM, command_set_hum)

register_action(C_INTERNAL, I_TIME, command_internal_time)
register_action(C_INTERNAL, I_CONFIG, command_internal_config)

def parse_command(command):
    parts = command.split(";")
    nodeid = parts[0]
    sensorid = parts[1]
    msgtype = int(parts[2])
    ack = parts[3]
    subtype = int(parts[4])
    payload = parts[5]

    replies = []
    if msgtype in HANDLERS and subtype in HANDLERS[msgtype]:
        for handler in HANDLERS[msgtype][subtype]:
            reply = handler(nodeid, sensorid, payload)
            if reply:
                replies.append(reply)
    else:
        print "Command not handled: %s" % command
    return replies

def encode_command(destid, sensorid, msgtype, ack, subtype, payload):
    return "%s;%s;%s;%s;%s;%s\n" % (destid, sensorid, msgtype, ack, \
            subtype, payload)

def listen(device):
    with serial.Serial(device, 115200) as gateway:
        while True:
            line = gateway.readline().strip()
            print line
            try:
                replies = parse_command(line)
                if replies:
                    for reply in replies:
                        print "Sending response: %s" % reply
                        gateway.write(reply)
            except Exception as err:
                print "Error while parsing command:"
                print err.message

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument("device",
            help="Serial device of MySensors gateway.")

    parser.add_argument("-d", "--debug", action="store_true", default=False,
            help="Enabled debug mode.")

    return parser.parse_args()

def run(device):
    listen(device=device)

def run_as_daemon(device):
    with daemon.DaemonContext():
        run(device)

def main():
    args = parse_args()

    global LOG_LEVEL
    if args.debug:
        LOG_LEVEL = logging.DEBUG

    logging.basicConfig(level=LOG_LEVEL)

    if args.debug:
        try:
            run(args.device)
        except KeyboardInterrupt:
            pass
    else:
        run_as_daemon(args.device)


if __name__ == "__main__":
    main()
