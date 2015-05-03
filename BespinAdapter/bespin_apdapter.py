#!/usr/bin/env python
import argparse
import httplib
import json
import logging
import serial
import time
import os

import daemon
import mysensors
import settings


LOG = logging.getLogger("bespin_adapter")

###############################################################################
# Bespin Cloud API
###############################################################################
DEFAULT_HEADERS = {"Content-type": "application/json",
                   "Accept": "application/json",
                   "Authorization": "Token %s" % settings.API_TOKEN}

def api_get(uri, headers=None):
    http_headers = DEFAULT_HEADERS.copy()
    if headers:
        http_headers.update(headers)
    conn = httplib.HTTPConnection(settings.API_HOST, settings.API_PORT)
    LOG.info("[API] GET %s", uri)
    conn.request("GET", uri, None, http_headers)
    response = conn.getresponse()
    LOG.info("[API] Status: %s, Reason: %s", response.status, response.reason)
    conn.close()
    return response

def api_post(uri, headers=None, body=None):
    http_headers = DEFAULT_HEADERS.copy()
    if headers:
        http_headers.update(headers)
    conn = httplib.HTTPConnection(settings.API_HOST, settings.API_PORT)
    if body:
        body = json.dumps(body)
    LOG.info("[API] POST %s", uri)
    LOG.debug(body)
    conn.request("POST", uri, body, http_headers)
    response = conn.getresponse()
    LOG.info("[API] Status: %s, Reason: %s", response.status, response.reason)
    conn.close()
    return response

def upload_data(nodeid, sensorid, payload):
    uri = "/api/data"
    body = {"controller_id": settings.API_CONTROLLERID,
            "node_id": int(nodeid),
            "sensor_id": int(sensorid),
            "payload": payload}
    response = api_post(uri, body=body)
    return response.read()


def api_get_node(controller_id, node_id):
    uri = "/api/controllers/%s/nodes/%s" % (controller_id, node_id)
    response = api_get(uri)
    if response.status == 200:
        return json.loads(response.read())

def api_create_node(controller_id, node_id):
    uri = "/api/controllers/%s/nodes" % controller_id
    body = {"node_id": int(node_id)}
    response = api_post(uri, body=body)
    return response.status is 201

def api_get_sensor(controller_id, node_id, sensor_id):
    uri = "/api/controllers/%s/nodes/%s/sensors/%s" % \
            (controller_id, node_id, sensor_id)
    response = api_get(uri)
    if response.status == 200:
        return json.loads(response.read())

def api_create_sensor(controller_id, node_id, sensor_id, sensor_type):
    uri = "/api/controllers/%s/nodes/%s/sensors" % (controller_id, node_id)
    body = {"sensor_id": int(sensor_id),
            "sensor_type": sensor_type}
    response = api_post(uri, body=body)
    return response.status is 201


###############################################################################
# Presentation Command Handlers
###############################################################################
def command_present_node(nodeid, sensorid, payload):
    """
    A new node has connected to the sensor network.
    """
    node = api_get_node(settings.API_CONTROLLERID, nodeid)
    if node is None:
        if api_create_node(settings.API_CONTROLLERID, nodeid):
            LOG.info("[API] Created node '%s'", nodeid)
        else:
            LOG.error("[API] Failed to create node '%s'", nodeid)
    LOG.info("Node '%s' has connected", nodeid)

def command_present_temp(nodeid, sensorid, payload):
    """
    A node is broadcasting it has a temperature sensor configured.
    """
    LOG.info("Node '%s' has temperature sensor with id: %s", nodeid, sensorid)
    sensor = api_get_sensor(settings.API_CONTROLLERID, nodeid, sensorid)
    if sensor is None:
        if api_create_sensor(settings.API_CONTROLLERID, nodeid, sensorid, \
                "temperature"):
            LOG.info("[API] Created sensor '%s' on node '%s'", \
                    sensorid, nodeid)
        else:
            LOG.error("[API] Failed to create sensor '%s' on node '%s'", \
                    sensorid, nodeid)

