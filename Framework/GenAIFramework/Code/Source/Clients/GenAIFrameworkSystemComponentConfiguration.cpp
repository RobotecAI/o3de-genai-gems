/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkSystemComponentConfiguration.h"
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    void GenAIFrameworkSystemComponentConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIFrameworkSystemComponentConfiguration>()
                ->Version(0)
                ->Field("ServiceProviders", &GenAIFrameworkSystemComponentConfiguration::m_serviceProviders)
                ->Field("ModelConfigurations", &GenAIFrameworkSystemComponentConfiguration::m_modelConfigurations)
                ->Field("FeaturesConversations", &GenAIFrameworkSystemComponentConfiguration::m_featuresConversationsStore);
        }
    }
} // namespace GenAIFramework
