#include "create_window.h"

#include <QButtonGroup>
#include <toml.hpp>

#include "selector_widget.h"
#include "ui_create_window.h"

CreateWindow::CreateWindow(QWidget* parent):
    QWidget(parent), ui(new Ui::CreateWindow) {
    ui->setupUi(this);

    load_skins_data();

    // Seteo sliders de personalizacion
    head_selector = new SelectorWidget("Cabeza", skin_ranges["head"][0],skin_ranges["head"][1]);
    body_selector = new SelectorWidget("Vestuario",skin_ranges["body"][0],skin_ranges["body"][1]);
    ui->appearence_selectors->addWidget(head_selector, 0, 0);
    ui->appearence_selectors->addWidget(body_selector, 0, 1);

    // Seteo preview de apariencia
    preview = new PreviewWidget(skins_paths);
    ui->preview->addWidget(preview, 0, 0);

    connect(body_selector, &SelectorWidget::option_changed, preview, &PreviewWidget::change_body);
    connect(head_selector, &SelectorWidget::option_changed, preview, &PreviewWidget::change_head);
}

void CreateWindow::load_skins_data() {
    auto root = toml::parse(DATA_PATH "/texture_files.toml");
    std::vector<std::string> categories = {"body", "head"};

    for (const auto& cat : categories) {
        auto ids = toml::find<std::vector<int>>(root, "file_ids", cat);
        const int start = ids[0]+1;
        const int finish = ids[1];
        auto category = QString::fromStdString(cat);

        QHash<int, QString> category_options;
        for (int i = start; i <= finish; ++i) {
            auto path = QString("%1/lobby/creator/partes_user/%2_%3.png").arg(DATA_PATH).arg(category).arg(i);

            category_options.insert(i, path);
        }
        skin_ranges.insert(category, {start, finish});
        skins_paths.insert(category, category_options);
    }
}

CreateWindow::~CreateWindow() { delete ui; }
