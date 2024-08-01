/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>
#include <AzCore/Math/Uuid.h>

namespace GenAIFramework
{
    //! Request bus enabling communication between UI and GenAIFramework interface. It is used to store the configuration of the
    //! GenAIFramework in a registry file.
    class GenAIFrameworkEditorRequests
    {
    public:
        AZ_RTTI(GenAIFrameworkEditorRequests, GenAIFrameworkEditorRequestsTypeId);
        virtual ~GenAIFrameworkEditorRequests() = default;

        virtual void SaveSystemConfiguration() = 0;
    };

    class GenAIFrameworkEditorBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GenAIFrameworkEditorRequestBus = AZ::EBus<GenAIFrameworkEditorRequests, GenAIFrameworkEditorBusTraits>;
    using GenAIFrameworkEditorInterface = AZ::Interface<GenAIFrameworkEditorRequests>;

} // namespace GenAIFramework
