/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIFramework
{
    //! Base class for all Generative AI Features; it is inherited by CppFeatureBase and PythonFeatureBase classes.
    //! It should NOT be inherited directly, but only using the specializations listed earlier.
    class FeatureBase
    {
    public:
        AZ_RTTI(FeatureBase, FeatureBaseTypeId);

        FeatureBase() = default;
        FeatureBase(AZ::u64 agentId, AZ::u64 conversationId)
            : m_agentId(agentId)
            , m_conversationId(conversationId)
        {
        }

        virtual ~FeatureBase() = default;

        static void Reflect(AZ::ReflectContext* context)
        {
            if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<FeatureBase>()->Version(0);
            }
        }

    protected:
        AZ::u64 m_agentId;
        AZ::u64 m_conversationId;
    };

} // namespace GenAIFramework
