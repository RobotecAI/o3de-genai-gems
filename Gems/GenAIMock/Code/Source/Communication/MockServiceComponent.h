/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/IO/Path/Path.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

namespace GenAIMockVendor
{
    class MockServiceComponentConfiguration : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(MockServiceComponentConfiguration, MockServiceComponentConfigurationTypeId);
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
        , public GenAIFramework::AIServiceProviderBus::Handler
    {
    public:
        AZ_COMPONENT(MockServiceComponent, MockServiceComponentTypeId);

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
        // AIServiceProviderBus overrides
        void SendRequest(
            const GenAIFramework::ModelAPIRequest& request, AZStd::function<void(GenAIFramework::ModelAPIResponse)> callback) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        MockServiceComponentConfiguration m_configuration;
        AZStd::vector<AZStd::string> m_testData; //!< Buffer with loaded mock prompt responses
        int m_lastCompleted{ -1 }; //!< Interator (number) of the last call that was completed (it can be reset)

        void ReloadAsset();
    };
} // namespace GenAIMockVendor
