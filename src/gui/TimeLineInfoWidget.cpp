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
        if(mProject->pConf != nullptr && mProject->mediaPlayer != nullptr && mPlayBack->isPlaying()){
            qDebug("-----");
            qDebug(QString("Current frame = ").append(QString::number(currentFrame)).toStdString().c_str());
            int currentPlayer = 0;
            for (const auto& file : *mProject->pConf) {
                qDebug(QString("Start frame = ").append(QString::number(file.startFrame)).toStdString().c_str());
                qDebug(QString("End frame = ").append(QString::number(file.endFrame)).toStdString().c_str());
                qDebug("-----");

                auto* player = mProject->mediaPlayer->players.at(currentPlayer);

                if(player->isPlaying()){
                    qDebug() << "Player at " << currentPlayer << " is playing.";
                    if (mProject->animator().isSuspended() || file.endFrame <= currentFrame || !file.playbackEnable) {
                        qDebug("Request player stop");
                        player->stop();
                    }
                }
                else{
                    qDebug() << "Player at " << currentPlayer << " is suspended.";
                    if (file.startFrame <= currentFrame  && file.endFrame >= currentFrame && file.playbackEnable) {
                        qDebug("Request player start");
                        player->play();
                        mProject->mediaPlayer->playing = true;
                    }
                }
                currentPlayer++;
            }
        }
    }
}
void TimeLineInfoWidget::setPlayback(PlayBackWidget* aPlayback) {
    mPlayBack = aPlayback;
}

} // namespace gui
