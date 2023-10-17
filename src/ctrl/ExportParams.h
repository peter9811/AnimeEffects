#ifndef ANIMEEFFECTS_EXPORTPARAMS_H
#define ANIMEEFFECTS_EXPORTPARAMS_H

#include <QString>
#include <QList>
// For UI
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>
// For exporter
#include <list>
#include <memory>
#include <functional>
#include <QString>
#include <QSize>
#include <QFileInfo>
#include <QProcess>
#include <QOpenGLFramebufferObject>
#include "qdir.h"
#include "qmessagebox.h"
#include "gl/Global.h"
#include "util/Range.h"
#include "util/IProgressReporter.h"
#include "ctrl/UILogger.h"
#include "gl/EasyTextureDrawer.h"
#include "core/Project.h"
#include "core/TimeInfo.h"
#include "core/TimeKeyBlender.h"
#include "core/ClippingFrame.h"
#include "core/DestinationTexturizer.h"
#include "ctrl/VideoFormat.h"
// I was not meant to do OOP...

// Formats other than video and image are for internal use only, ***DO NOT*** use them on export
enum class exportTarget {video, image, pxFmt, aviEnc, mkvEnc, movEnc, mp4Enc, webmEnc};
enum targetRatio {oneToOne, keep, custom};
enum class availableVideoFormats {
    apng, avi, f4v, flv, gif, mkv, mov, mp2, mp4, ogv, swf, webm, webp
};
enum class availableImageFormats{
    bmp, jpeg, jpg, png, ppm, xbm, xpm, tiff, webp
};
enum class pixelFormats{
    Auto, yuv420p, yuva420p, rgb24, rgba, bgr24, bgra, gray
};

enum class aviEncoders { Auto, mpeg2, mpeg4 };
enum class mkvEncoders { Auto, x264, x265, vp8, vp9, av1, ffv1, magicyuv, huffyuv, theora};
enum class movEncoders { Auto, libx264, h264, prores_ks, utvideo};
enum class mp4Encoders { Auto, mpeg4, h264};
enum class webmEncoders { Auto, vp8, vp9};

struct defaultEncoders{
    aviEncoders avi = aviEncoders::Auto;
    mkvEncoders mkv = mkvEncoders::Auto;
    movEncoders mov = movEncoders::Auto;
    mp4Encoders mp4 = mp4Encoders::Auto;
    webmEncoders webm = webmEncoders::Auto;
};

QStringList videoFormats {
    "apng","avi","f4v","flv","gif","mkv","mov","mp2","mp4","ogv","swf","webm","webp"
};
QStringList imageFormats {
    "bmp", "jpeg", "jpg", "png", "ppm", "xbm", "xpm", "tiff", "webp"
};
QStringList pxFormats {
    "Auto", "yuv420p", "yuva420p", "rgb24", "rgba", "bgr24", "bgra", "gray"
};
QStringList aviEnc {
    "Auto", "mpeg2", "mpeg4"
};
QStringList mkvEnc {
    "Auto", "x264", "x265", "vp8", "vp9", "av1", "ffv1", "magicyuv", "huffyuv", "theora"
};
QStringList movEnc {
    "Auto", "libx264", "h264", "prores_ks", "utvideo"
};
QStringList mp4Enc {
    "Auto", "mpeg4", "h264"
};
QStringList webmEnc {
    "Auto", "vp8", "vp9"
};

struct frameExportRange{ int firstFrame = 0; int lastFrame = 0; };

struct GeneralParams {
    QString fileName = "New Project";
    QString exportName = "New Project Export";
    QDir exportDirectory = QDir();
    QDir exportFileName = QDir();
    QString osExportTarget = "";
    int nativeWidth = 0;
    int nativeHeight = 0;
    int exportWidth = 0;
    int exportHeight = 0;
    targetRatio aspectRatio = keep;
    int nativeFPS = 24;
    int fps = 24;
    int bitrate = 0;
    bool allowTransparency = true;
    bool forcePipe = false;
    bool useCustomParam = false;
    bool useIntermediate = true;
    bool usePost = true;
    bool useCustomPalette = false;
    QDir palettePath = QDir();
    QVector<frameExportRange> exportRange = QVector<frameExportRange>();
    QString customInterCommand = "";
    QString customPostCommand = "";
    QVector<int> framesToBeExported = QVector<int>();
    frameExportRange nativeFrameRange = frameExportRange();
    bool exportAllFrames = false;
    bool exportToLast = false;
};