def command_present_hum(nodeid, sensorid, payload):
    """
    A node is broadcasting it has a humidity sensor configured.
    """
    LOG.info("Node '%s' has humidity sensor with id: %s", nodeid, sensorid)
    sensor = api_get_sensor(settings.API_CONTROLLERID, nodeid, sensorid)
    if sensor is None:
        if api_create_sensor(settings.API_CONTROLLERID, nodeid, \
                sensorid, "humidity"):
            LOG.info("[API] Created sensor '%s' on node '%s'", \
                    sensorid, nodeid)
        else:
            LOG.error("[API] Failed to create sensor '%s' on node '%s'", \
                    sensorid, nodeid)

def command_present_co2(nodeid, sensorid, payload):
    """
    A node is broadcasting it has a co2 sensor configured.
    """
    LOG.info("Node '%s' has co2 sensor with id: %s", nodeid, sensorid)
    sensor = api_get_sensor(settings.API_CONTROLLERID, nodeid, sensorid)
    if sensor is None:
        if api_create_sensor(settings.API_CONTROLLERID, nodeid, sensorid, "co2"):
            LOG.info("[API] Created sensor '%s' on node '%s'",
                    sensorid, nodeid)
        else:
            LOG.error("[API] Failed to create sensor '%s' on node '%s'",
                    sensorid, nodeid)

###############################################################################
# Req/Set Command Handlers
###############################################################################
def command_set_temp(nodeid, sensorid, payload):
    LOG.debug("Node: %s, Sensor: %s, Temperature: %sF", nodeid,
              sensorid, payload)
    upload_data(nodeid, sensorid, payload)

def command_set_hum(nodeid, sensorid, payload):
    LOG.debug("Node %s, Sesnor: %s, Humidity: %s%%", nodeid, sensorid, payload)
    upload_data(nodeid, sensorid, payload)

def command_set_co2(nodeid, sensorid, payload):
    LOG.debug("Node %s, Sesnor: %s, Co2: %s%%", nodeid, sensorid, payload)
    upload_data(nodeid, sensorid, payload)

###############################################################################
# Internal Command Handlers
###############################################################################
def command_internal_time(nodeid, sensorid, payload):
    """
    Node is requesting system time.
    """
    timestamp = int(time.time())
    return encode_command(nodeid, sensorid, mysensors.C_INTERNAL, 0, \
            mysensors.I_TIME, timestamp)

def command_internal_config(nodeid, sensorid, payload):
    """
    Node is requesting config
    """
    data = "I"
    if settings.USE_METRIC:
        data = "M"
    return encode_command(nodeid, sensorid, mysensors.C_INTERNAL, 0, \
            mysensors.I_CONFIG, data)

def command_internal_gateway_ready(nodeid, sensorid, payload):
    """
    Gateway node is reporting ready.
    """
    LOG.info("Gateway ready")

NODEID = 1

def command_id_request(nodeid, sensorid, payload):
    """
    Node is requesting id.
    """
    global NODEID 
    data = NODEID
    NODEID = NODEID + 1
    file(settings.NODEID_FILE, 'w+').write("%s\n" % NODEID)

    LOG.info("Id Request sending id:%d" % data)
    return encode_command(nodeid, sensorid, mysensors.C_INTERNAL, 0, \
            mysensors.I_ID_RESPONSE, data)

HANDLERS = {}
def register_action(msgtype, subtype, func):
    if msgtype not in HANDLERS:
        HANDLERS[msgtype] = {}
    if subtype not in HANDLERS[msgtype]:
        HANDLERS[msgtype][subtype] = []
    HANDLERS[msgtype][subtype].append(func)

register_action(mysensors.C_PRESENTATION, \
        mysensors.S_ARDUINO_NODE, \
        command_present_node)
register_action(mysensors.C_PRESENTATION, \
        mysensors.S_TEMP, \
        command_present_temp)
