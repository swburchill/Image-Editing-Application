#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QAction>
#include <QMenu>

#include "imageoperations.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadImageOne();
    void loadImageTwo();
    void operationModeQuantize();
    void operationModeBrighten();
    void operationModeSaturation();
    void operationModeScale();
    void operationModeRotate();
    void operationModeContrast();
    void operationModeDissolve();
    void popupMenuRequested(QPoint);
    void redrawModifiedImage(QImage* result);
    void saveModifiedImage();
    void checkRotateSpinBoxRange(int value);

signals:
    void updateImages(QImage* original, QImage* dissolve);
    void correctedRotateSpinBox(int value);

private:
    Ui::MainWindow *ui;

    QImage* imageOne;
    QImage* imageTwo;
    QImage* modifiedImage;
    QAction* menuLoadImageOne;
    QAction* menuLoadImageTwo;
    QAction* menuSaveModifiedImage;
    QAction* setOperationModeQuantize;
    QAction* setOperationModeBrighten;
    QAction* setOperationModeSaturation;
    QAction* setOperationModeScale;
    QAction* setOperationModeRotate;
    QAction* setOperationModeContrast;
    QAction* setOperationModeDissolve;
    QMenu* popupMenu;
    imageOperations* imgOp;

    void connectAllSlots();
    void enableAllMenuOptions();
    void hideAllOperations();
    void initializePopupMenu();
    void onlyLoadImageOnStartup();
    void resetSpinBoxes();
    void updateImageLinks();
};

#endif // MAINWINDOW_H
