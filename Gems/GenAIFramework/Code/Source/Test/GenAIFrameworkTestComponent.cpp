/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkTestComponent.h"
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    GenAIFrameworkTestComponent::GenAIFrameworkTestComponent(GenAIFrameworkActionTest GenAIFrameworkTest)
        : m_genAIFrameworkActionTest(GenAIFrameworkTest)
    {
    }

    void GenAIFrameworkTestComponent::Activate()
    {
        AZ_Printf("GenAIFrameworkTestComponent", "Activate\n");
    }

    void GenAIFrameworkTestComponent::Deactivate()
    {
        AZ_Printf("GenAIFrameworkTestComponent", "Deactivate\n");
    }

    void GenAIFrameworkTestComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIFrameworkTestComponent, AZ::Component>()->Version(0);
        };
    }
} // namespace GenAIFramework
