
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tetris.h"


// 创建一个俄罗斯方块类
// blockType 代表方块类型
// turnState代表方块状态
static int blockType;
static int score = 0;
static int turnState;

int x;
int y;
static int i = 0;
static int j = 0;
//static int flag = 0;

#define kNShapes  7
#define kNState   4

// 定义已经放下的方块x=0-11,y=0-21;
int map[kWallW][kWallH];

// 方块的形状 第一组代表方块类型有S,Z,L,J,I,O,T 7种 第二组 代表旋转几次 第三四组为 方块矩阵
static int shapes[][kNState][kShapeW*kShapeH] = {
    // I
    {   { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 } },
    // S
    {   { 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 } },
    // Z
    {   { 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },
    // J
    {   { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    // O
    {   { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    // L
    {   { 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    // T
    {   { 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 } },
};

extern int dummy1[sizeof(shapes) / sizeof(shapes[0]) == kNShapes ? 1 : -1];





int  gameover(int x, int y);
void newmap();
void drawwall();
int  blow(int x, int y, int blockType, int turnState);
void add(int x, int y, int blockType, int turnState);
void delline();


int activatingShapeFlag(int pos)
{
    assert(pos >= 0 && pos <= kShapeW * kShapeH);

    return shapes[blockType][turnState][pos];
}

// 生成新方块的方法
void newblock()
{
    blockType = rand() % kNShapes;
    turnState = rand() % kNState;
    x = kWallW / 2 - kShapeW / 2;
    y = 0;
    if (gameover(x, y) == 1) {
        newmap();
        drawwall();
        score = 0;
//        JOptionPane.showMessageDialog(null, "GAME OVER");
    }
}

// 画围墙
void drawwall()
{
    for (i = 0; i < kWallW; i++) {
        map[i][kWallH - 1] = 2;
    }
    for (j = 0; j < kWallH; j++) {
        map[0][j] = 2;
        map[kWallW - 1][j] = 2;
    }
}

// 初始化地图
void newmap()
{
    for (i = 0; i < kWallW; i++) {
        for (j = 0; j < kWallH; j++) {
            map[i][j] = 0;
        }
    }
}

// 初始化构造方法
int Tetrisblok()
{
    newblock();
    newmap();
    drawwall();
//    Timer timer = new Timer(1000, new TimerListener());
//    timer.start();
    return 1;
}

// 旋转的方法
int turn()
{
    int ret = 0;
    int tempturnState = turnState;
    turnState = (turnState + 1) % kNState;
    if (blow(x, y, blockType, turnState) == 1) {
//        repaint();
        ret = 1;
    } else {
        turnState = tempturnState;
        ret = 0;
    }
    return ret;
}

// 左移的方法
int left()
{
    if (blow(x - 1, y, blockType, turnState) == 1) {
        x = x - 1;
//        repaint();
        return 1;
    }
    return 0;
}

// 右移的方法
int right()
{
    if (blow(x + 1, y, blockType, turnState) == 1) {
        x = x + 1;
//        repaint();
        return 1;
    }
    return 0;
}

// 下落的方法
int down()
{
    if (blow(x, y + 1, blockType, turnState) == 1) {
        y = y + 1;
    } else {
        add(x, y, blockType, turnState);
        newblock();
    }
    delline();
//    repaint();
    return 1;
}

// 是否合法的方法
int blow(int x, int y, int blockType, int turnState)
{
    for (int a = 0; a < kShapeH; a++) {
        for (int b = 0; b < kShapeW; b++) {
            if ( (shapes[blockType][turnState][a * kShapeH + b] == 1) &&
                (x + b + 1 < kWallW) && (y + a < kWallH) &&
                ((map[x + b + 1][y + a] == 1)
                 || (map[x + b + 1][y + a] == 2)) ) // map[0][X] 是围墙!
            {
                return 0;
            }
        }
    }
    return 1;
}

// 消行的方法
void delline()
{
    int c = 0;
    for (int b = 0; b < kWallH; b++) {
        for (int a = 0; a < kWallW; a++) {
            if (map[a][b] == 1) {
                c = c + 1;
                if (c == kMapW) {
                    score += 10;
                    for (int d = b; d > 0; d--) {
                        for (int e = 1; e <= kMapW; e++) {
                            map[e][d] = map[e][d - 1];
                        }
                    }
                }
            }
        }
        c = 0;
    }
}

// 判断你挂的方法
int gameover(int x, int y)
{
    if (blow(x, y, blockType, turnState) == 0) {
        return 1;
    }
    return 0;
}

// 把当前添加map
void add(int x, int y, int blockType, int turnState)
{
    int j = 0;
    for (int a = 0; a < kShapeH; a++) {
        for (int b = 0; b < kShapeW; b++) {
            if (x + b + 1 < kWallW && y + a < kWallH &&
                map[x + b + 1][y + a] == 0) {
                map[x + b + 1][y + a] = shapes[blockType][turnState][j];
            }
            j++;
        }
    }
}

// 画方块的的方法
//void paintComponent(Graphics g) {
//    super.paintComponent(g);
//    // 画当前方块
//    for (j = 0; j < kShapeW * kShapeH; j++) {
//        if (shapes[blockType][turnState][j] == 1) {
//            g.fillRect((j % kShapeH + x + 1) * 10, (j / kShapeH + y) * 10, 10, 10);
//        }
//    }
//    // 画已经固定的方块
//    for (j = 0; j < kWallH; j++) {
//        for (i = 0; i < kWallW; i++) {
//            if (map[i][j] == 1) {
//                g.fillRect(i * 10, j * 10, 10, 10);
//
//            } else if (map[i][j] == 2) {
//                g.drawRect(i * 10, j * 10, 10, 10);
//
//            }
//        }
//    }
//    g.drawString("score=" + score, 125, 10);
//    g.drawString("抵制不良游戏, ", 125, 50);
//    g.drawString("拒绝盗版游戏. ", 125, 70);
//    g.drawString("注意自我保护, ", 125, 90);
//    g.drawString("谨防受骗上当. ", 125, 110);
//    g.drawString("适度游戏益脑, ", 125, 130);
//    g.drawString("沉迷游戏伤身. ", 125, 150);
//    g.drawString("合理安排时间, ", 125, 170);
//    g.drawString("享受健康生活. ", 125, 190);
//}

// 键盘监听
//void keyPressed(KeyEvent e) {
//    switch (e.getKeyCode()) {
//        case KeyEvent.VK_DOWN:
//            down();
//            break;
//        case KeyEvent.VK_UP:
//            turn();
//            break;
//        case KeyEvent.VK_RIGHT:
//            right();
//            break;
//        case KeyEvent.VK_LEFT:
//            left();
//            break;
//    }
//
//}

// 无用
//void keyReleased(KeyEvent e) {
//}

// 无用
//void keyTyped(KeyEvent e) {
//}

// 定时器监听
//TimerListener implements ActionListener {
//    void actionPerformed(ActionEvent e) {
//        repaint();
//        if (blow(x, y + 1, blockType, turnState) == 1) {
//            y = y + 1;
//            delline();
//        }
//        if (blow(x, y + 1, blockType, turnState) == 0) {
//            if (flag == 1) {
//                add(x, y, blockType, turnState);
//                delline();
//                newblock();
//                flag = 0;
//            }
//            flag = 1;
//        }
//    }
//}

int actionPerformed()
{
    // repaint();
    if (blow(x, y + 1, blockType, turnState) == 1) {
        y = y + 1;
        delline();
    }
    if (blow(x, y + 1, blockType, turnState) == 0) {
//        if (flag == 1) {
            add(x, y, blockType, turnState);
            delline();
            newblock();
//            flag = 0;
//        }
//        flag = 1;
    }
    return 1;
}
