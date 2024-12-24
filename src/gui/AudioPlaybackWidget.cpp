#include "AudioPlaybackWidget.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringRef>

void checkConnection(bool connection){
    if(!connection){
        QMessageBox::warning(nullptr, "Qt connection error", "Unable to connect UI component");
    }
}

bool AudioPlaybackWidget::serialize(std::vector<audioConfig>* pConf, const QString& outPath) {
    QJsonObject audioJson;
    audioJson["Tracks"] = (int)pConf->size();
    int x = 0;
    for(const auto& audioConfig: *pConf){
        QJsonObject track;
        track["Name"] = audioConfig.audioName;
        track["Path"] = audioConfig.audioPath.absoluteFilePath();
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
    return true;
}

bool AudioPlaybackWidget::deserialize(const QJsonObject& pConf, std::vector<audioConfig>* playbackConfig) {
    if(pConf.isEmpty()) { return false; }
    try {
        auto deserialized = std::vector<audioConfig>();
        for (int x = 0; x < pConf["Tracks"].toInt(); x++){
            deserialized.emplace_back();
            deserialized.at(x).audioName = pConf[QString::number(x)].toObject()["Name"].toString();
            deserialized.at(x).audioPath = QFileInfo(pConf[QString::number(x)].toObject()["Path"].toString());
            deserialized.at(x).playbackEnable = pConf[QString::number(x)].toObject()["Playback"].toBool();
            deserialized.at(x).volume = pConf[QString::number(x)].toObject()["Volume"].toInt();
            deserialized.at(x).startFrame = pConf[QString::number(x)].toObject()["StartFrame"].toInt();
            deserialized.at(x).endFrame = pConf[QString::number(x)].toObject()["EndFrame"].toInt();
        }
        playbackConfig->clear();
        for (const auto& config: deserialized){
            playbackConfig->emplace_back(config);
        }
    }
    // TODO: Fix this later
    catch (...) { return false; }
    return true;
}

void AudioPlaybackWidget::aPlayer(std::vector<audioConfig>* pConf, bool play, mediaState* state, int fps, int curFrame, int frameCount){
    qDebug("Media player requested");
    int x = 0;
    for(auto conf: *pConf){
        if(state->players.size() < x + 1 && state->outputs.size() < x + 1){
            auto* mediaPlayer = new QMediaPlayer;
            auto* audioOutput = new QAudioOutput;
            state->players.append(mediaPlayer);
            state->outputs.append(audioOutput);
        }
        QMediaPlayer* player = state->players.at(x);
        QAudioOutput* output = state->outputs.at(x);
        const audioConfig& config = pConf->at(x);
        if(player->audioOutput() == nullptr){ player->setAudioOutput(output); }
        if(player->source() != QUrl::fromLocalFile(config.audioPath.absoluteFilePath())){
            player->setSource(config.audioPath.absoluteFilePath());
        }
        if(output->volume() != getVol(config.volume)){ output->setVolume(getVol(config.volume)); }
        correctTrackPos(player, curFrame, frameCount, fps, const_cast<audioConfig&>(config));
        qDebug("---");
        qDebug() << "x = " << x << "; track = " << player->source() << "; playing = " << player->isPlaying() << "; play = " << play;
        qDebug("---");
        if(!play){ player->stop(); state->playing = false;}
        if(play && config.startFrame < curFrame && config.endFrame > curFrame && config.playbackEnable){
            player->play();
            state->playing = true;
        }
        x++;
    }
}

void AudioPlaybackWidget::connect(QWidget *audioWidget, mediaState *state, std::vector<audioConfig>* config){
    checkConnection(QToolButton::connect(saveConfigButton, &QToolButton::clicked, [=](){
        QFileDialog diag(audioWidget);
        diag.setAcceptMode(QFileDialog::AcceptSave);
        diag.setDirectory(QDir::currentPath());
        diag.setNameFilter(QCoreApplication::translate("SaveMus", "Anie audio configuration file (*.aemus)", nullptr));
        if(diag.exec()) {
            QString fileName = diag.selectedFiles().first();
            static QRegularExpression fileRegex("\\.aemus$");
            if (!fileRegex.match(fileName).hasMatch()) { fileName.append(".aemus"); }
            serialize(config, fileName);
        }
    }));
    checkConnection(QToolButton::connect(loadConfigButton, &QToolButton::clicked, [=]()mutable {
        QFileDialog diag(audioWidget);
        diag.setAcceptMode(QFileDialog::AcceptOpen);
        diag.setDirectory(QDir::currentPath());
        diag.setNameFilter(QCoreApplication::translate("LoadMus", "Anie audio configuration file (*.aemus)", nullptr));
        if(diag.exec()) {
            QFile file = QFile(diag.selectedFiles().first());
            if(!file.open(QIODevice::ReadOnly)) {
                QMessageBox::warning(audioWidget, "File error", file.errorString());
                return;
            }
            QJsonDocument json = QJsonDocument::fromJson(file.readAll());
            if(json.isEmpty() || json.isNull()){
                QMessageBox::warning(audioWidget, "Invalid file \"" + QFileInfo(file).baseName() + '"', "The requested file's json contents could not be parsed.");
                return;
            }
            auto configBackup = *config;
            if(!deserialize(json.object(), config)) {
                *config = configBackup;
                QMessageBox::warning(audioWidget, "Invalid file", "The requested file is not a valid aemus file.");
                return;
            }
            else{ QMessageBox::information(audioWidget, "Success", "Successfully loaded audio"); }
            rectifyUI(config, state);
        }
    }));

}

void AudioPlaybackWidget::rectifyUI(std::vector<audioConfig>* config, mediaState* mediaPlayer, bool bulk) {
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
        addUIState(config, x, mediaPlayer, bulk);
        x++;
    }
    gridLayout_2->update();
}

