/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "NewSegment.h"
#include <AICore/AICoreBus.h>
#include <AzCore/Math/Uuid.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    NewSegment::NewSegment(AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> registeredNameAndComponentTypeId, QWidget* parent)
        : QWidget(parent)
    {
        QVBoxLayout* verticalLayout = new QVBoxLayout(this);
        verticalLayout->setContentsMargins(15, 15, 15, 15);
        verticalLayout->setSpacing(10);

        m_componentSelection = new QComboBox();
        m_componentSelection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        for (auto& nameAndComponentTypeId : registeredNameAndComponentTypeId)
        {
            m_componentSelection->addItem(
                nameAndComponentTypeId.first.c_str(), QVariant::fromValue<AZ::Uuid>(nameAndComponentTypeId.second));
        }

        m_createButton = new QPushButton("Create new");
        m_createButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        connect(m_createButton, &QPushButton::clicked, this, &NewSegment::onCreateButtonPressed);

        m_nameInput = new QLineEdit();
        m_nameInput->setPlaceholderText("Name");
        m_nameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        verticalLayout->addWidget(m_componentSelection);
        verticalLayout->addWidget(m_nameInput);
        verticalLayout->addWidget(m_createButton);

        verticalLayout->addStretch();
    }

    void NewSegment::onCreateButtonPressed()
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            AZ::Uuid selectedComponentTypeId = m_componentSelection->currentData().value<AZ::Uuid>();
            AZStd::string name = m_nameInput->text().toUtf8().constData();
            AZ::Component* component = CreateNewComponentEntity(name, selectedComponentTypeId);
            emit NewComponentAdded(component, selectedComponentTypeId);
        }
    }

} // namespace AICore
