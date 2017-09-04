#include "block.h"
#include"dialog.h"
#include <QDialog>


Block::Block(QObject *parent) : QObject(parent)
{ }

Block::~Block()
{ }

Block::Block(int type, QDialog *parent)
{
    Parent = parent;
    Type = type;
    Initial(Type);
}


void Block::Initial(int Type)
{
    //方块内容清空
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
        {
            d[i][j] = 0;
        }
    //类型1方块初始化
    if(Type ==1)
    {
      d[0][1]=1;
      d[1][0]=1;
      d[1][1]=1;
      d[1][2]=1;
      dim=3;
    }
    else if(Type == 2)
    {
        d[0][1]=2;
        d[1][1]=2;
        d[2][1]=2;
        d[2][2]=2;
        dim=3;
    }
    else if(Type ==3)
    {
        d[0][2]=3;
        d[1][2]=3;
        d[2][1]=3;
        d[2][2]=3;
        dim=3;
    }
    else if(Type ==4)
    {
        d[0][1]=4;
        d[1][1]=4;
        d[1][2]=4;
        d[2][2]=4;
        dim=3;
    }
    else if(Type ==5)
    {
        d[0][2]=5;
        d[1][2]=5;
        d[1][1]=5;
        d[2][1]=5;
        dim=3;
    }
    else if(Type ==6)
    {
        d[0][1]=6;
        d[1][1]=6;
        d[2][1]=6;
        d[3][1]=6;
        dim=4;
    }
    else if(Type ==7)
    {
        d[0][1]=7;
        d[0][2]=7;
        d[1][1]=7;
        d[1][2]=7;
        dim=2;
    }
    //计算方块边界的内容
    CalContentPos(d, content_L, content_R, content_D);
}

void Block::tryRotate()
{
    int T[4][4] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    //用临时数组T存储转动后的方块
    if(dim==3)
    {
        T[0][0]=d[2][0];
        T[0][1]=d[1][0];
        T[0][2]=d[0][0];

        T[1][0]=d[2][1];
        T[1][1]=d[1][1];
        T[1][2]=d[0][1];

        T[2][0]=d[2][2];
        T[2][1]=d[1][2];
        T[2][2]=d[0][2];
    }
    else if(dim==4)
    {
        T[0][0]=d[3][0];
        T[0][1]=d[2][0];
        T[0][2]=d[1][0];
        T[0][3]=d[0][0];

        T[1][0]=d[3][1];
        T[1][1]=d[2][1];
        T[1][2]=d[1][1];
        T[1][3]=d[0][1];

        T[2][0]=d[3][2];
        T[2][1]=d[2][2];
        T[2][2]=d[1][2];
        T[2][3]=d[0][2];

        T[3][0]=d[3][3];
        T[3][1]=d[2][3];
        T[3][2]=d[1][3];
        T[3][3]=d[0][3];

    }

    int left, right, down;
    //计算移动后临时方块T的内容边界, 放入left, right, down
    CalContentPos(T, left, right, down);
    Dialog *ptr = (Dialog*) Parent; //父窗口指针
    int xLoc = ptr->blockLocX;
    int yLoc = ptr->blockLocY;
    //超出左边界
    if(yLoc + left < 0)
    {
        yLoc = -left;
    }
    //超出右边界
    if(yLoc + right >= 10)
    {
        yLoc = 9-right;
    }
    //超过下界
    if(xLoc + down >= 20)
    {
        xLoc = 19 - down;
    }

    //判断临时方块旋转后是否有冲突
    bool flag = true;
    for(int i=0; i<= down ; i++)
        for(int j=left ; j<=right; j++ )
        {
            if(T[i][j]!=0 && ptr->Tetri[xLoc+i][yLoc+j]!=0)
                flag = false;
        }
    //若可旋转, 则将临时方块及其边界复制给活动方块
    if(flag){
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                d[i][j] = T[i][j];
        ptr->blockLocX=xLoc;
        ptr->blockLocY=yLoc;
        content_L = left ;
        content_R = right ;
        content_D = down;
    }


}


void Block::CalContentPos(int t[4][4], int &L, int &R, int &D)
{
    L =0, R=0, D=0;
    //计算左边界
    for(int j=0; j<4; j++)
        if(t[0][j] + t[1][j] + t[2][j] + t[3][j] > 0)
        {  L=j; break; }
    //计算右边界
    for(int j=3; j>=0; j--)
        if(t[0][j] + t[1][j] + t[2][j] + t[3][j] > 0 )
        { R = j; break;}
    //计算下边界
    for(int i=3; i>=0; i--)
        if(t[i][0] + t[i][1] + t[i][2] + t[i][3] > 0 )
        { D=i; break;}

}

bool Block::canMoveDown()
{
    Dialog *ptr = ((Dialog*) Parent);
    bool flag = true;
    int x=ptr->blockLocX;
    int y=ptr->blockLocY;
    if(x + content_D + 1 >= 20 )
        return false;
    //判断有无下一行阻挡
    for(int i=0; i<=content_D; i++)
        for(int j=content_L; j<=content_R; j++)
        {
            if(d[i][j]!=0 && ptr->Tetri[x+i+1][y+j]!=0)
                flag = false;
        }
    //若返回true, 表示可移动
    return flag;

}


bool Block::canMoveLeft()
{
    Dialog *ptr = ((Dialog*)Parent);
    bool flag = true;
    int x = ptr->blockLocX;
    int y = ptr->blockLocY;
    //左侧相邻行超出边界
    if(y + content_L - 1 < 0)
        return false;
    //判断左侧相邻行有无阻挡
    for(int i=0; i<=content_D; i++)
        for(int j=content_L; j<=content_R; j++)
        {
            if(d[i][j]!=0 && ptr->Tetri[x+i][y+j-1]!=0)
                flag = false;
        }
    return flag;
    //若返回true 表示可移动
}


bool Block::canMoveRight()
{
    Dialog *ptr = ((Dialog*)Parent);
    bool flag = true;
    int x = ptr->blockLocX;
    int y = ptr->blockLocY;
    //左侧相邻行超出边界
    if(y + content_R + 1 >=  10)
        return false;
    //判断右侧相邻行有无阻挡
    for(int i=0; i<=content_D;i ++)
        for(int j=content_L; j<=content_R; j++)
        {
            if(d[i][j]!=0 && ptr->Tetri[x+i][y+j+1]!=0)
                flag = false;
        }
    return flag;
    //若返回true 表示可移动
}