struct VideoParams {
    availableVideoFormats format = availableVideoFormats::gif;
    availableImageFormats intermediateFormat = availableImageFormats::png;
    enum pixelFormats pixelFormat = pixelFormats::Auto;
    defaultEncoders encoders;
};

struct ImageParams {
    availableImageFormats format = availableImageFormats::png;
};

class exportParam {
public:
    exportParam();
    exportTarget exportType = exportTarget::video;
    GeneralParams generalParams;
    // One of these will be ignored on export //
    VideoParams videoParams;
    ImageParams imageParams;
    // They are available for ease of access ///
};

template <typename formatEnum>
QString getFormatAsString(exportTarget target, formatEnum formatIndex){
    switch(target){
    case exportTarget::video:
        return videoFormats[formatIndex];
    case exportTarget::image:
        return imageFormats[formatIndex];
    case exportTarget::pxFmt:
        return pxFormats[formatIndex];
    case exportTarget::aviEnc:
        return aviEnc[formatIndex];
    case exportTarget::mkvEnc:
        return mkvEnc[formatIndex];
    case exportTarget::movEnc:
        return movEnc[formatIndex];
    case exportTarget::mp4Enc:
        return mp4Enc[formatIndex];
    case exportTarget::webmEnc:
        return webmEnc[formatIndex];
    }
    return nullptr;
}
template <typename formatEnum>
formatEnum getFormatAsEnum(exportTarget target, const QString& format){
    switch (target){
    case exportTarget::video:
        return static_cast<formatEnum>(videoFormats.indexOf(format));
    case exportTarget::image:
        return static_cast<formatEnum>(imageFormats.indexOf(format));
    case exportTarget::pxFmt:
        return static_cast<formatEnum>(pxFormats.indexOf(format));
    case exportTarget::aviEnc:
        return static_cast<formatEnum>(aviEnc.indexOf(format));
    case exportTarget::mkvEnc:
        return static_cast<formatEnum>(mkvEnc.indexOf(format));
    case exportTarget::movEnc:
        return static_cast<formatEnum>(movEnc.indexOf(format));
    case exportTarget::mp4Enc:
        return static_cast<formatEnum>(mp4Enc.indexOf(format));
    case exportTarget::webmEnc:
        return static_cast<formatEnum>(webmEnc.indexOf(format));
    default: return static_cast<formatEnum> (0);
    }
}

bool formatAllowsTransparency(const QString& format){
    QStringList formatsWithTransparency{ "apng", "gif", "png", "webp", "webm", "tiff" };
    return formatsWithTransparency.contains(format);
}
bool pixelFormatAllowsTransparency(const QString& format){
    QStringList pxWithTransparency { "Auto", "yuva420p", "rgba", "bgra"};
    return pxWithTransparency.contains(format);
}


QString tr(const QString& str){
    return QCoreApplication::translate("ExportParams", str.toStdString().c_str());
}

