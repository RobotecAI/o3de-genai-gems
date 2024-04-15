/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)

#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Math/Uuid.h>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <UI/PropertyEditor/ReflectedPropertyEditor.hxx>

#endif

namespace GenAIFramework
{
    class Segment
        : public QWidget
        , public AzToolsFramework::IPropertyEditorNotify
    {
        Q_OBJECT
    public:
        explicit Segment(AZ::Component* component, AZ::Uuid componentTypeId, QWidget* parent = nullptr);
        virtual ~Segment() = default;

        void BeforePropertyModified(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void AfterPropertyModified(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void SetPropertyEditingActive(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void SetPropertyEditingComplete(AzToolsFramework::InstanceDataNode* /*node*/) override;
        void SealUndoStack() override;

    signals:
        void RemoveSegment(Segment* segment);

    protected:
        void SaveConfiguration();

        void onRemoveButtonPressed();

        QVBoxLayout* m_verticalLayout;
        AzToolsFramework::ReflectedPropertyEditor* m_propertyEditor;
        AZ::Component* m_component;
        QPushButton* m_removeButton;
        QLabel* m_componentName;
    };

} // namespace GenAIFramework
