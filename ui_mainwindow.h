/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
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
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_13;
    QLineEdit *bendFactor;
    QLabel *label_14;
    QLineEdit *caveFactor;
    QWidget *debugTab;
    QWidget *layoutWidget5;
    QVBoxLayout *verticalLayout;
    QCheckBox *showDebug;
    QPushButton *showReducedBtn;
    QComboBox *petalCombo;
    QCheckBox *printEdge;
    QCheckBox *printAllPoints;
    QPushButton *delaunay;
    QPushButton *saveBtn;
    QPushButton *saveReduced;
    QCheckBox *rotateToXZ;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1216, 935);
        actionLoad_Sketch = new QAction(MainWindow);
        actionLoad_Sketch->setObjectName(QString::fromUtf8("actionLoad_Sketch"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/image/loadSketch.png"), QSize(), QIcon::Normal, QIcon::On);
        actionLoad_Sketch->setIcon(icon);
        actionExport_Sketch = new QAction(MainWindow);
        actionExport_Sketch->setObjectName(QString::fromUtf8("actionExport_Sketch"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/image/export.png"), QSize(), QIcon::Normal, QIcon::On);
        actionExport_Sketch->setIcon(icon1);
        actionExport_Obj = new QAction(MainWindow);
        actionExport_Obj->setObjectName(QString::fromUtf8("actionExport_Obj"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icon/image/saveStroke.png"), QSize(), QIcon::Normal, QIcon::On);
        actionExport_Obj->setIcon(icon2);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icon/image/undo.png"), QSize(), QIcon::Normal, QIcon::On);
        actionUndo->setIcon(icon3);
        actionAdd_Ref = new QAction(MainWindow);
        actionAdd_Ref->setObjectName(QString::fromUtf8("actionAdd_Ref"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icon/image/addRef.png"), QSize(), QIcon::Normal, QIcon::On);
        actionAdd_Ref->setIcon(icon4);
        actionClearAll = new QAction(MainWindow);
        actionClearAll->setObjectName(QString::fromUtf8("actionClearAll"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icon/image/clear.png"), QSize(), QIcon::Normal, QIcon::On);
        actionClearAll->setIcon(icon5);
        actionFrame = new QAction(MainWindow);
        actionFrame->setObjectName(QString::fromUtf8("actionFrame"));
        actionFrame->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icon/image/frame.png"), QSize(), QIcon::Normal, QIcon::On);
        actionFrame->setIcon(icon6);
        actionConstruct = new QAction(MainWindow);
        actionConstruct->setObjectName(QString::fromUtf8("actionConstruct"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icon/image/construct.png"), QSize(), QIcon::Normal, QIcon::On);
        actionConstruct->setIcon(icon7);
        actionClearModel = new QAction(MainWindow);
        actionClearModel->setObjectName(QString::fromUtf8("actionClearModel"));
        actionClearModel->setIcon(icon5);
        actiontest = new QAction(MainWindow);
        actiontest->setObjectName(QString::fromUtf8("actiontest"));
        actiontest->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widget = new PainterView(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 521, 431));
        reducedWidget = new SketchView(centralWidget);
        reducedWidget->setObjectName(QString::fromUtf8("reducedWidget"));
        reducedWidget->setGeometry(QRect(10, 450, 521, 431));
        triangulationView = new TriangulationView(centralWidget);
        triangulationView->setObjectName(QString::fromUtf8("triangulationView"));
        triangulationView->setGeometry(QRect(540, 450, 471, 311));
        openGLWidget = new HJGWidget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(540, 10, 491, 431));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(1040, 20, 161, 421));
        settingTab = new QWidget();
        settingTab->setObjectName(QString::fromUtf8("settingTab"));
        layoutWidget = new QWidget(settingTab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 236, 141, 157));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        petalTex = new QComboBox(layoutWidget);
        petalTex->setObjectName(QString::fromUtf8("petalTex"));

        verticalLayout_2->addWidget(petalTex);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        branchTex = new QComboBox(layoutWidget);
        branchTex->setObjectName(QString::fromUtf8("branchTex"));

        verticalLayout_2->addWidget(branchTex);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        leafTex = new QComboBox(layoutWidget);
        leafTex->setObjectName(QString::fromUtf8("leafTex"));

        verticalLayout_2->addWidget(leafTex);

        label_5 = new QLabel(settingTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 0, 121, 16));
        layoutWidget1 = new QWidget(settingTab);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 278, 28));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        thresholdSlider = new QSlider(layoutWidget1);
        thresholdSlider->setObjectName(QString::fromUtf8("thresholdSlider"));
        thresholdSlider->setMinimum(4);
        thresholdSlider->setMaximum(30);
        thresholdSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(thresholdSlider);

        thresholdLabel = new QLabel(layoutWidget1);
        thresholdLabel->setObjectName(QString::fromUtf8("thresholdLabel"));

        horizontalLayout->addWidget(thresholdLabel);

        label_6 = new QLabel(settingTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 80, 121, 16));
        baseRadius = new QDoubleSpinBox(settingTab);
        baseRadius->setObjectName(QString::fromUtf8("baseRadius"));
        baseRadius->setGeometry(QRect(20, 100, 111, 24));
        baseRadius->setMinimum(0.010000000000000);
        baseRadius->setSingleStep(0.010000000000000);
        baseRadius->setValue(0.200000000000000);
        label_7 = new QLabel(settingTab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 130, 81, 16));
        label_8 = new QLabel(settingTab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 180, 81, 16));
        layoutWidget2 = new QWidget(settingTab);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 150, 177, 26));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_2->addWidget(label_9);

        branchGrowthPointGap = new QDoubleSpinBox(layoutWidget2);
        branchGrowthPointGap->setObjectName(QString::fromUtf8("branchGrowthPointGap"));
        branchGrowthPointGap->setDecimals(1);
        branchGrowthPointGap->setMinimum(0.100000000000000);
        branchGrowthPointGap->setSingleStep(0.100000000000000);
        branchGrowthPointGap->setValue(20.000000000000000);

        horizontalLayout_2->addWidget(branchGrowthPointGap);

        label_10 = new QLabel(layoutWidget2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_2->addWidget(label_10);

        leafGrowthPointGap = new QDoubleSpinBox(layoutWidget2);
        leafGrowthPointGap->setObjectName(QString::fromUtf8("leafGrowthPointGap"));
        leafGrowthPointGap->setDecimals(1);
        leafGrowthPointGap->setMinimum(0.100000000000000);
        leafGrowthPointGap->setSingleStep(0.100000000000000);
        leafGrowthPointGap->setValue(10.000000000000000);

        horizontalLayout_2->addWidget(leafGrowthPointGap);

        layoutWidget3 = new QWidget(settingTab);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 200, 177, 26));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_3->addWidget(label_11);

        branchGrowthRate = new QDoubleSpinBox(layoutWidget3);
        branchGrowthRate->setObjectName(QString::fromUtf8("branchGrowthRate"));
        branchGrowthRate->setDecimals(1);
        branchGrowthRate->setMinimum(0.100000000000000);
        branchGrowthRate->setSingleStep(0.100000000000000);
        branchGrowthRate->setValue(5.000000000000000);

        horizontalLayout_3->addWidget(branchGrowthRate);

        label_12 = new QLabel(layoutWidget3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_3->addWidget(label_12);

        leafGrowthRate = new QDoubleSpinBox(layoutWidget3);
        leafGrowthRate->setObjectName(QString::fromUtf8("leafGrowthRate"));
        leafGrowthRate->setDecimals(1);
        leafGrowthRate->setMinimum(0.100000000000000);
        leafGrowthRate->setSingleStep(0.100000000000000);
        leafGrowthRate->setValue(5.000000000000000);

        horizontalLayout_3->addWidget(leafGrowthRate);

        layoutWidget4 = new QWidget(settingTab);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(10, 50, 141, 26));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(layoutWidget4);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_4->addWidget(label_13);

        bendFactor = new QLineEdit(layoutWidget4);
        bendFactor->setObjectName(QString::fromUtf8("bendFactor"));

        horizontalLayout_4->addWidget(bendFactor);

        label_14 = new QLabel(layoutWidget4);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_4->addWidget(label_14);

        caveFactor = new QLineEdit(layoutWidget4);
        caveFactor->setObjectName(QString::fromUtf8("caveFactor"));

        horizontalLayout_4->addWidget(caveFactor);

        tabWidget->addTab(settingTab, QString());
        debugTab = new QWidget();
        debugTab->setObjectName(QString::fromUtf8("debugTab"));
        layoutWidget5 = new QWidget(debugTab);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(20, 10, 113, 170));
        verticalLayout = new QVBoxLayout(layoutWidget5);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        showDebug = new QCheckBox(layoutWidget5);
        showDebug->setObjectName(QString::fromUtf8("showDebug"));

        verticalLayout->addWidget(showDebug);

        showReducedBtn = new QPushButton(layoutWidget5);
        showReducedBtn->setObjectName(QString::fromUtf8("showReducedBtn"));

        verticalLayout->addWidget(showReducedBtn);

        petalCombo = new QComboBox(layoutWidget5);
        petalCombo->setObjectName(QString::fromUtf8("petalCombo"));

        verticalLayout->addWidget(petalCombo);

        printEdge = new QCheckBox(layoutWidget5);
        printEdge->setObjectName(QString::fromUtf8("printEdge"));

        verticalLayout->addWidget(printEdge);

        printAllPoints = new QCheckBox(layoutWidget5);
        printAllPoints->setObjectName(QString::fromUtf8("printAllPoints"));

        verticalLayout->addWidget(printAllPoints);

        delaunay = new QPushButton(layoutWidget5);
        delaunay->setObjectName(QString::fromUtf8("delaunay"));

        verticalLayout->addWidget(delaunay);

        saveBtn = new QPushButton(debugTab);
        saveBtn->setObjectName(QString::fromUtf8("saveBtn"));
        saveBtn->setGeometry(QRect(20, 240, 111, 32));
        saveReduced = new QPushButton(debugTab);
        saveReduced->setObjectName(QString::fromUtf8("saveReduced"));
        saveReduced->setGeometry(QRect(20, 270, 111, 32));
        rotateToXZ = new QCheckBox(debugTab);
        rotateToXZ->setObjectName(QString::fromUtf8("rotateToXZ"));
        rotateToXZ->setGeometry(QRect(20, 190, 111, 31));
        tabWidget->addTab(debugTab, QString());
        MainWindow->setCentralWidget(centralWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
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

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "PlantCreator", nullptr));
        actionLoad_Sketch->setText(QCoreApplication::translate("MainWindow", "LoadSketch", nullptr));
        actionExport_Sketch->setText(QCoreApplication::translate("MainWindow", "ExportSketch", nullptr));
        actionExport_Obj->setText(QCoreApplication::translate("MainWindow", "ExportObj", nullptr));
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        actionAdd_Ref->setText(QCoreApplication::translate("MainWindow", "Add Ref", nullptr));
        actionClearAll->setText(QCoreApplication::translate("MainWindow", "ClearAll", nullptr));
        actionFrame->setText(QCoreApplication::translate("MainWindow", "Frame", nullptr));
        actionConstruct->setText(QCoreApplication::translate("MainWindow", "Construct", nullptr));
        actionClearModel->setText(QCoreApplication::translate("MainWindow", "ClearModel", nullptr));
        actiontest->setText(QCoreApplication::translate("MainWindow", "test", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Petal Texture", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Branch Texture", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Leaf Texture", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Sampling Gap", nullptr));
        thresholdLabel->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Base branch radius", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "GrowthGap", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "GrowthRate", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "L", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "L", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Bend", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Cave", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(settingTab), QCoreApplication::translate("MainWindow", "Setting", nullptr));
        showDebug->setText(QCoreApplication::translate("MainWindow", "Debug View", nullptr));
        showReducedBtn->setText(QCoreApplication::translate("MainWindow", "Analyze", nullptr));
        printEdge->setText(QCoreApplication::translate("MainWindow", "edge points", nullptr));
        printAllPoints->setText(QCoreApplication::translate("MainWindow", "all points", nullptr));
        delaunay->setText(QCoreApplication::translate("MainWindow", "Delaunay", nullptr));
        saveBtn->setText(QCoreApplication::translate("MainWindow", "Save Sketch", nullptr));
        saveReduced->setText(QCoreApplication::translate("MainWindow", "Save Analysis", nullptr));
        rotateToXZ->setText(QCoreApplication::translate("MainWindow", "Rotate Flower\n"
"To XZ Plane", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(debugTab), QCoreApplication::translate("MainWindow", "Debug", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
