#include "AudioPlaybackWidget.h"
#include <QJsonObject>
#include <QJsonDocument>

bool AudioPlaybackWidget::serialize(std::vector<audioConfig>* pConf, const QString& outPath) {
    QJsonObject audioJson;
    audioJson["Tracks"] = (int)pConf->size();
    int x = 0;
    for(const auto& audioConfig: *pConf){
        QJsonObject track;
        track["Name"] = audioConfig.audioName;
        track["Path"] = audioConfig.audioPath.absolutePath();
        track["Playback"] = audioConfig.playbackEnable;
        track["Volume"] = audioConfig.volume;
        track["StartFrame"] = audioConfig.startFrame;
        track["EndFrame"] = audioConfig.endFrame;
        audioJson[QString::number(x)] = track;
        x++;
    }
    QFile file(outPath);
    file.open(QIODevice::ReadWrite);
    file.write(QJsonDocument(audioJson).toJson(QJsonDocument::Indented));
    file.close();
    return false;
}
bool AudioPlaybackWidget::deserialize(const QJsonObject& pConf, std::vector<audioConfig>* playbackConfig) {
    if(pConf.isEmpty()) { return false; }
    try {
        auto deserialized = std::vector<audioConfig>();
        for (int x = 0; x > pConf["Tracks"].toInt(); x++){
            deserialized.emplace_back();
            deserialized.at(x).audioName = pConf[QString::number(x)]["Name"].toString();
            deserialized.at(x).audioPath = QFileInfo(pConf[QString::number(x)]["Path"].toString());
            deserialized.at(x).playbackEnable = pConf[QString::number(x)]["Playback"].toBool();
            deserialized.at(x).volume = pConf[QString::number(x)]["Volume"].toInt();
            deserialized.at(x).startFrame = pConf[QString::number(x)]["StartFrame"].toInt();
            deserialized.at(x).endFrame = pConf[QString::number(x)]["EndFrame"].toInt();
        }
        playbackConfig->clear();
        *playbackConfig = deserialized;
    }
    // TODO: Fix this later
    catch (...) { return false; }
    return true;
}
void AudioPlaybackWidget::aPlayer(std::vector<audioConfig>* pConf, bool play, mediaState* state, int fps, int curFrame,
                                  int frameCount){
    qDebug("Media player requested");
    for(int x = 0; x < pConf->size(); x++){
        if(pConf->size() > x + 1){ return; }
        if(state->players.size() < x + 1 && state->outputs.size() < x + 1){
            auto* mediaPlayer = new QMediaPlayer;
            auto* audioOutput = new QAudioOutput;
            state->players.append(mediaPlayer);
            state->outputs.append(audioOutput);
        }
        QMediaPlayer* player = state->players.at(x);
        QAudioOutput* output = state->outputs.at(x);
        const audioConfig& config = pConf->at(x);
        if(player->audioOutput() == nullptr){
            player->setAudioOutput(output);
        }
        if(player->source() != QUrl::fromLocalFile(config.audioPath.absoluteFilePath())){
            player->setSource(config.audioPath.absoluteFilePath());
        }
        if(output->volume() != getVol(config.volume)){ output->setVolume(getVol(config.volume)); }

        // FIXME
        int framesInMs = (frameCount / fps) / 1000;
        int startFrameInMs = ((frameCount - (config.startFrame + 1)) / fps) / 1000;
        int curFrameInMs = (((curFrame + 1) - frameCount)/ fps) / 1000;

        uint initialFrameInMs = framesInMs - startFrameInMs;
        uint posInMs = framesInMs - curFrameInMs;

        if(player->hasAudio() && posInMs >= initialFrameInMs) {
            player->setPosition(posInMs);
        }

        if(!play && player->isPlaying()){ player->stop(); state->playing = false;}
        if(play && config.startFrame < curFrame && config.endFrame < curFrame && config.playbackEnable && !player->isPlaying()){
            player->play();
            state->playing = true;
        }
    }
}
void AudioPlaybackWidget::connectUI(QWidget *audioWidget, mediaState *state, std::vector<audioConfig>* config){
    Q_UNIMPLEMENTED();
}

void AudioPlaybackWidget::connectUIState(QVector<UIState> *state, std::vector<audioConfig>* config) {
    int idx = 0;
    for(auto qt: *state){
        QWidget::connect(qt.selectMusButton, &QToolButton::clicked, [=](){qt.selectMusButton->setText("Test");});
        QWidget::connect(qt.addNewTrack, &QToolButton::clicked, [=]() {
            if(qt.addTrack){
                config->emplace_back();
                this->addUIState(config, idx);
                if(idx == 0){
                    qt.addNewTrack->setDisabled(true);
                }
                else{
                    qt.addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
                    qt.addTrack = false;
                }
            }
            else{
                config->erase(config->begin() + idx);
                this->removeUIState(config);
                if(state->size() == 1){
                    state->at(0).addNewTrack->setDisabled(false);
                }
            }
            if(idx == 0 && state->size() > 1){
                state->at(0).addNewTrack->setDisabled(true);
            }
            connectUIState(&vecUIState, config);
        });
        idx++;
    }
    Q_UNIMPLEMENTED();
}
