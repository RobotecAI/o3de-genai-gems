/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>

#include <QTextEdit>
#include <QWidget>

namespace GenAIFramework
{
    class DetailsWidget : public QWidget
    {
        Q_OBJECT
    public:
        DetailsWidget(const AZStd::vector<AZStd::string>& messageDetails, QWidget* parent = nullptr);

    private:
        QTextEdit* m_details;
    };
} // namespace GenAIFramework
