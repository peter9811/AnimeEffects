#ifndef GUI_PLAYBACKWIDGET_H
#define GUI_PLAYBACKWIDGET_H

#include <vector>
#include <functional>
#include <QWidget>
#include <QPushButton>
#include "gui/GUIResources.h"
#include "gui/AudioPlaybackWidget.h"

namespace gui {

class PlayBackWidget: public QWidget {
    Q_OBJECT
public:
    enum PushType {
        PushType_Play,
        PushType_Pause,
        PushType_Step,
        PushType_StepBack,
        PushType_Rewind,
        PushType_Loop,
        PushType_NoLoop,
        PushType_Fast
    };
    typedef std::function<void(PushType)> PushDelegate;

    PlayBackWidget(GUIResources& aResources, QWidget* aParent);

    void setPushDelegate(const PushDelegate& aDelegate);
    bool isLoopChecked();
    void checkLoop(bool checkStatus);
    void PlayPause();
    bool isPlaying();
    static int constantWidth() ;
    void pushPauseButton();
    QWidget* audioUI = new QWidget(this, Qt::Window);

    // TODO: REMOVE AFTER TESTING //
    QString testFile = "i_forgor_before_so_it's_on_the_git_history_now_lol";
    std::vector<audioConfig>* pConf = new std::vector<audioConfig>{ audioConfig{"testFile", QFileInfo(testFile), true, 100, 20, 50}};
    mediaState mediaPlayer;
    //
    AudioPlaybackWidget* audioWidget = new AudioPlaybackWidget;


private:
    QPushButton* createButton(const QString& aName, bool aIsCheckable, int aColumn, const QString& aToolTip);
    GUIResources& mGUIResources;
    std::vector<QPushButton*> mButtons;
    PushDelegate mPushDelegate;
    void onThemeUpdated(theme::Theme&);
};

} // namespace gui

#endif // GUI_PLAYBACKWIDGET_H
