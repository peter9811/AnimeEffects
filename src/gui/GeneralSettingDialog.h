#ifndef GUI_GENERALSETTINGDIALOG_H
#define GUI_GENERALSETTINGDIALOG_H

#include "gui/GUIResources.h"
#include <QComboBox>
#include <QCheckBox>
#include <QFormLayout>
#include "gui/EasyDialog.h"
#include "core/TimeFormat.h"

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
    bool themeHasChanged();
    bool HSVBehaviourHasChanged();
    bool HSVSetColorHasChanged();
    bool HSVFolderHasChanged();
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

    bool bHSVSetColor;
    QCheckBox* mHSVSetColor;

    bool bHSVFolder;
    QCheckBox* mHSVFolder;

    int mInitialHSVBehaviour;
    QComboBox* mHSVBehaviour;

    int mInitialTimeFormatIndex;
    QComboBox* mTimeFormatBox;

    QString mInitialThemeKey;
    QComboBox* mThemeBox;

    GUIResources& mGUIResources;
};

} // namespace gui

#endif // GUI_GENERALSETTINGDIALOG_H
