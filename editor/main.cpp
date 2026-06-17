#include <QApplication>

#include "config/editor_config.h"

#include "editor.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    EditorConfig::get();
    Editor editor;
    editor.showMaximized();

    return QApplication::exec();
}