register_action(mysensors.C_PRESENTATION, \
        mysensors.S_HUM, \
        command_present_hum)
register_action(mysensors.C_PRESENTATION, \
        mysensors.S_AIR_QUALITY, \
        command_present_co2)

register_action(mysensors.C_SET, mysensors.V_TEMP, command_set_temp)
register_action(mysensors.C_SET, mysensors.V_HUM, command_set_hum)
register_action(mysensors.C_SET, mysensors.V_GAS, command_set_co2)

register_action(mysensors.C_INTERNAL, mysensors.I_TIME, command_internal_time)
register_action(mysensors.C_INTERNAL, mysensors.I_CONFIG, \
        command_internal_config)
register_action(mysensors.C_INTERNAL, mysensors.I_GATEWAY_READY, \
        command_internal_gateway_ready)
register_action(mysensors.C_INTERNAL, mysensors.I_ID_REQUEST, \
        command_id_request)

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
        LOG.debug("Command not handled: %s", command)
    return replies

def encode_command(destid, sensorid, msgtype, ack, subtype, payload):
    return "%s;%s;%s;%s;%s;%s\n" % (destid, sensorid, msgtype, ack, \
            subtype, payload)

def listen(device):
    with serial.Serial(device, 115200) as gateway:
        while True:
            line = gateway.readline().strip()
            LOG.debug("Receiving: %s", line)
            try:
                replies = parse_command(line)
                if replies:
                    for reply in replies:
                        LOG.debug("Sending: %s", reply.strip())
                        gateway.write(reply)
            except Exception as err:
                LOG.exception("Error while parsing command: %s", err.message)

def parse_args():
    parser = argparse.ArgumentParser(description="Bespin mysensors adapter.")
    subparsers = parser.add_subparsers(dest='action')

    start_parser = subparsers.add_parser("start", help="Start the adapter")
    start_parser.add_argument("device", \
            help="Serial device of MySensors gateway.")
    start_parser.add_argument("-d", "--debug", action="store_true", \
            default=False, help="Enabled debug mode.")
    start_parser.add_argument("-l", "--log", help="Set log file location")

    stop_parser = subparsers.add_parser("stop", help="Stop the adapter")

    status_parser = subparsers.add_parser("status", \
            help="Check adapter status.")

    return parser.parse_args()


class BespinAdapter(daemon.Daemon):

    def __init__(self, device=None, run_as_daemon=True, log_file=None):
        #Init Id File
        global NODEID
        try:
            with file(settings.NODEID_FILE, 'r') as idfile:
                NODEID = int(idfile.read().strip())
        except IOError:
                NODEID = 1
                file(settings.NODEID_FILE, 'w+').\
                                write("%s\n" % NODEID)

        if not log_file:
            log_file = settings.LOG_FILE
        super(BespinAdapter, self).__init__(settings.PID_FILE, \
                chroot="/tmp", stdout=log_file, stderr=log_file)
        self.device = device
        self.run_as_daemon = run_as_daemon

    def start(self):
        if self.run_as_daemon:
            super(BespinAdapter, self).start()
        else:
            try:
                self.run()
            except KeyboardInterrupt:
                pass

    def run(self):
        listen(self.device)


def main():
    args = parse_args()

    if args.action == "start":
        log_level = settings.LOG_LEVEL
        if args.debug:
            log_level = logging.DEBUG

        logging.basicConfig(level=log_level,
                            format="%(asctime)s - %(levelname)s - %(message)s")

        adapter = BespinAdapter(device=args.device, \
                run_as_daemon=(not args.debug))
        adapter.start()
    elif args.action == "stop":
        adapter = BespinAdapter()
        adapter.stop()
    elif args.action == "status":
        adapter = BespinAdapter()
        pid = adapter.status()
        if pid:
            print "Running (%s)" % pid
        else:
            print "Not running"



if __name__ == "__main__":
    main()