void AudioPlaybackWidget::addUIState(std::vector<audioConfig>* config, int index, mediaState* mediaPlayer, bool bulk) {
    // This is a great time to tell you that we accept pull requests that fix this kind of nonsense
    auto newState = UIState();
    // Grid layout pos
    int vecIdx = (int)vecUIState.size();
    int offset = vecIdx * 4;
    int idxRow0 = offset;
    int idxRow1 = offset + 1;
    int idxRow2 = offset + 2;
    int idxRow3 = offset + 3;
    // Playback
    newState.playAudio->setObjectName(QString::fromUtf8("playAudio"));
    gridLayout_2->addWidget(newState.playAudio, idxRow1, 0, 1, 1);
    // Frame end
    newState.endSpinBox->setObjectName(QString::fromUtf8("endSpinBox"));
    newState.endSpinBox->setMaximum(INT32_MAX);
    newState.endSpinBox->setMinimum(0);
    gridLayout_2->addWidget(newState.endSpinBox, idxRow1, 2, 1, 1);
    // Frame start
    newState.startSpinBox->setObjectName(QString::fromUtf8("startSpinBox"));
    newState.startSpinBox->setMaximum(INT32_MAX);
    newState.startSpinBox->setMinimum(0);
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
    newState.volumeLabel->setText(QCoreApplication::translate("audioWidget", "Media volume", nullptr) + " (" + QString::number(config->at(index).volume) + "%)");
    // Init
    newState.playAudio->setChecked(config->at(index).playbackEnable);
    newState.startSpinBox->setValue(config->at(index).startFrame);
    newState.endSpinBox->setValue(config->at(index).endFrame);
    newState.volumeSlider->setValue(config->at(index).volume);

    if(config->at(index).audioName != "Placeholder"){
        QString ref = QStringRef(&config->at(index).audioName, 0, 17).toString();
        if (config->at(index).audioName.size() > ref.size()) {
            ref.append("...");
        }
        newState.selectMusButton->setText(ref);
        newState.selectMusButton->setToolTip(config->at(index).audioName);
    }
    else{
        newState.selectMusButton->setText(QCoreApplication::translate("audioWidget", "Select audio file...", nullptr));
    }

    if(bulk){
        if(index == 0) {
            int x = 0;
            for(const auto& conf: *config){
                if(mediaPlayer->players.size() - 1 >= x){
                    mediaPlayer->players.at(x)->stop();
                    mediaPlayer->players.at(x)->deleteLater();
                    mediaPlayer->outputs.at(x)->deleteLater();
                    mediaPlayer->players.clear();
                    mediaPlayer->outputs.clear();
                }
                addTrack(mediaPlayer, conf.audioPath.absoluteFilePath());
                modifyTrack(mediaPlayer, config, x);
            }
        }
        if (index > 0 && index + 1 != config->size() || (index == 1 && config->size() == 2)) {
            newState.addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
            newState.addTrack = false;
        }
        else{
            newState.addTrack = true;
        }
    }
    vecUIState.append(newState);
    // Connect
    checkConnection(QToolButton::connect(newState.selectMusButton, &QToolButton::clicked, [=]() {
        auto file = QFileInfo(QFileDialog::getOpenFileName(
            this->musPlayer,
            QCoreApplication::translate("SelectMus", "Open Image", nullptr),
            QDir::currentPath(),
            QCoreApplication::translate("SelectMus", "Audio Files (*.mp3 *.mp4 *.wav *.ogg *.flac)", nullptr)
        ));
        config->at(index).audioPath = file;
        config->at(index).audioName = file.fileName();
        modifyTrack(mediaPlayer, config, index);

        {
            QString ref = QStringRef(&config->at(index).audioName, 0, 17).toString();
            if (config->at(index).audioName.size() > ref.size()) {
                ref.append("...");
            }
            newState.selectMusButton->setText(ref);
            newState.selectMusButton->setToolTip(file.fileName());
        }
    }));
    checkConnection(QToolButton::connect(newState.addNewTrack, &QToolButton::clicked, [=]() {
        if (index > 0 && index + 1 != config->size() || (index == 1 && config->size() == 2)) {
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
            addTrack(mediaPlayer, QUrl());
            this->addUIState(config, index + 1, mediaPlayer);
            if (index == 0) {
                vecUIState.at(1).addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
                vecUIState.at(1).addTrack = false;
                if (vecUIState.size() > 2) {
                    rectifyUI(config, mediaPlayer);
                    vecUIState.first().addNewTrack->setDisabled(true);
                }
            }
            else {
                newState.addNewTrack->setText(QCoreApplication::translate("audioWidget", "Remove audio track", nullptr));
                newState.addTrack = false;
            }
        }
        else {
            removeTrack(mediaPlayer, index);
            config->erase(config->begin() + index);
            if (config->size() >= vecUIState.size() && !config->empty()) { config->pop_back(); }
            rectifyUI(config, mediaPlayer);
            if (config->size() == 1 || config->size() == 2) { vecUIState.first().addNewTrack->setDisabled(false); }
            else { vecUIState.first().addNewTrack->setDisabled(true); }
        }
    }));
    checkConnection(QSpinBox::connect(newState.startSpinBox, &QSpinBox::valueChanged, [=](int val){
        config->at(index).startFrame = val;
        vecUIState.at(index).musDurationLabel->setText(QCoreApplication::translate("audioWidget","<html><head/><body><p align=\"center\">Duration (in frames): </p></body></html>", nullptr) +
                                       "<html><head/><body><p align=\"center\">" + QString::number(config->at(index).endFrame - val) + "</p></body></html>");
    }));
    checkConnection(QSpinBox::connect(newState.endSpinBox, &QSpinBox::valueChanged, [=](int val){
        config->at(index).endFrame = val;
        newState.musDurationLabel->setText(QCoreApplication::translate("audioWidget", "<html><head/><body><p align=\"center\">Duration (in frames): </p></body></html>", nullptr) +
                                       "<html><head/><body><p align=\"center\">" + QString::number(val - config->at(index).startFrame) + "</p></body></html>");
    }));
    checkConnection(QCheckBox::connect(newState.playAudio, &QCheckBox::stateChanged, [=](bool val){
        config->at(index).playbackEnable = val;
    }));
    checkConnection(QSlider::connect(newState.volumeSlider, &QSlider::valueChanged, [=](int val){
        config->at(index).volume = val;
        modifyTrack(mediaPlayer, config, index);
        newState.volumeLabel->setText(QCoreApplication::translate("audioWidget", "Media volume", nullptr) + " (" + QString::number(val) + "%)");
    }));
}

