/*************************************
         GAME: TREASURE HUNT
**************************************/
/*
            CS101 PROJECT
               GROUP-4
MEMBERS:
    1. ISHIKA SHARMA        200010020
    2. NIRMIT ARORA         200010034
    3. HRISHIKESH PABLE     200010037
    4. RAHUL PRAJAPAT       200010043
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
#include <unistd.h>
#include<windows.h>

#define GetCurrentDir _getcwd

void level(int x);
void mat(void);
void grid(int color);
void border(int color);
void set_up_game_board(void);
void guess(void);
void check(int x, int y);
void boom(void);
void game_over(void);
void treasue_found(void);
void coins(int x, int y);
void steps(void);
void scoring(char x);
void treasure_sensor(int i, int j);
void Blaze(int a, int b);
void blaze_check(int i, int j);
void retrieve_score(FILE* path);
void put_score(FILE* path);
void save_score(void);
void H_Scoremat(void);
void new_high_score(void);
void high_score_screen(void);

int maxX = 1000, maxY = 800;
int pixel = 50, w = 0;
int newHscore=0, score = 0, s[3];
int difficulty = 0, n = 0, z = 0, line_color = 15;
int sensor = 0, blaze = 0, counter = 0;
int M=3, total_bombs;
int bomb_positions[50][2];
int game_board[20][20];
int treasure[2];

int main()
{
    initwindow(maxX, maxY);
    level(difficulty);
    save_score();
    high_score_screen();
    getch();
    closegraph();
    return 0;
}

void level(int x)
{
    ///A DOOR TO NEXT LEVEL
    int color = 1;
    switch(difficulty/5)
    {
        case 0:
            pixel=70;
            color = difficulty+1;
            if(color == 3) line_color = 0;
            else line_color = 15;
            w = 12;
            break;
        case 1:
            pixel=46;
            color = difficulty+1;
            if(color == 7 || color == 10) line_color = 0;
            else line_color = 15;
            w = 8;
            break;
        case 2:
            pixel=30;
            if(difficulty < 14) color = difficulty+1;
            else color = 1;
            if(color == 11 || color == 14) line_color = 0;
            else line_color = 15;
            w = 4;
            break;
        default:
            return;
    }

    if(z==1) return;
    total_bombs = (M*M)/6;
    mat();
    grid(color);
    set_up_game_board();
    printf("%d %d", treasure[0], treasure[1]);
    guess();
    printf("\nBombs are %d\n", total_bombs);
    M++;
    difficulty++;
    level(difficulty);
}

void mat(void)
{
    //BACKGROUND MAT OVER WHICH GRID WILL BE PLACED
    for(int i=0, j=0; i < maxX; i += 10, j++)
    {
        int t = j % 2;
        int color;
        switch(t)
        {
            case 0:
                color = 1;
                break;
            case 1:
                color = 4;
                break;
        }
        setcolor(color);
        line(i-15, 0, maxX-20, i);
        line(0, maxY-i, maxX-i, maxY);
        delay(5);
    }
}

void grid(int color)
{
    //DRAWS GAME BOARD ON THE SCREEN
    border(1);
    delay(500);
    setfillstyle(SOLID_FILL, color);
    bar((maxX-(M*pixel))/2, (maxY-(M*pixel))/2, (maxX+(M*pixel))/2, (maxY+(M*pixel))/2);

    setcolor(line_color);
    for(int i=0; i<=M; i++)
    {
        line((maxX-(M*pixel))/2, (maxY-(M*pixel))/2+(pixel*i), (maxX+(M*pixel))/2, (maxY-(M*pixel))/2+(pixel*i));
        line((maxX-(M*pixel))/2+(pixel*i), (maxY-(M*pixel))/2, (maxX-(M*pixel))/2+(pixel*i), (maxY+(M*pixel))/2);
    }

    setcolor(WHITE);
    char a[10] = "Score:";
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
    outtextxy(50, 65, a);

    setfillstyle(7, 4);
    bar3d(600, 70, 725, 120, 4, 1);
    outtextxy(605, 87, (char*)"T sensor" );

    setfillstyle(7, 3);
    bar3d(420, 70, 500, 120, 4, 1);
    outtextxy(425, 87, (char*)"BLAZE");

    char b[10];
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    sprintf(b, "LEVEL : %d", difficulty+1);
    outtextxy(maxX/2 +100, maxY - 150, b);
}

void border(int color)
{
    //MARKS THE PLAY AREA
    setfillstyle(SOLID_FILL, color);
    bar(0, 0, 15, maxY);
    bar(0, 0, maxX, 15);
    bar(0, maxY-20, maxX, maxY);
    bar(maxX-20, 0, maxX, maxY);
}

void set_up_game_board(void)
{
    //GAME BOARD IS SET UP BEHIND THE SCENES
    int i=0, j=0;

    srand(time(NULL));

    while(i < M)
    {
        while(j < M)
        {
            game_board[i][j] = '0';
            j++;
        }
        i++;
        j = 0;
    }

    for(int k=0; k <= total_bombs; k++)
    {
        i = rand() % M;
        j = rand() % M;
        if(i==0 && j==0)
        {
            k--;
            continue;
        }
        else
        {
            if(game_board[i][j] != '*')
            {
                if(k == total_bombs)
                {
                    game_board[i][j] = '#';
                    treasure[0] = i;
                    treasure[1] = j;
                    break;
                }
                game_board[i][j] = '*';
                bomb_positions[k][0] = i;
                bomb_positions[k][1] = j;
                /*if*/
            }
            else k--;
        }
    }
}