bool isExportParamValid(exportParam *exParam, QWidget *widget) {
    QStringList warnings;
    QStringList warningDetail;
    QStringList errors;
    QStringList errorDetail;
    // For convenience
    GeneralParams params(exParam->generalParams);

    if (params.exportHeight == 0) {
        errors.append(tr("Export height is zero"));
        errorDetail.append(tr("Export height was set to zero, please increase the resolution."));
    }
    if (params.exportWidth == 0) {
        errors.append(tr("Export width is zero"));
        errorDetail.append(tr("Export width was set to zero, please increase the resolution."));
    }
    if (params.exportHeight % 2 != 0) {
        warnings.append(tr("Export height contains an odd number."));
        warningDetail.append(
            tr("Height is ") + QString::number(params.exportHeight) + tr(", please consider changing it to ") +
            QString::number(params.exportHeight + 1) + tr(" or to ") + QString::number(params.exportHeight - 1) +
            tr(" to avoid potential issues.")
        );
    }
    if (params.exportWidth % 2 != 0) {
        warnings.append(tr("Export width contains an odd number."));
        warningDetail.append(
            tr("Width is ") + QString::number(params.exportWidth) + tr(", please consider changing it to ") +
            QString::number(params.exportWidth + 1) + tr(" or to ") + QString::number(params.exportWidth - 1) +
            tr(" to avoid potential issues.")
        );
    }
    if (params.fps == 0){
        errors.append(tr("Export FPS is equal to zero."));
        errorDetail.append(tr("Target FPS was set to zero, cannot proceed with a framerate lower than 1."));
    }

    double a = double(params.nativeHeight) / double(params.nativeWidth);
    double b = double(params.exportHeight) / double(params.exportWidth);
    double epsilon = std::numeric_limits<double>::epsilon();
    bool aspectRatioKindaEqual = fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
    if (params.aspectRatio == targetRatio::keep && !aspectRatioKindaEqual) {
        warnings.append(tr("Target aspect ratio is not met"));
        warnings.append(tr("The aspect ratio was set to keep but the aspect ratio of the export is very "
                           "different to that of the original image, consider setting the aspect ratio to \"Custom\" or to change"
                           "the resolution while having \"Keep aspect ratio\" selected.")
        );
    }
    if (!params.exportDirectory.exists()) {
        errors.append(tr("Target directory could not be found"));
        errorDetail.append(tr("The directory \"") + params.osExportTarget + tr("\" could not be found."));
    }
    QFileInfo exDir(params.exportDirectory.absolutePath());
    if (!exDir.isWritable()) {
        errors.append(tr("Target directory is protected"));
        errorDetail.append(
            tr("The directory \"") + params.osExportTarget +
            tr("\" could not be written into as it's protected or otherwise inaccessible, "
               "if you wish to save onto it please launch AnimeEffects as admin.")
        );
    }
    if (params.fps > 10 && params.nativeFPS < 10 && params.fps != 0) {
        warnings.append(tr("Target framerate is low"));
        warningDetail.append(tr(
            "The target framerate for exporting is generally considered very low for "
            "exporting animations and does not to seem intended as the project's fps is higher, consider raising it."
        ));
    }
    if (params.bitrate > 5 * 1000) { // Recommended for standard 720p encoding at 24 fps (5 Mbps)
        warnings.append(tr("Target bitrate is low"));
        warningDetail.append(
            tr("The target bitrate for encoding will generate a low quality export, we "
               "recommend letting FFmpeg decide for you unless you know what you're doing.")
        );
    }

    QString format = exParam->exportType == exportTarget::video
                     ? getFormatAsString(exportTarget::video, (int)exParam->videoParams.format)
                     : getFormatAsString(exportTarget::image, (int)exParam->imageParams.format);
    QString intermediateFormat =  getFormatAsString(exportTarget::image,
                                                    (int)exParam->videoParams.intermediateFormat);
    QString pixelFormat = getFormatAsString(exportTarget::pxFmt,
                                            (int)exParam->videoParams.pixelFormat);

    if(params.allowTransparency && !formatAllowsTransparency(format)){
        warnings.append(tr("Export target does not support transparency"));
        warningDetail.append(tr("Export with transparency was marked but the selected format does not "
                                "support an alpha layer, this means that if the export proceeds it will not have transparency."));
    }
    if(params.allowTransparency && !pixelFormatAllowsTransparency(pixelFormat)){
        warnings.append(tr("Pixel format does not support transparency"));
        warningDetail.append(tr("Export with transparency was marked but the selected pixel format does not "
                                "support alpha channels, this means that if the export proceeds it will not have transparency."));
    }
    if (!params.allowTransparency && pixelFormatAllowsTransparency(pixelFormat) && formatAllowsTransparency(format) &&
        (exParam->exportType != exportTarget::video || formatAllowsTransparency(intermediateFormat))){
        warnings.append(tr("Forced pixel format with transparency"));
        warningDetail.append(
            tr("A specific pixel format with an alpha channel, with all relevant formats having support "
               "for transparency but the option for a transparent export was not selected, please "
               "select it or change your format to one without the 'a' in it to avoid unintentional "
               "transparency.")
        );
    }
    if(exParam->exportType == exportTarget::video) {
        if(params.allowTransparency && params.forcePipe && format == "gif") {
            warnings.append(tr("Export pipe does not support transparency"));
            warningDetail.append(
                tr("Forcing a piped export will make FFmpeg immediately begin the encoding "
                   "process with no intermediate types, this makes it so we cannot gather the necessary components to "
                   "allow for transparency for the gif format.")
            );
        }
        if(params.allowTransparency && !formatAllowsTransparency(intermediateFormat)){
            warnings.append(tr("Intermediate export target does not support transparency"));
            warningDetail.append(tr("Export with transparency was marked but the selected intermediate format "
                                    "does not support an alpha layer, this means that if the export proceeds it will not have transparency."));
        }
    }
    if(params.useCustomPalette && params.palettePath.exists()) {
        if (!params.palettePath.isReadable()) {
            errors.append(tr("Selected palette is unreadable"));
            errorDetail.append(
                tr("The palette located at \"" + params.palettePath.absolutePath() +
                   tr("\" could not be read, please ensure it's a valid file."))
            );
        }
        QFileInfo customPalette(params.palettePath.absolutePath());
        if (!customPalette.suffix().contains(QRegExp("(png|\\.png)"))) {
            errors.append(tr("Selected palette format unsupported"));
            errorDetail.append(
                tr("The palette located at \"" + params.palettePath.absolutePath() +
                   tr("\" seems to not be a PNG file, please convert it into one."))
            );
        }
    }
    else if(!params.palettePath.exists()){
        errors.append(tr("Selected palette does not exist"));
        errorDetail.append(
            tr("The palette referenced at \"" + params.palettePath.absolutePath() +
               tr("\" does not exist or is otherwise inaccessible."))
        );
    }
    if(params.useCustomParam) {
        if (params.customPostCommand.isEmpty() && params.customInterCommand.isEmpty()) {
            warnings.append(tr("Param field is empty"));
            warningDetail.append(
                tr("Export with custom params was selected but both command fields are empty, "
                   "the export process can continue without them should you choose to do so.")
            );
        }
        else if (params.customPostCommand.isEmpty() && params.usePost ||
                 params.customInterCommand.isEmpty() && params.usePost){
            if(params.customPostCommand.isEmpty()) {
                warnings.append(tr("Export with custom post parameter was selected but no command was "
                                   "given."));
                warningDetail.append(tr("Custom post parameter not found, proceeding without it."));
            }
            else{
                warnings.append(tr("Export with custom intermediate parameter was selected but no command was "
                                   "given."));
                warningDetail.append(tr("Custom intermediate parameter not found, proceeding without it."));
            }
        }
        if (!params.useIntermediate && !params.usePost) {
            warnings.append(tr("Custom parameters disabled"));
            warningDetail.append(
                tr("Export with custom params was selected but both command types are disabled, "
                   "the export process can continue as is should you choose to do so.")
            );
        }
    }

    if (params.exportRange.isEmpty()
        || params.exportRange.size() == 1 && params.exportRange.first().firstFrame == 0 && params.exportRange.first().lastFrame == 0
        || params.exportRange.first().firstFrame - params.exportRange.first().lastFrame >= 0){
        errors.append(tr("Export range is empty"));
        errorDetail.append(tr("The export range is either empty, set to zero or lasts less than one frame,"
                              " please select a range of one frame or more."));
    }

    if (params.exportRange.size() > 1) {
        int x = 0;
        for (auto exportRange : params.exportRange) {
            if (exportRange.firstFrame > exportRange.lastFrame) {
                warnings.append(tr("Export range at index ") + QString::number(x) + tr(" is invalid, swapping."));
                int firstFrame = exportRange.lastFrame;
                int lastFrame = exportRange.firstFrame;
                params.exportRange[x].firstFrame = firstFrame;
                params.exportRange[x].lastFrame = lastFrame;
                warningDetail.append(tr("Swapped frames from range at index ") + QString::number(x) + tr("."));
            }
            if (exportRange.firstFrame == exportRange.lastFrame) {
                warnings.append(tr("Export range at index " + QString::number(x) + tr(" is the same, removing.")));
                params.exportRange.removeAt(x);
                x--; // To compensate for the deletion range
                warningDetail.append(tr("Removing zero frame range at ") + QString::number(x) + tr("."));
            }
            x++;
        }
    }

    for(auto range : params.exportRange){
        int rangeLow = range.firstFrame;
        while(rangeLow > range.lastFrame + 1){
            params.framesToBeExported.append(rangeLow);
            rangeLow++;
        }
    }
    // Hopefully remove duplicates
    std::sort(params.framesToBeExported.begin(), params.framesToBeExported.end());
    params.framesToBeExported.erase(std::unique(params.framesToBeExported.begin(), params.framesToBeExported.end()),
                                    params.framesToBeExported.end());
    // Check if the user is exporting all frames
    int summedNative = params.nativeFrameRange.firstFrame + params.nativeFrameRange.lastFrame;
    int summedExport = params.framesToBeExported.first() + params.framesToBeExported.last();
    if (summedNative == summedExport){
        params.exportAllFrames = true;
    }
    // If only one range check if it's a continuous export
    if (params.exportRange.size() == 1 && params.exportRange.first().firstFrame == 0
        && params.exportRange.first().lastFrame != params.nativeFrameRange.lastFrame){
        params.exportToLast = true;
    }
    QSettings settings;
    QVariant customEncoderWarning = settings.value("export_custom_encoder_warning_shown");
    if(!customEncoderWarning.isValid()){
        settings.setValue("export_custom_encoder_warning_shown", false);
        settings.sync();
    }
    bool customEncoderWarningShown = customEncoderWarning.toBool();
    auto encoders = exParam->videoParams.encoders;
    bool isUsingCustomEncoder =
        encoders.webm != webmEncoders::Auto || encoders.mp4 != mp4Encoders::Auto || encoders.mkv != mkvEncoders::Auto
        || encoders.avi != aviEncoders::Auto || encoders.mov != movEncoders::Auto;
    if(!customEncoderWarningShown && isUsingCustomEncoder){
        warnings.append(tr("Using custom encoder"));
        warningDetail.append(tr("\nThis warning will only be shown once."
                                "You're using a custom encoder for one of our supported formats, these may "
                                "not support some of the features you may be expecting out of your export such as transparency, please "
                                "learn about the features, advantages and disadvantages of your selected encoder before using it."));
    }
    // Sync changes made by the error handler
    exParam->generalParams = params;
    bool proceedToExport = true;
    bool shouldDisplayErrors = !errors.empty() || !warnings.empty();
    if(shouldDisplayErrors){
        QMessageBox msg;
        bool error = !errors.empty();
        msg.setIcon(error? QMessageBox::Icon::Critical : QMessageBox::Warning);
        msg.setWindowTitle(error? tr("Critical errors have been found, cannot continue exporting.")
                                : tr("Minor errors have been found, please review."));
        QString conMsg = error? tr("You cannot proceed with the export until the critical issues found have been "
                                   "resolved.")
                              : tr("You may ignore the errors and proceed should you choose to, click \"Ok\" "
                                   "to export anyway or \"Cancel\" if you wish to cancel the export.");
        QString textMsg =
            tr("Some issues have been found while exporting, you can review them down bellow:\nCritical errors: ")
            + QString::number(errors.size()) + tr(" | Warnings: ") + QString::number(warnings.size()) + "\n-----\n"
            + conMsg;
        msg.setText(textMsg);
        QString detMsg = "-----\n" + tr("Critical errors (" + QString::number(errors.size()) +"): \n");
        if(!errors.empty()){
            int x = 0;
            for(const auto& err: errors){
                detMsg.append("-----\n" + tr("Error: ") + err + "\n");
                detMsg.append(tr("Error detail: ") + errorDetail[x] + "\n");
                x++;
            }
        }
        else{ detMsg.append("-----\n" + tr("No errors found.\n")); }

        detMsg.append("-----\n" + tr("Warnings (" + QString::number(warnings.size()) + "): \n"));
        if(!warnings.empty()){
            int x = 0;
            for(const auto& warn : warnings){
                detMsg.append(tr("Warning: ") + warn + "\n");
                detMsg.append(tr("Warning detail: ") + warningDetail[x] + "\n");
                x++;
            }
        }
        else{ detMsg.append("-----\n" + tr("No warnings found.\n")); }
        msg.setDetailedText(detMsg);
        msg.setStandardButtons(error? QMessageBox::Cancel : QMessageBox::Ok | QMessageBox::Cancel);
        msg.setParent(widget);
        msg.setWindowFlag(Qt::Window);
        int ret = msg.exec();
        if(ret == QMessageBox::Cancel){
            proceedToExport = false;
        }
    }
    return proceedToExport;
}

