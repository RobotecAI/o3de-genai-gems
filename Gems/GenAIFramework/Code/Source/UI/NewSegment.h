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
#include <AzCore/Math/Uuid.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <QBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaType>
#include <QPushButton>
#include <QWidget>
#endif

Q_DECLARE_METATYPE(AZ::Uuid)

namespace GenAIFramework
{
    class NewSegment : public QWidget
    {
        Q_OBJECT
    public:
        explicit NewSegment(
            AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> registeredNameAndComponentTypeId, QWidget* parent = nullptr);
        virtual ~NewSegment() = default;

        virtual AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GetRegisteredNameAndComponentTypeId() = 0;
        virtual AZ::Component* CreateNewComponentEntity(const AZStd::string& name, AZ::Uuid selectedComponentTypeId) = 0;

    signals:
        void NewComponentAdded(AZ::Component* component, AZ::Uuid componentTypeId);

    protected:
        void onCreateButtonPressed();

        QComboBox* m_componentSelection;
        QPushButton* m_createButton;
        QLineEdit* m_nameInput;
    };

} // namespace GenAIFramework
