#include <QtGui>
#include <QFileDialog>
#include <QGraphicsPixmapItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    imageOne = new QImage();
    imageTwo = new QImage();
    modifiedImage = new QImage();
    imgOp = new imageOperations();
    initializePopupMenu();
    hideAllOperations();
    connectAllSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializePopupMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    popupMenu = new QMenu();
    menuLoadImageOne = popupMenu->addAction("Load Image to Modify");
    menuLoadImageTwo = popupMenu->addAction("Load Image to Dissolve");
    menuSaveModifiedImage = popupMenu->addAction("Save Modified Image");
    setOperationModeQuantize = popupMenu->addAction("Set Operation Mode Quantize");
    setOperationModeBrighten = popupMenu->addAction("Set Operation Mode Brighten");
    setOperationModeSaturation = popupMenu->addAction("Set Operation Mode Saturation");
    setOperationModeScale = popupMenu->addAction("Set Operation Mode Scale");
    setOperationModeRotate = popupMenu->addAction("Set Operation Mode Rotate");
    setOperationModeContrast = popupMenu->addAction("Set Operation Mode Contrast");
    setOperationModeDissolve = popupMenu->addAction("Set Operation Mode Dissolve");
    onlyLoadImageOnStartup();
}

void MainWindow::onlyLoadImageOnStartup()
{
    menuLoadImageTwo->setVisible(false);
    menuSaveModifiedImage->setVisible(false);
    setOperationModeQuantize->setVisible(false);
    setOperationModeBrighten->setVisible(false);
    setOperationModeSaturation->setVisible(false);
    setOperationModeScale->setVisible(false);
    setOperationModeRotate->setVisible(false);
    setOperationModeContrast->setVisible(false);
    setOperationModeDissolve->setVisible(false);
}

void MainWindow::hideAllOperations()
{

    ui->quantizeLabel->hide();
    ui->quantizeUnitLabel->hide();
    ui->quantizeSpinBox->hide();

    ui->brightenLabel->hide();
    ui->brightenUnitLabel->hide();
    ui->brightenSpinBox->hide();

    ui->saturationLabel->hide();
    ui->saturationUnitLabel->hide();
    ui->saturationSpinBox->hide();

    ui->scaleLabel->hide();
    ui->scaleUnitLabel->hide();
    ui->scaleSpinBox->hide();

    ui->rotateLabel->hide();
    ui->rotateUnitLabel->hide();
    ui->rotateSpinBox->hide();

    ui->contrastLabel->hide();
    ui->contrastUnitLabel->hide();
    ui->contrastSpinBox->hide();

    menuLoadImageTwo->setVisible(false);
    ui->dissolveImageView->hide();
    ui->dissolveLabel->hide();
    ui->dissolveUnitLabel->hide();
    ui->dissolveSpinBox->hide();

    resetSpinBoxes();
    redrawModifiedImage(imageOne);
}

void MainWindow::connectAllSlots()
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popupMenuRequested(QPoint)));
    connect(menuLoadImageOne, SIGNAL(triggered()), this, SLOT(loadImageOne()));
    connect(menuLoadImageTwo, SIGNAL(triggered()), this, SLOT(loadImageTwo()));
    connect(menuSaveModifiedImage, SIGNAL(triggered()), this, SLOT(saveModifiedImage()));
    connect(setOperationModeQuantize, SIGNAL(triggered()), this, SLOT(operationModeQuantize()));
    connect(setOperationModeBrighten, SIGNAL(triggered()), this, SLOT(operationModeBrighten()));
    connect(setOperationModeSaturation, SIGNAL(triggered()), this, SLOT(operationModeSaturation()));
    connect(setOperationModeScale, SIGNAL(triggered()), this, SLOT(operationModeScale()));
    connect(setOperationModeRotate, SIGNAL(triggered()), this, SLOT(operationModeRotate()));
    connect(setOperationModeContrast, SIGNAL(triggered()), this, SLOT(operationModeContrast()));
    connect(setOperationModeDissolve, SIGNAL(triggered()), this, SLOT(operationModeDissolve()));

    connect(ui->quantizeSpinBox, SIGNAL(valueChanged(int)), imgOp, SLOT(quantize(int)));
    connect(ui->brightenSpinBox, SIGNAL(valueChanged(int)), imgOp, SLOT(brighten(int)));
    connect(ui->saturationSpinBox, SIGNAL(valueChanged(int)), imgOp, SLOT(saturation(int)));
    connect(ui->scaleSpinBox, SIGNAL(valueChanged(int)), imgOp, SLOT(scale(int)));
    connect(ui->rotateSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkRotateSpinBoxRange(int)));
    connect(this, SIGNAL(correctedRotateSpinBox(int)), imgOp, SLOT(rotate(int)));
    connect(ui->contrastSpinBox, SIGNAL(valueChanged(int)), imgOp, SLOT(contrast(int)));
    connect(ui->dissolveSpinBox, SIGNAL(valueChanged(int)), imgOp, SLOT(dissolve(int)));
    connect(imgOp, SIGNAL(operationPerformed(QImage*)), this, SLOT(redrawModifiedImage(QImage*)));
    connect(this, SIGNAL(updateImages(QImage*, QImage*)), imgOp, SLOT(updateImages(QImage*, QImage*)));
}

void MainWindow::updateImageLinks()
{
    emit updateImages(imageOne, imageTwo);
}

