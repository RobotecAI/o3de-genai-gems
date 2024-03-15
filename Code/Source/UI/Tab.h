/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include "NewSegment.h"
#include "Segment.h"
#include <AICoreSettingsRegistryManager/AICoreSettingsRegistryManager.h>
#include <AzQtComponents/Components/Widgets/SegmentControl.h>
#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <QWidget>
#include <QtCore/qstringliteral.h>
#include <UI/PropertyEditor/ReflectedPropertyEditor.hxx>
#include <qobjectdefs.h>

#endif

namespace AICore
{
    class Tab : public QWidget
    {
        Q_OBJECT
    public:
        explicit Tab(
            AZStd::vector<AZ::Component*> existingComponents, NewSegment* newSegment, AZStd::string name, QWidget* parent = nullptr);

    public slots:
        void AddNewComponent(AZ::Component* component, AZ::Uuid componentTypeId);
        void RemoveSegment(Segment* segment);

    private:
        AzQtComponents::SegmentControl* m_segmentControl;
        NewSegment* m_newSegment;
        AZStd::vector<Segment*> m_segments;
    };
} // namespace AICore
