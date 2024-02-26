/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "CustomEditorRequests.h"
#include <AzCore/RTTI/BehaviorContext.h>

namespace AICore
{
    void CustomEditorTest::Reflect(AZ::ReflectContext* context)
    {
        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<CustomEditorRequestBus>("CustomEditorRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ->Attribute(AZ::Script::Attributes::Category, "AI")
                ->Attribute(AZ::Script::Attributes::Module, "test")
                ->Event("DoTheTestThing", &CustomEditorRequests::DoTheTestThing);
        }
    }

    void CustomEditorTest::Connect()
    {
        CustomEditorRequestBus::Handler::BusConnect();
    }

    void CustomEditorTest::Disconnect()
    {
        CustomEditorRequestBus::Handler::BusDisconnect();
    }

    AZ::EntityId CustomEditorTest::DoTheTestThing(const AZ::EntityId& entityId)
    {
        AZ_Printf("CustomEditorTest", "Bus event called with %s", entityId.ToString().c_str());
        return entityId;
    }
} // namespace AICore
