#ifndef ANIMEEFFECTS_EXPORTPARAMS_HPP
#define ANIMEEFFECTS_EXPORTPARAMS_HPP

#include <QString>
#include <QList>
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
    bool exportAllFrames = false;
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

exportParam::exportParam(){
    exportType = exportTarget::video;
}


#endif // ANIMEEFFECTS_EXPORTPARAMS_HPP
