#include "widget.h"
#include "ui_widget.h"

#include <QImage>
#include <QDebug>
#include <QPainter>

// 代码逻辑
// 打开色板默认是选中的H  根据当前颜色的rgb 计算出 h s v 三个值 根据sv两个值确定当前颜色值在色域中的位置 gamut
// 选中h 水平方向变化的是s 垂直方向变化的是v
// 选中s 水平方向变化的是h 垂直方向变化的是v
// 选中v 水平方向变化的是h 垂直方向变化的是s

// 当选中r 水平方向变化的是blue 垂直方向变化的是green
// 当选中g 水平方向变化的是blue 垂直方向变化的是red
// 当选中b 水平方向变化的是red 垂直方向变化的是green



Widget::Widget(QColor curColor, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_curColor(curColor)
{
    ui->setupUi(this);
    qRegisterMetaType<Widget::ColorParamType>("Widget::ColorParamType");
}

Widget::~Widget()
{
    delete ui;
}

// 色彩值坐标系统 和 qt 的图像坐标系统不一样，qt 的图像坐标系统是左上角是原点, 色彩值坐标系统是左下角是原点
void Widget::onSelectColorParamTypeChanged(ColorParamType type, int curSelectColorParamValue)
{
    m_curSelectColorParamType = type;
    m_curSelectColorParamValue = curSelectColorParamValue;
    reFillPixelValue(type);
}


void Widget::onRedValueChanged(int value)
{
    m_curColor.setRed(value);
    if (m_curSelectColorParamType == ColorParamType::Red || isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::Red);
    }
}

void Widget::onBlueValueChanged(int value)
{
    m_curColor.setBlue(value);
    if (m_curSelectColorParamType == ColorParamType::Blue || isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::Blue);
    }
}

void Widget::onGreenValueChanged(int value)
{
    m_curColor.setGreen(value);
    if (m_curSelectColorParamType == ColorParamType::Green || isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::Green);
    }
}

void Widget::onHueValueChanged(int value)
{
    int saturation, brightNess;
    m_curColor.getHsv(nullptr, &saturation, &brightNess);
    m_curColor.setHsv(value, saturation, brightNess);
    if (m_curSelectColorParamType == ColorParamType::Hue || isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::Hue);
    }
}

void Widget::onSaturationValueChanged(int value)
{
    int hue, brightNess;
    m_curColor.getHsv(&hue, nullptr, &brightNess);
    m_curColor.setHsv(hue, value, brightNess);
    if (m_curSelectColorParamType == ColorParamType::Saturation || isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::Saturation);
    }
}

void Widget::onBrightNessValueChanged(int value)
{
    int hue, saturation;
    m_curColor.getHsv(&hue, &saturation, nullptr);
    m_curColor.setHsv(hue, saturation, value);

    if (m_curSelectColorParamType == ColorParamType::BrightNess || isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::BrightNess);
    }
}

void Widget::onCyanValueChanged(int value)
{
    int m, y, k;
    m_curColor.getCmyk(nullptr, &m, &y, &k);
    m_curColor.setCmyk(value, m, y, k);
    if (isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::BrightNess);
    }
}

void Widget::onMagentaValueChanged(int value)
{
    int c, y, k;
    m_curColor.getCmyk(&c, nullptr, &y, &k);
    m_curColor.setCmyk(c, value, y, k);
    if (isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::BrightNess);
    }
}

void Widget::onYellowValueChanged(int value)
{
    int m, c, k;
    m_curColor.getCmyk(&c, &m, nullptr, &k);
    m_curColor.setCmyk(c, m, value, k);
    if (isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::BrightNess);
    }
}

void Widget::onBlackValueChanged(int value)
{
    int m, y, c;
    m_curColor.getCmyk(&c, &m, &y, nullptr);
    m_curColor.setCmyk(c, m, y, value);
    if (isNeedRefillImagePixel(m_curColor))
    {
        reFillPixelValue(ColorParamType::BrightNess);
    }
}


// TODO 标记出当前颜色m_curColor在色域中的位置
void Widget::paintEvent(QPaintEvent *)
{
    if (m_gamutImage.isNull())
    {
        m_gamutImage = QImage(width(), height(), QImage::Format_RGB32);
        reFillPixelValue(ColorParamType::Hue);
    }
    QPainter painter(this);
    painter.drawImage(this->rect(), m_gamutImage);
}

void Widget::reFillPixelValue(ColorParamType type)
{
    int imageWidth = m_gamutImage.width();
    int imageHeight = m_gamutImage.height();

#define SetPixelColor(getColorValueExpression) \
    for (int j = 0; j < imageHeight; j++) \
    { \
    for (int i = 0; i < imageWidth; i++) \
    { \
    m_gamutImage.setPixelColor(i, j, getColorValueExpression); \
} \
}

    if (type == ColorParamType::Red)
    {
        qreal red = m_curColor.redF();
        SetPixelColor(QColor::fromRgbF(red, j / (qreal)imageHeight, i / (qreal)imageWidth))
    }
    else if (type == ColorParamType::Green)
    {
        qreal green = m_curColor.greenF();
        SetPixelColor(QColor::fromRgbF(j / (qreal)imageHeight, green, i / (qreal)imageWidth))
    }
    else if (type == ColorParamType::Blue)
    {
        qreal blue = m_curColor.blueF();
        SetPixelColor(QColor::fromRgbF(i / (qreal)imageWidth, j / (qreal)imageHeight, blue))
    }
    else if (type == ColorParamType::Hue)
    {
        qreal hue = m_curColor.hsvHueF();
        SetPixelColor(QColor::fromHsvF(hue,i / (qreal)imageWidth, (imageHeight - j) / (qreal)imageHeight));
    }
    else if (type == ColorParamType::Saturation)
    {
        qreal saturation = m_curColor.hsvSaturationF();
        SetPixelColor(QColor::fromHsvF(i / (qreal)imageWidth, saturation, (imageHeight - j) / (qreal)imageHeight));
    }
    else if (type == ColorParamType::BrightNess)
    {
        qreal brightNess = m_curColor.valueF();
        SetPixelColor(QColor::fromHsvF(i / (qreal)imageWidth, (imageHeight - j) / (qreal)imageHeight, brightNess));
    }
    // cmyk 不能选中 所以没有cmyk的判断
#undef SetPixelColor
    update();
}

bool Widget::isNeedRefillImagePixel(const QColor &color)
{
    if ((m_curSelectColorParamType == ColorParamType::Hue && color.hsvHue() == m_curSelectColorParamValue)
            || (m_curSelectColorParamType == ColorParamType::Saturation && color.hsvSaturation() == m_curSelectColorParamValue)
            || (m_curSelectColorParamType == ColorParamType::BrightNess && color.value() == m_curSelectColorParamValue)
            || (m_curSelectColorParamType == ColorParamType::Red && color.red() == m_curSelectColorParamValue)
            || (m_curSelectColorParamType == ColorParamType::Green && color.green() == m_curSelectColorParamValue)
            || (m_curSelectColorParamType == ColorParamType::Blue && color.blue() == m_curSelectColorParamValue))

    {
        return false;
    }
    return true;
}

