#ifndef CORE_MESHTRANSFORMER_H
#define CORE_MESHTRANSFORMER_H

#include "core/LayerMesh.h"
#include "core/TimeKeyExpans.h"
#include "gl/BufferObject.h"
#include "gl/Vector3.h"
#include "util/ArrayBlock.h"
#include "util/NonCopyable.h"
#include <QScopedPointer>
namespace core {
class MeshTransformerResource;
}

namespace core {

class MeshTransformer: private util::NonCopyable {
public:
    MeshTransformer(const QString& aShaderPath);
    MeshTransformer(MeshTransformerResource& aResource);
    ~MeshTransformer();

    void callGL(const TimeKeyExpans& aExpans, LayerMesh::MeshBuffer& aMeshBuffer, const QVector2D& aOriginOffset,
        util::ArrayBlock<const gl::Vector3> aPositions, bool aNonPosed = false, bool aUseInfluence = true);

    gl::BufferObject& positions() {
        return *mOutPositions;
    }
    const gl::BufferObject& positions() const {
        return *mOutPositions;
    }

    gl::BufferObject& xArrows() {
        return *mOutXArrows;
    }
    const gl::BufferObject& xArrows() const {
        return *mOutXArrows;
    }

    gl::BufferObject& yArrows() {
        return *mOutYArrows;
    }
    const gl::BufferObject& yArrows() const {
        return *mOutYArrows;
    }

private:
    MeshTransformerResource& mResource;
    bool mResourceOwns;
    gl::BufferObject* mOutPositions;
    gl::BufferObject* mOutXArrows;
    gl::BufferObject* mOutYArrows;
};

} // namespace core

#endif // CORE_MESHTRANSFORMER_H
