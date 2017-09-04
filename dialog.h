#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QPainter>
#include<QPixmap>
#include"dialog.h"
#include"block.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    //主场景每个小方格的数据, 0是 空, 1-7是填充
    int Tetri[20][10];

    //7种可填充小方格的位图(图片为本人[周相鑫], 以及好朋友[刘琪]
    QPixmap pix[7];
    //7种方块样例
    QPixmap pixNext[7];
    //方块左上角在主场景的坐标
    int blockLocX, blockLocY;
    //游戏是否暂停
    bool Paused;
    //方块是不是活动的(没有固定)
    bool isBlockActive;
    //方块对象指针
    Block *block;
    //下一个方块型号
    int nextBlockType;

    void button_update();
protected:
    void paintEvent(QPaintEvent *);
    //定时器响应函数
    void timerEvent(QTimerEvent *event);
    //键盘事件处理函数
    void keyPressEvent(QKeyEvent *);
    //删除填满的行
    void DestroyLines();

    //定时器编号
    int m_nTimerId;
    //计分器
    int score;

    scoreBoard* score_board;
    int history_number;


private:
    Ui::Dialog *ui;


public slots:
    void GoPause(); // 开始/暂停按钮的响应函数
    void Restart(); //重置按钮单机响应函数
    void resizeEvent(QResizeEvent *);
private slots:

    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_quitButton_clicked();
};

#endif // DIALOG_H
