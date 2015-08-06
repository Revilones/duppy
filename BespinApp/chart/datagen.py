#!/usr/bin/env python
import argparse
import httplib
import ssl
import json
# import logging
# import serial
# import time
import os

# import datetime
# from django.utils import timezone

import settings
from time import sleep
from random import randrange

ERROR = -1
SUCCESS = 0

# LOG = logging.getLogger("bespin_adapter")

###############################################################################
# Bespin Cloud API
###############################################################################
DEFAULT_HEADERS = {"Content-type": "application/json",
                   "Accept": "application/json",
                   "Authorization": "Token %s" % settings.API_TOKEN}

# def api_get(uri, headers=None):
#     http_headers = DEFAULT_HEADERS.copy()
#     if headers:
#         http_headers.update(headers)
#     conn = httplib.HTTPSConnection(settings.API_HOST, settings.API_PORT)
#     conn.request("GET", uri, None, http_headers)
#     response = conn.getresponse()
#     conn.close()
#     return response

def api_post(uri, headers=None, body=None):
    http_headers = DEFAULT_HEADERS.copy()
    if headers:
        http_headers.update(headers)
    conn = httplib.HTTPConnection(settings.API_HOST, settings.API_PORT)
    if body:
        body = json.dumps(body)
    conn.request("POST", uri, body, http_headers)
    response = conn.getresponse()
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

# def api_get_nodes(controller_id):
#     uri = "/api/controllers/%s/nodes" % (controller_id)
#     response = api_get(uri)
#     if response.status == 200:
#         try:
#             return json.loads(response.read())
#         except Exception as Error:
#             return []

# def api_get_node(controller_id, node_id):
#     uri = "/api/controllers/%s/nodes/%s" % (controller_id, node_id)
#     response = api_get(uri)
#     if response.status == 200:
#         return json.loads(response.read())

# def api_create_node(controller_id, node_id):
#     uri = "/api/controllers/%s/nodes" % controller_id
#     body = {"node_id": int(node_id)}
#     response = api_post(uri, body=body)
#     return response.status is 201

# def api_get_sensor(controller_id, node_id, sensor_id):
#     uri = "/api/controllers/%s/nodes/%s/sensors/%s" % \
#             (controller_id, node_id, sensor_id)
#     response = api_get(uri)
#     if response.status == 200:
#         return json.loads(response.read())

# def api_create_sensor(controller_id, node_id, sensor_id, sensor_type):
#     uri = "/api/controllers/%s/nodes/%s/sensors" % (controller_id, node_id)
#     body = {"sensor_id": int(sensor_id),
#             "sensor_type": sensor_type}
#     response = api_post(uri, body=body)
#     return response.status is 201

def main():
    # args = parse_args()

    for i in range(30):
        upload_data(1, 1, randrange(20, 25))
        sleep(1)


if __name__ == "__main__":
    main()