void guess(void)
{
    //TAKES IN INPUT(MOVE MADE BY THE PLAYER) USING THE MOUSE
    POINT cursorPosition;
    int mX, mY;
    n = 0;

    while(1)
    {
        GetCursorPos(&cursorPosition);

        mX = cursorPosition.x-7;
        mY = cursorPosition.y-30;

        if(GetAsyncKeyState(VK_LBUTTON))
        {
           if(mX > (maxX-(M*pixel))/2 && mX < (maxX+(M*pixel))/2 && mY > (maxY-(M*pixel))/2 && mY < (maxY+(M*pixel))/2)
           {
                for(int i=1; i<=M; i++)
                {
                    int y = (maxY-(M*pixel))/2+(pixel*i);
                    for(int j=1; j<=M; j++)
                    {
                        int x = (maxX-(M*pixel))/2+(pixel*j);
                        if(mX < x && mY < y)
                        {
                            check(i, j);
                            break;
                        }
                    }
                    if(mY < y)
                    {
                        break;
                    }
                }
            }
            if(mX > 600 && mY > 70 && mX < 725 && mY < 120) {sensor = 1;}
            if(mX > 420 && mY > 70 && mX < 500 && mY < 120 && difficulty!=0 && counter<2) {blaze = 1;}
        }
        delay(100);
        if(n == 1)
        {
            break;
        }
    }
}

void check(int i, int j)
{
    /*CHECKS WHAT IS ENCOUNTERED BY THE PLAYER IN ONE MOVE
        IT CAN BE
            A BLANK (SAFE) PLACE,
            A BOMB, OR
            EVEN TREASURE*/
    switch(game_board[i-1][j-1])
    {
        case '0':
            setfillstyle(SOLID_FILL, WHITE);
            bar((maxX-(M*pixel))/2+(pixel*(j-1)), (maxY-(M*pixel))/2+(pixel*(i-1)), (maxX-(M*pixel))/2+(pixel*j), (maxY-(M*pixel))/2+(pixel*i));
            scoring(game_board[i-1][j-1]);
            game_board[i-1][j-1] = '1';
            if(sensor == 1) treasure_sensor(i-1, j-1);
            if(blaze == 1 && difficulty != 0) Blaze( i , j );
            break;
        case '*':
            setfillstyle(SOLID_FILL, BLACK);
            bar((maxX-(M*pixel))/2+(pixel*(j-1)), (maxY-(M*pixel))/2+(pixel*(i-1)), (maxX-(M*pixel))/2+(pixel*j), (maxY-(M*pixel))/2+(pixel*i));
            boom();
            n = 1;
            z = 1;
            break;
        case '#':
            for(int k=0; k<5; k++)
            {
                setfillstyle(SOLID_FILL, k);
                bar((maxX-(M*pixel))/2+(pixel*(j-1))+(w*k), (maxY-(M*pixel))/2+(pixel*(i-1))+(w*k), (maxX-(M*pixel))/2+(pixel*j), (maxY-(M*pixel))/2+(pixel*i));
                delay(100);
            }
            scoring(game_board[i-1][j-1]);
            game_board[i-1][j-1] = '1';
            delay(500);
            treasue_found();
            n = 1;
            break;
        default:
            break;
    }
}