void MainWindow::enableAllMenuOptions()
{
    menuSaveModifiedImage->setVisible(true);
    setOperationModeQuantize->setVisible(true);
    setOperationModeBrighten->setVisible(true);
    setOperationModeSaturation->setVisible(true);
    setOperationModeScale->setVisible(true);
    setOperationModeRotate->setVisible(true);
    setOperationModeContrast->setVisible(true);
    setOperationModeDissolve->setVisible(true);
}

void MainWindow::resetSpinBoxes()
{
    ui->quantizeSpinBox->blockSignals(true);
    ui->brightenSpinBox->blockSignals(true);
    ui->saturationSpinBox->blockSignals(true);
    ui->scaleSpinBox->blockSignals(true);
    ui->rotateSpinBox->blockSignals(true);
    ui->contrastSpinBox->blockSignals(true);
    ui->dissolveSpinBox->blockSignals(true);

    ui->quantizeSpinBox->setValue(0);
    ui->brightenSpinBox->setValue(100);
    ui->saturationSpinBox->setValue(100);
    ui->scaleSpinBox->setValue(1);
    ui->rotateSpinBox->setValue(0);
    ui->contrastSpinBox->setValue(100);
    ui->dissolveSpinBox->setValue(0);

    ui->quantizeSpinBox->blockSignals(false);
    ui->brightenSpinBox->blockSignals(false);
    ui->saturationSpinBox->blockSignals(false);
    ui->scaleSpinBox->blockSignals(false);
    ui->rotateSpinBox->blockSignals(false);
    ui->contrastSpinBox->blockSignals(false);
    ui->dissolveSpinBox->blockSignals(false);
}

void MainWindow::popupMenuRequested(QPoint point)
{
    popupMenu->popup(mapToGlobal(point));
}

void MainWindow::loadImageOne()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.bmp)"));
    if(fileName!=NULL)
    {
        enableAllMenuOptions();
        hideAllOperations();
        imageOne = new QImage(fileName);
        modifiedImage = new QImage(*imageOne);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*imageOne));
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addItem(item);
        ui->originalImageView->setScene(scene);
        ui->modifiedImageView->setScene(scene);
        updateImageLinks();
    }
}

void MainWindow::loadImageTwo()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.bmp)"));
    if(fileName!=NULL)
    {
        resetSpinBoxes();
        imageTwo = new QImage(fileName);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*imageTwo));
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addItem(item);
        ui->dissolveImageView->setScene(scene);
        updateImageLinks();
    }
    else
    {
        enableAllMenuOptions();
        hideAllOperations();
    }
}

void MainWindow::saveModifiedImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("Files (*.bmp)"));
    if(fileName!=NULL)
    {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        modifiedImage->save(&file, "BMP");
    }
}

void MainWindow::redrawModifiedImage(QImage* result)
{
    modifiedImage = result;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*modifiedImage));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(item);
    ui->modifiedImageView->setScene(scene);
    ui->modifiedImageView->resize(scene->itemsBoundingRect().width(),scene->itemsBoundingRect().height());

}

void MainWindow::operationModeQuantize()
{
    enableAllMenuOptions();
    setOperationModeQuantize->setVisible(false);
    hideAllOperations();
    ui->quantizeLabel->show();
    ui->quantizeUnitLabel->show();
    ui->quantizeSpinBox->show();
}

void MainWindow::operationModeBrighten()
{
    enableAllMenuOptions();
    setOperationModeBrighten->setVisible(false);
    hideAllOperations();
    ui->brightenLabel->show();
    ui->brightenUnitLabel->show();
    ui->brightenSpinBox->show();
}

void MainWindow::operationModeSaturation()
{
    enableAllMenuOptions();
    setOperationModeSaturation->setVisible(false);
    hideAllOperations();
    ui->saturationLabel->show();
    ui->saturationUnitLabel->show();
    ui->saturationSpinBox->show();
}

void MainWindow::operationModeScale()
{
    enableAllMenuOptions();
    setOperationModeScale->setVisible(false);
    hideAllOperations();
    ui->scaleLabel->show();
    ui->scaleUnitLabel->show();
    ui->scaleSpinBox->show();
}

void MainWindow::operationModeRotate()
{
    enableAllMenuOptions();
    setOperationModeRotate->setVisible(false);
    hideAllOperations();
    ui->rotateLabel->show();
    ui->rotateUnitLabel->show();
    ui->rotateSpinBox->show();
}

void MainWindow::checkRotateSpinBoxRange(int value)
{
    int newValue = value;
    ui->rotateSpinBox->blockSignals(true);
    if(value > 359)
    {
        newValue = 0;
        ui->rotateSpinBox->setValue(0);
    }
    if(value < 0)
    {
        newValue = 359;
        ui->rotateSpinBox->setValue(359);
    }
    ui->rotateSpinBox->blockSignals(false);
    emit correctedRotateSpinBox(newValue);
}

void MainWindow::operationModeContrast()
{
    enableAllMenuOptions();
    setOperationModeContrast->setVisible(false);
    hideAllOperations();
    ui->contrastLabel->show();
    ui->contrastUnitLabel->show();
    ui->contrastSpinBox->show();
}

void MainWindow::operationModeDissolve()
{
    enableAllMenuOptions();
    setOperationModeDissolve->setVisible(false);
    hideAllOperations();
    menuLoadImageTwo->setVisible(true);
    ui->dissolveImageView->show();
    ui->dissolveLabel->show();
    ui->dissolveUnitLabel->show();
    ui->dissolveSpinBox->show();
    loadImageTwo();
}
