"""
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
"""

from typing import Any
from utils import prefab_dict_to_xml
import azlmbr


def get_overlapping_prefabs_pairs(
    prefabs: set["SpawnedPrefab"],
) -> list[tuple["SpawnedPrefab", "SpawnedPrefab"]]:
    """Gets pairs of overlapping prefabs. Ignores prefabs with different z coord."""
    prefabs = list(prefabs)
    overlapping_prefabs_pairs = []
    for i, prefab_1 in enumerate(prefabs):
        if i < len(prefabs) - 1:
            prefabs_to_check = prefabs[i + 1 :]
            overlapping_prefabs = prefab_1.calculate_overlapping_prefabs(prefabs_to_check)
            for prefab_2 in overlapping_prefabs:
                overlapping_prefabs_pairs.append((prefab_1, prefab_2))
    return overlapping_prefabs_pairs


class EditorEntityId:
    def __init__(self, azlmbr_entity_id: azlmbr.entity.EntityId) -> None:
        self.azlmbr_entity_id: azlmbr.entity.EntityId = azlmbr_entity_id

    def __int__(self):
        return int(self.azlmbr_entity_id.to_json()["id"])

    def __repr__(self) -> str:
        return str(int(self))

    def __hash__(self) -> int:
        return hash(int(self))

    def __eq__(self, other) -> bool:
        return int(self) == int(other)


class AvailablePrefab:
    def __init__(
        self,
        name: str,
        path: str,
        description: str,
        size: list[float],
        outer_margin: list[float] = None,
        anchor_transform: dict[str, list[float]] = None,
        excluded_from_overlap_check: bool = False,
        can_be_put_on: list[str] = None,
        shelves_zs: list[float] = None,
        accesibility_requirements: str = None,
    ) -> None:
        self.name = name
        self.path = path
        self.description = description
        self.size = size
        self.outer_margin = outer_margin
        self.shelves_zs = shelves_zs
        self.can_be_put_on = can_be_put_on
        self.anchor_transform = anchor_transform or {
            "translation": [0, 0, 0],
            "rotation": [0, 0, 0],
        }
        self.accesibility_requirements = accesibility_requirements
        self.excluded_from_overlap_check = excluded_from_overlap_check

    @property
    def xml(self) -> str:
        return prefab_dict_to_xml(self.to_dict())

    def to_dict(self) -> dict[str, Any]:
        d = {}
        l = [
            "name",
            "path",
            "description",
            "size",
            "outer_margin",
            "shelves_zs",
            "can_be_put_on",
            "accesibility_requirements",
        ]
        for attr in l:
            if getattr(self, attr):
                d[attr] = getattr(self, attr)
        return d

    @classmethod
    def from_dict(cls, prefab_dict: dict[str, Any]) -> "AvailablePrefab":
        name = prefab_dict.get("name")
        size = prefab_dict.get("size")
        path = prefab_dict.get("path")
        description = prefab_dict.get("description")
        outer_margin = prefab_dict.get("outer_margin")
        shelves_zs = prefab_dict.get("shelves_zs", None)
        can_be_put_on = prefab_dict.get("can_be_put_on", None)
        anchor_transform = prefab_dict.get("anchor_transform", None)
        accesibility_requirements = prefab_dict.get("accesibility_requirements", None)
        excluded_from_overlap_check = prefab_dict.get("excluded_from_overlap_check", False)

        return AvailablePrefab(
            name=name,
            path=path,
            description=description,
            size=size,
            outer_margin=outer_margin,
            shelves_zs=shelves_zs,
            can_be_put_on=can_be_put_on,
            anchor_transform=anchor_transform,
            accesibility_requirements=accesibility_requirements,
            excluded_from_overlap_check=excluded_from_overlap_check,
        )

    def __repr__(self) -> str:
        return self.xml

    def __hash__(self) -> int:
        return hash(self.name)

    def __eq__(self, other) -> bool:
        return self.name == other.name


