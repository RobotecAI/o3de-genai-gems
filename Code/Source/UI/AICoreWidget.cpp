/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreWidget.h"
#include "UI/ModelConfiguration/NewModelConfigurationSegment.h"
#include "UI/ServiceRequester/NewServiceRequesterSegment.h"
#include <AICore/AICoreBus.h>

namespace AICore
{
    AICoreWidget::AICoreWidget(QWidget* parent)
        : QWidget(parent)
    {
        QVBoxLayout* verticalLayout = new QVBoxLayout(this);
        verticalLayout->setContentsMargins(0, 5, 0, 0);
        verticalLayout->setSpacing(0);

        m_tabs = new AzQtComponents::TabWidget(this);
        m_tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        auto interface = AICoreInterface::Get();
        if (interface)
        {
            AZStd::vector<AZ::Component*> activeModelConfigurations = interface->GetActiveModelConfigurations();
            m_newModelConfigurationSegment = new NewModelConfigurationSegment();
            m_modelsConfigurationTab = new Tab(activeModelConfigurations, m_newModelConfigurationSegment, "Model configurations", m_tabs);
            m_tabs->addTab(m_modelsConfigurationTab, "Model configurations");

            AZStd::vector<AZ::Component*> activeRequesters = interface->GetActiveServiceRequesters();
            m_newServiceRequesterSegment = new NewServiceRequesterSegment();
            m_serviceRequestersTab = new Tab(activeRequesters, m_newServiceRequesterSegment, "AI service requesters", m_tabs);
            m_tabs->addTab(m_serviceRequestersTab, "AI service requesters");
        }

        verticalLayout->addWidget(m_tabs);
    }
} // namespace AICore
