/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <UI/AgentConfigurationWidget/Tab.h>
#include <UI/ModelConfiguration/NewModelConfigurationSegment.h>
#include <UI/ServiceProvider/NewServiceProviderSegment.h>

#include <AzQtComponents/Components/Widgets/TabWidget.h>
#include <QWidget>
#include <qboxlayout.h>
#include <qwindowdefs.h>
#endif

namespace GenAIFramework
{
    class GenAIFrameworkWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit GenAIFrameworkWidget(QWidget* parent = nullptr);

    private:
        AzQtComponents::TabWidget* m_tabs;
        Tab* m_modelsConfigurationTab;
        NewModelConfigurationSegment* m_newModelConfigurationSegment;
        Tab* m_serviceProvidersTab;
        NewServiceProviderSegment* m_newServiceProviderSegment;
    };
} // namespace GenAIFramework
