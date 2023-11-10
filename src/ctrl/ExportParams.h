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
#include <QSize>
#include <QFileInfo>
#include <QProcess>
#include <QOpenGLFramebufferObject>
#include <utility>
#include "qdir.h"
#include "qmessagebox.h"
#include "gl/Global.h"
#include "util/Range.h"
#include "gl/EasyTextureDrawer.h"
#include "core/Project.h"
#include "core/TimeInfo.h"
#include "core/TimeKeyBlender.h"
#include "core/ClippingFrame.h"
#include "core/DestinationTexturizer.h"
#include "ctrl/VideoFormat.h"
#include "gl/Util.h"


#include <QBuffer>
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
template<typename formatEnum>
formatEnum getFormatAsEnum(exportTarget target, const QString& format) {
    switch (target) {
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
    default:
        return static_cast<formatEnum>(0);
    }
}

inline bool formatAllowsTransparency(const QString& format) {
    QStringList formatsWithTransparency{"apng", "gif", "png", "webp", "webm", "tiff"};
    return formatsWithTransparency.contains(format);
}
inline bool pixelFormatAllowsTransparency(const QString& format) {
    QStringList pxWithTransparency{"Auto", "yuva420p", "rgba", "bgra"};
    return pxWithTransparency.contains(format);
}


inline QString tr(const QString& str) { return QCoreApplication::translate("ExportParams", str.toStdString().c_str()); }

