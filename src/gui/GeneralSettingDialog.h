#ifndef GUI_GENERALSETTINGDIALOG_H
#define GUI_GENERALSETTINGDIALOG_H

#include "gui/GUIResources.h"
#include <QComboBox>
#include <QCheckBox>
#include <QFormLayout>
#include "gui/EasyDialog.h"
#include "core/TimeFormat.h"
#include "qspinbox.h"

namespace gui
{

class GeneralSettingDialog : public EasyDialog
{
    Q_OBJECT
public:
    GeneralSettingDialog(GUIResources& aGUIResources, QWidget* aParent);
    QFormLayout* createTab(const QString& aTitle, QFormLayout *aForm);
    bool easingHasChanged();
    bool rangeHasChanged();
    bool languageHasChanged();
    bool timeFormatHasChanged();
    bool autoSaveHasChanged();
    bool autoSaveDelayHasChanged();
    bool themeHasChanged();
    bool HSVBehaviourHasChanged();
    bool HSVSetColorHasChanged();
    bool HSVFolderHasChanged();
    bool keyDelayHasChanged();
    QString theme();
private:
    void saveSettings();

    QTabWidget* mTabs;

    int mInitialLanguageIndex;
    QComboBox* mLanguageBox;

    void mResetRecents();
    QPushButton* mResetButton;

    int mInitialEasingIndex;
    QComboBox* mEasingBox;

    int mInitialRangeIndex;
    QComboBox* mRangeBox;

    bool bAutoSave;
    QCheckBox* mAutoSave;

    int mAutoSaveDelay;
    QSpinBox* mAutoSaveDelayBox;

    bool bHSVBlendColor;
    QCheckBox* mHSVBlendColor;

    bool bHSVFolder;
    QCheckBox* mHSVFolder;

    int mInitialHSVBehaviour;
    QComboBox* mHSVBehaviour;

    int mInitialTimeFormatIndex;
    QComboBox* mTimeFormatBox;

    QString mInitialThemeKey;
    QComboBox* mThemeBox;

    int mKeyDelay;
    QSpinBox* mKeyDelayBox;

    GUIResources& mGUIResources;
};

} // namespace gui

#endif // GUI_GENERALSETTINGDIALOG_H
