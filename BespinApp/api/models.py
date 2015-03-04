"""
API Objects
"""


class Controller(object):
    """
    Controller Object
    """

    def __init__(self, controller_id, name, location, uri=None, nodes=None):
        self.controller_id = controller_id
        self.name = name
        self.location = location
        self.uri = uri
        self.nodes = nodes


class Data(object):
    """
    Data Object
    """

    def __init__(self, controller_id, node_id, sensor_id, payload,
                 created=None):
        self.controller_id = controller_id
        self.node_id = node_id
        self.sensor_id = sensor_id
        self.payload = payload
        self.created = created


class Node(object):
    """
    Node Object
    """

    def __init__(self, controller_id, node_id, name):
        self.controller_id = controller_id
        self.node_id = node_id
        self.name = name


class Sensor(object):
    """
    Sensor Object
    """

    def __init__(self, controller_id, node_id, sensor_id, sensor_type, name):
        self.controller_id = controller_id
        self.node_id = node_id
        self.sensor_id = sensor_id
        self.sensor_type = sensor_type
        self.name = name
        #self.interval = interval


