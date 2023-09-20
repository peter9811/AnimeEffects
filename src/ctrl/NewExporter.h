//
// Created by Yukusai on 20 Sep 2023.
//

#ifndef ANIMEEFFECTS_NEWEXPORTER_H
#define ANIMEEFFECTS_NEWEXPORTER_H

#include <QDir>
#include "core/Project.h"
#include "ctrl/ExportParams.hpp"

namespace ctrl {

class NewExporter {
public:
    NewExporter(core::Project& aProject, const exportParam& exParam);
    ~NewExporter();
    core::Project& mProject;
    const exportParam& exParam;
    void exportProject();
};

} // namespace ctrl

#endif // ANIMEEFFECTS_NEWEXPORTER_H
