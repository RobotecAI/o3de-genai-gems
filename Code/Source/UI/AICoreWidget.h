/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include "UI/Generator/NewGeneratorSegment.h"
#include "UI/Requester/NewRequesterSegment.h"
#include "UI/Tab.h"
#include <qwindowdefs.h>

#include <AzQtComponents/Components/Widgets/TabWidget.h>
#include <QWidget>
#include <qboxlayout.h>
#endif

namespace AICore
{
    class AICoreWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit AICoreWidget(QWidget* parent = nullptr);

    private:
        AzQtComponents::TabWidget* m_tabs;
        Tab* m_generatorsTab;
        NewGeneratorSegment* m_newGeneratorSegment;
        Tab* m_requestersTab;
        NewRequesterSegment* m_newRequesterSegment;
    };
} // namespace AICore