void boom(void)
{
    //SHOWS ALL BOMB LOCATIONS ONE BY ONE, WHEN PLAYER UNCOVERS A BOMB
    int i, j;
     for(int k=0; k < total_bombs; k++)
    {
        i = bomb_positions[k][0];
        j = bomb_positions[k][1];
        setfillstyle(SOLID_FILL, BLACK);
        bar((maxX-(M*pixel))/2+(pixel*(j)), (maxY-(M*pixel))/2+(pixel*(i)), (maxX-(M*pixel))/2+(pixel*(j+1)), (maxY-(M*pixel))/2+(pixel*(i+1)));
        setcolor(YELLOW);
        circle((maxX + pixel*((2*j)-M+1))/2, (maxY + pixel*((2*i)-M+1))/2, pixel/2);
        circle((maxX + pixel*((2*j)-M+1))/2, (maxY + pixel*((2*i)-M+1))/2, pixel/4);
        circle((maxX + pixel*((2*j)-M+1))/2, (maxY + pixel*((2*i)-M+1))/2, pixel/8);
        Beep(750,300);
    }
    delay(500);
    game_over();
}

void game_over(void)
{
    //GAME OVER SCREEN
    //GAME IS OVER WHEN ENCOUNTERED A BOMB
    delay(500);
    cleardevice();
    border(4);
    for(int i=25, color=13; i>2; i--, color--)
    {
        setcolor(color);
        circle(maxX/2, maxY/2, (maxY-60)/i);
        delay(50);
        if(color==1) color = 13;
    }

    int width, height;

    for(int i=1; i<=8; i++)
    {
        setcolor(RED);
        settextstyle(SIMPLEX_FONT, HORIZ_DIR, i);
        char a[15] = "GAME OVER!";
        width = textwidth(a);
        height = textheight(a);
        outtextxy((maxX-width)/2, (maxY-height)/2, a);
        delay(50);
    }

    Beep(300 , 300);
    Beep(200 , 300);
    Beep(400 , 300);
    Beep(100 , 650);
    delay(500);

    for(int i=0; i<=20; i++)
    {
        setcolor(WHITE);
        settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);

        char b[30] = "Press any key to continue...";
        width = textwidth(b);
        height = textheight(b);

        outtextxy((maxX-width)/2, (maxY-height)/2 + 350, b);
        setfillstyle(SOLID_FILL, BLACK);
        bar((maxX-width)/2 +(10*i), (maxY-height)/2 + 350, (maxX+width)/2 -(10*i), (maxY+height)/2 + 350);
        delay(50);
    }
    getch();
}

void treasue_found(void)
{
    //FOLLOWING SCREENS APPEAR IN SUCCESSION TO FINDING TREASURE
    delay(500);
    cleardevice();

    border(2);
    coins(140, 400); coins(860, 400); coins(385, 400); coins(635, 400);
    coins(275, 600); coins(775, 600); coins(525, 600);

    setcolor(GREEN);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 8);
    char a[15] = "YOU WON!";
    int width = textwidth(a);
    int height = textheight(a);
    outtextxy((maxX-width)/2 , (maxY-height)/2, a);
    delay(1000);

    for(int i=0; i<=20; i++)
    {
        setcolor(WHITE);
        settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);

        char b[30] = "Press any key to continue...";
        width = textwidth(b);
        height = textheight(b);

        outtextxy((maxX-width)/2 , maxY - 100, b);
        setfillstyle(SOLID_FILL, BLACK);
        bar((maxX-width)/2 +(10*i), maxY - 100, (maxX+width)/2 -(10*i), maxY + height - 100);
        delay(50);
    }
    getch();
    cleardevice();

    if(difficulty != 14)
    {
        setcolor(7);
        settextstyle(SIMPLEX_FONT, VERT_DIR, 8);
        char c[25] = "LEVEL UP!";
        width = textwidth(c);
        height = textheight(c);
        border(7);
        outtextxy(height+50, (maxY-width)/2 ,c);

        steps();
        delay(1000);

        for(int i=0; i<=20; i++)
        {
            setcolor(WHITE);
            settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);

            char d[30] = "Press any key to continue...";
            width = textwidth(d);
            height = textheight(d);

            outtextxy((maxX-width)/2 , maxY - 100, d);
            setfillstyle(SOLID_FILL, BLACK);
            bar((maxX-width)/2 +(10*i), maxY - 100, (maxX+width)/2 -(10*i), maxY + height - 100);
            delay(50);
        }
        getch();
        cleardevice();
    }
}

