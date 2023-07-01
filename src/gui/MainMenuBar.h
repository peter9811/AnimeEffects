#ifndef GUI_MAINMENUBAR_H
#define GUI_MAINMENUBAR_H

#include <QMenuBar>
#include <QAction>
#include <QCheckBox>
#include <QSpinBox>
#include "core/Project.h"
#include "ctrl/VideoFormat.h"
#include "gui/EasyDialog.h"
#include "gui/GUIResources.h"
#include "qprocess.h"

namespace gui {
class MainWindow;
}
namespace gui {
class ViaPoint;
}
namespace gui {

//-------------------------------------------------------------------------------------------------
class MainMenuBar: public QMenuBar {
    Q_OBJECT
public:
    MainMenuBar(MainWindow& aMainWindow, ViaPoint& aViaPoint, GUIResources& aGUIResources, QWidget* aParent);
    void setProject(core::Project* aProject);
    void setShowResourceWindow(bool aShow);
    QStringList recentfiles;

public:
    // signals
    util::Signaler<void()> onVisualUpdated;
    util::Signaler<void()> onProjectAttributeUpdated;
    util::Signaler<void()> onTimeFormatChanged;

private:
    QScopedPointer<QProcess> mProcess;
    void loadVideoFormats();
    void onCanvasSizeTriggered();
    void onMaxFrameTriggered();
    void onLoopTriggered();
    void onFPSTriggered();
    bool confirmMaxFrameUpdating(int aNewMaxFrame) const;

    ViaPoint& mViaPoint;
    core::Project* mProject;
    QVector<QAction*> mProjectActions;
    QAction* mShowResourceWindow;
    QList<ctrl::VideoFormat> mVideoFormats;
    GUIResources& mGUIResources;
};

//-------------------------------------------------------------------------------------------------
class ProjectCanvasSizeSettingDialog: public EasyDialog {
    Q_OBJECT
public:
    ProjectCanvasSizeSettingDialog(ViaPoint& aViaPoint, core::Project& aProject, QWidget* aParent);
    QSize canvasSize() const {
        return QSize(mWidthBox->value(), mHeightBox->value());
    }

private:
    ViaPoint& mViaPoint;
    core::Project& mProject;
    QSpinBox* mWidthBox;
    QSpinBox* mHeightBox;
};

//-------------------------------------------------------------------------------------------------
class ProjectMaxFrameSettingDialog: public EasyDialog {
    Q_OBJECT
public:
    ProjectMaxFrameSettingDialog(core::Project& aProject, QWidget* aParent);
    int maxFrame() const {
        return mMaxFrameBox->value();
    }

private:
    bool confirmMaxFrameUpdating(int aNewMaxFrame) const;
    core::Project& mProject;
    QSpinBox* mMaxFrameBox;
};

//-------------------------------------------------------------------------------------------------
class ProjectFPSSettingDialog: public EasyDialog {
    Q_OBJECT
public:
    ProjectFPSSettingDialog(core::Project& aProject, QWidget* aParent);
    int fps() const {
        return mFPSBox->value();
    }

private:
    bool confirmFPSUpdating(int aNewMaxFrame) const;
    core::Project& mProject;
    QSpinBox* mFPSBox;
};

//-------------------------------------------------------------------------------------------------
class ProjectLoopSettingDialog: public EasyDialog {
    Q_OBJECT
public:
    ProjectLoopSettingDialog(core::Project& aProject, QWidget* aParent);
    bool isCheckedLoopBox() const {
        return mLoopBox->isChecked();
    }

private:
    QCheckBox* mLoopBox;
};


} // namespace gui

#endif // GUI_MAINMENUBAR_H