void AudioPlaybackWidget::addTrack(mediaState* state, const QUrl& source) {
    auto* mediaPlayer = new QMediaPlayer;
    auto* audioOutput = new QAudioOutput;
    mediaPlayer->setSource(source);
    mediaPlayer->setAudioOutput(audioOutput);
    state->players.append(mediaPlayer);
    state->outputs.append(audioOutput);
}

void AudioPlaybackWidget::removeTrack(mediaState* state, int index) {
    state->players.at(index)->stop();
    state->players.at(index)->deleteLater();
    state->outputs.at(index)->deleteLater();
    state->players.removeAt(index);
    state->outputs.removeAt(index);
}

void AudioPlaybackWidget::modifyTrack(mediaState* state, std::vector<audioConfig>* config, int index) {
    const auto& modifiedConfig = config->at(index);
    if(state->players.isEmpty() || state->players.size() - 1 < index){
        addTrack(state, QUrl());
    }
    auto* currentPlayer = state->players.at(index);
    auto* currentOutput = state->outputs.at(index);
    if(currentPlayer->isPlaying()) { currentPlayer->stop(); }
    if(currentPlayer->source().isEmpty() || currentPlayer->source().fileName() != modifiedConfig.audioPath.fileName()){
        currentPlayer->setSource(modifiedConfig.audioPath.absoluteFilePath());
    }
    if(currentOutput->volume() != getVol(modifiedConfig.volume)){
        currentOutput->setVolume(getVol(modifiedConfig.volume));
    }
}

void AudioPlaybackWidget::correctTrackPos(QMediaPlayer* player, int curFrame,  int frameCount, int fps, audioConfig& config) {
    // Calculate times in milliseconds
    const float toMillis = 1000.0;
    int frame = frameCount - ((frameCount - config.startFrame) - curFrame);
    auto positionMs = static_cast<qint64>((static_cast<float>(frame) / static_cast<float>(fps)) * toMillis);
    if (player->hasAudio() && positionMs >= 0) { player->setPosition(positionMs); }
}
