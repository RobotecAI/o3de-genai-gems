/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/AIComponentBase/AIComponentBase.h>

namespace GenAIFramework
{
    class PromptComponent : public AIComponentBase
    {
    public:
        AZ_COMPONENT(PromptComponent, "{649a1139-ed22-4fe3-a81d-76c77faeea4e}", AIComponentBase);

        PromptComponent() = default;
        ~PromptComponent() = default;

        static void Reflect(AZ::ReflectContext* context);

        void Activate() override;
        void Deactivate() override;

        AZ::Crc32 PromptInput();

        AZStd::string m_modelOutput;
        AZStd::string m_modelInput;
    };
} // namespace GenAIFramework