class SpawnedPrefab:
    def __init__(
        self,
        entity_id: EditorEntityId,
        translation: list[float],
        available_prefab: AvailablePrefab,
        overlapping_entity_ids: set[int] = None,
        semantic_info: str = "",
    ) -> None:
        self.entity_id = entity_id
        self.translation = translation
        self.overlapping_entity_ids = overlapping_entity_ids or set()
        self.semantic_info = semantic_info
        self.available_prefab = available_prefab

    @property
    def name(self) -> str:
        return self.available_prefab.name

    @property
    def size(self) -> list[float]:
        return self.available_prefab.size

    @property
    def outer_margin(self) -> list[float]:
        return self.available_prefab.outer_margin

    @property
    def path(self) -> str:
        return self.available_prefab.path

    @property
    def description(self) -> str:
        return self.available_prefab.description

    @property
    def anchor_transform(self) -> dict[str, list[float]]:
        return self.available_prefab.anchor_transform

    @property
    def excluded_from_overlap_check(self) -> bool:
        return self.available_prefab.excluded_from_overlap_check

    @property
    def can_be_put_on(self) -> list[str]:
        return self.available_prefab.can_be_put_on

    # ----------------- Warehouse specific attributes -----------------
    # NOTE: These attributes should be created dynamically as they are specific to warehouses
    @property
    def shelves_zs(self) -> list[float]:
        return self.available_prefab.shelves_zs

    @property
    def accesibility_requirements(self) -> str:
        return self.available_prefab.accesibility_requirements

    # ----------------- Warehouse specific attributes -----------------

    def __repr__(self) -> str:
        return self.xml

    def __hash__(self) -> int:
        return hash(self.entity_id)

    def __eq__(self, other) -> bool:
        return self.entity_id == other.entity_id

    def to_dict(self) -> dict[str, Any]:
        d = {}
        l = [
            "entity_id",
            "name",
            "translation",
            "size",
            "semantic_info",
            "overlapping_entity_ids",
        ]
        for attr_name in l:
            if isinstance(attr_name, property):
                d[attr_name] = attr_name.fget(self)
            else:
                attr = getattr(self, attr_name)
                if attr:
                    d[attr_name] = attr if attr_name != "entity_id" else int(attr)
        return d

    @property
    def xml(self) -> str:
        return prefab_dict_to_xml(self.to_dict())

    @property
    def bounds(self) -> list[float]:
        x_min = self.translation[0]
        x_max = self.translation[0] + self.size[0]
        y_min = self.translation[1]
        y_max = self.translation[1] + self.size[1]
        return [x_min, x_max, y_min, y_max]

    def intersects(self, other_prefab: "SpawnedPrefab") -> bool:
        bounds1 = self.bounds
        bounds2 = other_prefab.bounds
        x_overlap = max(0, min(bounds1[1], bounds2[1]) - max(bounds1[0], bounds2[0]))
        y_overlap = max(0, min(bounds1[3], bounds2[3]) - max(bounds1[2], bounds2[2]))
        return x_overlap > 0 and y_overlap > 0

    def refresh_overlapping_entity_ids_only_for_this_prefab(
        self, prefabs_to_check: dict[int, "SpawnedPrefab"]
    ):
        prefabs_to_check = {v for k, v in prefabs_to_check.items() if k != self.entity_id}
        overlapping_prefabs = self.calculate_overlapping_prefabs(prefabs_to_check)
        self.overlapping_entity_ids = {int(p.entity_id) for p in overlapping_prefabs}

    def calculate_overlapping_prefabs(
        self, prefabs_to_check: set["SpawnedPrefab"]
    ) -> set["SpawnedPrefab"]:
        """Gets overlaps only for prefabs with the same z. Based on simple bounds overlap check"""
        if self.excluded_from_overlap_check:
            return set()
        prefabs_to_check = {p for p in prefabs_to_check if not p.excluded_from_overlap_check}

        # remove prefabs that have different z
        prefabs_to_check = {
            p for p in prefabs_to_check if p.translation[2] == self.translation[2]
        }
        overlapping_prefabs = set()
        for p in prefabs_to_check:
            if self.intersects(p):
                overlapping_prefabs.add(p)
        return overlapping_prefabs