void coins(int x, int y)
{
    //(140,400), (860,400), (385,400), (635, 400), (275, 600), (775,600), (525,600)
    for(int i=1; i<=7; i++)
    {
        setcolor(i);
        ellipse(x+(pow(-1,i)*15), y-40*i, 0, 360, 100, 50);
        ellipse(x+(pow(-1,i)*15), y+10-40*i, 175, 5, 100, 50);
        setfillstyle(9, 14);
        floodfill(x, y-40*i, i);
        setfillstyle(SOLID_FILL,DARKGRAY);
        floodfill(x, y+52-40*i, i);
        delay(100);
    }
}

void steps(void)
{
    //STEPS
    setfillstyle(SOLID_FILL, RED);
    bar3d(maxX/2-180, maxY/2+60, maxX/2+180, maxY/2+180, 60, 1);
    delay(300);

    setfillstyle(SOLID_FILL, BLUE);
    bar3d(maxX/2-60, maxY/2-60, maxX/2+180, maxY/2+60, 60, 1);
    delay(300);

    setfillstyle(SOLID_FILL, GREEN);
    bar3d(maxX/2+60, maxY/2-180, maxX/2+180, maxY/2-60, 60, 1);
    delay(300);

    //ARROW SHAFT
    int shaft[10] = {maxX/2-180, maxY/2, maxX/2+60, maxY/2-240, maxX/2+40, maxY/2-260, maxX/2-200, maxY/2-20, maxX/2-180, maxY/2};
    setfillstyle(8, 5);
    fillpoly(5, shaft);
    setcolor(WHITE);
    drawpoly(5, shaft);

    line(maxX/2-180, maxY/2, maxX/2-180, maxY/2+15); //bot perp down low
    line(maxX/2-200, maxY/2-20, maxX/2-200, maxY/2-5); //bot perp down high
    line(maxX/2-200, maxY/2-5, maxX/2-180, maxY/2+15); //join ends

    line(maxX/2+60, maxY/2-240, maxX/2+60, maxY/2-225); //top perp down low
    line(maxX/2-180, maxY/2+15, maxX/2+60, maxY/2-225); //join ends

    //ARROW HEAD
    int head[8] = {maxX/2+75, maxY/2-225, maxX/2+25, maxY/2-275, maxX/2+100, maxY/2-300, maxX/2+75, maxY/2-225};
    setfillstyle(8, 5);
    fillpoly(4, head);
    setcolor(WHITE);
    drawpoly(4, head);

    line(maxX/2+75, maxY/2-225, maxX/2+75, maxY/2-210); //base of head perp down low
    line(maxX/2+60, maxY/2-225, maxX/2+75, maxY/2-210); //join ends
    line(maxX/2+100, maxY/2-300, maxX/2+100, maxY/2-285); //tip of head perp down
    line(maxX/2+75, maxY/2-210, maxX/2+100, maxY/2-285); //join ends
}