inline bool isExportParamValid(exportParam* exParam, QWidget* widget) {
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
    if (params.fps == 0) {
        errors.append(tr("Export FPS is equal to zero."));
        errorDetail.append(tr("Target FPS was set to zero, cannot proceed with a framerate lower than 1."));
    }

    double a = static_cast<double>(params.nativeHeight) / static_cast<double>(params.nativeWidth);
    double b = static_cast<double>(params.exportHeight) / static_cast<double>(params.exportWidth);
    double epsilon = std::numeric_limits<double>::epsilon();
    bool aspectRatioKindaEqual = fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
    if (params.aspectRatio == targetRatio::keep && !aspectRatioKindaEqual) {
        warnings.append(tr("Target aspect ratio is not met"));
        warnings.append(
            tr("The aspect ratio was set to keep but the aspect ratio of the export is very "
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
        ? getFormatAsString(exportTarget::video, static_cast<int>(exParam->videoParams.format))
        : getFormatAsString(exportTarget::image, static_cast<int>(exParam->imageParams.format));
    QString intermediateFormat =
        getFormatAsString(exportTarget::image, static_cast<int>(exParam->videoParams.intermediateFormat));
    QString pixelFormat = getFormatAsString(exportTarget::pxFmt, static_cast<int>(exParam->videoParams.pixelFormat));

    if (params.allowTransparency && !formatAllowsTransparency(format)) {
        warnings.append(tr("Export target does not support transparency"));
        warningDetail.append(
            tr("Export with transparency was marked but the selected format does not "
               "support an alpha layer, this means that if the export proceeds it will not have transparency.")
        );
    }
    if (params.allowTransparency && !pixelFormatAllowsTransparency(pixelFormat)) {
        warnings.append(tr("Pixel format does not support transparency"));
        warningDetail.append(
            tr("Export with transparency was marked but the selected pixel format does not "
               "support alpha channels, this means that if the export proceeds it will not have transparency.")
        );
    }
    if (!params.allowTransparency && pixelFormatAllowsTransparency(pixelFormat) && formatAllowsTransparency(format) &&
        (exParam->exportType != exportTarget::video || formatAllowsTransparency(intermediateFormat))) {
        warnings.append(tr("Forced pixel format with transparency"));
        warningDetail.append(
            tr("A specific pixel format with an alpha channel, with all relevant formats having support "
               "for transparency but the option for a transparent export was not selected, please "
               "select it or change your format to one without the 'a' in it to avoid unintentional "
               "transparency.")
        );
    }
    if (exParam->exportType == exportTarget::video) {
        if (params.allowTransparency && params.forcePipe && format == "gif") {
            warnings.append(tr("Export pipe does not support transparency"));
            warningDetail.append(
                tr("Forcing a piped export will make FFmpeg immediately begin the encoding "
                   "process with no intermediate types, this makes it so we cannot gather the necessary components to "
                   "allow for transparency for the gif format.")
            );
        }
        if (params.allowTransparency && !formatAllowsTransparency(intermediateFormat)) {
            warnings.append(tr("Intermediate export target does not support transparency"));
            warningDetail.append(tr(
                "Export with transparency was marked but the selected intermediate format "
                "does not support an alpha layer, this means that if the export proceeds it will not have transparency."
            ));
        }
    }
    if (params.useCustomPalette && params.palettePath.exists()) {
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
    } else if (!params.palettePath.exists()) {
        errors.append(tr("Selected palette does not exist"));
        errorDetail.append(
            tr("The palette referenced at \"" + params.palettePath.absolutePath() +
               tr("\" does not exist or is otherwise inaccessible."))
        );
    }
    if (params.useCustomParam) {
        if (params.customPostCommand.isEmpty() && params.customInterCommand.isEmpty()) {
            warnings.append(tr("Param field is empty"));
            warningDetail.append(
                tr("Export with custom params was selected but both command fields are empty, "
                   "the export process can continue without them should you choose to do so.")
            );
        } else if (params.customPostCommand.isEmpty() && params.usePost || params.customInterCommand.isEmpty() && params.usePost) {
            if (params.customPostCommand.isEmpty()) {
                warnings.append(
                    tr("Export with custom post parameter was selected but no command was "
                       "given.")
                );
                warningDetail.append(tr("Custom post parameter not found, proceeding without it."));
            } else {
                warnings.append(
                    tr("Export with custom intermediate parameter was selected but no command was "
                       "given.")
                );
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

    if (params.exportRange.isEmpty() ||
        params.exportRange.size() == 1 && params.exportRange.first().firstFrame == 0 &&
            params.exportRange.first().lastFrame == 0 ||
        params.exportRange.first().firstFrame - params.exportRange.first().lastFrame >= 0) {
        errors.append(tr("Export range is empty"));
        errorDetail.append(
            tr("The export range is either empty, set to zero or lasts less than one frame,"
               " please select a range of one frame or more.")
        );
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

    for (auto range : params.exportRange) {
        int rangeLow = range.firstFrame;
        while (rangeLow > range.lastFrame + 1) {
            params.framesToBeExported.append(rangeLow);
            rangeLow++;
        }
    }
    // Hopefully remove duplicates
    std::sort(params.framesToBeExported.begin(), params.framesToBeExported.end());
    params.framesToBeExported.erase(
        std::unique(params.framesToBeExported.begin(), params.framesToBeExported.end()), params.framesToBeExported.end()
    );
    // Check if the user is exporting all frames
    int summedNative = params.nativeFrameRange.firstFrame + params.nativeFrameRange.lastFrame;
    int summedExport = params.framesToBeExported.first() + params.framesToBeExported.last();
    if (summedNative == summedExport) {
        params.exportAllFrames = true;
    }
    // If only one range check if it's a continuous export
    if (params.exportRange.size() == 1 && params.exportRange.first().firstFrame == 0 &&
        params.exportRange.first().lastFrame != params.nativeFrameRange.lastFrame) {
        params.exportToLast = true;
    }
    QSettings settings;
    QVariant customEncoderWarning = settings.value("export_custom_encoder_warning_shown");
    if (!customEncoderWarning.isValid()) {
        settings.setValue("export_custom_encoder_warning_shown", false);
        settings.sync();
    }
    bool customEncoderWarningShown = customEncoderWarning.toBool();
    auto encoders = exParam->videoParams.encoders;
    bool isUsingCustomEncoder = encoders.webm != webmEncoders::Auto || encoders.mp4 != mp4Encoders::Auto ||
        encoders.mkv != mkvEncoders::Auto || encoders.avi != aviEncoders::Auto || encoders.mov != movEncoders::Auto;
    if (!customEncoderWarningShown && isUsingCustomEncoder) {
        warnings.append(tr("Using custom encoder"));
        warningDetail.append(
            tr("\nThis warning will only be shown once."
               "You're using a custom encoder for one of our supported formats, these may "
               "not support some of the features you may be expecting out of your export such as transparency, please "
               "learn about the features, advantages and disadvantages of your selected encoder before using it.")
        );
    }
    // Sync changes made by the error handler
    exParam->generalParams = params;
    bool proceedToExport = true;
    bool shouldDisplayErrors = !errors.empty() || !warnings.empty();
    if (shouldDisplayErrors) {
        QMessageBox msg;
        bool error = !errors.empty();
        msg.setIcon(error ? QMessageBox::Icon::Critical : QMessageBox::Warning);
        msg.setWindowTitle(
            error ? tr("Critical errors have been found, cannot continue exporting.")
                  : tr("Minor errors have been found, please review.")
        );
        QString conMsg = error ? tr("You cannot proceed with the export until the critical issues found have been "
                                    "resolved.")
                               : tr("You may ignore the errors and proceed should you choose to, click \"Ok\" "
                                    "to export anyway or \"Cancel\" if you wish to cancel the export.");
        QString textMsg =
            tr("Some issues have been found while exporting, you can review them down bellow:\nCritical errors: ") +
            QString::number(errors.size()) + tr(" | Warnings: ") + QString::number(warnings.size()) + "\n-----\n" +
            conMsg;
        msg.setText(textMsg);
        QString detMsg = "-----\n" + tr("Critical errors (" + QString::number(errors.size()) + "): \n");
        if (!errors.empty()) {
            int x = 0;
            for (const auto& err : errors) {
                detMsg.append("-----\n" + tr("Error: ") + err + "\n");
                detMsg.append(tr("Error detail: ") + errorDetail[x] + "\n");
                x++;
            }
        } else {
            detMsg.append("-----\n" + tr("No errors found.\n"));
        }

        detMsg.append("-----\n" + tr("Warnings (" + QString::number(warnings.size()) + "): \n"));
        if (!warnings.empty()) {
            int x = 0;
            for (const auto& warn : warnings) {
                detMsg.append(tr("Warning: ") + warn + "\n");
                detMsg.append(tr("Warning detail: ") + warningDetail[x] + "\n");
                x++;
            }
        } else {
            detMsg.append("-----\n" + tr("No warnings found.\n"));
        }
        msg.setDetailedText(detMsg);
        msg.setStandardButtons(error ? QMessageBox::Cancel : QMessageBox::Ok | QMessageBox::Cancel);
        msg.setParent(widget);
        msg.setWindowFlag(Qt::Window);
        int ret = msg.exec();
        if (ret == QMessageBox::Cancel) {
            proceedToExport = false;
        }
    }
    return proceedToExport;
}

inline exportParam::exportParam(){
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
    QPushButton* cancelButton;

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

        cancelButton = new QPushButton();
        cancelButton->setParent(Form);
        QSizePolicy sizePol1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePol1.setHorizontalStretch(0);
        sizePol1.setVerticalStretch(0);
        sizePol1.setHeightForWidth(frameCounter->sizePolicy().hasHeightForWidth());
        cancelButton->setSizePolicy(sizePol1);

        gridLayout->addWidget(cancelButton, 4, 0, 1, 1);


        frameCounter = new QLabel(Form);
        frameCounter->setObjectName(QString::fromUtf8("frameCounter"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frameCounter->sizePolicy().hasHeightForWidth());
        frameCounter->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(frameCounter, 5, 0, 1, 1);

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
        cancelButton->setText(QCoreApplication::translate(
            "Form", "<html><head/><body><p align=\"center\">Cancel</p></body></html>", nullptr
        ));
        frameCounter->setText(QCoreApplication::translate(
            "Form",
            "<html><head/><body><p align=\"center\">Frame rendered x/y | Frame exported x/y</p></body></html>",
            nullptr
        ));
        pixmapLabel->setText(QCoreApplication::translate(
            "Form", "<html><head/><body><p align=\"center\">Pixmap goes here</p></body></html>", nullptr
        ));
    } // retranslateUi
};
inline bool overwrite(const QString& path) {
        QMessageBox msgBox;
        msgBox.setText(tr("File already exists."));
        msgBox.setInformativeText(tr("Do you want to overwrite the file") + path + tr("?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        return ret == QMessageBox::Ok;
    }
}

namespace ffmpeg {
    enum exportResult {
    ExportNotInitialized,
    ExportOngoing,
    ExportSuccess,
    ExportCanceled,
    ExportWriteError,
    ExportArgError,
    ExportUnknownError
};
inline bool isExportFinished(const exportResult& result){
        if(result == ExportNotInitialized || result == ExportOngoing){
            return false;
        }
        return true;
    }
inline bool export_errored(const exportResult& result) {
    if(result == ExportWriteError || result == ExportArgError || result == ExportUnknownError) {
        return true;
    }
    return false;
}
    // "Weird spacing it is" -CLion 2023
    struct ffmpegObject {
        QString executable;
        QString argument;
        int tick = 0;
        exportResult result = ExportNotInitialized;
        QStringList log; // Shown in a messagebox after export
        QStringList errorLog; // Same as above
    };
    inline bool isAuto(const QString& str){
        if(str == "Auto" || str == "auto"){
            return true;
        }
        return false;
    }
    // Only animations get to build arguments so don't worry about image output
    inline QString buildPipedArgument(const exportParam& exParam, bool loopGif){
        // Default
        QString argument = " -y -f image2pipe -i -";
        // FPS
        argument.append(" -r " + QString::number(exParam.generalParams.fps));
        // Codec
        switch(exParam.videoParams.format){
        case(availableVideoFormats::avi):
            if(isAuto(aviEnc[static_cast<int>(exParam.videoParams.encoders.avi)])) { break; }
            argument.append(" -c:v " + aviEnc[static_cast<int>(exParam.videoParams.encoders.avi)]);
            break;
        case(availableVideoFormats::mkv):
            if(isAuto(mkvEnc[static_cast<int>(exParam.videoParams.encoders.mkv)])) { break; }
            argument.append(" -c:v " + mkvEnc[static_cast<int>(exParam.videoParams.encoders.mkv)]);
            break;
        case(availableVideoFormats::mov):
            if(isAuto(movEnc[static_cast<int>(exParam.videoParams.encoders.mov)])) { break; }
            argument.append(" -c:v " + movEnc[static_cast<int>(exParam.videoParams.encoders.mov)]);
            break;
        case(availableVideoFormats::mp4):
            if(isAuto(mp4Enc[static_cast<int>(exParam.videoParams.encoders.mp4)])) { break; }
            argument.append(" -c:v " + mp4Enc[static_cast<int>(exParam.videoParams.encoders.mp4)]);
            break;
        case(availableVideoFormats::webm):
            if(isAuto(webmEnc[static_cast<int>(exParam.videoParams.encoders.webm)])) { break; }
            argument.append(" -c:v " + webmEnc[static_cast<int>(exParam.videoParams.encoders.webm)]);
            break;
        default:
            break; // We leave it to ffmpeg
        }
        // Bitrate
        argument.append("\n-b:v " + QString::number(exParam.generalParams.bitrate));

        // Pixel format
        if(isAuto(pxFormats[static_cast<int>(exParam.videoParams.pixelFormat)])){
            if(exParam.generalParams.allowTransparency){
                if(formatAllowsTransparency(videoFormats[static_cast<int>(exParam.videoParams.format)])){
                    argument.append(" -pix_fmt yuva420p");
                }
                else{ argument.append(" -pix_fmt yuv420p"); }
            }
            else{ argument.append(" -pix_fmt yuv420p"); }
        }
        else{
            argument.append(" -pix_fmt " + pxFormats[static_cast<int>(exParam.videoParams.pixelFormat)]);
        }
        // Format
        argument.append(" -f " + videoFormats[static_cast<int>(exParam.videoParams.format)]);

        // Custom palette. To be completely honest with you, I've no idea the difference between these.
        if(exParam.generalParams.useCustomPalette){
            argument.append(" -i \"" + exParam.generalParams.palettePath.absolutePath() + "\"");
            if(exParam.videoParams.format == availableVideoFormats::gif && exParam.generalParams.allowTransparency){
                argument.append( " -lavfi paletteuse=alpha_threshold=128 -gifflags -offsetting");
            }
            else {
                argument.append(" -filter_complex \"paletteuse\"");
            }
        }
        else if(exParam.videoParams.format == availableVideoFormats::gif){
            if(exParam.generalParams.allowTransparency) {
                argument.append(
                    " -lavfi split[v],palettegen,[v]paletteuse"
                );
            }
            else{
                argument.append(
                    " -filter_complex\"split [a][b];[a] palettegen=stats_mode=single [p];[b][p] paletteuse=new=1\""
                );
            }
        }

        // Looping gif
        if(exParam.videoParams.format == availableVideoFormats::gif){
            if(loopGif) { argument.append(" -loop 0"); }
            else { argument.append(" -loop 1"); }
        }
        // Output
        argument.append(" -o \"" + exParam.generalParams.exportFileName.absolutePath()) + "\"";

        return argument;
    }
}

namespace projectExporter {
class Exporter {
public:
    Exporter(core::Project& aProject, QDialog* widget, const exportParam& exParam, ffmpeg::ffmpegObject& ffmpeg);
    ~Exporter();
    // Class initializers and other stuff
    typedef std::unique_ptr<QOpenGLFramebufferObject> FramebufferPtr;
    typedef std::list<FramebufferPtr> FramebufferList;
    FramebufferList mFramebuffers;
    QScopedPointer<core::ClippingFrame> mClippingFrame;
    QScopedPointer<core::DestinationTexturizer> mDestinationTexturizer;
    gl::EasyTextureDrawer mTextureDrawer;
    core::TimeInfo mOriginTimeInfo;
    QScopedPointer<QProcess> mProcess;
    ffmpeg::ffmpegObject& export_obj;
    core::Project& mProject;
    QDialog* mWidget;
    const exportParam& mParam;
    int mIndex;
    int mDigitCount;
    int mFIndex;
    int mFrameCount;
    float mProgress;
    bool mExporting;
    bool mCancelled;
    bool mSaveAsImage;
    bool fpsIsConsistent;
    bool mOverwriteConfirmation;
    QImage currentFrame;
    QString dirAddress;
    QString imageSuffix;

    enum ResultType { Success, Cancelled, Errored, Queued, Ongoing };

    struct ExportResult {
        ResultType resultType = Queued;
        QString errorType;
    };

    void finish() { Q_UNIMPLEMENTED(); }
    void destroyFramebuffers() {
        for (auto& fbo : mFramebuffers) {
            fbo.reset();
        }
    }

    static void setTextureParam(QOpenGLFramebufferObject& aFbo) {
        auto id = aFbo.texture();
        gl::Global::Functions& ggl = gl::Global::functions();
        ggl.glBindTexture(GL_TEXTURE_2D, id);
        ggl.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        ggl.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ggl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ggl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        ggl.glBindTexture(GL_TEXTURE_2D, 0);
    }
    void createFramebuffers(const QSize& aOriginSize, const QSize& aExportSize) {
        destroyFramebuffers();
        mFramebuffers.emplace_back(std::make_unique<QOpenGLFramebufferObject>(aOriginSize));
        // setup buffers for scaling
        if (aOriginSize != aExportSize) {
            static constexpr int kMaxCount = 3;
            QSize size = aOriginSize;

            for (int i = 0; i < kMaxCount; ++i) {
                const double scaleX = aExportSize.width() / static_cast<double>(size.width());
                const double scaleY = aExportSize.height() / static_cast<double>(size.height());
                const double scaleMax = std::max(scaleX, scaleY);

                if (scaleMax >= 0.5 || i == kMaxCount - 1) {
                    mFramebuffers.emplace_back(std::make_unique<QOpenGLFramebufferObject>(aExportSize));
                } else {
                    size.setWidth(static_cast<int>(size.width() * 0.5));
                    size.setHeight(static_cast<int>(size.height() * 0.5));
                    mFramebuffers.emplace_back(std::make_unique<QOpenGLFramebufferObject>(size));
                }
            }
        }
        // setup texture
        for (auto& fbo : mFramebuffers) {
            setTextureParam(*fbo);
        }
    }
    ResultType initialize() {
        // Reset values
        mIndex = 0;
        mProgress = 0.0f;
        mDigitCount = 0;
        // Value initialization
        mFrameCount = mParam.generalParams.framesToBeExported.count();
        fpsIsConsistent = mParam.generalParams.nativeFPS == mParam.generalParams.fps;
        mSaveAsImage = mParam.exportType == exportTarget::image;
        int ffc = mFrameCount;
        while (ffc /= 10) {
            mDigitCount++;
        }
        if(mSaveAsImage) {
            QDir dir = mParam.generalParams.exportDirectory.absolutePath();
            if (!QDir(dirAddress).exists()) {
                if (!dir.mkdir(mParam.generalParams.exportName)) {
                    export_obj.errorLog.append("Could not create directory for export");
                    return Errored;
                }
            }
        }
        dirAddress = mParam.generalParams.exportDirectory.absolutePath() + "/" + mParam.generalParams.exportName;
        imageSuffix = imageFormats[static_cast<int>(mParam.imageParams.format)];
        // Initialize graphics
        {
            gl::Global::makeCurrent();
            // texture drawer
            if (!mTextureDrawer.init()) {
                export_obj.errorLog.append("Failed to initialize TextureDrawer");
                return Errored;
            }
            // framebuffers
            const QSize size(mParam.generalParams.exportWidth, mParam.generalParams.exportHeight);
            createFramebuffers(mProject.attribute().imageSize(), size);
            // clipping frame
            mClippingFrame.reset(new core::ClippingFrame());
            mClippingFrame->resize(mProject.attribute().imageSize());
            // create texturizer for destination colors of the framebuffer
            mDestinationTexturizer.reset(new core::DestinationTexturizer());
            mDestinationTexturizer->resize(mProject.attribute().imageSize());
        }
        mExporting = true;
        return Success;
    }
    // html centering because there is no "center text" function :(
    QString sCenterL = "<html><head/><body><p align=\"center\">";
    QString sCenterR = "</p></body></html>";
    // message generation
    QString getProgressMessage(int dotTick) const {
        QString pDots;
        const QString currentStatus = mIndex == mFrameCount ? tr("Rendering") : tr("Exporting");
        if (dotTick == 0) {
            return sCenterL + currentStatus + sCenterR;
        }
        for (int x = 0; x < dotTick; x++) {
            pDots.append(".");
        }
        return sCenterL + currentStatus + pDots + sCenterR;
    }
    QString getFrameMessage() const {
        QString rString = sCenterL + tr("Frame rendered: ") + QString::number(mIndex) + "/" +
            QString::number(mFrameCount) + " | " + tr("Frame exported: ") + QString::number(mFIndex) + "/" +
            QString::number(mFrameCount) + sCenterR;
        return rString;
    }

    bool checkOverwriting(const QFileInfo& aPath) {
        if (!mOverwriteConfirmation && aPath.exists()) {
            if (aPath.isDir() || !exportUI::overwrite(aPath.filePath())) {
                return false;
            }
            mOverwriteConfirmation = true;
        }
        return true;
    }

    bool decideImagePath(int aIndex, QFileInfo& aPath) {
        QString number = QString("%1").arg(aIndex, mDigitCount, 10, QChar('0'));
        QFileInfo filePath(dirAddress + "/" + mParam.generalParams.exportName + number + "." + imageSuffix);
        // check overwrite
        if (!checkOverwriting(filePath)) {
            return false;
        }
        aPath = filePath;
        return true;
    }
    void write(const QByteArray& bytes) const {
        XC_ASSERT(mProcess);
        mProcess->write(bytes);
    }

    bool exportImage(const QImage& aFboImage, int aIndex) {
        if (mSaveAsImage) {
            QFileInfo savePath;
            if (!decideImagePath(aIndex, savePath)) {
                export_obj.errorLog.append("Unable to get image path");
                return false;
            }
            bool saveImage = aFboImage.save(
                savePath.filePath(),
                imageFormats[static_cast<int>(mParam.imageParams.format)].toUpper().toStdString().c_str()
            );
            if (!saveImage) {
                export_obj.errorLog.append("Unable to save image");
                return false;
            }
        }
        else {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::ReadWrite);
            aFboImage.save(
                &buffer, videoFormats[static_cast<int>(mParam.videoParams.format)].toStdString().c_str(), -1
                );
            buffer.close();
            write(byteArray);
            if (export_errored(export_obj.result)) {
                export_obj.errorLog.append("FFmpeg error occurred.\n" + export_obj.errorLog.join("\n"));
                return false;
            }
        }
        return true;
    }

    bool updateTime(core::TimeInfo& aDst) {
        aDst = mOriginTimeInfo;
        double current;
        if (fpsIsConsistent) {
            current = mIndex;
        } else {
            current = mIndex * mOriginTimeInfo.fps / static_cast<double>(mParam.generalParams.fps);
        }
        double frame = mParam.generalParams.framesToBeExported.first() + current;
        // end of export
        if (0 < mIndex && mParam.generalParams.framesToBeExported.last() < frame) {
            return false;
        }
        if (mOriginTimeInfo.frameMax < static_cast<int>(frame)) {
            return false;
        }
        aDst.frame = core::Frame::fromDecimal(static_cast<float>(frame));
        mProgress = static_cast<float>(current) / static_cast<float>(mFrameCount);
        // to next index
        ++mIndex;
        return true;
    }

    bool update() {
        if (!mExporting) {
            return false;
        }
        const int currentIndex = mIndex;
        // setup parameter
        core::TimeInfo timeInfo;
        if (!updateTime(timeInfo)) {
            return false;
        }
        if (mParam.generalParams.exportAllFrames ||
            mParam.generalParams.framesToBeExported.contains(timeInfo.frame.get())) {
            // begin rendering
            gl::Global::makeCurrent();
            gl::Global::Functions& ggl = gl::Global::functions();
            const QSize originSize = mProject.attribute().imageSize();
            // clear clipping
            mClippingFrame->clearTexture();
            mClippingFrame->resetClippingId();
            // clear destination texture
            mDestinationTexturizer->clearTexture();
            // bind framebuffer
            if (!mFramebuffers.front()->bind()) {
                XC_FATAL_ERROR("OpenGL Error", "Failed to bind framebuffer.", "");
            }
            // setup
            gl::Util::setViewportAsActualPixels(originSize);
            gl::Util::clearColorBuffer(0.0, 0.0, 0.0, 0.0);
            gl::Util::resetRenderState();
            // render
            core::RenderInfo renderInfo;
            renderInfo.camera.reset(originSize, 1.0, originSize, QPoint());
            renderInfo.time = timeInfo;
            renderInfo.framebuffer = mFramebuffers.front()->handle();
            renderInfo.dest = mFramebuffers.front()->texture();
            renderInfo.isGrid = false;
            renderInfo.clippingId = 0;
            renderInfo.clippingFrame = mClippingFrame.data();
            renderInfo.destTexturizer = mDestinationTexturizer.data();
            // assert info and render
            XC_ASSERT(renderInfo.framebuffer != 0);
            XC_ASSERT(renderInfo.dest != 0);
            mProject.objectTree().render(renderInfo, true);
            // unbind framebuffer
            if (!mFramebuffers.front()->release()) {
                XC_FATAL_ERROR("OpenGL Error", "Failed to bind framebuffer.", "");
            }
            // manage scaling
            {
                QOpenGLFramebufferObject* prev = nullptr;
                for (auto& fbo : mFramebuffers) {
                    if (prev) {
                        fbo->bind();
                        const QSize size = fbo->size();
                        gl::Util::setViewportAsActualPixels(size);
                        gl::Util::clearColorBuffer(0.0, 0.0, 0.0, 0.0);
                        mTextureDrawer.draw(prev->texture());
                        fbo->release();
                    }
                    prev = fbo.get();
                }
            }
            // Create image, pass to UI and save or write to ffmpeg
            {
                auto outImage = mFramebuffers.back()->toImage();
                currentFrame = outImage;
                // flush
                ggl.glFlush();
                // export
                if (!exportImage(outImage, currentIndex)) {
                    return false;
                }
            }
        }
        return true;
    };

    ExportResult renderAndExport() {
        auto exportResult = ExportResult();
        auto* widget = new QWidget(mWidget);
        auto* form = new exportUI::form;
        form->setupUi(widget);
        form->loadingMessage->setText(tr("<html><head/><body><p align=\"center\">Initializing</p></body></html>"));
        QPushButton::connect(form->cancelButton, &QPushButton::clicked, [=]() { mCancelled = true; });
        widget->show();
        auto initResult = initialize();
        if (initResult == Errored) {
            exportResult.errorType = "Failed to initialize";
            exportResult.resultType = Errored;
            return exportResult;
        }
        int progressTicks = 0;
        while (true) {
            if (mCancelled) {
                exportResult.errorType = "Export cancelled";
                exportResult.resultType = Cancelled;
                finish();
                return exportResult;
            }
            if (!update()) {
                break;
            }
            progressTicks++;
            if (progressTicks == 3) {
                progressTicks = 0;
            }
            form->loadingMessage->setText(getProgressMessage(progressTicks));
            form->frameCounter->setText(getFrameMessage());
            form->pixmapLabel->setPixmap(QPixmap::fromImage(currentFrame));
            form->progressBar->setValue(static_cast<int>(100 * mProgress));
        }
    }
};

inline Exporter::Exporter(
    core::Project& aProject, QDialog* widget, const exportParam& exParam, ffmpeg::ffmpegObject& ffmpeg
):
    mWidget(widget),
    mParam(exParam),
    mIndex(0),
    mDigitCount(0),
    mFIndex(0),
    mFrameCount(0),
    mProgress(0),
    mExporting(false),
    mCancelled(false),
    mSaveAsImage(false),
    fpsIsConsistent(false),
    mOverwriteConfirmation(false),
    export_obj(ffmpeg),
    mProject(aProject) {}
inline Exporter::~Exporter() {
        finish();
        // kill buffer
        gl::Global::makeCurrent();
        destroyFramebuffers();
}
}
#endif // ANIMEEFFECTS_EXPORTPARAMS_H
