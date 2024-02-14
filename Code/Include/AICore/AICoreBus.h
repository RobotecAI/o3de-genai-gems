/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#pragma once

#include <AICore/AICoreTypeIds.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace AICore
{
    class AICoreRequests
    {
    public:
        AZ_RTTI(AICoreRequests, AICoreRequestsTypeId);
        virtual ~AICoreRequests() = default;
        // Put your public methods here
    };

    class AICoreBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using AICoreRequestBus = AZ::EBus<AICoreRequests, AICoreBusTraits>;
    using AICoreInterface = AZ::Interface<AICoreRequests>;
} // namespace AICore
