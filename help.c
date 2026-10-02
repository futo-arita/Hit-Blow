#include<ncurses.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>



void help(int y,int x){
    FILE*fp;
    char buf[256];
    int y_num=3;
    int ch=0;

    clear();
    fp=fopen("help.txt","r");
    if(fp==NULL){
        mvprintw(y/2,x/2-12,"help.txtが見つかりません");
        refresh();
        sleep(2);
        endwin();
        return;
    }

    crmode();
    noecho();
    curs_set(0);

    /*ファイルの文字は日本語のみ対応*/
    while(fgets(buf,sizeof(buf),fp)!=NULL){
        buf[strcspn(buf,"\r\n")]='\0';
        mvprintw(y_num++,x/2-strlen(buf)/3,"%s",buf);
    }
    fclose(fp);
    mvprintw(++y_num,x/2-5,"0- 終了");
    refresh();
    while(true){
        ch=getch();
        if(ch=='0'){
            break;
        }
    }
    endwin();
}