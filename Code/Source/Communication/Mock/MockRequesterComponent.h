/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIServiceRequesterBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/string/string.h>

namespace GenAIFramework
{
    class MockRequesterComponentConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(MockRequesterComponentConfiguration, "{47ab736f-33f1-453b-9cdb-96faffd5d33e}");
        AZ_CLASS_ALLOCATOR(MockRequesterComponentConfiguration, AZ::SystemAllocator);

        MockRequesterComponentConfiguration() = default;
        ~MockRequesterComponentConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::string m_modelId = "super_mock_v1";
    };

    class MockRequesterComponentComponent
        : public AZ::Component
        , public AIServiceRequesterBus::Handler
    {
    public:
        AZ_COMPONENT(MockRequesterComponentComponent, "{db01be29-ec0b-41e1-bf68-12d70dd6b630}");

        MockRequesterComponentComponent() = default;
        explicit MockRequesterComponentComponent(const MockRequesterComponentConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Controller component
        void Init();
        void Activate();
        void Deactivate();
        void SetConfiguration(const MockRequesterComponentConfiguration& config);
        const MockRequesterComponentConfiguration& GetConfiguration() const;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AIServiceRequesterBus overrides
        void SendRequest(const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback);
        //////////////////////////////////////////////////////////////////////////

    private:
        MockRequesterComponentConfiguration m_configuration;
    };
} // namespace GenAIFramework