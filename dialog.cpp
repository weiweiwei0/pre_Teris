#include "dialog.h"
#include "ui_dialog.h"
#include"block.h"
#include<QDialog>
#include<QPixmap>
#include<QPainter>
#include<QTime>
#include<QKeyEvent>
#include<QBitmap>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    score_board = new scoreBoard;
    score_board->Initial();
    history_number = score_board->get_sum_number();
    if(history_number > 0)
    {
        score_board->set_pos(1);
        ui->historyScoreLabel->setText(QString::number(score_board->get_historyScore()));
        ui->historyDateAndTimeLabel->setText(score_board->get_historyDateAndTime());
        ui->recordScoreLabel->setText(QString::number((score_board->get_highestScore())));
        ui->recordDateAndTimeLabel->setText(score_board->get_highestDateAndTime());
    }
    button_update();


    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(GoPause()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(Restart()));
    //加载填充小方格的位图
    pix[0].load(tr(":/rc/pic1.png"));
    pix[1].load(tr(":/rc/pic2.png"));
    pix[2].load(tr(":/rc/pic3.png"));
    pix[3].load(tr(":/rc/pic4.png"));
    pix[4].load(tr(":/rc/pic5.png"));
    pix[5].load(tr(":/rc/pic6.png"));
    pix[6].load(tr(":/rc/pic7.png"));
    for(int i=0; i<20; i++)
        for(int j=0; j<10; j++)
        {
            Tetri[i][j]=0;

        }
    //使方向键可用
    setFocusPolicy(Qt::StrongFocus);
    //加载方块样例位图
    pixNext[0].load(tr(":/rc/pic11.png"));
    pixNext[1].load(tr(":/rc/pic22.png"));
    pixNext[2].load(tr(":/rc/pic33.png"));
    pixNext[3].load(tr(":/rc/pic44.png"));
    pixNext[4].load(tr(":/rc/pic55.png"));
    pixNext[5].load(tr(":/rc/pic66.png"));
    pixNext[6].load(tr(":/rc/pic77.png"));

    //设置随机数种子,是的每次游戏开始时方块的类型随机
    qsrand(QTime::currentTime().msec());
    //产生一个方块类型
    nextBlockType= qrand()%7 + 1;
    //创建一个方块对象
    block = new Block(nextBlockType, this);
    ui->labelNext->setText(tr(""));
    //设定NEXT区域显示下一个方块
    ui->labelNext->setPixmap(pixNext[nextBlockType - 1]);
    //没有活动的方块
    isBlockActive=false;
    //初始时刻处于暂停状态
    Paused=true;
    //得分清零
    score = 0;
}

Dialog::~Dialog()
{
    delete ui;
}



void Dialog::paintEvent(QPaintEvent *)
{
    //主场景白色方框左上角实际坐标
    int xOrg=220, yOrg=1;
    int cellw=pix[0].width();
    int cellh=pix[0].height();
    QPainter painter(this);
    //绘制主场景中的已经固定的方块
    for(int i=0; i<20; i++)
        for(int j=0; j<10; j++)
            if(Tetri[i][j]>0)
            {
                painter.drawPixmap(xOrg + j * cellw, yOrg + i * cellh, cellw, cellh, pix[Tetri[i][j]-1]);
            }

    //场景中有活动的方块
    if(isBlockActive)
    {
        //绘制当前活动方块
        int x = blockLocX;
        int y = blockLocY;
        for(int i=0; i<=block->content_D; i++)
        for(int j=block->content_L; j<=block->content_R; j++)
        {
            if(block->d[i][j]!=0)
                painter.drawPixmap(xOrg + (y+j) * cellw, yOrg + (x+i) * cellh, cellw, cellh, pix[block->d[i][j]-1]);
        }
    }

    //在NEXT区显示下一个方块
    ui->labelNext->setPixmap(pixNext[nextBlockType-1]);
}

void Dialog::GoPause()
{
    //按钮checked为真, 游戏为运行状态
    if(ui->startButton->isChecked())
    {
        //设置重玩按钮可用
        ui->resetButton->setEnabled(true);
        //建立定时器
        m_nTimerId = startTimer(500);
        Paused = false;
    }
    else
    {
        //按钮checked状态为假, 游戏处于暂停状态

        //设置暂停变量
        Paused = true;
        //清楚定时器
        killTimer(m_nTimerId);
    }
}


void Dialog::Restart()
{

    score_board->addScore(score);
    history_number = score_board->get_sum_number();
    if(history_number > 0)
    {
        ui->historyScoreLabel->setText(QString::number(score_board->get_historyScore()));
        ui->historyDateAndTimeLabel->setText(score_board->get_historyDateAndTime());
        ui->recordScoreLabel->setText(QString::number((score_board->get_highestScore())));
        ui->recordDateAndTimeLabel->setText(score_board->get_highestDateAndTime());
    }
    ui->labelScore->setText(QString("0"));
    button_update();

    //主场景中方块清空
    for(int i=0; i<20; i++)
        for(int j=0; j<10; j++)
        {
            Tetri[i][j] = 0;
        }

    //分数清零
    score = 0;
    QString str = QString::number(score);
    ui->labelScore->setText(str);
    Paused = true;
    isBlockActive = false;
    //start按钮恢复初始状态
    ui->startButton->setChecked(false);
    ui->startButton->setEnabled(true);
    //reset按钮失效
    ui->resetButton->setEnabled(false);
    //清除定时器
    killTimer(m_nTimerId);
    update();
}


