"""
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
"""

from io import StringIO
from contextlib import redirect_stdout
import sys
import traceback

exec_global_scope = {}


def run_code(code):
    exception = ""
    try:
        f = StringIO()
        with redirect_stdout(f):
            exec(code, exec_global_scope)
    except:
        _, _, exc_traceback = sys.exc_info()
        frame_summary = traceback.extract_tb(exc_traceback)[1]
        line_number = frame_summary.lineno
        line = code.split("\n")[line_number - 1]
        exception_message = traceback.format_exc()
        exception = f"<message>{exception_message}</message>"
        exception += f"\n<line_number>{line_number}</line_number>"
        exception += f"\n<line>{line}</line>"
    finally:
        output = f.getvalue()
        return f"<python_interpreter_result>\n<std_out>\n{output}</std_out>\n<exception>\n{exception}</exception>\n</python_interpreter_result>"
