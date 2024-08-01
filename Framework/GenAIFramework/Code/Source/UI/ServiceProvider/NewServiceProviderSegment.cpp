/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "NewServiceProviderSegment.h"
#include <GenAIFramework/GenAIFrameworkBus.h>

namespace GenAIFramework
{
    NewServiceProviderSegment::NewServiceProviderSegment(QWidget* parent)
        : NewSegment(GetRegisteredNameAndComponentTypeId(), parent)
    {
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> NewServiceProviderSegment::GetRegisteredNameAndComponentTypeId()
    {
        auto interface = GenAIFrameworkInterface::Get();
        if (!interface)
        {
            return {};
        }
        return interface->GetServiceProviderNamesAndComponentTypeIds();
    }

    AZ::Component* NewServiceProviderSegment::CreateNewComponentEntity(const AZStd::string& name, AZ::Uuid selectedComponentTypeId)
    {
        auto interface = GenAIFrameworkInterface::Get();
        if (!interface)
        {
            return nullptr;
        }
        return interface->CreateServiceProvider(name, selectedComponentTypeId);
    }

} // namespace GenAIFramework
