#ifndef IMAGECREATOR_H
#define IMAGECREATOR_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QDateTime>
#include <QBuffer>
#include <math.h>
#include <memory>

class ImageCreator final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ImageCreator)

    #define PI 3.14159265f

public:
    explicit ImageCreator(QObject *parent = nullptr);
    ~ImageCreator() override;
    QByteArray createImage();

private:
    template<typename T> std::pair<T, T> MsCoord(int val, int hlen);
    template<typename T> std::pair<T, T> HrCoord(int hval, int mval, int hlen);

    const int FONT_SIZE = 108;
    const int MAXX = 1920;
    const int MAXY = 1080;
    const int CLOCK_DIAMETER = 800;
    const int CLOCK_X = MAXX / 2 - CLOCK_DIAMETER / 2;
    const int CLOCK_Y = 30;
    const int CLOCK_CENTER_X = CLOCK_X + (CLOCK_DIAMETER / 2);
    const int CLOCK_CENTER_Y = CLOCK_Y + (CLOCK_DIAMETER / 2);
    const int SECHAND = 370;
    const int MINHAND = 295;
    const int HRHAND = 220;
    const int CX = CLOCK_DIAMETER / 2;
    const int CY = CLOCK_DIAMETER / 2;
    const QString FONTNAME = "Tahoma";
};

#endif // IMAGECREATOR_H
