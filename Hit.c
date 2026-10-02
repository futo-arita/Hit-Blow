#include<ncurses.h>
#include<unistd.h>
#include <stdio.h>
#include <string.h>
#include<locale.h>

void help(int y,int x);
void game(int y,int x);

int main(int argc,char**argv){
    int x,y;
    int ch=0;

    if(argc!=1){
        fprintf(stderr,"./Hitのみ入力してください\n");
        return 1;
    }

    setlocale(LC_ALL, "");

    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    getmaxyx(stdscr,y,x);
    if(x<68||y<28){
        endwin();
        fprintf(stderr,"画面の大きさを縦28×横68以上にしてください\n");
        return 1;
    }
    crmode();
    noecho();
    curs_set(0);
    while(ch!='0'){
        clear();
        mvprintw(y/3,x/2-8/2,"Hit&Blow");
        mvprintw(y/2,x/2-18/2,"1- ゲームスタート");
        mvprintw(y/2+2,x/2-10/2,"2- ヘルプ");
        mvprintw(y/2+4,x/2-8/2,"0- 終了");
        refresh();
        ch=getch();
        if(ch=='1'){
            game(y,x);
        }
        if(ch=='2'){
            help(y,x);
        }
    }
    endwin();
    return 0;
}