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
    void AICoreSystemComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreSystemComponentConfiguration>()
                ->Version(1)
                ->Field("ServiceRequesters", &AICoreSystemComponentConfiguration::m_serviceRequesters)
                ->Field("ModelConfigurations", &AICoreSystemComponentConfiguration::m_modelConfigurations);
        }
    }
} // namespace AICore
