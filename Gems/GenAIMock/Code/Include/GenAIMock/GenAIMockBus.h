/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIMock/GenAIMockTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GenAIMock
{
    class GenAIMockRequests
    {
    public:
        AZ_RTTI(GenAIMockRequests, GenAIMockRequestsTypeId);
        virtual ~GenAIMockRequests() = default;
    };

    class GenAIMockBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIMockRequestBus = AZ::EBus<GenAIMockRequests, GenAIMockBusTraits>;
    using GenAIMockInterface = AZ::Interface<GenAIMockRequests>;

} // namespace GenAIMock