void Dialog::timerEvent(QTimerEvent *)
{
    if(Paused) return ;     //暂停时不做任何动作
    if(!isBlockActive)          //没有活动方块
    {
        //放入一个方块
        blockLocX = 0;
        blockLocY = 3;
        block->Initial(nextBlockType);
        //产生下一个方块的类型
        nextBlockType = qrand()%7 + 1;
        //方块进入活动状态
        isBlockActive = true;
    }
    else        //方块可以向下移动
    {
        if(block->canMoveDown())
        {
            this->blockLocX++;
        }
        else
        {
            int x = blockLocX, y = blockLocY;
            //将方块内容合并到主场景中
            for(int i=0; i<= block->content_D ; i++)
                for(int j=block->content_L; j<=block->content_R; j++)
                {
                    if(block->d[i][j]!=0)
                        Tetri[x+i][y+j] = block->d[i][j];
                    block->d[i][j] = 0;
                }
            //方块处于非活动状态
            isBlockActive = false;

            //若新进的方块一步也不能向下移动, 则游戏结束
            if(blockLocX == 0)
            {
                Paused = true;
                ui->startButton->setChecked(false);
                ui->startButton->setEnabled(false);

            }

        }

        //方块落地后, 消除填满的行
        DestroyLines();

    }
    this->update();
}




void Dialog::keyPressEvent(QKeyEvent *e)
{
    if(Paused) return;      //暂停时 不做任何动作
    switch (e->key()) {
    case Qt::Key_Left:
        if(block->canMoveLeft())
            blockLocY--;
        break;

    case Qt::Key_Right:
        if(block->canMoveRight())
            blockLocY++;
        break;

    case Qt::Key_Down:
        if(block->canMoveDown())
            blockLocX++;
        break;

    case Qt::Key_Up:
        block->tryRotate();
        break;

    default:
        break;
    }
    DestroyLines();
    this->update();
    QDialog::keyPressEvent(e);
}


void Dialog::DestroyLines()
{
    for(int i=0; i<20; i++){
        //从上向下扫描
        bool full = true;
        //判断第i行是否满
        for(int j=0; j<10; j++){
            if(Tetri[i][j] == 0)
                full = false;
        }
        //若第i行满则删除
        if(full){
            int k=i;
            while(k>0)
            {
                for(int j=0; j<10; j++)
                {
                    Tetri[k][j]=Tetri[k-1][j];
                }
                k--;
            }
            for(int j=0; j<10; j++)
                Tetri[0][j] = 0;
            //分数更新
            score = score + 10;
            QString str= QString::number(score);
            ui->labelScore->setText(str);
        }
    }


}



void Dialog::resizeEvent(QResizeEvent *)
{
QPixmap pixmap(":/rc/mask.png");
setMask( pixmap.mask() );
}

void Dialog::button_update()
{
    history_number = score_board->get_sum_number();

    if(score_board->get_pos() <=1 )
    {
        ui->prevButton->close();
    }
    else
    {
        ui->prevButton->show();
    }

    if(score_board->get_pos() >= history_number)
    {
        ui->nextButton->close();
    }
    else
    {
        ui->nextButton->show();
    }
}

void Dialog::on_prevButton_clicked()
{
    score_board->prev_historyScore();
    history_number = score_board->get_sum_number();
    if(history_number > 0)
    {
        ui->historyScoreLabel->setText(QString::number(score_board->get_historyScore()));
        ui->historyDateAndTimeLabel->setText(score_board->get_historyDateAndTime());
        ui->recordScoreLabel->setText(QString::number((score_board->get_highestScore())));
        ui->recordDateAndTimeLabel->setText(score_board->get_highestDateAndTime());
    }
    button_update();
}

void Dialog::on_nextButton_clicked()
{
    score_board->next_historyScore();
    history_number = score_board->get_sum_number();
    if(history_number > 0)
    {
        ui->historyScoreLabel->setText(QString::number(score_board->get_historyScore()));
        ui->historyDateAndTimeLabel->setText(score_board->get_historyDateAndTime());
        ui->recordScoreLabel->setText(QString::number((score_board->get_highestScore())));
        ui->recordDateAndTimeLabel->setText(score_board->get_highestDateAndTime());
    }
    button_update();
}

void Dialog::on_quitButton_clicked()
{
    score_board->addScore(score);
    history_number = score_board->get_sum_number();
    if(history_number > 0)
    {
        ui->historyScoreLabel->setText(QString::number(score_board->get_historyScore()));
        ui->historyDateAndTimeLabel->setText(score_board->get_historyDateAndTime());
        ui->recordScoreLabel->setText(QString::number((score_board->get_highestScore())));
        ui->recordDateAndTimeLabel->setText(score_board->get_highestDateAndTime());
    }
    ui->labelScore->setText(QString("0"));
    button_update();
    this->close();
}