void scoring(char x)
{
    char a[25];
    char b[25];
    switch(x)
    {
        case '0':
            for(int i=1; i<=5; i++)
            {
                score+=1;
                settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
                sprintf(a, "Score: %d", score);
                outtextxy(50, 65, a);
                delay(100);
            }
            break;
        case '*':
            break;
        case '#':
            settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
            score += (difficulty+1)*500;
            sprintf(a, "+%d", (difficulty+1)*500);
            outtextxy(100, 100, a);
            delay(500);
            setfillstyle(SOLID_FILL, BLACK);
            bar(100, 100, 200, 150);
            sprintf(b, "Score: %d", score);
            outtextxy(50, 65, b);
            break;
        default:
            break;
    }
}

void treasure_sensor(int i, int j)
{
    int d1, d2;
    int x = 100, y = 40, r = 15;   //Center and radius of the circle

    int arr[10] = {x-r, y-r, x+r, y-r, x+r, y+r, x-r, y+r, x-r, y-r};
    setfillstyle( SOLID_FILL, 0);
    fillpoly(4, arr);

    d1 = i - treasure[0];
    d2 = j - treasure[1];

    if(d1 == 0)
    {
        if(d2 < 0)
        {
            //RIGHT();
            circle(x, y, r);
            line(x, y, x+2*r/3, y);
        }
        else
        {
            //LEFT();
            circle(x, y, r);
            line(x, y, x-2*r/3, y);
        }
    }
    else if(d2 == 0)
    {
        if(d1 < 0)
        {
            //DOWN();
            circle(x, y, r);
            line(x, y, x, y+2*r/3);
        }
        else
        {
            //UP();
            circle(x, y, r);
            line(x, y, x, y-2*r/3);
        }
    }
    else if(d1 > 0)
    {
        if(d2 < 0)
        {
            //UP_RIGHT():
            circle(x , y , r);
            line(x, y, x+2*r/3, y-2*r/3);
        }
        else
        {
            //UP_LEFT();
            circle(x, y, r);
            line(x, y, x-2*r/3, y-2*r/3);
        }
    }
    else if(d1 < 0)
    {
        if(d2 < 0)
        {
            //DOWN_RIGHT();
            circle(x, y, r);
            line(x, y, x+2*r/3, y+2*r/3);
        }
        else
        {
            //DOWN_LEFT();
            circle(x, y, r);
            line(x, y, x-2*r/3, y+2*r/3);
        }
    }
}

void Blaze(int a, int b)
{
    ///SPECIAL FEATURE TO REVEAL DIAGONALS OF A PARTICULAR MOVE
        /*SILENCES THE BOMB ON THE DIGONALS IF ANY*/

    char arr1[25];
    for(int i=50; i>0; i--)
    {
        score-=1;
        settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
        sprintf(arr1, "Score: %d", score);
        outtextxy(50, 65, arr1);
        delay(50);
    }
    for( int i=a, j=b ; i<=M && j <=M ; i++ , j++)
    {
        blaze_check(i, j);
        delay(30);
    }

    for( int i=a, j=b ; i>=0 && j>=0 ; i-- , j--)
    {
        blaze_check(i, j);
        delay(30);
    }

    for( int i=a, j=b ; i>=0 && j<=M ; i-- , j++)
    {
        blaze_check(i, j);
        delay(30);
    }

    for( int i=a, j=b ; i<=M && j>=0 ; i++ , j--)
    {
        blaze_check(i, j);
        delay(30);
    }

    blaze = 0;
    counter++;
}

void blaze_check(int i, int j)
{
    switch(game_board[i-1][j-1])
    {
        case '0':
            setfillstyle(SOLID_FILL, WHITE);
            bar((maxX-(M*pixel))/2+(pixel*(j-1)), (maxY-(M*pixel))/2+(pixel*(i-1)), (maxX-(M*pixel))/2+(pixel*j), (maxY-(M*pixel))/2+(pixel*i));
            break;
        case '*':
            setfillstyle(SOLID_FILL, BLACK);
            bar((maxX-(M*pixel))/2+(pixel*(j-1)), (maxY-(M*pixel))/2+(pixel*(i-1)), (maxX-(M*pixel))/2+(pixel*j), (maxY-(M*pixel))/2+(pixel*i));
            break;
        case '#':
            setfillstyle(SOLID_FILL, CYAN);
            bar((maxX-(M*pixel))/2+(pixel*(j-1)), (maxY-(M*pixel))/2+(pixel*(i-1)), (maxX-(M*pixel))/2+(pixel*j), (maxY-(M*pixel))/2+(pixel*i));
            scoring(game_board[i-1][j-1]);
            delay(500);
            getch();
            treasue_found();
            n = 1;
            break;
        default:
            break;
    }
}

