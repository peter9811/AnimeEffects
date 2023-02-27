#include "NetworkUtil.h"
#include "qapplication.h"
#include "qdir.h"
#include "qjsonarray.h"
#include "gui/menu/menu_ProgressReporter.h"
#include <QEventLoop>
#include <QJsonObject>
#include <QRegularExpression>

namespace util
{

NetworkUtil::NetworkUtil()
{
}
    QByteArray NetworkUtil::getByteArray(QString aURL){
        QProcess mProcess;
        QByteArray response;
        QList<QString> arguments = libArgs({aURL}, "json");
        if (arguments[1] == "failed"){
            return QByteArray::fromStdString("NetworkUtil Error");
        }
        QString program = arguments[0];
        arguments.pop_front();
        mProcess.start(program, arguments);
        mProcess.waitForFinished();
        if (mProcess.exitCode() == 0){
            response = mProcess.readAll();
        }
        else{
            qDebug("-----------");
            qDebug() << "program : " << program;
            qDebug() << "args : " << arguments;
            qDebug() << "std err : " << mProcess.readAllStandardError();
            qDebug() << "std out : " << mProcess.readAllStandardOutput();
            qDebug("-----------");
            return QByteArray::fromStdString("NetworkUtil Error");
        }
        // qDebug() << response.data();
        return response;
    }

    QJsonDocument NetworkUtil::getJsonFrom(QString aURL){
        QByteArray data = getByteArray(aURL);
        if (data == "NetworkUtil Error"){
            return QJsonDocument();
        }
        return QJsonDocument::fromJson(data.data());
    }

    bool NetworkUtil::libExists(QString aLib){
        QProcess process;
        process.start(aLib, {"-V"}, QProcess::ReadWrite);
        process.waitForFinished();
        if(process.exitStatus() == 0)
        {
            return true;
        }
        else{
            return false;
        }
    }

    // The "json" type accepts only lists of size 1.
    // The "download" type requires index 0 to be the url and index 1 to be the path.
    QList<QString> NetworkUtil::libArgs(QList<QString> aArgument, QString aType){
        QString lib;
        QList<QString> args;
        if (os() == "win" || os() == "mac"){
            lib = "curl";
        }
        else if (os() == "linux"){
            lib = "wget";
        }
        if (!libExists(lib)){
            qDebug() << lib << " couldn't be found.";
            auto failedLib = lib;
            if (lib == "wget"){
                lib = "curl";
            }
            else{
                lib = "wget";
            }
            if (!libExists(lib)){
                return QList<QString>({failedLib, "failed"});
            }
        }
        // Arguments to get a json string through standard output
        if (aType == "json"){
            if (lib == "curl"){
                args << "curl";
                args << aArgument[0] << "-H 'Accept: application/json'";
            }
            else if (lib == "wget"){
                args << "wget";
                args << "-qO-" << aArgument[0] << "--header='Content-Type: application/json'";
            }
        }
        // Arguments to download a file to a certain path. We use "-L" with curl to avoid issues with redirections.
        if (aType == "download"){
            if (lib == "curl"){
                args << "curl";
                args << "-L" << aArgument[0] << "--output" << aArgument[1];
            }
            else if (lib == "wget"){
                args << "wget";
                args << aArgument[0] << "-O" << aArgument[1];
            }
        }
        return args;
    }

    // Function written in consideration of the "releases/{version}" api, e.g. https://api.github.com/repos/author/project/releases/latest
    QFileInfo NetworkUtil::downloadGithubFile(QString aURL, QString aFile, int aID, QWidget *aParent){
        QJsonObject jsonResponse = this->getJsonFrom(aURL).object();
        QString downloadURL = "null";

        // Get name of assets and then check against file and ID.
        for(auto assets : jsonResponse.value("assets").toArray()){
            qDebug() << assets.toObject().value("name").toString();
            if(assets.toObject().value("name").toString() == aFile){
                // If ID field, check it.
                if (aID != 0){
                    int urlID = assets.toObject().value("id").toInt();
                    if (aID == urlID){
                        downloadURL = assets.toObject().value("browser_download_url").toString();
                    }
                    else{
                        qDebug() << "ID Mismatch. \nID Expected:" << aID << "- ID Value:" << urlID;
                    }
                }
                // Otherwise just get the asset url
                else{
                    downloadURL = assets.toObject().value("browser_download_url").toString();
                }
            }
        }
        if (downloadURL == "null"){
            return QFileInfo();
        }
        QString downloadPath = QDir::tempPath() + "/" + aFile;
        QList<QString> args = this->libArgs({downloadURL, downloadPath}, "download");
        // qDebug() << args;
        QScopedPointer<QProcess> mProcess;
        mProcess.reset(new QProcess(nullptr));
        mProcess->setProcessChannelMode(QProcess::MergedChannels);
        auto processData = mProcess.data();
        QString program = args[0];
        gui::menu::ProgressReporter progress(true, aParent);
        gui::menu::ProgressReporter* progressptr = &progress;
        progress.setProgress(0);
        args.pop_front();
        mProcess->connect(processData, &QProcess::readyRead, [=]() mutable {
            QString data = processData->readAll().data();
            if (program == "curl"){
                qDebug() << "Download percentage : " << data.mid(1, 3);
                progressptr->setProgress(data.midRef(1, 3).toInt());
            }
            else if(program=="wget"){
                // Finds a number with a percentage sign.
                QRegularExpression re("(\\d{1,3})%");
                QString percentage = re.match(data).captured(0);
                percentage.remove("%");
                if (percentage.toInt() < 101 && percentage.toInt() != 0){
                    qDebug() << "Download percentage : "<< percentage.toInt();
                    progressptr->setProgress(percentage.toInt());
                }
            }
        });
        progress.setSection(QCoreApplication::translate("NetworkUtil", "Currently downloading:") + "\n\n" + aFile);
        progress.setMaximum(100);
        mProcess->start(program, args);
        if (mProcess->waitForReadyRead(15000)){
            // Max time for this is five minutes.
            mProcess->waitForFinished(60000*5);
        }
        if(QFile(downloadPath).exists()){
            return QFileInfo(QFile(downloadPath));
        }
        return QFileInfo();
    }
} // namespace util
