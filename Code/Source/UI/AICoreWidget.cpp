/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreWidget.h"
#include "UI/Generator/NewGeneratorSegment.h"
#include "UI/Requester/NewRequesterSegment.h"
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
            AZStd::vector<AZ::Component*> activeGenerators = interface->GetActiveGenerators();
            m_newGeneratorSegment = new NewGeneratorSegment();
            m_generatorsTab = new Tab(activeGenerators, m_newGeneratorSegment, "Generator", m_tabs);
            m_tabs->addTab(m_generatorsTab, "Generators");

            AZStd::vector<AZ::Component*> activeRequesters = interface->GetActiveRequesters();
            m_newRequesterSegment = new NewRequesterSegment();
            m_requestersTab = new Tab(activeRequesters, m_newRequesterSegment, "Requester", m_tabs);
            m_tabs->addTab(m_requestersTab, "Requesters");
        }

        verticalLayout->addWidget(m_tabs);
    }
} // namespace AICore
