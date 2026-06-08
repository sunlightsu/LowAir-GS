/**
 * Demo-02: Static 3D Asset Viewer
 * LowAir-GS — 静态三维资产加载与渲染演示
 *
 * 功能：加载外部 OBJ / PLY / GLB 静态三维模型，
 *       显示模型几何、包围盒、基础材质，支持鼠标交互式浏览。
 *
 * 依赖：Qt6, OpenGL, Assimp
 * 编译：见 CMakeLists.txt
 */

#include <QApplication>
#include <QSurfaceFormat>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("LowAir-GS Demo-02 Static Asset Viewer");
    app.setApplicationVersion("0.2.0");

    // 深色主题 QSS
    app.setStyleSheet(R"(
        QWidget {
            background-color: #1E1E2E;
            color: #E0E0E0;
            font-family: "Segoe UI", "Source Han Sans", sans-serif;
            font-size: 12px;
        }
        QMainWindow, QDialog {
            background-color: #1E1E2E;
        }
        QMenuBar {
            background-color: #252535;
            color: #E0E0E0;
            border-bottom: 1px solid #3A3A4A;
        }
        QMenuBar::item:selected {
            background-color: #3A3A5A;
        }
        QMenu {
            background-color: #252535;
            color: #E0E0E0;
            border: 1px solid #3A3A4A;
        }
        QMenu::item:selected {
            background-color: #3A3A5A;
        }
        QToolBar {
            background-color: #252535;
            border-bottom: 1px solid #3A3A4A;
            spacing: 4px;
        }
        QPushButton {
            background-color: #2D2D45;
            color: #C0C0D0;
            border: 1px solid #3A3A5A;
            border-radius: 3px;
            padding: 4px 10px;
            min-height: 24px;
        }
        QPushButton:hover {
            background-color: #3A3A5A;
            color: #E0E0F0;
        }
        QPushButton:pressed {
            background-color: #4A4A6A;
        }
        QLabel {
            color: #C0C0D0;
        }
        QLabel#sectionTitle {
            color: #8888AA;
            font-size: 11px;
            font-weight: bold;
        }
        QLabel#valueLabel {
            color: #E0E0FF;
            font-family: monospace;
        }
        QGroupBox {
            border: 1px solid #3A3A5A;
            border-radius: 4px;
            margin-top: 8px;
            padding-top: 4px;
            color: #8888AA;
            font-size: 11px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 8px;
            padding: 0 4px;
        }
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QTextEdit {
            background-color: #141420;
            color: #A0A0B0;
            border: 1px solid #2A2A3A;
            font-family: monospace;
            font-size: 11px;
        }
        QSplitter::handle {
            background-color: #3A3A4A;
        }
        QCheckBox {
            color: #C0C0D0;
        }
        QCheckBox::indicator:checked {
            background-color: #5A5A8A;
            border: 1px solid #7A7AAA;
        }
    )");

    // 请求 OpenGL 4.1 Core Profile
    QSurfaceFormat fmt;
    fmt.setVersion(4, 1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);  // MSAA x4
    QSurfaceFormat::setDefaultFormat(fmt);

    // 解析命令行 --model 参数
    QString modelPath;
    QStringList args = app.arguments();
    int mi = args.indexOf("--model");
    if (mi >= 0 && mi + 1 < args.size()) {
        modelPath = args[mi + 1];
    }

    MainWindow w(modelPath);
    w.resize(1400, 900);
    w.show();

    return app.exec();
}
