#ifndef EXPORTDIAG_H
#define EXPORTDIAG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include "core/Project.h"
#include "ctrl/Exporter.h"
#include "ctrl/VideoFormat.h"
#include "gui/EasyDialog.h"

namespace gui {

//-------------------------------------------------------------------------------------------------
class ExportDiag: public EasyDialog {
    Q_OBJECT
public:
    ExportDiag(core::Project& aProject, QWidget* aParent);
    const ctrl::Exporter::CommonParam& commonParam() const { return mCommonParam; }
    ctrl::Exporter::CommonParam& commonParam() { return mCommonParam; }

protected:
    void pushSizeBox(QFormLayout& aLayout);
    void pushFrameBox(QFormLayout& aLayout);
    void pushFpsBox(QFormLayout& aLayout);

private:
    core::Project& mProject;
    ctrl::Exporter::CommonParam mCommonParam;
    QSize mSize;
    int mFrameMax;
    bool mFixAspect;
    bool mSizeUpdating;
    bool mWarningShown = false;
};

//-------------------------------------------------------------------------------------------------

class ExportClasses: public ExportDiag {
    Q_OBJECT
public:
    ExportClasses(core::Project& aProject, const QString& aSuffix, QWidget* aParent);
    const ctrl::Exporter::ImageParam& imageParam() const { return mImageParam; }

private:
    QLayout* createExporter();

    ctrl::Exporter::ImageParam mImageParam;
};


} // namespace gui

#endif // EXPORTDIAG_H
