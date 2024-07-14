"""
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
"""

from pathlib import Path
import re


def parse_vector(vector_str) -> list[float]:
    match = re.match(r"\(x=(.*),y=(.*),z=(.*)\)", vector_str)
    if match:
        return [
            round(float(match.group(1)), 2),
            round(float(match.group(2)), 2),
            round(float(match.group(3)), 2),
        ]
    else:
        raise ValueError(f"Invalid vector string: {vector_str}")


def extract_text_from_xml_tags(text: str, tag: str = "python") -> str | None:
    start_tag = f"<{tag}>"
    end_tag = f"</{tag}>"
    start_index = text.find(start_tag)
    if start_index == -1:
        return None  # start tag not found
    start_index += len(start_tag)  # move index to end of start tag
    end_index = text.find(end_tag, start_index)
    if end_index == -1:
        return None  # end tag not found
    return text[start_index:end_index].strip()  # extract and strip leading/trailing whitespace


def setup_prompt(path: Path, prompt_parts: dict[str, str]) -> str:
    with open(path, "r") as f:
        prompt = f.read()
        if prompt_parts:
            prompt = prompt.format(**prompt_parts)
        return prompt
