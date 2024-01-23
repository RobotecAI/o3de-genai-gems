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

namespace AICore
{
    class AICoreSystemRegistrationContext : public AZ::ReflectContext
    {
    public:
        AICoreSystemRegistrationContext() = default;
        ~AICoreSystemRegistrationContext() = default;

        AZ_CLASS_ALLOCATOR(AICoreSystemRegistrationContext, AZ::SystemAllocator);
        AZ_RTTI(AICoreSystemRegistrationContext, "{e162937c-0177-4ced-87b3-3b037a44c394}", AZ::ReflectContext);

        template<class C>
        void RegisterAICoreRequester()
        {
            m_registeredRequesters.push_back(C::RTTI_Type());
        }

        template<class C>
        void RegisterAICoreGenerator()
        {
            m_registeredGenerators.push_back(C::RTTI_Type());
        }

        inline AZStd::vector<AZ::Uuid> GetRegisteredRequesters() const
        {
            return m_registeredRequesters;
        }

        inline AZStd::vector<AZ::Uuid> GetRegisteredGenerators() const
        {
            return m_registeredGenerators;
        }

    private:
        AZStd::vector<AZ::Uuid> m_registeredRequesters;
        AZStd::vector<AZ::Uuid> m_registeredGenerators;
    };
} // namespace AICore
