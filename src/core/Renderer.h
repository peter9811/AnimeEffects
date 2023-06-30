#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "XC.h"
#include "core/RenderInfo.h"
#include "core/TimeCacheAccessor.h"
#include "img/BlendMode.h"
#include <QList>
#include <QVector3D>

namespace core {

class Renderer {
public:
    struct SortUnit {
        SortUnit(): renderer(), depth() {}
        Renderer* renderer;
        float depth;
        TimeLine* timeline;
    };

    virtual ~Renderer() {}

    virtual void prerender(const RenderInfo& aInfo, const TimeCacheAccessor&) = 0;

    virtual void render(const RenderInfo& aInfo, const TimeCacheAccessor&) = 0;

    virtual void renderClipper(const RenderInfo& aInfo, const TimeCacheAccessor&, uint8 aClipperId) = 0;

    virtual void renderHSV(const RenderInfo& aInfo, const TimeCacheAccessor&, const QList<int>& HSVData) = 0;

    virtual void setClipped(bool aIsClipped) = 0;
    virtual bool isClipped() const = 0;

    virtual bool hasBlendMode() const {
        return false;
    }
    virtual img::BlendMode blendMode() const {
        return img::BlendMode_TERM;
    }
    virtual void setBlendMode(img::BlendMode) {}
};

} // namespace core

#endif // CORE_RENDERER_H
