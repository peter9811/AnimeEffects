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
#include <QPainter>

const int DefaultSize = 1028;
const std::string DefaultString = "Default";

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
    Blend blend = NORMAL;
    PorterDuff pdComposite = SRC_OVER;
};
struct layer{
    std::string name;
    QImage image;
    composite composite_op;
    float opacity{};
    bool isVisible{};
    int x{};
    int y{};
};
// No composite as ANIE does not support composing folders
struct stack{
    std::string name;
    float opacity{};
    bool isVisible{};
    bool isRoot = false;
    QVector<layer> layers;
    QVector<stack> folders;
};
struct oraImage{
    int w{};
    int h{};
    std::string version{};
    QVector<stack> stack;
};

class ORAReader {
public:
    miniz_cpp::zip_file* oraFile;
    QVector<stack> stackList;
    QXmlStreamReader* reader{};
    oraImage image;
    // These have either been accounted for by the function or are unsupported
    QStringList ignoreList{"image", "a", "annotations", "stack", "layer"};

    explicit ORAReader(miniz_cpp::zip_file* file){
        oraFile = file;
    }
    // TODO: Implement this into ImageFileLoader.cpp

    template<typename T>
    static bool isAttrValid(const T& type, const QXmlStreamAttribute& attribute){
        if(attribute.value().isEmpty() || attribute.value().isNull()){ return false; }
        if constexpr (std::is_same_v<T, std::string>) { return (!attribute.value().toString().isEmpty());}
        else if constexpr (std::is_same_v<T, int>) { return !std::isinf(attribute.value().toInt());}
        else if constexpr (std::is_same_v<T, float>) { return !std::isinf(attribute.value().toFloat());}
        else { return false; }
    }
    template <typename T>
    [[nodiscard]] T getAttrIfValid(const T& failsafe, const QXmlStreamAttribute& attribute) const {
        if (!isAttrValid(failsafe, attribute)) { qDebug() << "Invalid value of attribute: " << attribute.name(); return failsafe; }
        if constexpr (std::is_same_v<T, std::string>) { return attribute.value().toString().toStdString();}
        else if constexpr (std::is_same_v<T, int>) { return attribute.value().toInt();}
        else if constexpr (std::is_same_v<T, float>) { return attribute.value().toFloat();}
        else { return failsafe; }
    }
    static QPixmap createPattern(const QSize & size, const QColor & background=Qt::white, const QColor & foreground=Qt::black){
        QPixmap pixmap(size);
        pixmap.fill(background);
        {
            QPainter painter(&pixmap);
            painter.setPen(foreground);
            painter.drawLine(0, size.height()/2, size.width(), size.height()/2);
            painter.drawLine(size.width()/2, 0, size.width()/2, size.height());
        }
        return pixmap;
    }
    void parseStack(stack* curStack){ // NOLINT(*-no-recursion)
        auto token = reader->tokenType();
        while(!reader->atEnd() && reader->name().toString() != "image"){
            // Read self
            while (!reader->atEnd() && reader->name().toString() == "stack") {
                for (const auto& attribute : reader->attributes()) {
                    if (attribute.name().toString() == "name") {
                        curStack->name = getAttrIfValid(DefaultString, attribute);
                    } else if (attribute.name().toString() == "opacity") {
                        curStack->opacity = getAttrIfValid(1.0f, attribute);
                    } else if (attribute.name().toString() == "visibility") {
                        curStack->isVisible = getAttrIfValid(std::string("visible"), attribute) == "visible";
                    } else if (attribute.name().toString() == "composite-op") {
                        // Ignored in ANIE
                        continue;
                    } else {
                        qDebug() << "-stack- Unknown attribute: " << attribute.name() << ", with value: " << attribute.value();
                    }
                }
                token = reader->readNext();
            }
            // Read layers
            while (!reader->atEnd() && reader->name().toString() == "layer") {
                if (token != QXmlStreamReader::EndElement) {
                    parseLayer(&curStack->layers.emplace_back());
                }
                token = reader->readNext();
            }
            // Read children
            while (!reader->atEnd() && reader->name().toString() == "stack") {
                if (token != QXmlStreamReader::EndElement) {
                    parseStack(&curStack->folders.emplace_back());
                }
                token = reader->readNext();
            }
            // Safe ignore certain objects
            if (reader->name().isEmpty() | ignoreList.contains(reader->name().toString())) {}
            // Log unknown objects
            else {
                qDebug() << "Unknown stack object: " << reader->name() << ", with " << reader->attributes().size()
                         << " attributes.";
            }
            token = reader->readNext();
        }
    }
    void parseLayer(layer* curLayer) const{
        for (const auto& attribute : reader->attributes()) {
            if (attribute.name().toString() == "name") { curLayer->name = getAttrIfValid(DefaultString, attribute); }
            else if (attribute.name().toString() == "src") {
                std::string srcImg;
                // If the value is false it will throw, so using a failsafe is irrelevant in this case
                try { srcImg = oraFile->read(attribute.value().toString().toStdString()); }
                catch (...){ srcImg = "src"; }
                curLayer->image = QImage::fromData(QByteArray::fromStdString(srcImg));
                // If the image is missing we create a missing texture placeholder
                if(curLayer->image.isNull()){
                    QPixmap sourceCheckerboard(DefaultSize, DefaultSize);
                    {
                        QPainter painter(&sourceCheckerboard);
                        QBrush brush;
                        brush.setTexture(createPattern(QSize(25, 25), Qt::darkMagenta, Qt::black));
                        painter.fillRect(sourceCheckerboard.rect(), brush);
                    }
                    curLayer->image = sourceCheckerboard.toImage();
                }
            }
            else if (attribute.name().toString() == "opacity") { curLayer->opacity = getAttrIfValid(1.0f, attribute); }
            else if (attribute.name().toString() == "visibility") {
                curLayer->isVisible = getAttrIfValid(std::string("visible"), attribute) == "visible";
            }
            else if (attribute.name().toString() == "x") { curLayer->x = getAttrIfValid(DefaultSize, attribute); }
            else if (attribute.name().toString() == "y") { curLayer->y = getAttrIfValid(DefaultSize, attribute); }
            else if (attribute.name().toString() == "name") {
                curLayer->name = getAttrIfValid(DefaultString, attribute);
            }
            else if (attribute.name().toString() == "composite-op") {
                curLayer->composite_op = stringToComposite(getAttrIfValid(std::string(""), attribute));
            }
            else {
                qDebug() << "Unknown layer attribute: " << attribute.name() << ", with value: " << attribute.value();
            }
        }
    }
    bool initialize(){
        std::string stackStr = oraFile->read("stack.xml");
        reader = new QXmlStreamReader(stackStr);
        while(!reader->atEnd() && !reader->hasError()) {
            reader->readNext();
            // Read self
            if(reader->name().toString() == "image"){
                for(const auto& attribute: reader->attributes()){
                    if(attribute.name().toString() == "version"){
                        image.version = getAttrIfValid(std::string("Unknown"), attribute);
                    }
                    else if(attribute.name().toString() == "w"){
                        image.w = getAttrIfValid(DefaultSize, attribute);
                    }
                    else if(attribute.name().toString() == "h"){
                        image.h = getAttrIfValid(DefaultSize, attribute);
                    }
                    else{
                        qDebug() << "Unknown image attribute: " << attribute.name() << ", with value: " << attribute.value();
                    }
                }
            }
            // Read main stack
            else if (reader->name().toString() == "stack"){
                auto* mainStack = &image.stack.emplace_back();
                mainStack->isRoot = true;
                parseStack(mainStack);
            }
            // Ignore common invalid objects
            else if (reader->name().isEmpty() || ignoreList.contains(reader->name().toString())){
                continue;
            }
            // Log unknown objects
            else{ qDebug() << "Unknown image object: " << reader->name().toString() << ", with " << reader->attributes().size() << " attributes."; }
        }
        bool status = !reader->hasError();
        reader->clear();
        delete reader;
        return status;
    }
    static void printComposite(const composite& comp, const std::string& depth){
        switch (comp.blend) {
        case NORMAL:        qDebug() << depth.c_str() << "layer blend: " << "NORMAL"; break;
        case MULTIPLY:      qDebug() << depth.c_str() << "layer blend: " << "MULTIPLY"; break;
        case SCREEN:        qDebug() << depth.c_str() << "layer blend: " << "SCREEN"; break;
        case OVERLAY:       qDebug() << depth.c_str() << "layer blend: " << "OVERLAY"; break;
        case DARKEN:        qDebug() << depth.c_str() << "layer blend: " << "DARKEN"; break;
        case LIGHTEN:       qDebug() << depth.c_str() << "layer blend: " << "LIGHTEN"; break;
        case COLOR_DODGE:   qDebug() << depth.c_str() << "layer blend: " << "COLOR_DODGE"; break;
        case COLOR_BURN:    qDebug() << depth.c_str() << "layer blend: " << "COLOR_BURN"; break;
        case HARD_LIGHT:    qDebug() << depth.c_str() << "layer blend: " << "HARD_LIGHT"; break;
        case SOFT_LIGHT:    qDebug() << depth.c_str() << "layer blend: " << "SOFT_LIGHT"; break;
        case DIFF:          qDebug() << depth.c_str() << "layer blend: " << "DIFFERENCE"; break;
        case COLOR:         qDebug() << depth.c_str() << "layer blend: " << "COLOR"; break;
        case LUMINOSITY:    qDebug() << depth.c_str() << "layer blend: " << "LUMINOSITY"; break;
        case HUE:           qDebug() << depth.c_str() << "layer blend: " << "HUE"; break;
        case SATURATION:    qDebug() << depth.c_str() << "layer blend: " << "SATURATION"; break;
        case PLUS:          qDebug() << depth.c_str() << "layer blend: " << "PLUS"; break;
        }
        switch (comp.pdComposite) {
        case SRC_OVER:      qDebug() << depth.c_str() << "layer composite: " << "SRC_OVER"; break;
        case LIGHTER:       qDebug() << depth.c_str() << "layer composite: " << "LIGHTER"; break;
        case DST_IN:        qDebug() << depth.c_str() << "layer composite: " << "DST_IN"; break;
        case DST_OUT:       qDebug() << depth.c_str() << "layer composite: " << "DST_OUT"; break;
        case SRC_ATOP:      qDebug() << depth.c_str() << "layer composite: " << "SRC_ATOP"; break;
        case DST_ATOP:      qDebug() << depth.c_str() << "layer composite: " << "DST_ATOP"; break;
        }
    }
    static void printLayer(const layer& lyr, const std::string& depth){
        qDebug() << QString::fromStdString(depth).removeLast().toStdString().c_str() << depth.c_str() << "<layer>";
        qDebug() << depth.c_str() << "layer name: " << lyr.name;
        qDebug() << depth.c_str() << "layer x: " << lyr.x;
        qDebug() << depth.c_str() << "layer y: " << lyr.y;
        qDebug() << depth.c_str() << "layer opacity: " << lyr.opacity;
        qDebug() << depth.c_str() << "layer visible: " << lyr.isVisible;
        printComposite(lyr.composite_op, depth);
        qDebug() << depth.c_str() << "layer image: " << lyr.image;
        qDebug() << QString::fromStdString(depth).removeLast().toStdString().c_str() << depth.c_str() << "</layer>";
    }
    static void printStack(const stack& stk, int stackDepth){ // NOLINT(*-no-recursion)
        QString chara = "-";
        auto depth = chara.repeated(stackDepth * 4).append('|').toStdString();
        qDebug() << QString::fromStdString(depth).removeLast().toStdString().c_str() << depth.c_str() << "<stack>";
        if(!stk.isRoot){
            qDebug() << depth.c_str() << "stack name: " << stk.name;
            qDebug() << depth.c_str() << "stack opacity: " << stk.opacity;
            qDebug() << depth.c_str() << "stack is visible: " << stk.isVisible;
        }
        else{ qDebug() << depth.c_str() << "image root stack"; }
        for (const auto& sStk : stk.layers) {
            printLayer(sStk, depth);
        }
        for (const auto& sStk : stk.folders) {
            stackDepth += 1;
            printStack(sStk, stackDepth);
        }
        qDebug() << QString::fromStdString(depth).removeLast().toStdString().c_str() << depth.c_str() << "</stack>";
    }
    void printSelf(){
        qDebug("<|IMAGE|>");
        qDebug() << "image height: " << image.h;
        qDebug() << "image width: " << image.w;
        qDebug() << "openRaster spec version: " << (image.version.empty()? "Unknown": image.version);
        int stackDepth = 1;
        for(const auto& stk: image.stack){
            printStack(stk, stackDepth);
            stackDepth++;
        }
        qDebug("<|IMAGE|>");
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
