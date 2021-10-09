#include "imagecreator.h"

ImageCreator::ImageCreator(QObject *parent) : QObject{parent}
{
    qDebug("Constructor ImageCreator");
}

ImageCreator::~ImageCreator()
{
    qDebug("Destructor ImageCreator");
}

QByteArray ImageCreator::createImage()
{
    QImage image{MAXX, MAXY, QImage::Format_RGB32};
    image.fill(QColor::fromRgb(0x00, 0xbb, 0x55));
    QPainter p;
    p.begin(&image);
    QPen pen{Qt::white, 3};
    p.setPen(pen);
    QFont font{p.font()};
    font.setPixelSize(FONT_SIZE);
    font.setFamily(FONTNAME);
    p.setFont(font);
    const QRect rectangle{0, 0, MAXX, MAXY};

    // Draw clock
    p.drawEllipse(CLOCK_X, CLOCK_Y, CLOCK_DIAMETER, CLOCK_DIAMETER);

    auto dt = QDateTime::currentDateTimeUtc();
    //draw seconds hand
    auto[xs, ys] = MsCoord<int>(dt.time().second(), SECHAND);
    p.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_X + xs, CLOCK_Y + ys);

    //draw minutes hand
    auto[xm, ym] = MsCoord<int>(dt.time().minute(), MINHAND);
    p.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_X + xm, CLOCK_Y + ym);

    //draw hours hand
    auto[xh, yh] = HrCoord<int>(dt.time().hour() % 12, dt.time().minute(), HRHAND);
    p.drawLine(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_X + xh, CLOCK_Y + yh);

    // Draw text
    p.drawText(rectangle, Qt::AlignBottom | Qt::AlignHCenter, dt.toString("dd/MM/yyyy hh:mm:ss UTC"));
    p.end();

    QByteArray ba;
    QBuffer buffer{&ba};
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG", 80);
    return ba;
}

template<typename T>
std::pair<T, T> ImageCreator::MsCoord(const int& val, const int& hlen)
{
    T x, y;
    float valx = float(val) * 6; // note: each minute and seconds make a 6 degree
    if (valx >= 0 && valx <= 100)
    {
        x = CX + static_cast<T>(float(hlen) * sin(PI * valx / 180));
        y = CY - static_cast<T>(float(hlen) * cos(PI * valx / 180));
    }
    else
    {
        x = CX - static_cast<T>(float(hlen) * -sin(PI * valx / 180));
        y = CY - static_cast<T>(float(hlen) * cos(PI * valx / 180));
    }

    return std::make_pair(x, y);
}

template<typename T>
std::pair<T, T> ImageCreator::HrCoord(const int& hval, const int& mval, const int& hlen)
{
    T x, y;
    //each hour makes 60 degree with min making 0.5 degree
    float val = (float(hval) * 30) + (float(mval) * 0.5f);
    if (val >= 0 && val <= 180)
    {
        x = CX + static_cast<T>(float(hlen) * sin(PI * val / 180));
        y = CY - static_cast<T>(float(hlen) * cos(PI * val / 180));
    }
    else
    {
        x = CX - static_cast<T>(float(hlen) * -sin(PI * val / 180));
        y = CY - static_cast<T>(float(hlen) * cos(PI * val / 180));
    }

    return std::make_pair(x, y);
}
