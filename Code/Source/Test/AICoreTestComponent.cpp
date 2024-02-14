/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#include "AICoreTestComponent.h"
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    AICoreTestComponent::AICoreTestComponent(AICoreActionTest aiCoreTest)
        : m_aiCoreActionTest(aiCoreTest)
    {
    }

    void AICoreTestComponent::Activate()
    {
        AZ_Printf("AICoreTestComponent", "Activate\n");
        m_aiCoreActionTest.Test();
    }

    void AICoreTestComponent::Deactivate()
    {
        AZ_Printf("AICoreTestComponent", "Deactivate\n");
        m_aiCoreActionTest.Test();
    }

    void AICoreTestComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreTestComponent, AZ::Component>()->Version(0);
        };
    }
} // namespace AICore
