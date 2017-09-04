#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include<QDialog>
#include <QTime>
#include <QDate>
#include <QString>
#include<QtCore>
#include<fstream>
using namespace std;

class item
{
public:
    item() {}
    ~item() {}
    virtual void Initial()=0;          //初始化

};



class Block : public QObject
{
    Q_OBJECT

public:
    explicit Block(QObject *parent = 0);
    ~Block();
    int d[4][4];        //方块数据内容, 0是空, 1~7有填充
    int content_L, content_R, content_D;  //实际内容边界
    int Type, dim;      //方块类型,B数据内容
    QDialog *Parent;        //父窗口指针

    Block(int, QDialog *);      //构造函数
    void Initial(int);          //方块初始化

    //计算方块数组t的内容边界, 放入L(左) R(右) D(下)
    void CalContentPos(int t[4][4], int &L, int &R, int &D);
    //尝试旋转, 可行则旋转, 否则保持原状
    void tryRotate();
    //判断能否向下移动
    bool canMoveDown();
    //判断能向左移动
    bool canMoveLeft();
    //判断能向右移动
    bool canMoveRight();
signals:
public slots:

};



struct score_element
{
    score_element* prev;
    score_element* next;
    int data;
    QDateTime currentDateTime;

};

class scoreBoard: virtual public item
{
public:
    scoreBoard() {}
    ~scoreBoard()
    {    }

    virtual void Initial()
    {
        pos = 0;


        ifstream testfile("score.dat", ios::in|ios::binary);

        if (!testfile)
        {
            ofstream outfilen("score.dat", ios::binary);
            outfilen.close();
        }
        else
        {
            testfile.close();
        }

        score_head = new score_element;
        score_tail = new score_element;
        score_head->prev = NULL;
        score_head->next = score_tail;
        score_tail->prev = score_head;
        score_tail->next = NULL;
    }

    void addScore(int input_score)
    {
        QFile readfile("score.dat");
        readfile.open(QIODevice::ReadOnly);
        QDataStream in(&readfile);
        int score;
        QDateTime date;
//        QTime time;
        while(!in.atEnd())
        {
            in>>score>>date;
            score_element *tmp = new score_element;
            tmp->data = score;
            tmp->currentDateTime = date;


            score_head->next->prev = tmp;
            tmp->next = score_head->next;
            score_head->next = tmp;
            tmp->prev = score_head;

        }
        readfile.close();

        if(input_score>=0)
        {
            score_element *tmp = new score_element;
            tmp->data = input_score;
            tmp->currentDateTime = QDateTime::currentDateTime();


            score_head->next->prev = tmp;
            tmp->next = score_head->next;
            score_head->next = tmp;
            tmp->prev = score_head;
        }

        QFile writefile("score.dat");
        writefile.open(QIODevice::WriteOnly|QIODevice::Truncate);
        QDataStream out(&writefile);

        while(score_head->next!=score_tail)
        {
            score_element *tmp;
            tmp = score_head->next;

            out<<tmp->data;
            out<<tmp->currentDateTime;
//            out<<tmp->current_time;

            score_head->next = tmp->next;
            tmp->next->prev = score_head;
            delete tmp;
        }
        writefile.close();

        pos = 1;
    }

    int get_highestScore()
    {
        QFile readfile("score.dat");
        readfile.open(QIODevice::ReadOnly);
        QDataStream in(&readfile);
        int score;
        QDateTime date;
//        QTime time;
        int max = 0;
        while(!in.atEnd())
        {
            in>>score>>date;
            if(score>max)
                max = score;

        }
        readfile.close();
        return max;
    }

    QString get_highestDateAndTime()
    {
        QFile readfile("score.dat");
        readfile.open(QIODevice::ReadOnly);
        QDataStream in(&readfile);
        int score;
        QDateTime date;
//        QTime time;
        QString tmp_date;

        int max = 0;
        while(!in.atEnd())
        {
            in>>score>>date;
            if(score>max)
                max = score;

        }
        readfile.close();


        QFile readfile1("score.dat");
        readfile1.open(QIODevice::ReadOnly);
        QDataStream in1(&readfile1);


//        QTime time;

        while(!in1.atEnd())
        {
            in1>>score>>date;
            if(score == max)
            {
                tmp_date = date.toString("yyyy-MM-dd hh:mm:ss");
                readfile1.close();
                return tmp_date;
            }

        }
        readfile1.close();
        return QString("");
    }

    int get_historyScore()
    {
        QFile readfile("score.dat");
        readfile.open(QIODevice::ReadOnly);
        QDataStream in(&readfile);
        int score;
        QDateTime date;
//        QTime time;
        int tmp_pos = pos;
        int historyScore = 0;
        while(  !in.atEnd() )
        {
            in>>score>>date;
            tmp_pos--;
            if(tmp_pos == 0)
            {
               historyScore = score;
               break;
            }

        }
        readfile.close();
        return score;
    }

    QString get_historyDateAndTime()
    {
        QString tmp_string;

        QFile readfile("score.dat");
        readfile.open(QIODevice::ReadOnly);
        QDataStream in(&readfile);
        int score;
        QDateTime date;
//        QTime time;

        int tmp_pos = pos;

        while(  !in.atEnd() )
        {
            in>>score>>date;
            tmp_pos--;
            if(tmp_pos == 0)
            {
               tmp_string = date.toString("yyyy-MM-dd hh:mm:ss");
               readfile.close();
               return tmp_string;
            }

        }
        readfile.close();
        return QString("");

    }

    int get_sum_number()
    {
        QFile readfile("score.dat");
        readfile.open(QIODevice::ReadOnly);
        QDataStream in(&readfile);
        int score;
        QDateTime date;
//        QTime time;
        int count = 0;

        while(  !in.atEnd() )
        {
            in>>score>>date;
            count++;

        }
        readfile.close();
        return count;
    }

    void prev_historyScore()
    {
        pos = pos-1;
    }

    void next_historyScore()
    {
        pos = pos+1;
    }

    int get_pos()
    {
        return pos;
    }

    void set_pos(int n)
    {
        pos = n;
    }

private:
    int pos;
    score_element* score_head;
    score_element* score_tail;
    int sum_number;
};

#endif // BLOCK_H
