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
    void selectTab(int aIndex){
        //General - 0 ; Project settings - 1 ; FFmpeg settings - 2 ; Animation keys - 3 ; Keybindings - 4
        mTabs->setCurrentIndex(aIndex);
    }
    bool easingHasChanged();
    bool rangeHasChanged();
    bool languageHasChanged();
    bool timeFormatHasChanged();
    bool autoSaveHasChanged();
    bool autoSaveDelayHasChanged();
    bool themeHasChanged();
    bool cbCopyHasChanged();
    bool keyDelayHasChanged();
    QString theme();
private:
    void saveSettings();

    QTabWidget* mTabs;

    int mInitialLanguageIndex;
    QComboBox* mLanguageBox;

    void mResetRecents();
    QPushButton* mResetButton;

    void mResetKeybinds();
    QPushButton* mResetKeybindsButton;

    int mInitialEasingIndex;
    QComboBox* mEasingBox;

    int mInitialRangeIndex;
    QComboBox* mRangeBox;

    bool bAutoSave;
    QCheckBox* mAutoSave;

    int mAutoSaveDelay;
    QSpinBox* mAutoSaveDelayBox;

    bool bAutoCbCopy;
    QCheckBox* mAutoCbCopy;

    int mInitialTimeFormatIndex;
    QComboBox* mTimeFormatBox;

    QString mInitialThemeKey;
    QComboBox* mThemeBox;

    int mKeyDelay;
    QSpinBox* mKeyDelayBox;

    bool bAutoShowMesh;
    QCheckBox* mAutoShowMesh;

    QPushButton* ffmpegTroubleshoot;
    QPushButton* selectFromExe;
    QPushButton* autoSetup;

    GUIResources& mGUIResources;
};

} // namespace gui

#endif // GUI_GENERALSETTINGDIALOG_H
