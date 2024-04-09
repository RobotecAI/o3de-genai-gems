import azlmbr.bus
import azlmbr.editor
import azlmbr.math

camera_pos = azlmbr.math.Vector3()
found = azlmbr.editor.EditorCameraRequestBus(azlmbr.bus.Broadcast, 'GetActiveCameraPosition', camera_pos)
if not found:
    print("Failed to get camera position")
else:
    print(camera_pos)
