/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreSystemComponentConfiguration.h"
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    int AICoreSystemComponentConfiguration::m_instanceCount = 0;

    void AICoreSystemComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreSystemComponentConfiguration>()
                ->Version(1)
                ->Field("Requesters", &AICoreSystemComponentConfiguration::m_requesters)
                ->Field("Generators", &AICoreSystemComponentConfiguration::m_generators);
        }
    }

    AICoreSystemComponentConfiguration::AICoreSystemComponentConfiguration()
    {
        AICoreSystemComponentConfiguration::m_instanceCount++;
    }

    AICoreSystemComponentConfiguration::~AICoreSystemComponentConfiguration()
    {
        if (AICoreSystemComponentConfiguration::m_instanceCount == 1)
        {
            for (auto* entity : m_requesters)
            {
                if (entity)
                {
                    delete entity;
                }
            }
            m_requesters.clear();
            for (auto* entity : m_generators)
            {
                if (entity)
                {
                    delete entity;
                }
            }
            m_generators.clear();
        }
    }
} // namespace AICore
