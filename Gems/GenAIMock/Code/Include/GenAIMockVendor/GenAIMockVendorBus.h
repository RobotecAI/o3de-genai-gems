/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GenAIMockVendor
{
    class GenAIMockVendorRequests
    {
    public:
        AZ_RTTI(GenAIMockVendorRequests, GenAIMockVendorRequestsTypeId);
        virtual ~GenAIMockVendorRequests() = default;
    };

    class GenAIMockVendorBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIMockVendorRequestBus = AZ::EBus<GenAIMockVendorRequests, GenAIMockVendorBusTraits>;
    using GenAIMockVendorInterface = AZ::Interface<GenAIMockVendorRequests>;

} // namespace GenAIMockVendor
