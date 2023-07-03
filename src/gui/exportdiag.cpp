#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStackedLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QFrame>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileInfo>
#include <QPlainTextEdit>
#include <QFontMetrics>
#include "MainMenuBar.h"
#include "util/SelectArgs.h"
#include "MainWindow.h"
#include "exportdiag.h"

namespace {
template<class tEdit>
void setMinMaxOptionWidth(tEdit* aEdit) {
    aEdit->setMaximumWidth(200);
    aEdit->setMinimumWidth(50);
}
} // namespace


namespace gui {
//-------------------------------------------------------------------------------------------------
ExportDiag::ExportDiag(core::Project& aProject, QWidget* aParent):
    EasyDialog(tr("Export Animation..."), aParent),
    mProject(aProject),
    mCommonParam(),
    mSize(),
    mFrameMax(),
    mFixAspect(true),
    mSizeUpdating(false) {
    mCommonParam.size = mProject.attribute().imageSize();
    XC_ASSERT(mCommonParam.size.width() > 0);
    XC_ASSERT(mCommonParam.size.height() > 0);

    auto time = mProject.currentTimeInfo();
    mCommonParam.frame = util::Range(0, time.frameMax);
    mCommonParam.fps = time.fps;

    mSize = mProject.attribute().imageSize();
    mFrameMax = mProject.currentTimeInfo().frameMax;
}

void ExportDiag::pushSizeBox(QFormLayout& aLayout) {
    // size
    {
        auto x = new QSpinBox();
        auto y = new QSpinBox();
        auto fix = new QCheckBox();
        x->setRange(1, 32767);
        y->setRange(1, 32767);
        if (!(mCommonParam.size.width() % 2 == 0) || !(mCommonParam.size.height() % 2 == 0)) {
            MainWindow::showInfoPopup(
                tr("Value is Odd"),
                tr("The width or height of the image ends with an odd number. "
                   "Please change these parameters to an even number as they may cause the export to fail."),
                "Warn"
            );
            mWarningShown = true;
        }
        x->setValue(mCommonParam.size.width());
        y->setValue(mCommonParam.size.height());
        setMinMaxOptionWidth(x);
        setMinMaxOptionWidth(y);
        fix->setChecked(mFixAspect);

        this->connect(x, util::SelectArgs<int>::from(&QSpinBox::valueChanged), [=](int aValue) {
            if (mSizeUpdating)
                return;
            if (!(aValue % 2 == 0) && !mWarningShown) {
                MainWindow::showInfoPopup(
                    tr("Value is Odd"),
                    tr("A width or height ending in an odd number"
                       " may make the exporting process fail, please try another value."),
                    "Warn"
                );
                mWarningShown = true;
                return;
            }
            mSizeUpdating = true;
            QSize& size = this->mCommonParam.size;
            size.setWidth(aValue);
            if (mFixAspect) {
                size.setHeight((int)(mSize.height() * aValue / (double)mSize.width()));
                y->setValue(size.height());
            }
            mSizeUpdating = false;
        });

        this->connect(y, util::SelectArgs<int>::from(&QSpinBox::valueChanged), [=](int aValue) {
            if (mSizeUpdating)
                return;

            if (!(aValue % 2 == 0) && !mWarningShown) {
                MainWindow::showInfoPopup(
                    tr("Value is Odd"),
                    tr("A height or width ending in an odd number"
                       " may make the exporting process fail, please try another value."),
                    "Warn"
                );
                mWarningShown = true;
                return;
            }

            mSizeUpdating = true;
            QSize& size = this->mCommonParam.size;
            size.setHeight(aValue);
            if (mFixAspect) {
                size.setWidth((int)(mSize.width() * aValue / (double)mSize.height()));
                x->setValue(size.width());
            }
            mSizeUpdating = false;
        });

        this->connect(fix, &QCheckBox::clicked, [=](bool aCheck) { this->mFixAspect = aCheck; });

        aLayout.addRow(tr("Image width :"), x);
        aLayout.addRow(tr("Image height :"), y);
        aLayout.addRow(tr("Fix aspect ratio :"), fix);
    }
}

void ExportDiag::pushFrameBox(QFormLayout& aLayout) {
    // start frame
    auto start = new QSpinBox();
    start->setRange(0, mFrameMax);
    start->setValue(mCommonParam.frame.min());
    setMinMaxOptionWidth(start);

    // end frame
    auto end = new QSpinBox();
    end->setRange(0, mFrameMax);
    end->setValue(mCommonParam.frame.max());
    setMinMaxOptionWidth(end);

    this->connect(start, &QSpinBox::editingFinished, [=]() {
        const int v = start->value();
        this->mCommonParam.frame.setMin(v);
        if (this->mCommonParam.frame.max() < v) {
            end->setValue(v);
            this->mCommonParam.frame.setMax(v);
        }
    });

    this->connect(end, &QSpinBox::editingFinished, [=]() {
        const int v = end->value();
        this->mCommonParam.frame.setMax(v);
        if (v < this->mCommonParam.frame.min()) {
            start->setValue(v);
            this->mCommonParam.frame.setMin(v);
        }
    });

    aLayout.addRow(tr("Initial frame :"), start);
    aLayout.addRow(tr("Last frame :"), end);
}

void ExportDiag::pushFpsBox(QFormLayout& aLayout) {
    auto fps = new QSpinBox();
    fps->setRange(1, 60);
    fps->setValue(mCommonParam.fps);
    setMinMaxOptionWidth(fps);

    this->connect(fps, &QSpinBox::editingFinished, [=]() { this->mCommonParam.fps = fps->value(); });

    aLayout.addRow(tr("FPS :"), fps);
}

//-------------------------------------------------------------------------------------------------
ExportClasses::ExportClasses(core::Project& aProject, const QString& aSuffix, QWidget* aParent):
    ExportDiag(aProject, aParent), mImageParam() {
    {
        QString baseName = QFileInfo(aProject.fileName()).baseName();
        if (baseName.isEmpty())
            baseName = "nameless";
        mImageParam.name = baseName + "_export";
        mImageParam.suffix = aSuffix;
    }

    // option
    auto group = new QGroupBox(tr("Parameters"));
    group->setLayout(createExporter());
    this->setMainWidget(group);

    // button box
    this->setOkCancel();
    this->fixSize();
}

QLayout* ExportClasses::createExporter() {
    auto form = new QFormLayout();
    // form->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    form->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    form->setLabelAlignment(Qt::AlignRight);

    // prefix name
    {
        auto name = new QLineEdit();
        // setMinMaxOptionWidth(name);
        name->setText(mImageParam.name);

        this->connect(name, &QLineEdit::editingFinished, [=]() { this->mImageParam.name = name->text(); });

        form->addRow(tr("Prefix name :"), name);
    }

    // quality
    if (mImageParam.suffix == "jpg") {
        auto quality = new QSpinBox();
        // setMinMaxOptionWidth(quality);
        quality->setMinimum(-1);
        quality->setMaximum(100);
        quality->setValue(mImageParam.quality);

        this->connect(quality, &QSpinBox::editingFinished, [=]() { this->mImageParam.quality = quality->value(); });

        form->addRow(tr("Quality :"), quality);
    }

    this->pushSizeBox(*form);
    this->pushFrameBox(*form);
    this->pushFpsBox(*form);

    return form;
}


} // namespace gui
