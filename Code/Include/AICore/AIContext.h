/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#pragma once

#include <AzCore/Component/EntityId.h>

namespace AICore
{
    struct AIContext
    {   // Could be vendor-dependent. Expected to capture persistent, possibly multi-instance context.
        AZ::EntityId m_contextEntityId; // May be invalid if in global context
    };
} // namespace AICore
