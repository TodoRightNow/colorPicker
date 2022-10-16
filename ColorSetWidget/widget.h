#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class QImage;

class Widget : public QWidget
{
    Q_OBJECT


public:
    // hsv中的v全称是value 为了防止歧义，用hsv的别名hsb中的b的全称BrightNess表示
    enum class ColorParamType
    {
        Red,
        Green,
        Blue,
        Hue,
        Saturation,
        BrightNess,
        Cyan,
        Magenta,
        Yellow,
        Black,
        Unknown
    };

    Q_ENUM(ColorParamType)

public:
    Widget(QColor curColor, QWidget *parent = nullptr);
    ~Widget();


public slots:
    //  输入框值发生更改后调用这个函数  输入框要限定只能输入数字

    void onSelectColorParamTypeChanged(ColorParamType type, int curSelectColorParamValue);

    // 下面这些会影响到 m_curColor
    void onRedValueChanged(int value);
    void onBlueValueChanged(int value);
    void onGreenValueChanged(int value);

    void onHueValueChanged(int value);
    void onSaturationValueChanged(int value);
    void onBrightNessValueChanged(int value);


    void onCyanValueChanged(int value);
    void onMagentaValueChanged(int value);
    void onYellowValueChanged(int value);
    void onBlackValueChanged(int value);

protected:
    void paintEvent(QPaintEvent *) override;
private:
    void reFillPixelValue(ColorParamType type);

    bool isNeedRefillImagePixel(const QColor& color);
private:
    Ui::Widget *ui;

    QColor m_curColor;

    ColorParamType m_curSelectColorParamType;
    int m_curSelectColorParamValue;

    // 当前颜色在 m_gamutImage 中的位置
    QPoint m_curColorPos;
    // 绘制的图像
    QImage m_gamutImage;
};
#endif // WIDGET_H
