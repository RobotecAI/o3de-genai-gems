/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "NewRequesterSegment.h"
#include <AICore/AICoreBus.h>

namespace AICore
{
    NewRequesterSegment::NewRequesterSegment(QWidget* parent)
        : NewSegment(GetRegisteredNameAndComponentTypeId(), parent)
    {
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> NewRequesterSegment::GetRegisteredNameAndComponentTypeId()
    {
        auto interface = AICoreInterface::Get();
        if (!interface)
        {
            return {};
        }
        return interface->GetRegisteredRequestersNameAndComponentTypeId();
    }

    AZ::Component* NewRequesterSegment::CreateNewComponentEntity(const AZStd::string& name, const AZ::Uuid& selectedComponentTypeId)
    {
        auto interface = AICoreInterface::Get();
        if (!interface)
        {
            return nullptr;
        }
        return interface->CreateNewRequester(name, selectedComponentTypeId);
    }

} // namespace AICore
