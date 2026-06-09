#include "creator_window.h"

#include <QMoveEvent>
#include <string>
#include <vector>

#include <toml.hpp>

#include "login_window.h"
#include "widgets/selector_widget.h"

#include "ui_creator_window.h"

CreatorWindow::CreatorWindow(const QString& username, QWidget* parent):
        QWidget(parent), ui(new Ui::CreatorWindow) {
    ui->setupUi(this);

    // Seteo borde de ventana custom
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    connect(ui->btnClose, &QPushButton::clicked, this, &CreatorWindow::exit_window);
    connect(ui->btnMinimize, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &CreatorWindow::exit_window);

    load_skins_data();

    // Seteo sliders de personalizacion
    head_selector = new SelectorWidget("Cabeza", skin_ranges["head"][0], skin_ranges["head"][1]);
    body_selector = new SelectorWidget("Vestuario", skin_ranges["body"][0], skin_ranges["body"][1]);
    ui->appearence_selectors->addWidget(head_selector, 0, 0);
    ui->appearence_selectors->addWidget(body_selector, 0, 1);

    // Seteo preview de apariencia
    ui->username->setText(username);
    preview = new PreviewWidget(skins_paths);
    ui->preview->addWidget(preview, 0, 0);

    connect(body_selector, &SelectorWidget::option_changed, preview, &PreviewWidget::change_body);
    connect(head_selector, &SelectorWidget::option_changed, preview, &PreviewWidget::change_head);
    connect(ui->btnStart, &QPushButton::clicked, this, &CreatorWindow::start_game);
}

void CreatorWindow::load_skins_data() {
    auto root = toml::parse(CONFIG_PATH "/texture_files.toml");
    std::vector<std::string> categories = {"body", "head"};

    for (const auto& cat: categories) {
        auto ids = toml::find<std::vector<int>>(root, "file_ids", cat);
        const int start = ids[0] + 1;
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

void CreatorWindow::start_game() {
    if (validate_data()) {
        const uint8_t body = body_selector->get_value();
        const uint8_t head = head_selector->get_value();
        const uint8_t race = ui->races_buttons->checkedButton()->property("race_id").toInt();
        const uint8_t archetype = ui->archetypes_buttons->checkedButton()->property("archetype_id").toInt();

        emit finish_creation(CreatePlayerDTO(body, head, archetype, race));
    } else {
        ui->validation_err->show();
    }
}

void CreatorWindow::exit_window() {
    emit exit_creator();
}

bool CreatorWindow::validate_data() {
    return ui->races_buttons->checkedButton() && ui->archetypes_buttons->checkedButton();
}

CreatorWindow::~CreatorWindow() { delete ui; }
