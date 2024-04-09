import azlmbr.legacy.general
import azlmbr.math
import azlmbr.prefab
import azlmbr.bus

# 1. locate parent entity AITest
entityID = azlmbr.legacy.general.find_editor_entity("AITest")
# 2. use default local pose
pose = azlmbr.math.Vector3()
# 3. instantiate a ROSBot_slamtec prefab
result=azlmbr.prefab.PrefabPublicRequestBus(azlmbr.bus.Broadcast, 'InstantiatePrefab',"ROSBot_slamtec.prefab", entityID, pose)
