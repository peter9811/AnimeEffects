#ifndef GL_GLOBAL_H
#define GL_GLOBAL_H

#include <QOpenGLFunctions_3_1>
#include <QOpenGLExtensions>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QGLFormat>

namespace gl
{

class Global
{
public:
    typedef QOpenGLFunctions_3_1 Functions;
    typedef QOpenGLExtension_ARB_sync SyncExtension;
    static const QPair<int, int> kVersion;

    static void setFunctions(Functions& aFunctions);
    static void clearFunctions();
    static Functions& functions();

    static void setSyncExtension(SyncExtension& aSyncExtension);
    static void clearSyncExtension();
    static SyncExtension& syncExtension();

    //static void setContext(QOpenGLContext& aContext, QSurface& aSurface);
    static void setContext(QOpenGLWidget& aWidget);
    static void clearContext();
    static void makeCurrent();
    static void doneCurrent();

private:
    Global() {}
};

} // namespace gl


#include "XCAssert.h"

#define GL_CHECK_ERROR()                                  \
    do {                                                  \
        GLuint e = gl::Global::functions().glGetError();  \
        if (e != GL_NO_ERROR) {                           \
            XC_DEBUG_REPORT("OpenGL Error (code:%u)", e); \
            Q_ASSERT(0);                                  \
        }                                                 \
    } while (0)


#endif // GL_GLOBAL_H
