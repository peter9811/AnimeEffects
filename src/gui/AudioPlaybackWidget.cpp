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

void AudioPlaybackWidget::aPlayer(std::vector<audioConfig>* pConf, bool play, mediaState* state, int fps, int curFrame, int frameCount){
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

void AudioPlaybackWidget::connect(QWidget *audioWidget, mediaState *state, std::vector<audioConfig>* config){
    Q_UNIMPLEMENTED();
}

void checkConnection(bool connection){
    if(!connection){
        QMessageBox::warning(nullptr, "Qt connection error", "Unable to connect UI component");
    }
}

void AudioPlaybackWidget::rectifyUI(std::vector<audioConfig>* config, mediaState* mediaPlayer) {
    // We reconstruct the entire state so we don't mismatch the UI indices
    {
        QLayoutItem* item;
        while ((item = gridLayout_2->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete gridLayout_2;
    }
    gridLayout_2 = new QGridLayout(musTab);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    QCoreApplication::processEvents();
    vecUIState.clear();

    int x = 0;
    if(config->empty()){
        config->emplace_back();
    }
    for(auto conf: *config){
        addUIState(config, x, mediaPlayer, true);
        x++;
    }
    gridLayout_2->update();
}
void AudioPlaybackWidget::addUIState(std::vector<audioConfig>* config, int index, mediaState* mediaPlayer, bool bulk) {
    // This is a great time to tell you that we accept pull requests that fix this kind of nonsense
    auto newState =
        UIState{new QToolButton(musPlayer), new QToolButton(musPlayer), new QCheckBox(musPlayer),
                new QSpinBox(musPlayer), new QSpinBox(musPlayer), new QLabel(musPlayer), new QLabel(musPlayer),
                new QLabel(musPlayer), new QLabel(musPlayer), new QSlider(musPlayer), new QFrame(musPlayer)
        };
    // Grid layout pos
    int idx = (int)vecUIState.size();
    int offset = idx * 4;
    int idxRow0 = offset;
    int idxRow1 = offset + 1;
    int idxRow2 = offset + 2;
    int idxRow3 = offset + 3;
    // Playback
    newState.playAudio->setObjectName(QString::fromUtf8("playAudio"));
    gridLayout_2->addWidget(newState.playAudio, idxRow1, 0, 1, 1);
    // Frame end
    newState.endSpinBox->setObjectName(QString::fromUtf8("endSpinBox"));
    gridLayout_2->addWidget(newState.endSpinBox, idxRow1, 2, 1, 1);
    // Frame start
    newState.startSpinBox->setObjectName(QString::fromUtf8("startSpinBox"));
    gridLayout_2->addWidget(newState.startSpinBox, idxRow1, 1 , 1, 1);
    // New track
    newState.addNewTrack->setObjectName(QString::fromUtf8("startLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(newState.addNewTrack->sizePolicy().hasHeightForWidth());
    newState.addNewTrack->setSizePolicy(sizePolicy);
    gridLayout_2->addWidget(newState.addNewTrack, idxRow0, 0, 1, 1);
    // Frame start text
    newState.startLabel->setObjectName(QString::fromUtf8("startLabel"));
    sizePolicy.setHeightForWidth(newState.startLabel->sizePolicy().hasHeightForWidth());
    newState.startLabel->setSizePolicy(sizePolicy);
    gridLayout_2->addWidget(newState.startLabel, idxRow0, 1, 1, 1);
    // Frame end text
    newState.endLabel->setObjectName(QString::fromUtf8("endLabel"));
    sizePolicy.setHeightForWidth(newState.endLabel->sizePolicy().hasHeightForWidth());
    newState.endLabel->setSizePolicy(sizePolicy);
    gridLayout_2->addWidget(newState.endLabel, idxRow0, 2, 1, 1);
    // Volume text
    newState.volumeLabel->setObjectName(QString::fromUtf8("volumeLabel"));
    newState.volumeLabel->setSizePolicy(sizePolicy);
    gridLayout_2->addWidget(newState.volumeLabel, idxRow2, 0, 1, 1);
    // Volume
    newState.volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
    newState.volumeSlider->setMaximum(100);
    newState.volumeSlider->setMinimum(0);
    newState.volumeSlider->setOrientation(Qt::Horizontal);
    newState.volumeSlider->setSizePolicy(sizePolicy);
    gridLayout_2->addWidget(newState.volumeSlider, idxRow2, 1, 1, 3);
    // Separator
    newState.line->setObjectName(QString::fromUtf8("line"));
    newState.line->setFrameShape(QFrame::HLine);
    newState.line->setFrameShadow(QFrame::Sunken);
    gridLayout_2->addWidget(newState.line, idxRow3, 0, 1, 4);
    // Frame text
    newState.musDurationLabel->setObjectName(QString::fromUtf8("musDurationLabel"));
    gridLayout_2->addWidget(newState.musDurationLabel, idxRow0, 3, 1, 1);
    // Audio selector
    newState.selectMusButton->setObjectName(QString::fromUtf8("selectMusButton"));
    newState.selectMusButton->setAutoRepeat(false);
    gridLayout_2->addWidget(newState.selectMusButton, idxRow1, 3, 1, 1);
    // Translate
    newState.playAudio->setText(QCoreApplication::translate("audioWidget", "Enable playback", nullptr));
    newState.addNewTrack->setText(QCoreApplication::translate("audioWidget", "Add new audio track", nullptr));
    newState.startLabel->setText(QCoreApplication::translate("audioWidget", "<html><head/><body><p align=\"center\">Playback start frame</p></body></html>", nullptr));
    newState.endLabel->setText(QCoreApplication::translate("audioWidget", "<html><head/><body><p align=\"center\">Playback end frame</p></body></html>", nullptr));
    newState.musDurationLabel->setText(QCoreApplication::translate("audioWidget", "<html><head/><body><p align=\"center\">Duration (in frames): </p></body></html>", nullptr) +
                                       "<html><head/><body><p align=\"center\">" + QString::number(config->at(index).endFrame - config->at(index).startFrame) + "</p></body></html>");
    newState.volumeLabel->setText(QCoreApplication::translate("audioWidget", "Media volume", nullptr) + " (%" + QString::number(config->at(index).volume) + ")");
    // Init
    newState.playAudio->setChecked(config->at(index).playbackEnable);
    newState.startSpinBox->setValue(config->at(index).startFrame);
    newState.endSpinBox->setValue(config->at(index).endFrame);
    newState.volumeSlider->setValue(config->at(index).volume);
    newState.selectMusButton->setText(config->at(index).audioName == "Placeholder"? QCoreApplication::translate("audioWidget", "Select audio file...", nullptr) : config->at(index).audioName);
    if(bulk){
        if (index > 0 && index + 1 > idx) {
            newState.addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
            newState.addTrack = false;
        } else if (idx > 1) {
            newState.addNewTrack->setDisabled(true);
        }
        else{
            newState.addTrack = true;
        }
    }
    vecUIState.append(newState);
    // Connect
    idx = index;
    checkConnection(QToolButton::connect(newState.selectMusButton, &QToolButton::clicked, [=]() {
        auto file = QFileInfo(QFileDialog::getOpenFileName(
            this->musPlayer,
            QCoreApplication::translate("SelectMus", "Open Image", nullptr),
            QDir::currentPath(),
            QCoreApplication::translate("SelectMus", "Audio Files (*.mp3 *.mp4 *.wav *.ogg *.flac)", nullptr)
        ));
        newState.selectMusButton->setText(file.fileName());
        config->at(idx).audioPath = file;
        config->at(idx).audioName = file.fileName();
    }));
    checkConnection(QToolButton::connect(newState.addNewTrack, &QToolButton::clicked, [=]() {
        //FIXME
        if (idx != 0 && (idx + 1 != config->size() || vecUIState.first().addNewTrack->isEnabled())) {
            newState.addTrack = false;
        }
        if (newState.addTrack) {
            if (config->size() != vecUIState.size()) {
                if (config->size() > vecUIState.size()) {
                    config->pop_back();
                }
                if (config->size() < vecUIState.size()) {
                    config->emplace_back();
                }
                rectifyUI(config, mediaPlayer);
                return;
            }
            config->emplace_back();
            this->addUIState(config, idx + 1, mediaPlayer);
            if (idx == 0) {
                vecUIState.at(1).addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
                vecUIState.at(1).addTrack = false;
                if (vecUIState.size() > 2) { vecUIState.first().addNewTrack->setDisabled(true); }
            }
            else {
                newState.addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
                newState.addTrack = false;
            }
        }
        else {
            config->erase(config->begin() + idx);
            if (config->size() >= vecUIState.size() && !config->empty()) { config->pop_back(); }
            rectifyUI(config, mediaPlayer);
            if (config->size() == 1) { vecUIState.first().addNewTrack->setDisabled(false); }
            else { vecUIState.first().addNewTrack->setDisabled(true); }
        }
    }));
}

void AudioPlaybackWidget::addTrack(mediaState* state, QUrl source) {
    auto* mediaPlayer = new QMediaPlayer;
    auto* audioOutput = new QAudioOutput;
    mediaPlayer->setSource(source);
    mediaPlayer->setAudioOutput(audioOutput);
    state->players.append(mediaPlayer);
    state->outputs.append(audioOutput);
}
void AudioPlaybackWidget::modifyTrack(mediaState* state, std::vector<audioConfig>* config, int index, audioConfig modifiedConfig) {
    config->at(index) = std::move(modifiedConfig);
    auto* currentPlayer = state->players.at(index);
    auto* currentOutput = state->outputs.at(index);
    if(currentPlayer->isPlaying()) { currentPlayer->stop(); }
    currentPlayer->setSource(modifiedConfig.audioPath.absoluteFilePath());
    currentOutput->setVolume((float)modifiedConfig.volume);
}
void AudioPlaybackWidget::removeTrack(mediaState* state, std::vector<audioConfig>* config, int index) {
    Q_UNIMPLEMENTED();
}
