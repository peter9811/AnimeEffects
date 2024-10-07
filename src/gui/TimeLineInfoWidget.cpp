#include "gui/TimeLineInfoWidget.h"

namespace gui {

TimeLineInfoWidget::TimeLineInfoWidget(GUIResources& aResources, QWidget* aParent):
    QLabel(aParent), mResources(aResources), mProject(), mIsFirstTime(true), mSuspendCount(0) {
    this->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
    this->setContentsMargins(2, 0, 2, 2);
}

void TimeLineInfoWidget::setProject(core::Project* aProject) {
    mProject = aProject;
    onUpdate();
}

void TimeLineInfoWidget::onUpdate() {
    if (mProject != nullptr) {
        core::TimeInfo timeInfo = mProject->currentTimeInfo();
        const int frameMax = timeInfo.frameMax;
        const int fps = timeInfo.fps;
        const int currentFrame = timeInfo.frame.get();
        const core::TimeFormat timeFormat(util::Range(0, frameMax), fps);
        QString frameNumber = timeFormat.frameToString(currentFrame, formatType);
        QString frameMaxNumber = timeFormat.frameToString(frameMax, formatType);
        this->setText(
            frameNumber.rightJustified(frameMaxNumber.length() + 1, ' ') + " / " + frameMaxNumber + " @" +
            QString::number(fps) + " " + "FPS"
        );
        // Audio
        if(mProject->pConf != nullptr && mProject->mediaPlayer != nullptr){
            qDebug("Current frame = "); qDebug() << currentFrame;
            if(mProject->mediaPlayer->playing){
                int currentPlayer = 0;
                for (const auto& file : *mProject->pConf) {
                    qDebug("Playback");
                    if (file.endFrame >= currentFrame && file.startFrame < currentFrame) {
                        if(mProject->mediaPlayer->players.at(currentPlayer)->isPlaying()) {
                            mProject->mediaPlayer->players.at(currentPlayer)->stop();
                            mProject->mediaPlayer->playing = false;
                        }
                    }
                    currentPlayer++;
                }
            }
            else{
                qDebug("Not playing");
                int currentPlayer = 0;
                for (const auto& file : *mProject->pConf) {
                    if (file.startFrame == currentFrame && file.playbackEnable) {
                        if(!mProject->mediaPlayer->players.at(currentPlayer)->isPlaying()) {
                            mProject->mediaPlayer->players.at(currentPlayer)->play();
                            mProject->mediaPlayer->playing = true;
                        }
                    }
                    if (file.startFrame >= currentFrame && file.endFrame <= currentFrame) {
                        if(!mProject->mediaPlayer->players.at(currentPlayer)->isPlaying()) {
                            mProject->mediaPlayer->players.at(currentPlayer)->play();
                            mProject->mediaPlayer->playing = true;
                        }
                    }
                    currentPlayer++;
                }
            }
        }
    }
}

} // namespace gui
