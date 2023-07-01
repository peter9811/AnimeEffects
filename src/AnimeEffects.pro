TEMPLATE = subdirs
TARGET = AnimeEffects

# Specify subdirectories to build
SUBDIRS = util thr cmnd gl img core ctrl gui

# Build in ordered mode
CONFIG += ordered

# Common paths
TRANSLATION_DIR = $$PWD/../data/locale
DATA_DIR = $$PWD/../data
TOOLS_DIR = $$PWD/../tools

# Add translations
TRANSLATIONS += $$TRANSLATION_DIR/translation_ja.ts
TRANSLATIONS += $$TRANSLATION_DIR/translation_zh.ts

# copy resources
win32 {
    copydata.commands = $(COPY_DIR) $$shell_path($$DATA_DIR) $$shell_path($$OUT_PWD/data)
    copytools.commands = $(COPY_DIR) $$shell_path($$TOOLS_DIR) $$shell_path($$OUT_PWD/tools)
}
unix:!macx {
    copydata.commands = rsync -ru $$shell_path($$DATA_DIR) $$shell_path($$OUT_PWD)
    copytools.commands = rsync -ru $$shell_path($$TOOLS_DIR) $$shell_path($$OUT_PWD)
}
macx {
    copydata.commands = rsync -ru $$shell_path($$DATA_DIR) $$shell_path($$OUT_PWD/AnimeEffects.app)
    copytools.commands = rsync -ru $$shell_path($$TOOLS_DIR) $$shell_path($$OUT_PWD/AnimeEffects.app)
}

first.depends = copydata copytools
QMAKE_EXTRA_TARGETS += first copydata copytools

unix{
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    target.path = $$PREFIX/bin

    shortcutfiles.files = dist/AnimeEffects.desktop
    shortcutfiles.path = $$PREFIX/share/applications/
    iconfiles.files = dist/AnimeEffects.png
    iconfiles.path = $$PREFIX/share/icons/hicolor/256x256/

    INSTALLS += target shortcutfiles iconfiles
}
