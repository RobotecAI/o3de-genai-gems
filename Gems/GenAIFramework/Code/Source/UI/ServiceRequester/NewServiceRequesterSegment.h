/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include "UI/NewSegment.h"

#endif

namespace GenAIFramework
{
    class NewServiceRequesterSegment : public NewSegment
    {
    public:
        explicit NewServiceRequesterSegment(QWidget* parent = nullptr);
        ~NewServiceRequesterSegment() = default;

        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredNameAndComponentTypeId() override;
        AZ::Component* CreateNewComponentEntity(const AZStd::string& name, const AZ::Uuid& selectedComponentTypeId) override;
    };
} // namespace GenAIFramework