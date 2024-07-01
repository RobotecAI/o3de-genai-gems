/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIMockVendor/GenAIMockVendorTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/ReflectContext.h>

namespace GenAIMockVendor
{
    class MockModelComponent
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MockModelComponent, MockModelComponentTypeId);

        static void Reflect(AZ::ReflectContext* context);

        MockModelComponent() = default;
        ~MockModelComponent() = default;

        void Activate() override;
        void Deactivate() override;

        //////////////////////////////////////////////////////////////////////////
        // AIModelRequestBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const GenAIFramework::AIMessages& prompt) override;
        GenAIFramework::ModelAPIExtractedResponse ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;
    };
} // namespace GenAIMockVendor
