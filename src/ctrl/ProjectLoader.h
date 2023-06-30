#ifndef CTRL_PROJECTLOADER_H
#define CTRL_PROJECTLOADER_H

#include "core/Project.h"
#include "gl/DeviceInfo.h"
#include "util/IProgressReporter.h"
#include "util/StreamReader.h"
#include <QStringList>
#include <QVersionNumber>

namespace ctrl {

class ProjectLoader {
public:
    ProjectLoader();

    bool load(const QString& aPath, core::Project& aProject, const gl::DeviceInfo& aGLDeviceInfo,
        util::IProgressReporter& aReporter);

    const QStringList& log() const {
        return mLog;
    }

private:
    bool readHeader(util::LEStreamReader& aReader);
    bool readGlobalBlock(util::LEStreamReader& aReader, core::Project& aProject);
    QStringList mLog;
    QVersionNumber mVersion;
};

} // namespace ctrl

#endif // CTRL_PROJECTLOADER_H
