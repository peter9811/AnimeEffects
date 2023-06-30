#ifndef GUI_INFOLABELWIDGET_H
#define GUI_INFOLABELWIDGET_H

#include "core/Animator.h"
#include "core/Project.h"
#include "core/TimeFormat.h"
#include "core/TimeInfo.h"
#include "gui/GUIResources.h"
#include "gui/ViaPoint.h"
#include "util/Range.h"
#include <QLabel>
#include <QSettings>

namespace gui {

class TimeLineInfoWidget: public QLabel {
public:
    TimeLineInfoWidget(GUIResources& aResources, QWidget* aParent);
    void setProject(core::Project* aProject);

    void onUpdate();

private:
    GUIResources& mResources;

    core::Project* mProject;

    QSettings mSettings;
    bool mIsFirstTime;
    int mSuspendCount;
};

} // namespace gui

#endif // GUI_INFOLABELWIDGET_H
