//
// Created by yukusai on 22/12/2024.
//

#ifndef ANIMEEFFECTS_ORAREADER_H
#define ANIMEEFFECTS_ORAREADER_H

#include "util/zip_file.h"
#include "img/BlendMode.h"
#include <QImage>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

enum PorterDuff {
    SRC_OVER,
    LIGHTER,
    DST_IN,
    DST_OUT,
    SRC_ATOP,
    DST_ATOP
};
enum Blend {
    NORMAL,
    MULTIPLY,
    SCREEN,
    OVERLAY,
    DARKEN,
    LIGHTEN,
    COLOR_DODGE,
    COLOR_BURN,
    HARD_LIGHT,
    SOFT_LIGHT,
    // Not named DIFFERENCE due to define conflict
    DIFF,
    COLOR,
    LUMINOSITY,
    HUE,
    SATURATION,
    PLUS,
};

struct composite{
    Blend blend;
    PorterDuff pdComposite = SRC_OVER;
};

struct layer{
    std::string name;
    QImage image;
    composite composite_op;
    float opacity;
    bool isVisible;
    int x;
    int y;
};
struct stack{
    std::string name;
    QVector<layer> layers;
    QVector<stack> folders;
};

class ORAReader {
public:
    miniz_cpp::zip_file* oraFile;
    QVector<stack> stackList;
    QXmlStreamReader* reader = nullptr;
    explicit ORAReader(miniz_cpp::zip_file* file){
        oraFile = file;
    }
    bool initialize(){
        std::string stackStr = oraFile->read("stack.xml");
        reader = new QXmlStreamReader(stackStr);
        // Fix attribute checking with https://stackoverflow.com/questions/3092387/parse-a-xml-file-in-qt
        // Oh also finish this later
        while(!reader->atEnd() && !reader->hasError()) {
            if(reader->name().toString() == "image"){
                if(reader->attributes().isEmpty()){
                    qDebug("----ImageEnd----");
                }
                else { qDebug("----Image----"); }
                for(auto attribute: reader->attributes()){
                    qDebug() << attribute.name();
                    qDebug() << attribute.value();
                }
            }
            if(reader->name().toString() == "stack"){
                if(reader->attributes().isEmpty()){
                    qDebug("----StackEnd----");
                }
                else { qDebug("----Stack----"); }
                for(auto attribute: reader->attributes()){
                    qDebug() << attribute.name();
                    qDebug() << attribute.value();
                }
            }
            if(reader->name().toString() == "layer"){
                if(reader->attributes().isEmpty()){
                    qDebug("----LayerEnd----");
                }
                else { qDebug("----Layer----"); }
                for(auto attribute: reader->attributes()){
                    qDebug() << attribute.name();
                    qDebug() << attribute.value();
                }
            }
            reader->readNext();
        }
        bool status = !reader->hasError();
        reader->clear();
        delete reader;
        return status;
    }
    static img::BlendMode oraBlendToPSDBlend(Blend blend){
        switch (blend) {
        case NORMAL:
            return img::BlendMode_Normal;
        case MULTIPLY:
            return img::BlendMode_Multiply;
        case SCREEN:
            return img::BlendMode_Screen;
        case OVERLAY:
            return img::BlendMode_Overlay;
        case DARKEN:
            return img::BlendMode_Darken;
        case LIGHTEN:
            return img::BlendMode_Lighten;
        case COLOR_DODGE:
            return img::BlendMode_ColorDodge;
        case COLOR_BURN:
            return img::BlendMode_ColorBurn;
        case HARD_LIGHT:
            return img::BlendMode_HardLight;
        case SOFT_LIGHT:
            return img::BlendMode_SoftLight;
        case DIFF:
            return img::BlendMode_Difference;
             // Unsupported blend modes
        case COLOR:
        case LUMINOSITY:
        case HUE:
        case SATURATION:
        case PLUS:
            return img::BlendMode_Normal;
        }
    }
    static composite stringToComposite(const std::string& blend){
        if(blend == "svg:src-over") return {NORMAL};
        if(blend == "svg:multiply") return {MULTIPLY};
        if(blend == "svg:screen") return {SCREEN};
        if(blend == "svg:overlay") return {OVERLAY};
        if(blend == "svg:darken") return {DARKEN};
        if(blend == "svg:lighten") return {LIGHTEN};
        if(blend == "svg:color-dodge") return {COLOR_DODGE};
        if(blend == "svg:color-burn") return {COLOR_BURN};
        if(blend == "svg:hard-light") return {HARD_LIGHT};
        if(blend == "svg:soft-light") return {SOFT_LIGHT};
        if(blend == "svg:difference") return {DIFF};
        if(blend == "svg:color") return {COLOR};
        if(blend == "svg:luminosity") return {LUMINOSITY};
        if(blend == "svg:hue") return {HUE};
        if(blend == "svg:saturation") return {SATURATION};
        if(blend == "svg:plus") return {NORMAL, LIGHTER};
        if(blend == "svg:dst-in") return {NORMAL, DST_IN};
        if(blend == "svg:dst-out") return {NORMAL, DST_OUT};
        if(blend == "svg:src-atop") return {NORMAL, SRC_ATOP};
        if(blend == "svg:dst-atop") return {NORMAL, DST_ATOP};
        return {NORMAL};
    }
};


#endif // ANIMEEFFECTS_ORAREADER_H