void retrieve_score(FILE* path)
{
    fscanf(path, "%d %d %d ",&s[0],&s[1],&s[2]);
    printf("%d %d %d \n", s[0], s[1], s[2]);
    //s[0], s[1], s[2] now have the scores
}

void put_score(FILE* path)
{
    if(score<s[0] && score<s[1] && score>s[2])
    {
        s[2]=score;
    }
    else if(score<s[0] && score>s[1] && score>s[2])
    {
        s[2]=s[1];
        s[1]=score;
    }
    else if(score>s[0] && score>s[1] && score>s[2])
    {
        s[2]=s[1];
        s[1]=s[0];
        s[0]=score;
        newHscore=1;
    }
    fprintf(path,"%d %d %d ",s[0],s[1],s[2]);
}

void save_score(void)
{
    char cCurrentPath[FILENAME_MAX];
    getcwd(cCurrentPath, sizeof(cCurrentPath));
    strcat(cCurrentPath,"/save_score.txt");

    FILE *fp;
    fp=fopen(cCurrentPath,"a+");

    //Setting the file if the program is run first time on pc
    if(fgetc(fp)==-1)
    {
        fseek(fp,0,SEEK_SET);
        fclose(fp);
        fp=fopen(cCurrentPath,"w+");
        fprintf(fp,"0 0 0 ");
        fclose(fp);
    }

    fp=fopen(cCurrentPath,"a+");
    printf("Old High Scores : ");
    retrieve_score(fp);
    fclose(fp);

    fp=fopen(cCurrentPath,"w+");
    put_score(fp);
    fclose(fp);

    fp=fopen(cCurrentPath,"a+");
    printf("New High Scores : ");
    retrieve_score(fp);
    fclose(fp);
}

void H_Scoremat(void)
{
    //BACKGROUND MAT OVER WHICH GRID WILL BE PLACED
    for(int i=0, j=0 ; i<maxX; i+=10 , j+=1)
    {
        int t =j%2;
        int color;
        switch(t)
        {
            case 0:
                color = 1;
                break;
            case 1:
                color = 4;
                break;
        }
        setcolor(color);
        line(i-15, 0, maxX-20, i);
        line(i-15, maxY, maxX-20, maxY-i);  //change new
        line(0, maxY-i, maxX-i, maxY);
        line(0, i, maxX-i, 0);  //change new
        delay(5);
    }
}

void new_high_score(void)
{
    int width, height;
    char b[30], d[25];
    if(newHscore == 1)
    {
        setcolor(YELLOW);
        settextstyle(SIMPLEX_FONT, HORIZ_DIR, 7);
        char a[15] = "High Scores";
        width = textwidth(a);
        height = textheight(a);
        outtextxy((maxX-width)/2 , (maxY/3-height)/2, a);

        setcolor(RED);
        settextstyle(SIMPLEX_FONT, HORIZ_DIR, 7);
        sprintf(b,"NEW HIGH SCORE!!!");
        width = textwidth(b);
        height = textheight(b);
        outtextxy((maxX-width)/2 , (maxY-height)/1.25, b);
    }
    else
    {
        setcolor(RED);
        settextstyle(SIMPLEX_FONT, HORIZ_DIR, 6);
        sprintf(d,"BETTER LUCK NEXT TIME");
        width = textwidth(d);
        height = textheight(d);
        outtextxy((maxX-width)/2 , (maxY-height)/1.25, d);
    }
}

void high_score_screen(void)
{
    cleardevice();
    H_Scoremat();

    border(14);
    setcolor(BLUE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 4);

    char bestscore[50];
    sprintf(bestscore, "Score : %d", score);

    int width = textwidth(bestscore);
    int height = textheight(bestscore);
    outtextxy((maxX-width)/2 , ((maxY-height)/2), bestscore);

    new_high_score();
    getch();
}

