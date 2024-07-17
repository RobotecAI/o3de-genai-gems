/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/TemplateInfo.h>

namespace GenAIFramework
{
    //! EBus interface for non-blocking requests to LLM models.
    class AsyncRequests
    {
    public:
        AZ_RTTI(AsyncRequests, AsyncRequestsTypeId);

        //! Set the selected model configuration and service provider with entity ids.
        //! @param modelConfigurationId Entity id of the target model configuration.
        //! @param serviceProviderId Entity id of the target service provider.
        virtual void SetProviderAndModel(AZ::EntityId modelConfigurationId, AZ::EntityId serviceProviderId) = 0;
    };

    class RequestsUtilsBusTraits : public AZ::EBusTraits
    {
    public:
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
    };

    using AsyncRequestBus = AZ::EBus<AsyncRequests, RequestsUtilsBusTraits>;
    using AsyncRequestBusInterface = AZ::Interface<AsyncRequests>;

} // namespace GenAIFramework
