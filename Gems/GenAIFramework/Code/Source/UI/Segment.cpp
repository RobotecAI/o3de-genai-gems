/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "Segment.h"
#include <AzCore/Math/Uuid.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/GenAIFrameworkEditorBus.h>

namespace GenAIFramework
{
    Segment::Segment(AZ::Component* component, AZ::Uuid componentTypeId, QWidget* parent)
        : QWidget(parent)
    {
        m_component = component;

        SaveConfiguration();

        m_verticalLayout = new QVBoxLayout(this);
        m_verticalLayout->setContentsMargins(15, 15, 15, 0);
        m_verticalLayout->setSpacing(15);

        AZ::SerializeContext* m_serializeContext;
        AZ::ComponentApplicationBus::BroadcastResult(m_serializeContext, &AZ::ComponentApplicationRequests::GetSerializeContext);
        AZ_Assert(m_serializeContext, "Failed to retrieve serialize context.");

	const AZStd::string name = component->GetNamedEntityId().GetName();
        m_componentName = new QLabel(name.c_str());

        const int propertyLabelWidth = 250;
        m_propertyEditor = new AzToolsFramework::ReflectedPropertyEditor(this);
        m_propertyEditor->Setup(m_serializeContext, this, true, propertyLabelWidth);
        m_propertyEditor->show();
        m_propertyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        m_propertyEditor->SetAutoResizeLabels(true);
        m_propertyEditor->SetHideRootProperties(true);

        blockSignals(true);
        m_propertyEditor->ClearInstances();
        m_propertyEditor->AddInstance(component, componentTypeId);
        m_propertyEditor->InvalidateAll();
        blockSignals(false);

        m_removeButton = new QPushButton("Remove");
        m_removeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        connect(m_removeButton, &QPushButton::clicked, this, &Segment::onRemoveButtonPressed);

        m_verticalLayout->addWidget(m_componentName);
        m_verticalLayout->addWidget(m_propertyEditor);
        m_verticalLayout->addWidget(m_removeButton);

        m_verticalLayout->addStretch();
    }

    void Segment::BeforePropertyModified(AzToolsFramework::InstanceDataNode* /*node*/)
    {
    }
    void Segment::AfterPropertyModified(AzToolsFramework::InstanceDataNode* /*node*/)
    {
    }
    void Segment::SealUndoStack()
    {
    }
    void Segment::SetPropertyEditingActive(AzToolsFramework::InstanceDataNode* /*node*/)
    {
    }
    void Segment::SetPropertyEditingComplete(AzToolsFramework::InstanceDataNode* /*node*/)
    {
        SaveConfiguration();
    }

    void Segment::SaveConfiguration()
    {
        auto interfaceEditor = GenAIFrameworkEditorInterface::Get();
        if (interfaceEditor)
        {
            interfaceEditor->SaveSystemConfiguration();
        }
    }

    void Segment::onRemoveButtonPressed()
    {
        AZStd::string question =
            AZStd::string::format("Are you sure you want to remove %s?", m_component->GetNamedEntityId().GetName().data());

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", question.c_str(), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            auto interface = GenAIFrameworkInterface::Get();
            if (interface)
            {
                interface->RemoveComponent(m_component);
            }
            SaveConfiguration();
            emit RemoveSegment(this);
        }
    }

} // namespace GenAIFramework
