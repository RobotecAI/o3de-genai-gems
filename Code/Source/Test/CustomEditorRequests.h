/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Component/EntityId.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/string/string.h>

#pragma once

namespace GenAIFramework
{
    class CustomEditorRequests
    {
    public:
        AZ_RTTI(CustomEditorRequests, "{033F6E1C-3048-47A0-BF9F-8E2B94DD3C10}");
        virtual ~CustomEditorRequests() = default;

        virtual AZ::EntityId DoTheTestThing(const AZ::EntityId& entityId) = 0;
    };

    class CustomEditorBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using CustomEditorRequestBus = AZ::EBus<CustomEditorRequests, CustomEditorBusTraits>;
    using CustomEditorInterface = AZ::Interface<CustomEditorRequests>;

    class CustomEditorTest : public CustomEditorRequestBus::Handler
    {
    public:
        AZ_RTTI(CustomEditorTest, "{D53DFE28-D586-4A6C-95D4-45AF6D4D76BE}");
        void Connect();
        void Disconnect();
        static void Reflect(AZ::ReflectContext* context);
        AZ::EntityId DoTheTestThing(const AZ::EntityId& entityId) override;
    };
} // namespace GenAIFramework
