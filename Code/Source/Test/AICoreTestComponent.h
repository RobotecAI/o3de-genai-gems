/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AICoreTest.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    class AICoreTestComponent : public AZ::Component
    {
    private:
        AICoreActionTest m_aiCoreActionTest;

    public:
        AZ_COMPONENT(AICoreTestComponent, "{B7EC169D-55AB-458D-AB89-1714C79B3803}", AZ::Component);
        AICoreTestComponent(AICoreActionTest aiCoreTest);
        AICoreTestComponent() = default;
        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
    };
} // namespace AICore
