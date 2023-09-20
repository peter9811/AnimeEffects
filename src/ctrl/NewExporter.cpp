//
// Created by Yukusai on 20 Sep 2023.
//

#include "NewExporter.h"

namespace ctrl {

NewExporter::NewExporter(core::Project& aProject, const exportParam& exParam):
    mProject(aProject),
    exParam(exParam){}
void NewExporter::exportProject() {
    qDebug("So uh, the exporter... kinda hard isn't it?");
}
NewExporter::~NewExporter() = default;
}
// namespace ctrl