#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int select(int x,int y){
    int ch=0;
    clear();
    while(1){
        mvprintw(y/2-1,x/2-24,"挑戦する桁数を下の数字から選んで入力してください");
        mvprintw(y/2,x/2-4,"3 4 5 6");
        ch=getch();
        if(ch>='3'&&ch<='6'){
            return ch-'0';
        }
    }
}

void answer(int ans[],int DIGITS) {
    int used[10] = {0};
    int i = 0;

    while (i < DIGITS) {
        int d = rand() % 10;
        if (!used[d]) {
            used[d] = 1;
            ans[i] = d;
            i++;
        }
    }
}

void judge(int ans[], int guess[], int *hit, int *blow,int DIGITS) {
    int i, j;
    *hit = 0;
    *blow = 0;
    for (i = 0; i < DIGITS; i++) {
        if (guess[i] == ans[i]) {
            (*hit)++;
        } else {
            for (j = 0; j < DIGITS; j++) {
                if (guess[i] == ans[j]) {
                    (*blow)++;
                    break;
                }
            }
        }
    }
}

/* 入力取得 */
void get_input(char *input, int row, int attempt,int DIGITS,int x) {
    mvprintw(row, x/2-19, "[%d回目] 推測: ", attempt);
    echo();
    getnstr(input,DIGITS);
    noecho();
}

/* 入力チェック（桁・数字・重複） */
int validate_input(char *input, int guess[],int DIGITS) {
    int used_digit[10] = {0};
    int j;

    if ((int)strlen(input) != DIGITS) return 1;

    for (j = 0; j < DIGITS; j++) {
        if (input[j] < '0' || input[j] > '9') return 2;
        guess[j] = input[j] - '0';
        if (used_digit[guess[j]]) return 3;
        used_digit[guess[j]] = 1;
    }

    return 0;
}

/* エラー処理（3回で終了） */
int handle_error(int *error_count, int *attempt, int row, int error_type,int DIGITS,int x) {
    const char *msg[] = {
        "",
        "※ %d桁を入力してください",
        "※ 数字のみを入力してください",
        "※ 数字は重複なしで入力してください"
    };

    attron(COLOR_PAIR(3));
    mvprintw(row + 1, x/2-19, msg[error_type], DIGITS);

    (*error_count)++;
    (*attempt)--;

    if (*error_count >= 3) {
        mvprintw(row + 2, x/2-19, "エラーが3回に到達したため終了します");
        attroff(COLOR_PAIR(3));
        return 1;
    }
    attroff(COLOR_PAIR(3));

    return 0;
}

void game(int y, int x) {
    int DIGITS=select(x,y);
    int ans[6];
    int guess[6];
    int hit, blow;
    int row = 3;
    int error_count = 0;
    char input[100];
    int i,ch=0;

    srand((unsigned int)time(NULL));
    answer(ans,DIGITS);

    clear();
    mvprintw(0, x/2-9, "=== Hit & Blow ===");
    mvprintw(1, x/2-19, "重複なしで%d桁の数字を入力してください", DIGITS);

    for (i = 1; i <= 10; i++) {
        int err;

        get_input(input, row, i,DIGITS,x);

        err = validate_input(input, guess,DIGITS);
        if (err != 0) {
            if (handle_error(&error_count, &i, row, err,DIGITS,x)) break;
            row += 2;
            continue;
        }

        judge(ans, guess, &hit, &blow,DIGITS);

        attron(COLOR_PAIR(1));
        mvprintw(row + 1, x/2-19, "Hit: %d", hit);
        attroff(COLOR_PAIR(1));

        attron(COLOR_PAIR(2));
        mvprintw(row + 1, x/2-9, "Blow: %d", blow);
        attroff(COLOR_PAIR(2));

        row += 2;

        if (hit == DIGITS) {
            refresh();
            sleep(2);
            clear();
            mvprintw(y/2-5, x/2-3, "CLEAR!");
            mvprintw(y/2-3,x/2-14,"あなたは%d回目で正解しました",i);
            break;
        }
    }
    if(hit!=DIGITS){
        refresh();
        sleep(2);
        clear();
        mvprintw(y/2-2,x/2-5,"GAME OVER");
    }

    mvprintw(y/4*3-1,x/2-9-DIGITS/2,("正解の数列は"));
    for(i=0;i<DIGITS;i++){
        mvprintw(y/4*3-1,x/2-DIGITS/2+i+2,"%dでした",ans[i]);
    }
    mvprintw(y/4*3,x/2-17, "qキーを入力するとメニューに戻ります");
    while(ch!='q'){
        ch=getch();
    }
}
