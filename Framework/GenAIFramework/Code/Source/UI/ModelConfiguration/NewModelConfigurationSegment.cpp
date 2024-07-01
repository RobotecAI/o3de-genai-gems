/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "NewModelConfigurationSegment.h"
#include <GenAIFramework/GenAIFrameworkBus.h>

namespace GenAIFramework
{
    NewModelConfigurationSegment::NewModelConfigurationSegment(QWidget* parent)
        : NewSegment(GetRegisteredNameAndComponentTypeId(), parent)
    {
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> NewModelConfigurationSegment::GetRegisteredNameAndComponentTypeId()
    {
        auto interface = GenAIFrameworkInterface::Get();
        if (!interface)
        {
            return {};
        }
        return interface->GetModelConfigurationNamesAndComponentTypeIds();
    }

    AZ::Component* NewModelConfigurationSegment::CreateNewComponentEntity(const AZStd::string& name, AZ::Uuid selectedComponentTypeId)
    {
        auto interface = GenAIFrameworkInterface::Get();
        if (!interface)
        {
            return nullptr;
        }
        return interface->CreateModelConfiguration(name, selectedComponentTypeId);
    }

} // namespace GenAIFramework
