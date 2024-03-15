/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "Tab.h"
#include "UI/NewSegment.h"
#include <AzCore/std/algorithm.h>
#include <qwidget.h>

namespace AICore
{
    Tab::Tab(AZStd::vector<AZ::Component*> existingComponents, NewSegment* newSegment, AZStd::string name, QWidget* parent)
        : QWidget(parent)
    {
        QVBoxLayout* verticalLayout = new QVBoxLayout(this);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setSpacing(0);

        m_segmentControl = new AzQtComponents::SegmentControl(this);
        m_segmentControl->setTabPosition(AzQtComponents::SegmentControl::West);
        m_segmentControl->setTabOrientation(Qt::Vertical);

        m_newSegment = newSegment;
        m_newSegment->setParent(m_segmentControl);
        m_newSegment->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        connect(m_newSegment, &NewSegment::NewComponentAdded, this, &Tab::AddNewComponent);
        auto segmentName = AZStd::string::format("Create new %s", name.c_str());
        m_segmentControl->addTab(m_newSegment, QString(segmentName.c_str()));

        for (AZ::Component* component : existingComponents)
        {
            AZ::Uuid componentTypeId = component->RTTI_GetType();
            AddNewComponent(component, componentTypeId);
        }

        verticalLayout->addWidget(m_segmentControl);
    }

    void Tab::AddNewComponent(AZ::Component* component, AZ::Uuid componentTypeId)
    {
        Segment* segment = new Segment(component, componentTypeId, m_segmentControl);
        m_segments.push_back(segment);
        AZStd::string name = component->GetNamedEntityId().GetName();
        m_segmentControl->addTab(segment, QString(name.c_str()));
        connect(segment, &Segment::RemoveSegment, this, &Tab::RemoveSegment);
    }

    void Tab::RemoveSegment(Segment* segment)
    {
        disconnect(segment, &Segment::RemoveSegment, this, &Tab::RemoveSegment);
        m_segmentControl->removeTab(m_segmentControl->indexOf(segment));
        m_segments.erase(AZStd::find(m_segments.begin(), m_segments.end(), segment));
        delete segment;
    }
} // namespace AICore
