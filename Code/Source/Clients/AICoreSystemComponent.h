/*
* Copyright (c) Contributors to the Open 3D Engine Project.
* For complete copyright and license terms please see the LICENSE at the root of this distribution.
*
* SPDX-License-Identifier: Apache-2.0 OR MIT
*
*/

#pragma once

#include "AzCore/std/smart_ptr/shared_ptr.h"
#include <AICore/AICoreBus.h>
#include <AICore/Communication/Requester.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

namespace AICore
{
    class AICoreSystemComponent
        : public AZ::Component
        , protected AICoreRequestBus::Handler
        , public AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(AICoreSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        AICoreSystemComponent();
        ~AICoreSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // AICoreRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZTickBus interface implementation
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        ////////////////////////////////////////////////////////////////////////

        AZStd::vector<AZStd::shared_ptr<RequesterBase>> m_requesters;
    };

} // namespace AICore
