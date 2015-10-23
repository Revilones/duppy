from django.contrib import admin
from django.utils.translation import ugettext_lazy as _

from .models import Room, RoomSensorsRelationship, RoomMonitorRelationship

class RoomFilter(admin.SimpleListFilter):

    title = _("room")
    parameter_name = "room"

    def lookups(self, request, model_admin):
        return [(room.pk, room.name) for room in Room.objects.all()]

    def queryset(self, request, queryset):
        if self.value():
            return queryset.filter(room=int(self.value()))

class RoomAdmin(admin.ModelAdmin):
    list_display = ("name", "user")

class RoomSensorsRelationshipAdmin(admin.ModelAdmin):
    list_display = ("pk","room", "sensor", "order")
    list_filter = (RoomFilter,)
    list_editable = ("order",)

admin.site.register(Room, RoomAdmin)
admin.site.register(RoomSensorsRelationship, RoomSensorsRelationshipAdmin)
admin.site.register(RoomMonitorRelationship)
