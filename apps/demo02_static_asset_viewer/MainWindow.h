/**
 * MainWindow.h — Demo-02 主窗口
 *
 * 布局：
 *   顶部：菜单栏 + 工具栏
 *   左侧：模型加载面板 + 属性信息面板（固定宽度 280px）
 *   中间：三维模型显示窗口（RenderWidget）
 *   底部：日志输出窗口
 */

#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include "asset/ModelAsset.h"

class RenderWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &initialModel = QString(), QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void openModel();
    void reloadModel();
    void resetView();
    void fitToModel();
    void takeScreenshot();
    void toggleWireframe(bool checked);
    void toggleBBox(bool checked);
    void onModelLoaded(uint32_t vertices, uint32_t triangles);
    void appendLog(const QString &msg);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void loadModelFromPath(const QString &path);

    // Widgets
    RenderWidget *m_renderWidget  = nullptr;
    QTextEdit    *m_logWidget     = nullptr;

    // Info panel labels
    QLabel *m_lblPath       = nullptr;
    QLabel *m_lblFormat     = nullptr;
    QLabel *m_lblVertices   = nullptr;
    QLabel *m_lblTriangles  = nullptr;
    QLabel *m_lblMaterials  = nullptr;
    QLabel *m_lblBBoxMin    = nullptr;
    QLabel *m_lblBBoxMax    = nullptr;
    QLabel *m_lblBBoxSize   = nullptr;

    // Controls
    QCheckBox *m_chkWireframe = nullptr;
    QCheckBox *m_chkBBox      = nullptr;

    QString m_currentModelPath;
};
