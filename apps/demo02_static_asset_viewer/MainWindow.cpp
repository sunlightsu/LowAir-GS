/**
 * MainWindow.cpp — Demo-02 主窗口实现
 *
 * 深色主题 GCS 风格 UI（#1E1E2E 背景，#E0E0E0 文字）
 */

#include "MainWindow.h"
#include "RenderWidget.h"
#include "asset/AssimpModelLoader.h"

#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QFormLayout>
#include <QStatusBar>
#include <QDateTime>
#include <QScreen>
#include <QPixmap>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QScrollArea>
#include <QFrame>
#include <QMessageBox>
#include <QTimer>

// ─── 深色主题 QSS ────────────────────────────────────────────────────────────

static const char *DARK_QSS = R"(
QMainWindow, QWidget {
    background-color: #1E1E2E;
    color: #E0E0E0;
    font-family: "Segoe UI", "Noto Sans CJK SC", sans-serif;
    font-size: 12px;
}
QMenuBar {
    background-color: #252535;
    color: #E0E0E0;
    border-bottom: 1px solid #3A3A4A;
}
QMenuBar::item:selected { background-color: #3A3A5A; }
QMenu {
    background-color: #252535;
    color: #E0E0E0;
    border: 1px solid #3A3A4A;
}
QMenu::item:selected { background-color: #3A3A5A; }
QToolBar {
    background-color: #252535;
    border-bottom: 1px solid #3A3A4A;
    spacing: 4px;
    padding: 2px;
}
QToolButton {
    background-color: #2D2D3F;
    color: #E0E0E0;
    border: 1px solid #3A3A4A;
    border-radius: 3px;
    padding: 4px 8px;
    min-height: 26px;
}
QToolButton:hover { background-color: #3A3A5A; }
QToolButton:pressed { background-color: #4A4A6A; }
QPushButton {
    background-color: #2D4A7A;
    color: #E0E0E0;
    border: 1px solid #3A5A9A;
    border-radius: 3px;
    padding: 5px 12px;
    min-height: 28px;
}
QPushButton:hover { background-color: #3A5A9A; }
QPushButton:pressed { background-color: #4A6AAA; }
QGroupBox {
    background-color: #252535;
    border: 1px solid #3A3A4A;
    border-radius: 4px;
    margin-top: 8px;
    padding-top: 4px;
    font-weight: bold;
    color: #A0A0C0;
}
QGroupBox::title {
    subcontrol-origin: margin;
    left: 8px;
    padding: 0 4px;
}
QLabel { color: #E0E0E0; }
QLabel#infoValue {
    color: #90D0FF;
    font-family: monospace;
}
QCheckBox { color: #E0E0E0; spacing: 6px; }
QCheckBox::indicator {
    width: 14px; height: 14px;
    border: 1px solid #5A5A7A;
    border-radius: 2px;
    background: #2D2D3F;
}
QCheckBox::indicator:checked { background: #4A7ACC; }
QTextEdit {
    background-color: #12121E;
    color: #90FF90;
    border: 1px solid #3A3A4A;
    font-family: monospace;
    font-size: 11px;
}
QScrollBar:vertical {
    background: #1E1E2E;
    width: 8px;
}
QScrollBar::handle:vertical {
    background: #3A3A5A;
    border-radius: 4px;
}
QStatusBar {
    background-color: #252535;
    color: #A0A0C0;
    border-top: 1px solid #3A3A4A;
}
QSplitter::handle { background-color: #3A3A4A; }
)";

// ─── 构造函数 ────────────────────────────────────────────────────────────────

MainWindow::MainWindow(const QString &initialModel, QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("LowAir-GS  Demo-02 — 静态三维资产查看器");
    resize(1280, 800);
    setStyleSheet(DARK_QSS);

    setupUI();
    setupMenuBar();
    setupToolBar();

    statusBar()->showMessage("就绪 — 请通过「文件 → 打开模型」或拖放 OBJ/PLY/GLB 文件加载三维资产");

    if (!initialModel.isEmpty()) {
        // 延迟到事件循环启动后加载，确保 QOpenGLWidget::initializeGL() 已被调用
        QTimer::singleShot(200, this, [this, initialModel]() {
            loadModelFromPath(initialModel);
        });
    }
}

// ─── UI 构建 ─────────────────────────────────────────────────────────────────

void MainWindow::setupUI()
{
    // ── 中央 splitter ──────────────────────────────────────────────────────
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(4);

    // ── 左侧面板 ───────────────────────────────────────────────────────────
    auto *leftWidget = new QWidget;
    leftWidget->setFixedWidth(280);
    auto *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(6, 6, 6, 6);
    leftLayout->setSpacing(6);

    // 加载按钮
    auto *btnLoad = new QPushButton("打开模型文件…");
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::openModel);
    leftLayout->addWidget(btnLoad);

    auto *btnReload = new QPushButton("重新加载");
    connect(btnReload, &QPushButton::clicked, this, &MainWindow::reloadModel);
    leftLayout->addWidget(btnReload);

    // 模型信息面板
    auto *grpInfo = new QGroupBox("模型信息");
    auto *formInfo = new QFormLayout(grpInfo);
    formInfo->setContentsMargins(8, 12, 8, 8);
    formInfo->setSpacing(4);

    auto makeVal = [](QLabel *&lbl, const QString &init = "—") {
        lbl = new QLabel(init);
        lbl->setObjectName("infoValue");
        lbl->setWordWrap(true);
        return lbl;
    };

    formInfo->addRow("文件路径：", makeVal(m_lblPath));
    formInfo->addRow("格式：",     makeVal(m_lblFormat));
    formInfo->addRow("顶点数：",   makeVal(m_lblVertices));
    formInfo->addRow("三角面数：", makeVal(m_lblTriangles));
    formInfo->addRow("材质数：",   makeVal(m_lblMaterials));
    formInfo->addRow("包围盒最小：", makeVal(m_lblBBoxMin));
    formInfo->addRow("包围盒最大：", makeVal(m_lblBBoxMax));
    formInfo->addRow("尺寸(XYZ)：", makeVal(m_lblBBoxSize));
    leftLayout->addWidget(grpInfo);

    // 显示选项
    auto *grpView = new QGroupBox("显示选项");
    auto *viewLayout = new QVBoxLayout(grpView);
    viewLayout->setContentsMargins(8, 12, 8, 8);
    viewLayout->setSpacing(4);

    m_chkWireframe = new QCheckBox("线框模式 (Wireframe)");
    m_chkBBox      = new QCheckBox("显示包围盒 (BBox)");
    connect(m_chkWireframe, &QCheckBox::toggled, this, &MainWindow::toggleWireframe);
    connect(m_chkBBox,      &QCheckBox::toggled, this, &MainWindow::toggleBBox);
    viewLayout->addWidget(m_chkWireframe);
    viewLayout->addWidget(m_chkBBox);
    leftLayout->addWidget(grpView);

    // 视图控制
    auto *grpCtrl = new QGroupBox("视图控制");
    auto *ctrlLayout = new QVBoxLayout(grpCtrl);
    ctrlLayout->setContentsMargins(8, 12, 8, 8);
    ctrlLayout->setSpacing(4);

    auto *btnReset = new QPushButton("重置视角 (Reset View)");
    auto *btnFit   = new QPushButton("适配模型 (Fit To Model)");
    auto *btnShot  = new QPushButton("截图保存");
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::resetView);
    connect(btnFit,   &QPushButton::clicked, this, &MainWindow::fitToModel);
    connect(btnShot,  &QPushButton::clicked, this, &MainWindow::takeScreenshot);
    ctrlLayout->addWidget(btnReset);
    ctrlLayout->addWidget(btnFit);
    ctrlLayout->addWidget(btnShot);
    leftLayout->addWidget(grpCtrl);

    // 操作说明
    auto *grpHelp = new QGroupBox("操作说明");
    auto *helpLayout = new QVBoxLayout(grpHelp);
    helpLayout->setContentsMargins(8, 12, 8, 8);
    auto *lblHelp = new QLabel(
        "左键拖拽：旋转视角\n"
        "右键拖拽：平移场景\n"
        "滚轮：缩放\n"
        "支持格式：OBJ, PLY, GLB,\n"
        "GLTF, FBX, STL, DAE"
    );
    lblHelp->setStyleSheet("color: #808090; font-size: 11px;");
    helpLayout->addWidget(lblHelp);
    leftLayout->addWidget(grpHelp);

    leftLayout->addStretch();

    // ── 右侧：渲染窗口 + 日志 ──────────────────────────────────────────────
    auto *rightWidget = new QWidget;
    auto *rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    m_renderWidget = new RenderWidget;
    connect(m_renderWidget, &RenderWidget::modelLoaded,
            this, &MainWindow::onModelLoaded);

    m_logWidget = new QTextEdit;
    m_logWidget->setReadOnly(true);
    m_logWidget->setFixedHeight(100);
    m_logWidget->setPlaceholderText("日志输出…");

    rightLayout->addWidget(m_renderWidget, 1);
    rightLayout->addWidget(m_logWidget, 0);

    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
}

void MainWindow::setupMenuBar()
{
    auto *fileMenu = menuBar()->addMenu("文件(&F)");
    auto *actOpen   = fileMenu->addAction("打开模型…"); actOpen->setShortcut(QKeySequence::Open);   connect(actOpen,   &QAction::triggered, this, &MainWindow::openModel);
    auto *actReload = fileMenu->addAction("重新加载");   actReload->setShortcut(QKeySequence("Ctrl+R")); connect(actReload, &QAction::triggered, this, &MainWindow::reloadModel);
    fileMenu->addSeparator();
    auto *actShot   = fileMenu->addAction("截图保存");   actShot->setShortcut(QKeySequence("Ctrl+S"));   connect(actShot,   &QAction::triggered, this, &MainWindow::takeScreenshot);
    fileMenu->addSeparator();
    auto *actQuit   = fileMenu->addAction("退出");         actQuit->setShortcut(QKeySequence::Quit);        connect(actQuit,   &QAction::triggered, qApp, &QApplication::quit);

    auto *viewMenu = menuBar()->addMenu("视图(&V)");
    auto *actReset  = viewMenu->addAction("重置视角"); actReset->setShortcut(QKeySequence("Ctrl+0")); connect(actReset, &QAction::triggered, this, &MainWindow::resetView);
    auto *actFit    = viewMenu->addAction("适配模型"); actFit->setShortcut(QKeySequence("Ctrl+F"));   connect(actFit,   &QAction::triggered, this, &MainWindow::fitToModel);
}

void MainWindow::setupToolBar()
{
    auto *tb = addToolBar("主工具栏");
    tb->setMovable(false);
    tb->addAction("打开",   this, &MainWindow::openModel);
    tb->addAction("重载",   this, &MainWindow::reloadModel);
    tb->addSeparator();
    tb->addAction("重置视角", this, &MainWindow::resetView);
    tb->addAction("适配模型", this, &MainWindow::fitToModel);
    tb->addSeparator();
    tb->addAction("截图",   this, &MainWindow::takeScreenshot);
}

// ─── 模型加载 ────────────────────────────────────────────────────────────────

void MainWindow::openModel()
{
    QString path = QFileDialog::getOpenFileName(
        this, "打开三维模型文件", "",
        "三维模型 (*.obj *.ply *.glb *.gltf *.fbx *.stl *.dae *.3ds);;"
        "OBJ 文件 (*.obj);;"
        "PLY 文件 (*.ply);;"
        "GLB/GLTF 文件 (*.glb *.gltf);;"
        "所有文件 (*.*)"
    );
    if (!path.isEmpty())
        loadModelFromPath(path);
}

void MainWindow::reloadModel()
{
    if (!m_currentModelPath.isEmpty())
        loadModelFromPath(m_currentModelPath);
    else
        appendLog("[警告] 尚未加载任何模型，无法重新加载");
}

void MainWindow::loadModelFromPath(const QString &path)
{
    appendLog(QString("[信息] 正在加载：%1").arg(path));
    statusBar()->showMessage("正在加载模型…");

    AssimpModelLoader loader;
    QFileInfo fi(path);
    std::string ext = fi.suffix().toLower().toStdString();

    if (!loader.supportsFormat(ext)) {
        QString msg = QString("[错误] 不支持的格式：.%1").arg(fi.suffix());
        appendLog(msg);
        statusBar()->showMessage(msg);
        return;
    }

    std::string errMsg;
    auto asset = loader.load(path.toStdString(), errMsg);
    if (!asset) {
        QString msg = QString("[错误] 加载失败：%1").arg(QString::fromStdString(errMsg));
        appendLog(msg);
        statusBar()->showMessage(msg);
        return;
    }

    m_currentModelPath = path;

    // 更新信息面板
    m_lblPath->setText(fi.fileName());
    m_lblFormat->setText(fi.suffix().toUpper());
    m_lblVertices->setText(QString::number(asset->totalVertices));
    m_lblTriangles->setText(QString::number(asset->totalTriangles));
    m_lblMaterials->setText(QString::number(asset->materials.size()));
    m_lblBBoxMin->setText(QString("(%1, %2, %3)")
        .arg(asset->bboxMin[0], 0, 'f', 2)
        .arg(asset->bboxMin[1], 0, 'f', 2)
        .arg(asset->bboxMin[2], 0, 'f', 2));
    m_lblBBoxMax->setText(QString("(%1, %2, %3)")
        .arg(asset->bboxMax[0], 0, 'f', 2)
        .arg(asset->bboxMax[1], 0, 'f', 2)
        .arg(asset->bboxMax[2], 0, 'f', 2));
    float sx = asset->bboxMax[0] - asset->bboxMin[0];
    float sy = asset->bboxMax[1] - asset->bboxMin[1];
    float sz = asset->bboxMax[2] - asset->bboxMin[2];
    m_lblBBoxSize->setText(QString("%1 × %2 × %3")
        .arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(sz, 0, 'f', 2));

    m_renderWidget->loadModel(*asset);

    appendLog(QString("[成功] 已加载：%1 顶点=%2 三角面=%3")
        .arg(fi.fileName())
        .arg(asset->totalVertices)
        .arg(asset->totalTriangles));
    statusBar()->showMessage(QString("已加载：%1  顶点=%2  三角面=%3")
        .arg(fi.fileName())
        .arg(asset->totalVertices)
        .arg(asset->totalTriangles));
}

// ─── 槽函数 ──────────────────────────────────────────────────────────────────

void MainWindow::onModelLoaded(uint32_t vertices, uint32_t triangles)
{
    appendLog(QString("[渲染] GPU 上传完成：顶点=%1 三角面=%2").arg(vertices).arg(triangles));
}

void MainWindow::appendLog(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logWidget->append(QString("[%1] %2").arg(ts, msg));
}

void MainWindow::resetView()
{
    m_renderWidget->resetView();
    appendLog("[视图] 重置视角");
}

void MainWindow::fitToModel()
{
    m_renderWidget->fitToModel();
    appendLog("[视图] 适配模型");
}

void MainWindow::toggleWireframe(bool checked)
{
    m_renderWidget->setWireframe(checked);
    appendLog(checked ? "[显示] 线框模式 ON" : "[显示] 线框模式 OFF");
}

void MainWindow::toggleBBox(bool checked)
{
    m_renderWidget->setShowBBox(checked);
    appendLog(checked ? "[显示] 包围盒 ON" : "[显示] 包围盒 OFF");
}

void MainWindow::takeScreenshot()
{
    QString path = QFileDialog::getSaveFileName(
        this, "保存截图", "demo02_screenshot.png",
        "PNG 图像 (*.png);;JPEG 图像 (*.jpg)");
    if (path.isEmpty()) return;

    QPixmap pix = m_renderWidget->grab();
    if (pix.save(path))
        appendLog(QString("[截图] 已保存：%1").arg(path));
    else
        appendLog(QString("[错误] 截图保存失败：%1").arg(path));
}
