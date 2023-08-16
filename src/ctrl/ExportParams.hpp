#ifndef ANIMEEFFECTS_EXPORTPARAMS_HPP
#define ANIMEEFFECTS_EXPORTPARAMS_HPP

#include <QString>
#include <QList>

namespace ctrl {

    enum exportTarget { video, image };
    enum availableVideoFormats {
        apng, avi, f4v, flv, gif, mkv, mov, mp2, mp4, ogv, swf, webm, webp
    };
    QStringList videoFormats {
        "apng","avi","f4v","flv","gif","mkv","mov","mp2","mp4","ogv","swf","webm","webp"
    };
    enum availableImageFormats{
        bmp, jpeg, jpg, png, ppm, xbm, xpm
    };
    QStringList imageFormats {
        "bmp", "jpeg", "jpg", "png", "ppm", "xbm", "xpm"
    };
    QString returnVideoFormat(availableVideoFormats index){
        return videoFormats[index];
    }
    QString returnImageFormats(availableImageFormats index){
        return imageFormats[index];
    }

    struct VideoParams {
        availableVideoFormats format;
    };

    struct ImageParams {};

    class exportParam {
        public:
            exportParam();
            QString name;
            QString path;
            exportTarget exportType;
        };
    exportParam::exportParam() {
        name = "New Project";
        exportType = exportTarget::video;
    }
    }

#endif // ANIMEEFFECTS_EXPORTPARAMS_HPP