exportParam::exportParam(){
    exportType = exportTarget::video;
}

// Sorry for this

namespace exportUI {
    class form {
    public:
        QGridLayout* gridLayout;
        QLabel* loadingMessage;
        QLabel* frameCounter;
        QProgressBar* progressBar;
        QLabel* pixmapLabel;

        void setupUi(QWidget* Form) {
            if (Form->objectName().isEmpty())
                Form->setObjectName(QString::fromUtf8("Form"));
            Form->resize(500, 450);
            gridLayout = new QGridLayout(Form);
            gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
            loadingMessage = new QLabel(Form);
            loadingMessage->setObjectName(QString::fromUtf8("loadingMessage"));
            QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(loadingMessage->sizePolicy().hasHeightForWidth());
            loadingMessage->setSizePolicy(sizePolicy);

            gridLayout->addWidget(loadingMessage, 0, 0, 1, 1);

            frameCounter = new QLabel(Form);
            frameCounter->setObjectName(QString::fromUtf8("frameCounter"));
            QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
            sizePolicy1.setHorizontalStretch(0);
            sizePolicy1.setVerticalStretch(0);
            sizePolicy1.setHeightForWidth(frameCounter->sizePolicy().hasHeightForWidth());
            frameCounter->setSizePolicy(sizePolicy1);

            gridLayout->addWidget(frameCounter, 4, 0, 1, 1);

            progressBar = new QProgressBar(Form);
            progressBar->setObjectName(QString::fromUtf8("progressBar"));
            QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
            sizePolicy2.setHorizontalStretch(0);
            sizePolicy2.setVerticalStretch(0);
            sizePolicy2.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
            progressBar->setSizePolicy(sizePolicy2);
            progressBar->setValue(0);

            gridLayout->addWidget(progressBar, 1, 0, 1, 1);

            pixmapLabel = new QLabel(Form);
            pixmapLabel->setObjectName(QString::fromUtf8("pixmapLabel"));
            QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
            sizePolicy3.setHorizontalStretch(0);
            sizePolicy3.setVerticalStretch(0);
            sizePolicy3.setHeightForWidth(pixmapLabel->sizePolicy().hasHeightForWidth());
            pixmapLabel->setSizePolicy(sizePolicy3);

            gridLayout->addWidget(pixmapLabel, 2, 0, 2, 1);


            retranslateUi(Form);

            QMetaObject::connectSlotsByName(Form);
        } // setupUi

