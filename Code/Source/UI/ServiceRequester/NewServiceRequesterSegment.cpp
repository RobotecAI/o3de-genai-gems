/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "NewServiceRequesterSegment.h"
#include <GenAIFramework/GenAIFrameworkBus.h>

namespace GenAIFramework
{
    NewServiceRequesterSegment::NewServiceRequesterSegment(QWidget* parent)
        : NewSegment(GetRegisteredNameAndComponentTypeId(), parent)
    {
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> NewServiceRequesterSegment::GetRegisteredNameAndComponentTypeId()
    {
        auto interface = GenAIFrameworkInterface::Get();
        if (!interface)
        {
            return {};
        }
        return interface->GetRegisteredServiceRequestersNameAndComponentTypeId();
    }

    AZ::Component* NewServiceRequesterSegment::CreateNewComponentEntity(const AZStd::string& name, const AZ::Uuid& selectedComponentTypeId)
    {
        auto interface = GenAIFrameworkInterface::Get();
        if (!interface)
        {
            return nullptr;
        }
        return interface->CreateNewServiceRequester(name, selectedComponentTypeId);
    }

} // namespace GenAIFramework
