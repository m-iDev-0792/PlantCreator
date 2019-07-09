/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <painterview.h>
#include <sketchview.h>
#include "HJGWidget.h"
#include "triangulationview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Sketch;
    QAction *actionExport_Sketch;
    QAction *actionExport_Obj;
    QAction *actionUndo;
    QAction *actionAdd_Ref;
    QAction *actionClearAll;
    QAction *actionFrame;
    QAction *actionConstruct;
    QAction *actionClearModel;
    QAction *actiontest;
    QWidget *centralWidget;
    PainterView *widget;
    SketchView *reducedWidget;
    TriangulationView *triangulationView;
    HJGWidget *openGLWidget;
    QTabWidget *tabWidget;
    QWidget *settingTab;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QComboBox *petalTex;
    QLabel *label_2;
    QComboBox *branchTex;
    QLabel *label_3;
    QComboBox *leafTex;
    QLabel *label_5;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QSlider *thresholdSlider;
    QLabel *thresholdLabel;
    QLabel *label_6;
    QDoubleSpinBox *baseRadius;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_9;
    QDoubleSpinBox *branchGrowthPointGap;
    QLabel *label_10;
    QDoubleSpinBox *leafGrowthPointGap;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_11;
    QDoubleSpinBox *branchGrowthRate;
    QLabel *label_12;
    QDoubleSpinBox *leafGrowthRate;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_13;
    QLineEdit *bendFactor;
    QLabel *label_14;
    QLineEdit *caveFactor;
    QWidget *debugTab;
    QWidget *layoutWidget4;
    QVBoxLayout *verticalLayout;
    QCheckBox *showDebug;
    QPushButton *showReducedBtn;
    QComboBox *petalCombo;
    QCheckBox *printEdge;
    QCheckBox *printAllPoints;
    QPushButton *delaunay;
    QPushButton *bullet;
    QPushButton *detectBtn;
    QLabel *label_4;
    QPushButton *saveBtn;
    QPushButton *saveReduced;
    QCheckBox *rotateToXZ;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1216, 935);
        actionLoad_Sketch = new QAction(MainWindow);
        actionLoad_Sketch->setObjectName(QStringLiteral("actionLoad_Sketch"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/image/loadSketch.png"), QSize(), QIcon::Normal, QIcon::On);
        actionLoad_Sketch->setIcon(icon);
        actionExport_Sketch = new QAction(MainWindow);
        actionExport_Sketch->setObjectName(QStringLiteral("actionExport_Sketch"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/image/export.png"), QSize(), QIcon::Normal, QIcon::On);
        actionExport_Sketch->setIcon(icon1);
        actionExport_Obj = new QAction(MainWindow);
        actionExport_Obj->setObjectName(QStringLiteral("actionExport_Obj"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/image/saveStroke.png"), QSize(), QIcon::Normal, QIcon::On);
        actionExport_Obj->setIcon(icon2);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon/image/undo.png"), QSize(), QIcon::Normal, QIcon::On);
        actionUndo->setIcon(icon3);
        actionAdd_Ref = new QAction(MainWindow);
        actionAdd_Ref->setObjectName(QStringLiteral("actionAdd_Ref"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/image/addRef.png"), QSize(), QIcon::Normal, QIcon::On);
        actionAdd_Ref->setIcon(icon4);
        actionClearAll = new QAction(MainWindow);
        actionClearAll->setObjectName(QStringLiteral("actionClearAll"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icon/image/clear.png"), QSize(), QIcon::Normal, QIcon::On);
        actionClearAll->setIcon(icon5);
        actionFrame = new QAction(MainWindow);
        actionFrame->setObjectName(QStringLiteral("actionFrame"));
        actionFrame->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icon/image/frame.png"), QSize(), QIcon::Normal, QIcon::On);
        actionFrame->setIcon(icon6);
        actionConstruct = new QAction(MainWindow);
        actionConstruct->setObjectName(QStringLiteral("actionConstruct"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icon/image/construct.png"), QSize(), QIcon::Normal, QIcon::On);
        actionConstruct->setIcon(icon7);
        actionClearModel = new QAction(MainWindow);
        actionClearModel->setObjectName(QStringLiteral("actionClearModel"));
        actionClearModel->setIcon(icon5);
        actiontest = new QAction(MainWindow);
        actiontest->setObjectName(QStringLiteral("actiontest"));
        actiontest->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new PainterView(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 521, 431));
        reducedWidget = new SketchView(centralWidget);
        reducedWidget->setObjectName(QStringLiteral("reducedWidget"));
        reducedWidget->setGeometry(QRect(10, 450, 521, 431));
        triangulationView = new TriangulationView(centralWidget);
        triangulationView->setObjectName(QStringLiteral("triangulationView"));
        triangulationView->setGeometry(QRect(540, 450, 471, 311));
        openGLWidget = new HJGWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(540, 10, 491, 431));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1040, 20, 161, 421));
        settingTab = new QWidget();
        settingTab->setObjectName(QStringLiteral("settingTab"));
        layoutWidget = new QWidget(settingTab);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 236, 141, 157));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        petalTex = new QComboBox(layoutWidget);
        petalTex->setObjectName(QStringLiteral("petalTex"));

        verticalLayout_2->addWidget(petalTex);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        branchTex = new QComboBox(layoutWidget);
        branchTex->setObjectName(QStringLiteral("branchTex"));

        verticalLayout_2->addWidget(branchTex);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        leafTex = new QComboBox(layoutWidget);
        leafTex->setObjectName(QStringLiteral("leafTex"));

        verticalLayout_2->addWidget(leafTex);

        label_5 = new QLabel(settingTab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 0, 121, 16));
        layoutWidget1 = new QWidget(settingTab);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 278, 28));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        thresholdSlider = new QSlider(layoutWidget1);
        thresholdSlider->setObjectName(QStringLiteral("thresholdSlider"));
        thresholdSlider->setMinimum(4);
        thresholdSlider->setMaximum(30);
        thresholdSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(thresholdSlider);

        thresholdLabel = new QLabel(layoutWidget1);
        thresholdLabel->setObjectName(QStringLiteral("thresholdLabel"));

        horizontalLayout->addWidget(thresholdLabel);

        label_6 = new QLabel(settingTab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 80, 121, 16));
        baseRadius = new QDoubleSpinBox(settingTab);
        baseRadius->setObjectName(QStringLiteral("baseRadius"));
        baseRadius->setGeometry(QRect(20, 100, 111, 24));
        baseRadius->setMinimum(0.01);
        baseRadius->setSingleStep(0.01);
        baseRadius->setValue(0.2);
        label_7 = new QLabel(settingTab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 130, 81, 16));
        label_8 = new QLabel(settingTab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 180, 81, 16));
        layoutWidget2 = new QWidget(settingTab);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 150, 177, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_2->addWidget(label_9);

        branchGrowthPointGap = new QDoubleSpinBox(layoutWidget2);
        branchGrowthPointGap->setObjectName(QStringLiteral("branchGrowthPointGap"));
        branchGrowthPointGap->setDecimals(1);
        branchGrowthPointGap->setMinimum(0.1);
        branchGrowthPointGap->setSingleStep(0.1);
        branchGrowthPointGap->setValue(20);

        horizontalLayout_2->addWidget(branchGrowthPointGap);

        label_10 = new QLabel(layoutWidget2);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_2->addWidget(label_10);

        leafGrowthPointGap = new QDoubleSpinBox(layoutWidget2);
        leafGrowthPointGap->setObjectName(QStringLiteral("leafGrowthPointGap"));
        leafGrowthPointGap->setDecimals(1);
        leafGrowthPointGap->setMinimum(0.1);
        leafGrowthPointGap->setSingleStep(0.1);
        leafGrowthPointGap->setValue(10);

        horizontalLayout_2->addWidget(leafGrowthPointGap);

        layoutWidget3 = new QWidget(settingTab);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 200, 177, 26));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget3);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_3->addWidget(label_11);

        branchGrowthRate = new QDoubleSpinBox(layoutWidget3);
        branchGrowthRate->setObjectName(QStringLiteral("branchGrowthRate"));
        branchGrowthRate->setDecimals(1);
        branchGrowthRate->setMinimum(0.1);
        branchGrowthRate->setSingleStep(0.1);
        branchGrowthRate->setValue(5);

        horizontalLayout_3->addWidget(branchGrowthRate);

        label_12 = new QLabel(layoutWidget3);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_3->addWidget(label_12);

        leafGrowthRate = new QDoubleSpinBox(layoutWidget3);
        leafGrowthRate->setObjectName(QStringLiteral("leafGrowthRate"));
        leafGrowthRate->setDecimals(1);
        leafGrowthRate->setMinimum(0.1);
        leafGrowthRate->setSingleStep(0.1);
        leafGrowthRate->setValue(5);

        horizontalLayout_3->addWidget(leafGrowthRate);

        widget1 = new QWidget(settingTab);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(10, 50, 141, 26));
        horizontalLayout_4 = new QHBoxLayout(widget1);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(widget1);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_4->addWidget(label_13);

        bendFactor = new QLineEdit(widget1);
        bendFactor->setObjectName(QStringLiteral("bendFactor"));

        horizontalLayout_4->addWidget(bendFactor);

        label_14 = new QLabel(widget1);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_4->addWidget(label_14);

        caveFactor = new QLineEdit(widget1);
        caveFactor->setObjectName(QStringLiteral("caveFactor"));

        horizontalLayout_4->addWidget(caveFactor);

        tabWidget->addTab(settingTab, QString());
        debugTab = new QWidget();
        debugTab->setObjectName(QStringLiteral("debugTab"));
        layoutWidget4 = new QWidget(debugTab);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(20, 10, 113, 170));
        verticalLayout = new QVBoxLayout(layoutWidget4);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        showDebug = new QCheckBox(layoutWidget4);
        showDebug->setObjectName(QStringLiteral("showDebug"));

        verticalLayout->addWidget(showDebug);

        showReducedBtn = new QPushButton(layoutWidget4);
        showReducedBtn->setObjectName(QStringLiteral("showReducedBtn"));

        verticalLayout->addWidget(showReducedBtn);

        petalCombo = new QComboBox(layoutWidget4);
        petalCombo->setObjectName(QStringLiteral("petalCombo"));

        verticalLayout->addWidget(petalCombo);

        printEdge = new QCheckBox(layoutWidget4);
        printEdge->setObjectName(QStringLiteral("printEdge"));

        verticalLayout->addWidget(printEdge);

        printAllPoints = new QCheckBox(layoutWidget4);
        printAllPoints->setObjectName(QStringLiteral("printAllPoints"));

        verticalLayout->addWidget(printAllPoints);

        delaunay = new QPushButton(layoutWidget4);
        delaunay->setObjectName(QStringLiteral("delaunay"));

        verticalLayout->addWidget(delaunay);

        bullet = new QPushButton(debugTab);
        bullet->setObjectName(QStringLiteral("bullet"));
        bullet->setGeometry(QRect(20, 260, 111, 32));
        detectBtn = new QPushButton(debugTab);
        detectBtn->setObjectName(QStringLiteral("detectBtn"));
        detectBtn->setGeometry(QRect(20, 290, 111, 32));
        label_4 = new QLabel(debugTab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 230, 81, 31));
        saveBtn = new QPushButton(debugTab);
        saveBtn->setObjectName(QStringLiteral("saveBtn"));
        saveBtn->setGeometry(QRect(20, 320, 111, 32));
        saveReduced = new QPushButton(debugTab);
        saveReduced->setObjectName(QStringLiteral("saveReduced"));
        saveReduced->setGeometry(QRect(20, 350, 111, 32));
        rotateToXZ = new QCheckBox(debugTab);
        rotateToXZ->setObjectName(QStringLiteral("rotateToXZ"));
        rotateToXZ->setGeometry(QRect(20, 190, 111, 31));
        tabWidget->addTab(debugTab, QString());
        MainWindow->setCentralWidget(centralWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionLoad_Sketch);
        toolBar->addAction(actionExport_Sketch);
        toolBar->addAction(actionExport_Obj);
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionAdd_Ref);
        toolBar->addAction(actionClearAll);
        toolBar->addAction(actionClearModel);
        toolBar->addSeparator();
        toolBar->addAction(actionConstruct);
        toolBar->addSeparator();
        toolBar->addAction(actionFrame);
        toolBar->addAction(actiontest);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "PlantCreator", Q_NULLPTR));
        actionLoad_Sketch->setText(QApplication::translate("MainWindow", "LoadSketch", Q_NULLPTR));
        actionExport_Sketch->setText(QApplication::translate("MainWindow", "ExportSketch", Q_NULLPTR));
        actionExport_Obj->setText(QApplication::translate("MainWindow", "ExportObj", Q_NULLPTR));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", Q_NULLPTR));
        actionAdd_Ref->setText(QApplication::translate("MainWindow", "Add Ref", Q_NULLPTR));
        actionClearAll->setText(QApplication::translate("MainWindow", "ClearAll", Q_NULLPTR));
        actionFrame->setText(QApplication::translate("MainWindow", "Frame", Q_NULLPTR));
        actionConstruct->setText(QApplication::translate("MainWindow", "Construct", Q_NULLPTR));
        actionClearModel->setText(QApplication::translate("MainWindow", "ClearModel", Q_NULLPTR));
        actiontest->setText(QApplication::translate("MainWindow", "test", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Petal Texture", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Branch Texture", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Leaf Texture", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Sampling Gap", Q_NULLPTR));
        thresholdLabel->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Base branch radius", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "GrowthGap", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "GrowthRate", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "B", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "L", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "B", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "L", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "Bend", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "Cave", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(settingTab), QApplication::translate("MainWindow", "Setting", Q_NULLPTR));
        showDebug->setText(QApplication::translate("MainWindow", "Debug View", Q_NULLPTR));
        showReducedBtn->setText(QApplication::translate("MainWindow", "Analyze", Q_NULLPTR));
        printEdge->setText(QApplication::translate("MainWindow", "edge points", Q_NULLPTR));
        printAllPoints->setText(QApplication::translate("MainWindow", "all points", Q_NULLPTR));
        delaunay->setText(QApplication::translate("MainWindow", "Delaunay", Q_NULLPTR));
        bullet->setText(QApplication::translate("MainWindow", "Simulate", Q_NULLPTR));
        detectBtn->setText(QApplication::translate("MainWindow", "Detect", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Physical\n"
"Simulation", Q_NULLPTR));
        saveBtn->setText(QApplication::translate("MainWindow", "Save Sketch", Q_NULLPTR));
        saveReduced->setText(QApplication::translate("MainWindow", "Save Analysis", Q_NULLPTR));
        rotateToXZ->setText(QApplication::translate("MainWindow", "Rotate Flower\n"
"To XZ Plane", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(debugTab), QApplication::translate("MainWindow", "Debug", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
