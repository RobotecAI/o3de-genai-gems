/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/ReflectContext.h>

#include <GenAIFramework/Communication/AIModelRequestBus.h>

namespace GenAIMock
{
    class MockPromptGeneratorComponent
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MockPromptGeneratorComponent, "{b873470c-0ca8-49af-82b6-4de26c242c7b}");

        static void Reflect(AZ::ReflectContext* context);

        MockPromptGeneratorComponent() = default;
        ~MockPromptGeneratorComponent() = default;

        void Activate() override;
        void Deactivate() override;

        //////////////////////////////////////////////////////////////////////////
        // AIModelRequestBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const AZStd::string& prompt) override;
        GenAIFramework::ModelAPIResponse ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;

    private:
        unsigned int m_lastCompleted{ 0 }; //!< Interator (number) of the last call that was completed (it can be reset)
    };
} // namespace GenAIMock
