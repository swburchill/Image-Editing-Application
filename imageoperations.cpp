#include <QRgb>
#include <QColor>

#include <math.h>

#include "imageoperations.h"

imageOperations::imageOperations()
{
    imgOne = new QImage();
    imgTwo = new QImage();
    modifiedImg = new QImage();
}

imageOperations::~imageOperations(){}

void imageOperations::updateImages(QImage* original, QImage* dissolve)
{
    imgOne = original;
    imgTwo = dissolve;
}

int imageOperations::quantized(int value, int levels)
{
    //Code provided by Mario Costa Sousa in CPSC453 W15 Lecture Slides
    int q = floor(255*floor(value*(levels-1)/255.0)/(levels-1));
    return q;
}

int imageOperations::clamp(int value)
{
    int clamped = value;
    if(value < 0)
    {
        clamped = 0;
    }
    if(value > 255)
    {
        clamped = 255;
    }
    return clamped;
}

void imageOperations::quantize(int levels)
{
    modifiedImg = new QImage(*imgOne);
    if(levels == 0)
    {
        for(int i = 0; i < imgOne->width(); i++)
        {
            for(int j = 0; j < imgOne->height(); j++)
            {
                modifiedImg->setPixel(i, j, qRgb(0,0,0));
            }
        }
    }
    else
    {
        for(int i = 0; i < imgOne->width(); i++)
        {
            for(int j = 0; j < imgOne->height(); j++)
            {
                QRgb tempColourRgb = imgOne->pixel(QPoint(i, j));
                QColor colour(tempColourRgb);
                modifiedImg->setPixel(i, j, qRgb(clamp(quantized(colour.red(), levels)), clamp(quantized(colour.green(), levels)), clamp(quantized(colour.blue(), levels))));
            }
        }
    }
    notify();
}

void imageOperations::brighten(int factor)
{
    modifiedImg = new QImage(*imgOne);
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            double scale = factor;
            int red = (scale/100)*colours.red();
            int green = (scale/100)*colours.green();
            int blue = (scale/100)*colours.blue();
            modifiedImg->setPixel(i, j, qRgb(clamp(red), clamp(green), clamp(blue)));
        }
    }
    notify();
}

void imageOperations::saturation(int factor)
{
    modifiedImg = new QImage(*imgOne);
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            double luminanceValue = ((0.3*colours.red())+(0.59*colours.green())+(0.11*colours.blue()));
            double scale = factor;
            int red = ((((100-scale)/100)*luminanceValue)+((scale/100)*colours.red()));
            int green = ((((100-scale)/100)*luminanceValue)+((scale/100)*colours.green()));
            int blue = ((((100-scale)/100)*luminanceValue)+((scale/100)*colours.blue()));
            modifiedImg->setPixel(i, j, qRgb(clamp(red), clamp(green), clamp(blue)));
        }
    }
    notify();
}

void imageOperations::scale(int factor)
{
    modifiedImg = new QImage((imgOne->width()*factor),imgOne->height(),QImage::Format_RGB888);
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            modifiedImg->setPixel(i*factor, j, qRgb(clamp(colours.red()), clamp(colours.green()), clamp(colours.blue())));
        }
    }
    notify();
}

void imageOperations::rotate(int degrees)
{
    //Get max image size for when the image is spun
    int maxSize = sqrt(pow(imgOne->width(),2)+pow(imgOne->height(),2))+2;
    int centerX = imgOne->width()/2;
    int centerY = imgOne->height()/2;
    //Get the offset from the image size and the new max image size so we can keep the image centered
    int diffX = (maxSize-imgOne->width())/2;
    int diffY = (maxSize-imgOne->height())/2;
    modifiedImg = new QImage(maxSize,maxSize,QImage::Format_RGB888);
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            //calculate the rotation around the image center then move image to center of the view
            int newX = ((i-centerX)*cos(M_PI*degrees/180)+(j-centerY)*sin(M_PI*degrees/180))+centerX+diffX;
            int newY = (-(i-centerX)*sin(M_PI*degrees/180)+(j-centerY)*cos(M_PI*degrees/180))+centerY+diffY;;
            if(newX < maxSize && newX > -1 && newY > -1 && newY < maxSize)
            {
                modifiedImg->setPixel(newX, newY, qRgb(clamp(colours.red()), clamp(colours.green()), clamp(colours.blue())));
            }
        }
    }
    notify();
}

void imageOperations::contrast(int factor)
{
    modifiedImg = new QImage(*imgOne);
    double luminanceValue = 0;
    int pixelCount = 0;
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            luminanceValue += ((0.3*colours.red())+(0.59*colours.green())+(0.11*colours.blue()));
            pixelCount +=1;
        }
    }
    double averageLuminance = luminanceValue/pixelCount;
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            double scale = factor;
            int red = ((((100-scale)/100)*averageLuminance)+((scale/100)*colours.red()));
            int green = ((((100-scale)/100)*averageLuminance)+((scale/100)*colours.green()));
            int blue = ((((100-scale)/100)*averageLuminance)+((scale/100)*colours.blue()));
            modifiedImg->setPixel(i, j, qRgb(clamp(red), clamp(green), clamp(blue)));
        }
    }
    notify();
}

void imageOperations::dissolve(int distance)
{
    //Allow images of different size to dissolve together
    int maxSizeX = imgOne->width();
    int maxSizeY = imgOne->height();
    if(imgTwo->width() > maxSizeX)
    {
        maxSizeX = imgTwo->width();
    }
    if(imgTwo->height() > maxSizeY)
    {
        maxSizeY = imgTwo->height();
    }
    modifiedImg = new QImage(maxSizeX,maxSizeY,QImage::Format_RGB888);
    //Get the difference of image size and Max size so image can be placed in center
    int diffX = (maxSizeX-imgOne->width())/2;
    int diffY = (maxSizeY-imgOne->height())/2;
    for(int i = 0; i < imgOne->width(); i++)
    {
        for(int j = 0; j < imgOne->height(); j++)
        {
            QRgb tempColoursRgb = imgOne->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            double scale = distance;
            int red = ((100-scale)/100)*colours.red();
            int green = ((100-scale)/100)*colours.green();
            int blue = ((100-scale)/100)*colours.blue();
            modifiedImg->setPixel(i+diffX, j+diffY, qRgb(clamp(red), clamp(green), clamp(blue)));
        }
    }
    //Get the difference of image size and Max size so image can be placed in center
    diffX = (maxSizeX-imgTwo->width())/2;
    diffY = (maxSizeY-imgTwo->height())/2;
    for(int i = 0; i < imgTwo->width(); i++)
    {
        for(int j = 0; j < imgTwo->height(); j++)
        {
            QRgb tempColoursRgb = imgTwo->pixel(QPoint(i, j));
            QColor colours(tempColoursRgb);
            double scale = distance;
            //Only disolve pixels that are affected after centering the 2nd images
            QRgb modifiedColoursRgb = modifiedImg->pixel(QPoint(i+diffX, j+diffY));
            QColor modifiedColours(modifiedColoursRgb);
            int red = ((scale/100)*colours.red())+modifiedColours.red();
            int green = ((scale/100)*colours.green())+modifiedColours.green();
            int blue = ((scale/100)*colours.blue())+modifiedColours.blue();
            modifiedImg->setPixel(i+diffX, j+diffY, qRgb(clamp(red), clamp(green), clamp(blue)));
        }
    }
    notify();
}

void imageOperations::notify()
{
    emit operationPerformed(modifiedImg);
}
