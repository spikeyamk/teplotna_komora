#pragma once

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>

#include "control.hpp"
#include "console.hpp"
#include "charts.hpp"

namespace admin {
    class MainWindow : public QMainWindow {
        Q_OBJECT
    private:
        QDockWidget* control_dock_widget;
        QDockWidget* console_dock_widget;
        QDockWidget* charts_dock_widget;

        Control* control;
        Console* console;
        Charts* charts;

        QMenuBar* menu_bar;

        QMenu* file_menu;
        QAction* save_action;

        QMenu* view_menu;
        QMenu* help_menu;
    public:
        MainWindow() :
            QMainWindow {},
            control_dock_widget { new QDockWidget("Control", this) },
            console_dock_widget { new QDockWidget("Console", this) },
            charts_dock_widget { new QDockWidget("Charts", this) },
            control { new Control(this) },
            console { new Console(this) },
            charts { new Charts(this) },
            menu_bar { new QMenuBar(this) },

            file_menu { new QMenu("File", this) },
            save_action { file_menu->addAction("Save") },

            view_menu { new QMenu("View", this) },
            help_menu { new QMenu("Help", this) }

        {
            control_dock_widget->setWidget(control);
            console_dock_widget->setWidget(console);
            charts_dock_widget->setWidget(charts);

            QObject::connect(control, &Control::log, console, &Console::log);
            QObject::connect(charts, &Charts::log, console, &Console::log);

            QObject::connect(control, &Control::read_temp_ctl, this, [this](const magic::results::ReadTempCtl sample) {
                save_action->setEnabled(true);
                charts->push_to_charts(sample);
            });

            addDockWidget(Qt::TopDockWidgetArea, charts_dock_widget);
            addDockWidget(Qt::BottomDockWidgetArea, console_dock_widget);
            addDockWidget(Qt::BottomDockWidgetArea, control_dock_widget);
            tabifyDockWidget(control_dock_widget, console_dock_widget);
            control_dock_widget->raise(); 

            setMenuBar(menu_bar);

            connect(file_menu->addAction("Exit"), &QAction::triggered, this, &MainWindow::close);
            connect(save_action, &QAction::triggered, this, [this]() {
                const QString file_path {
                    QFileDialog::getSaveFileName(this, "Save File", "", "JSON Files (*.json)")
                };
                if(file_path.isEmpty() == false) {
                    charts->dump_to_file(file_path);
                }
            });
            save_action->setEnabled(false);

            QObject::connect(console, &Console::request_exit, this, &MainWindow::close);

            view_menu->addAction(control_dock_widget->toggleViewAction());
            view_menu->addAction(console_dock_widget->toggleViewAction());
            view_menu->addAction(charts_dock_widget->toggleViewAction());

            connect(help_menu->addAction("About"), &QAction::triggered, this, []() {
                QMessageBox::about(nullptr, "About", "This is a demo application.");
            });

            menu_bar->addMenu(file_menu);
            menu_bar->addMenu(view_menu);
            menu_bar->addMenu(help_menu);
            menu_bar->setNativeMenuBar(true);
        }
    protected:
        void closeEvent(QCloseEvent* event) override {
            if((control->is_connected() == false)
                && (save_action->isEnabled() == false)
            ) {
                event->accept();
                return;
            }

            if(QMessageBox::question(
                    this,
                    "Exit",
                    "Are you sure?",
                    QMessageBox::Yes | QMessageBox::No
                ) == QMessageBox::Yes
            ) {
                event->accept();
            } else {
                event->ignore();
            }
        }
    };
}