/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/TemplateInfo.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
namespace GenAIFramework
{
    class AIServiceProviderRequests : public AZ::ComponentBus
    {
    public:
        AZ_RTTI(AIServiceProviderRequests, "{0bcac4e2-09a9-4702-98dd-ef37006664a7}", AZ::ComponentBus);

        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        //! Send a request to a specified endpoint
        //! @param request The request to send
        //! @param callback The callback to be called when the request is complete. The callback will be called with the request and the
        //! result of the request. If the request was successful, the result will be a string. If the request failed, the error string will
        //! be returned.
        virtual void SendRequest(const ModelAPIRequest& request, AZStd::function<void(ModelAPIResponse)> callback) = 0;
    };

    using AIServiceProviderBus = AZ::EBus<AIServiceProviderRequests>;
} // namespace GenAIFramework
