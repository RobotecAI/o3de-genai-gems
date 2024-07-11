"""
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
"""

from pathlib import Path
import yaml
import xml.etree.ElementTree as ET
from xml.dom.minidom import parseString
import numpy as np
from utils import (
    parse_vector,
    prefab_dicts_to_xml,
)
from prefab import AvailablePrefab, SpawnedPrefab, get_overlapping_prefabs_pairs, EditorEntityId
import azlmbr


class SpawnedPrefabsDict(dict):
    def __getitem__(self, key: EditorEntityId | int) -> SpawnedPrefab:
        key = int(key)
        return super().__getitem__(key)

    def __setitem__(self, key: EditorEntityId | int, value: SpawnedPrefab):
        key = int(key)
        super().__setitem__(key, value)


class OverlapException(Exception):
    pass


class PrefabsManager:
    def __init__(
        self,
        prefabs_info_yaml_path: None | Path = None,
    ) -> None:
        self.prefabs_info_yaml_path = prefabs_info_yaml_path
        self.automatically_removed_prefabs = set()
        self.actions_log = []
        self.available_prefabs = {}
        self.spawned_prefabs = SpawnedPrefabsDict()

    @classmethod
    def from_yaml(cls, prefabs_info_yaml_path: Path, *args, **kwargs) -> "PrefabsManager":
        return cls(prefabs_info_yaml_path=prefabs_info_yaml_path, *args, **kwargs)

    def get_actions_log_xml(self, num_actions: int = 0) -> str:
        actions_log = "\n".join(self.actions_log[-int(num_actions) :])
        return f"<actions_log>\n{actions_log}\n</actions_log>"

    def refresh_prefabs(self):
        self.available_prefabs = self.get_available_prefabs()
        self.spawned_prefabs = self.get_spawned_prefabs()

    def get_available_prefabs(self) -> dict[str, AvailablePrefab]:
        prefabs = self.get_available_prefab_dicts_from_yaml()
        return {prefab.name: prefab for prefab in prefabs}

    @property
    def available_prefabs_xml(self) -> str:
        inner_xml = "".join([prefab.xml for prefab in self.available_prefabs.values()])
        return f"<available_prefabs>\n{inner_xml}</available_prefabs>"

    def get_available_prefab_dicts_from_yaml(self) -> list[AvailablePrefab]:
        with open(self.prefabs_info_yaml_path, "r") as f:
            prefabs = yaml.safe_load(f)
        for prefab in prefabs:
            prefab["path"] = str(prefab["path"])
        return [AvailablePrefab.from_dict(prefab) for prefab in prefabs]

    def get_spawned_prefabs(self) -> dict[int, SpawnedPrefab]:
        spawned_prefabs_list = self.get_spawned_prefabs_list_from_o3de()
        spawned_prefabs = SpawnedPrefabsDict({int(p.entity_id): p for p in spawned_prefabs_list})
        self.refresh_overlapping_entity_ids_for_prefabs(spawned_prefabs)
        return spawned_prefabs

    @property
    def spawned_prefabs_xml(self) -> str:
        inner_xml = "".join([prefab.xml for prefab in self.spawned_prefabs.values()])
        return f"<spawned_prefabs>\n{inner_xml}</spawned_prefabs>"

    def get_spawned_prefabs_list_from_o3de(self) -> list[SpawnedPrefab]:
        ids = self.find_prefab_ids()
        spawned_prefabs_list = []
        for entity_id in ids:
            name = azlmbr.editor.EditorEntityInfoRequestBus(
                azlmbr.bus.Event, "GetName", entity_id
            )
            available_prefab = self.available_prefabs[name]
            translation_azlmbr = azlmbr.components.TransformBus(
                azlmbr.bus.Event, "GetWorldTranslation", entity_id
            )
            translation = parse_vector(str(translation_azlmbr))
            translation_inv_anch, _ = self.transform_with_inv_anchor_transform(
                translation, available_prefab.anchor_transform
            )
            semantic_info = ""
            spawned_prefab = self.spawned_prefabs.get(EditorEntityId(entity_id), None)
            if spawned_prefab:
                semantic_info = spawned_prefab.semantic_info

            spawned_prefab = SpawnedPrefab(
                entity_id=EditorEntityId(entity_id),
                translation=translation_inv_anch,
                available_prefab=available_prefab,
                semantic_info=semantic_info,
            )
            spawned_prefabs_list.append(spawned_prefab)
        return spawned_prefabs_list[::-1]

    @property
    def overlapping_prefabs(self) -> list[SpawnedPrefab]:
        overlapping_prefabs = []
        for prefab in self.spawned_prefabs.values():
            if prefab.overlapping_entity_ids:
                overlapping_prefabs.append(prefab)
        return overlapping_prefabs

    def overlapping_prefabs_xml(self) -> str:
        op = self.overlapping_prefabs
        if op:
            return prefab_dicts_to_xml([prefab.to_dict() for prefab in op], "overlapping_prefabs")

    def refresh_overlapping_entity_ids_for_prefabs(
        self, spawned_prefabs: dict[int, SpawnedPrefab] = None
    ):
        if not spawned_prefabs:
            spawned_prefabs = self.spawned_prefabs
        overlapping_prefabs_pairs = get_overlapping_prefabs_pairs(set(spawned_prefabs.values()))
        for p1, p2 in overlapping_prefabs_pairs:
            p1.overlapping_entity_ids.add(int(p2.entity_id))
            p2.overlapping_entity_ids.add(int(p1.entity_id))

    def find_prefab_ids(self) -> list[azlmbr.entity.EntityId]:
        searchFilter = azlmbr.entity.SearchFilter()
        ids = azlmbr.entity.SearchBus(azlmbr.bus.Broadcast, "SearchEntities", searchFilter)
        ids_names = {
            id: azlmbr.editor.EditorEntityInfoRequestBus(azlmbr.bus.Event, "GetName", id)
            for id in ids
        }
        ids_with_parent_level = {}
        for id, name in ids_names.items():
            if name not in self.available_prefabs.keys():
                continue
            parent_entity_id = azlmbr.editor.EditorEntityInfoRequestBus(
                azlmbr.bus.Event, "GetParent", id
            )
            parent_name = azlmbr.editor.EditorEntityInfoRequestBus(
                azlmbr.bus.Event, "GetName", parent_entity_id
            )
            if parent_name == "Level":
                ids_with_parent_level[id] = name
        return ids_with_parent_level

    @staticmethod
    def transform_with_anchor_transform(
        translation: list[float], anchor_transform: dict[str, list[float]]
    ) -> tuple[list[float], list[float]]:
        "returns translation and rotation after applying anchor_transform"
        translation_anch = np.array(translation) - np.array(anchor_transform["translation"])
        # NOTE: no general rotation for now, so just from anchor_transform
        rotation_anch = np.deg2rad(anchor_transform["rotation"])
        return translation_anch.tolist(), rotation_anch.tolist()

    @staticmethod
    def transform_with_inv_anchor_transform(
        translation: list[float], anchor_transform: dict[str, list[float]]
    ) -> tuple[list[float], list[float]]:
        "returns translation and rotation after applying inverse anchor_transform"
        inv_translation_anch = np.array(translation) + np.array(anchor_transform["translation"])
        # NOTE: no general rotation for now, so just from anchor_transform
        inv_rotation_anch = -np.deg2rad(anchor_transform["rotation"])
        return inv_translation_anch.tolist(), inv_rotation_anch.tolist()

    def spawn_prefab(
        self,
        prefab_name: str,
        translation: list[float],
        semantic_info: str = "",
        raise_exception_on_overlap=False,
    ) -> SpawnedPrefab | None:
        translation = [round(float(p), 2) for p in translation]
        available_prefab = self.available_prefabs[prefab_name]

        translation_anch, rotation_anch = self.transform_with_anchor_transform(
            translation, available_prefab.anchor_transform
        )
        translation_anch_azlmbr = azlmbr.math.Vector3(*[float(p) for p in translation_anch])

        prefab_result = azlmbr.prefab.PrefabPublicRequestBus(
            azlmbr.bus.Broadcast,
            "InstantiatePrefab",
            available_prefab.path,
            azlmbr.entity.EntityId(),
            translation_anch_azlmbr,
        )
        azlmbr_entity_id: azlmbr.entity.EntityId = prefab_result.GetValue()
        entity_id = EditorEntityId(azlmbr_entity_id)

        if np.array(rotation_anch).any():
            rotation_anch_azlmbr = azlmbr.math.Vector3(*[float(r) for r in rotation_anch])
            azlmbr.components.TransformBus(
                azlmbr.bus.Event,
                "SetLocalRotation",
                azlmbr_entity_id,
                rotation_anch_azlmbr,
            )

        spawned_prefab = SpawnedPrefab(
            entity_id=entity_id,
            translation=translation,
            semantic_info=semantic_info,
            available_prefab=available_prefab,
        )

        self.spawned_prefabs[entity_id] = spawned_prefab
        self.refresh_overlapping_entity_ids_for_prefabs()
        try:
            if raise_exception_on_overlap:
                self.raise_exception_on_overlap(entity_id)
        except:
            self.remove_prefab(entity_id, log_action=False)
            raise
        else:
            self.actions_log.append(
                f'<action>Spawned {spawned_prefab.name} (id={entity_id}, semantic_info="{spawned_prefab.semantic_info}") at {spawned_prefab.translation}</action>'
            )
            return spawned_prefab

    def move_prefab(
        self,
        entity_id: int | EditorEntityId,
        new_translation: list[float],
        log_action: bool = True,
        raise_exception_on_overlap: bool = False,
    ):
        spawned_prefab = self.spawned_prefabs[entity_id]
        previous_translation = spawned_prefab.translation
        new_translation = [round(float(p), 2) for p in new_translation]
        new_translation_anch, _ = self.transform_with_anchor_transform(
            new_translation, spawned_prefab.available_prefab.anchor_transform
        )
        new_translation_anch_azlmbr = azlmbr.math.Vector3(
            *[float(p) for p in new_translation_anch]
        )

        azlmbr.components.TransformBus(
            azlmbr.bus.Event,
            "SetWorldTranslation",
            spawned_prefab.entity_id.azlmbr_entity_id,
            new_translation_anch_azlmbr,
        )

        spawned_prefab.translation = new_translation
        self.refresh_overlapping_entity_ids_for_prefabs()

        try:
            if raise_exception_on_overlap:
                self.raise_exception_on_overlap(entity_id)
        except OverlapException as e:
            self.move_prefab(
                entity_id,
                previous_translation,
                log_action=False,
                raise_exception_on_overlap=False,
            )
            raise e
        else:
            if log_action:
                self.actions_log.append(
                    f'<action>Moving {spawned_prefab.name} (id={entity_id}, semantic_info="{spawned_prefab.semantic_info}") from {spawned_prefab.translation} to {new_translation}</action>'
                )

    def raise_exception_on_overlap(self, entity_id: int | EditorEntityId):
        sp = self.spawned_prefabs[entity_id]
        if sp.overlapping_entity_ids:
            self.actions_log.append(
                f'<failed_action>\nFailed to put {sp.name} (id={sp.entity_id}, semantic_info="{sp.semantic_info}") at {sp.translation} due to overlap with {sp.overlapping_entity_ids}\n</failed_action>'
            )
            search_range = 2 * max(sp.size)
            area = [
                max(sp.translation[0] - search_range, 0),
                sp.translation[0] + search_range,
                max(sp.translation[1] - search_range, 0),
                sp.translation[1] + search_range,
            ]
            available_positions_in_nearby_area_along_x_and_y = (
                self.find_positions_along_x_and_along_y(sp, area, step=0.5)
            )
            message = ""
            message += f"\nRecent 10 actions:\n{self.get_actions_log_xml(-10)}\n"
            message += f"\nChecking for available positions along current x ({sp.translation[0]}) or along current y ({sp.translation[1]}) in a nearby <nearby_area>{search_range}x{search_range} m</nearby_area>"
            if available_positions_in_nearby_area_along_x_and_y:
                aps_xml = self.positions_to_xml(
                    available_positions_in_nearby_area_along_x_and_y, "available_positions"
                )
                message += (
                    f"\nAvailable positions (along same x or same y) in nearby area:\n{aps_xml}"
                )
            else:
                message += f"\nNo available positions in nearby area. Consider a new idea for this layout area."

            message += """
Before reacting to this problem think step by step in <thinking_how_to_solve_overlaps></thinking_how_to_solve_overlap_tags> XML tags. Follow steps defined in <overlap_handling></overlap_handling> and <helpful_advice></helpful_advice> XML tags below:

<overlap_handling>
    <step>0. Decide if the task is easy or hard. Usually if the layout is dense it means the task is hard</step>
    <step>1. If the task is easy ignore below steps and just move the prefab to a new position</step>
    <step>2. Analyze spawned prefabs xml above to get a general overview of the layout. It may be the case that there is no space left for your prefab - abort current step in such case</step>
    <step>3. Carefully analyze available positions listed in <available_positions></available_positions> XML tags above. Warning: the positions do not take into account the allowed area, only lack of potential overlaps</step>
    <step>4. Carefully analyze <actions_log> above and <line_number> the exception happened at to understand which actions where performed correctly before the exception</step>
    <step>5. Remove parts of code that were run succesfuly so that correctly run actions are not repeated</step>
    <step>6. Adjust the code with actions following the line of code the exception happened at so there are no overlaps later</step>
</overlap_handling>
<helpful_advice>
    <advice>If you spawn or move prefabs in a for loop be careful to start iteration from proper, higher values of coordinates so that prefabs in one area do not in prefabs in another area</advice>
    <advice>If you are rearranging existing layout, it is often better to set `raise_exception_on_overlap` to False</advice>
</helpful_advice>
"""
            raise OverlapException(message)

    def remove_prefab(self, entity_id: int, log_action=True):
        spawned_prefab = self.spawned_prefabs[entity_id]
        if log_action:
            self.actions_log.append(
                f'<action>Removing {spawned_prefab.name} (id={entity_id}, semantic_info="{spawned_prefab.semantic_info}") from {spawned_prefab.translation}</action>'
            )
        azlmbr.editor.ToolsApplicationRequestBus(
            azlmbr.bus.Broadcast,
            "DeleteEntityAndAllDescendants",
            spawned_prefab.entity_id.azlmbr_entity_id,
        )

        for prefab in self.spawned_prefabs.values():
            if spawned_prefab.entity_id in prefab.overlapping_entity_ids:
                prefab.overlapping_entity_ids.remove(spawned_prefab.entity_id)

        del self.spawned_prefabs[entity_id]

    def get_prefab_at(self, translation: list[float]) -> SpawnedPrefab | None:
        """
        Get prefab at given translation based on spawned prefabs
        """
        for prefab in self.spawned_prefabs.values():
            if (
                translation[0] >= prefab.translation[0]
                and translation[0] < prefab.translation[0] + prefab.size[0]
                and translation[1] >= prefab.translation[1]
                and translation[1] < prefab.translation[1] + prefab.size[1]
                and translation[2] >= prefab.translation[2]
                and translation[2] < prefab.translation[2] + prefab.size[2]
            ):
                return prefab
        return None

    def can_prefab_be_positioned_at(self, prefab_name: str, translation: list[float]) -> bool:
        available_prefab = self.available_prefabs[prefab_name]
        if not available_prefab:
            return False
        # tmp prefab
        sp = SpawnedPrefab(
            entity_id=azlmbr.entity.EntityId(),
            translation=translation,
            available_prefab=available_prefab,
        )
        sp.refresh_overlapping_entity_ids_only_for_this_prefab(self.spawned_prefabs)
        if sp.overlapping_entity_ids:
            return False
        return True

    def find_positions_along_x_and_along_y(
        self, prefab: SpawnedPrefab, area: list[float], step: float = 0.1
    ) -> list[list[float]]:
        """
        Find all possible positions along translation.x and translation.y (floats) in given area for given prefab
        """
        x_min, x_max, y_min, y_max = area
        available_positions = []
        for x in np.arange(x_min, x_max, step):
            if self.can_prefab_be_positioned_at(
                prefab.name, [x, prefab.translation[1], prefab.translation[2]]
            ):
                available_positions.append(
                    [round(x, 2), round(prefab.translation[1], 2), prefab.translation[2]]
                )
        for y in np.arange(y_min, y_max, step):
            if self.can_prefab_be_positioned_at(
                prefab.name, [prefab.translation[0], y, prefab.translation[2]]
            ):
                available_positions.append(
                    [round(prefab.translation[0], 2), round(y, 2), prefab.translation[2]]
                )
        return available_positions

    def positions_to_xml(self, positions: list[list[float]], root_name: str) -> str:
        """
        converts list of positions to xml in a form
        <root_name>
            <pos>[l[0], l[1], l[2]]<pos>
            ...
        </root_name>
        """
        root = ET.Element(root_name)
        for p in positions:
            pos_el = ET.SubElement(root, "pos")
            pos_el.text = str(p)
        xml_string = parseString(ET.tostring(root)).toprettyxml(indent="  ")
        xml_string = "\n".join(xml_string.split("\n")[1:])
        return xml_string

    def add_dirty_entities(self, spawned_prefabs: list[SpawnedPrefab] | None = None):
        if not spawned_prefabs:
            spawned_prefabs = self.spawned_prefabs.values()
        for sp in spawned_prefabs:
            azlmbr.editor.ToolsApplicationRequestBus(
                azlmbr.bus.Broadcast, "AddDirtyEntity", sp.entity_id.azlmbr_entity_id
            )
