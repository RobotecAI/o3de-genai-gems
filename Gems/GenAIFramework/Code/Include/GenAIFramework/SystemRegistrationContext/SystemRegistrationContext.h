/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include "AzCore/Memory/Memory_fwd.h"
#include "AzCore/RTTI/RTTIMacros.h"
#include <AzCore/Math/Uuid.h>
#include <AzCore/RTTI/ReflectContext.h>
#include <AzCore/std/containers/vector.h>

#include <iostream>

namespace GenAIFramework
{
    class SystemRegistrationContext : public AZ::ReflectContext
    {
    public:
        SystemRegistrationContext() = default;
        ~SystemRegistrationContext() = default;

        AZ_CLASS_ALLOCATOR(SystemRegistrationContext, AZ::SystemAllocator);
        AZ_RTTI(SystemRegistrationContext, "{e162937c-0177-4ced-87b3-3b037a44c394}", AZ::ReflectContext);

        template<class C>
        void RegisterGenAIFrameworkServiceProvider()
        {
            m_registeredServiceProviders.push_back(C::RTTI_Type());
        }

        template<class C>
        void RegisterModelConfiguration()
        {
            m_registeredModelConfigurations.push_back(C::RTTI_Type());
        }

        inline AZStd::vector<AZ::Uuid> GetRegisteredServiceProviders() const
        {
            return m_registeredServiceProviders;
        }

        inline AZStd::vector<AZ::Uuid> GetRegisteredModelConfigurations() const
        {
            return m_registeredModelConfigurations;
        }

    private:
        AZStd::vector<AZ::Uuid> m_registeredServiceProviders;
        AZStd::vector<AZ::Uuid> m_registeredModelConfigurations;
    };
} // namespace GenAIFramework
