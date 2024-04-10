/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIServiceRequesterBus.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/IO/FileIO.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace GenAIMock
{
    class MockServiceComponentConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(MockServiceComponentConfiguration, "{47ab736f-33f1-453b-9cdb-96faffd5d33e}");
        AZ_CLASS_ALLOCATOR(MockServiceComponentConfiguration, AZ::SystemAllocator);

        MockServiceComponentConfiguration() = default;
        ~MockServiceComponentConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);
        AZ::IO::Path GetAssetPath() const;

    private:
        AZ::IO::Path m_assetPath; //!< Path of the JSON asset
    };

    class MockServiceComponent
        : public AZ::Component
        , public GenAIFramework::AIServiceRequesterBus::Handler
    {
    public:
        AZ_COMPONENT(MockServiceComponent, "{db01be29-ec0b-41e1-bf68-12d70dd6b630}");

        MockServiceComponent() = default;
        explicit MockServiceComponent(const MockServiceComponentConfiguration& config);

        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Controller component
        void Init();
        void Activate();
        void Deactivate();
        void SetConfiguration(const MockServiceComponentConfiguration& config);
        const MockServiceComponentConfiguration& GetConfiguration() const;
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // AIServiceRequesterBus overrides
        void SendRequest(const AZStd::string& request, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback);
        //////////////////////////////////////////////////////////////////////////

    private:
        MockServiceComponentConfiguration m_configuration;
        AZStd::vector<AZStd::string> m_testData;
        int m_lastCompleted{ -1 }; //!< Interator (number) of the last call that was completed (it can be reset)

        void ReloadAsset();
    };
} // namespace GenAIMock