        void retranslateUi(QWidget* Form) const {
            Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
            loadingMessage->setText(QCoreApplication::translate(
                "Form", "<html><head/><body><p align=\"center\">Loading</p></body></html>", nullptr
            ));
            frameCounter->setText(QCoreApplication::translate(
                "Form", "<html><head/><body><p align=\"center\">Frame rendered x/y | Frame exported x/y</p></body></html>", nullptr
            ));
            pixmapLabel->setText(QCoreApplication::translate(
                "Form", "<html><head/><body><p align=\"center\">Pixmap goes here</p></body></html>", nullptr
            ));
        } // retranslateUi
    };
    bool overwrite() {
        QMessageBox msgBox;
        msgBox.setText(tr("File already exists."));
        msgBox.setInformativeText(tr("Do you want to overwrite the existing file?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        return (ret == QMessageBox::Ok);
    };
}

namespace ffmpeg {
    struct ffmpegExec{
        QString executable;
        int tick = 0;
        bool finished = false;
        bool errored = false;
        QStringList log; // Shown in a messagebox after export
        QString errorLog; // ^^
    };
}

namespace exportRender {
enum ExporterResult{
    ExportSuccess,
    ExportWriteError,
    ExportArgError,
    ExportCanceled,
    ExportUnknownError
    
};

class Exporter{
    public:
        Exporter(core::Project& aProject);
        ~Exporter();

        typedef std::unique_ptr<QOpenGLFramebufferObject> FramebufferPtr;
        typedef std::list<FramebufferPtr> FramebufferList;
        FramebufferList mFramebuffers;
        QScopedPointer<core::ClippingFrame> mClippingFrame;
        QScopedPointer<core::DestinationTexturizer> mDestinationTexturizer;
        gl::EasyTextureDrawer mTextureDrawer;
        core::TimeInfo mOriginTimeInfo;
        QScopedPointer<QProcess> mProcess;
        ffmpeg::ffmpegExec ffmpegExec;
        core::Project& mProject;

        void finish(){
            Q_UNIMPLEMENTED();
        }
        void destroyFramebuffers(){
            for (auto& fbo : mFramebuffers) {
                fbo.reset();
            }
        }

};
Exporter::Exporter(core::Project& aProject):
    mFramebuffers(),
    mClippingFrame(),
    mDestinationTexturizer(),
    mTextureDrawer(),
    mOriginTimeInfo(),
    mProject(aProject) {}
Exporter::~Exporter() {
        finish();
        // kill buffer
        gl::Global::makeCurrent();
        destroyFramebuffers();
}
}


#endif // ANIMEEFFECTS_EXPORTPARAMS_H
