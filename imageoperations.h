#ifndef IMAGEOPERATIONS_H
#define IMAGEOPERATIONS_H

#include <QImage>

class imageOperations : public QObject
{
    Q_OBJECT

public:
    imageOperations();
    ~imageOperations();
    void notify();
    int quantized(int value, int levels);
    int clamp(int value);

public slots:
    void quantize(int levels);
    void brighten(int factor);
    void saturation(int factor);
    void scale(int factor);
    void rotate(int degrees);
    void contrast(int factor);
    void dissolve(int distance);
    void updateImages(QImage* original, QImage* dissolve);

signals:
    void operationPerformed(QImage* result);

private:
    QImage* imgOne;
    QImage* imgTwo;
    QImage* modifiedImg;
};

#endif // IMAGEOPERATIONS_H
