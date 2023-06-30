#ifndef CTRL_PROJECTSAVER_H
#define CTRL_PROJECTSAVER_H

#include "core/Project.h"
#include "util/StreamWriter.h"
#include <QString>

namespace ctrl {

class ProjectSaver {
public:
    ProjectSaver();
    bool save(const QString& aFilePath, const core::Project& aProject);
    QString log() const {
        return mLog;
    }

private:
    bool writeHeader(util::StreamWriter& aWriter);
    bool writeGlobalBlock(util::StreamWriter& aWriter, const core::Project& aProject);
    QString mLog;
};

} // namespace ctrl

#endif // CTRL_PROJECTSAVER_H
