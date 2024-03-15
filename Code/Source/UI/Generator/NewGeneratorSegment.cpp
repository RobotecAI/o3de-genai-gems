/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "NewGeneratorSegment.h"
#include <AICore/AICoreBus.h>

namespace AICore
{
    NewGeneratorSegment::NewGeneratorSegment(QWidget* parent)
        : NewSegment(GetRegisteredNameAndComponentTypeId(), parent)
    {
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> NewGeneratorSegment::GetRegisteredNameAndComponentTypeId()
    {
        auto interface = AICoreInterface::Get();
        if (!interface)
        {
            return {};
        }
        return interface->GetRegisteredGeneratorsNameAndComponentTypeId();
    }

    AZ::Component* NewGeneratorSegment::CreateNewComponentEntity(const AZStd::string& name, const AZ::Uuid& selectedComponentTypeId)
    {
        auto interface = AICoreInterface::Get();
        if (!interface)
        {
            return nullptr;
        }
        return interface->CreateNewGenerator(name, selectedComponentTypeId);
    }

} // namespace AICore
