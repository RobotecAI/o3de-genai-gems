import azlmbr.bus as bus
import azlmbr.components
import azlmbr.entity
import azlmbr.editor
import azlmbr.math

# 1. Search for rosbot_xl_slamtec entity
search_filter = azlmbr.entity.SearchFilter()
search_filter.names = ['rosbot_xl_slamtec']
found = azlmbr.entity.SearchBus(bus.Broadcast, 'SearchEntities', search_filter)
robot = found[0]

# 2. Find its position
center = azlmbr.components.TransformBus(bus.Event, "GetWorldTranslation", robot)

# 3. Search for all entities in 1.0m radius around robot
search_filter = azlmbr.entity.SearchFilter()
search_filter.aabb = azlmbr.math.Aabb_CreateCenterRadius(center, 1.0)
found = azlmbr.entity.SearchBus(bus.Broadcast, 'SearchEntities', search_filter)

# 4. Print name and position of each found entity but exclude robot entity and its descendants
rosbot_entities = azlmbr.components.TransformBus(bus.Event, "GetEntityAndAllDescendants", robot)
for f in found:
    if f in rosbot_entities:
        continue
    position = azlmbr.components.TransformBus(bus.Event, "GetWorldTranslation", f)
    name = azlmbr.editor.EditorEntityInfoRequestBus(bus.Event, 'GetName', f)
    print(f"{name} at {position}")

