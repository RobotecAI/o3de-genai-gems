/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AICore/AICoreTypeIds.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>

namespace AICore
{
    class AICoreEditorRequests
    {
    public:
        AZ_RTTI(AICoreEditorRequests, "{b20ba166-2237-4f0c-afcd-fb17b63a9cf6}");
        virtual ~AICoreEditorRequests() = default;

        virtual void SaveSystemConfiguration() = 0;
    };

    class AICoreEditorBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using AICoreEditorRequestBus = AZ::EBus<AICoreEditorRequests, AICoreEditorBusTraits>;
    using AICoreEditorInterface = AZ::Interface<AICoreEditorRequests>;

} // namespace AICore
