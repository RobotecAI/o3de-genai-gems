/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "GenAIFrameworkTest.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/SerializeContext_fwd.h>

namespace GenAIFramework
{
    class GenAIFrameworkTestComponent : public AZ::Component
    {
    private:
        GenAIFrameworkActionTest m_genAIFrameworkActionTest;

    public:
        AZ_COMPONENT(GenAIFrameworkTestComponent, "{B7EC169D-55AB-458D-AB89-1714C79B3803}", AZ::Component);
        GenAIFrameworkTestComponent(GenAIFrameworkActionTest GenAIFrameworkTest);
        GenAIFrameworkTestComponent() = default;
        static void Reflect(AZ::ReflectContext* context);

        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GenAIFramework
