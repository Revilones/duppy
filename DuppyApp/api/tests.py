from django.contrib.auth.models import User
from django.core.urlresolvers import reverse
from django.test import TestCase

from rest_framework import status
from rest_framework.authtoken.models import Token
from rest_framework.test import APITestCase

import base.models

class BaseApiTestCase(APITestCase):
    fixtures = ["basic_test.json"]

    @property
    def user(self):
        return User.objects.get(pk=1)

    @property
    def controller(self):
        return base.models.Controller.objects.get(pk=1)

    @property
    def node1(self):
        return base.models.Node.objects.get(pk=1)

    @property
    def node2(self):
        return base.models.Node.objects.get(pk=2)

    def authenticate(self, client):
        token = self.user.auth_token
        self.assertIsNotNone(token, "API auth token was not found")
        client.credentials(HTTP_AUTHORIZATION="Token %s" % token.key)

class ControllerSetViewTest(BaseApiTestCase):
    """
    /api/controller
    """

    def test_get(self):
        self.authenticate(self.client)

        url = reverse("api-controller-set-view")
        response = self.client.get(url)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertTrue(len(response.data) == 1)

class ControllerViewTest(BaseApiTestCase):
    """
    /api/controller/<controller_id>
    """

    def test_get(self):
        self.authenticate(self.client)

        url = reverse("api-controller-set-view")
        response = self.client.get(url)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertTrue(len(response.data) == 1)

        controller_id = response.data[0].get("controller_id")
        self.assertIsNotNone(controller_id)

        url = reverse("api-controller-view", args=[controller_id])
        response = self.client.get(url)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data.get("controller_id"), controller_id)

class NodeSetViewTest(BaseApiTestCase):
    """
    /api/controller/<controller_id>/nodes
    """

    def test_get(self):
        self.authenticate(self.client)

        url = reverse("api-node-set-view", args=[self.controller.controller_id])
        response = self.client.get(url)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertTrue(len(response.data) == 2)

    def test_post(self):
        self.authenticate(self.client)

        url = reverse("api-node-set-view", args=[self.controller.controller_id])
        data = {"node_id": 10,
                "name": "Test Create Node"}
        response = self.client.post(url, data)
        self.assertEqual(response.status_code, status.HTTP_201_CREATED)

        url = reverse("api-node-view", args=[self.controller.controller_id,
                                             data["node_id"]])
        response = self.client.get(url)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data.get("name"), data["name"])
        self.assertEqual(response.data.get("node_id"), data["node_id"])

class NodeViewTest(BaseApiTestCase):
    """
    /api/controller/<controller_id>/nodes/<node_id>
    """

    def test_get(self):
        self.authenticate(self.client)

        url = reverse("api-node-view", args=[self.controller.controller_id,
                                             self.node1.node_id])
        response = self.client.get(url)
        self.assertEqual(response.status_code, status.HTTP_200_OK)
        self.assertEqual(response.data.get("controller_id"),
                                           self.node1.controller.controller_id)
        self.assertEqual(response.data.get("node_id"), self.node1.node_id)
        self.assertEqual(response.data.get("name"), self.node1.name)
