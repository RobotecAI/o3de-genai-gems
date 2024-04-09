import azlmbr.bus as bus
import azlmbr.components
import azlmbr.entity
import azlmbr.math

# 1. Search for rosbot_xl_slamtec entity
search_filter = azlmbr.entity.SearchFilter()
search_filter.names = ['rosbot_xl_slamtec']
found = azlmbr.entity.SearchBus(bus.Broadcast, 'SearchEntities', search_filter)
robot = found[0]

# 2. Move found entity one meter to the left
offset = azlmbr.math.Vector3_CreateAxisY(1.0)
center = azlmbr.components.TransformBus(bus.Event, "MoveEntity", robot, offset)
