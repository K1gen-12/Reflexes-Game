#include<ncurses.h>
#include<unistd.h>
#include<stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAX 255
/* 日本語利用するため */            
#include <locale.h>

/* 構造体をセット */
typedef struct{
    char  name[20];
    float time;
} PLAYER;

void SetTitle(int ceneter_x,int center_y);
void ShowDescription(int center_X,int center_Y);
void ShowOpt(int center_X,int center_Y);
void DrawCircle(int center_X,int center_Y);
double CountTime(int center_X,int center_Y);
void ShowRanking(int center_X,int center_Y);
void swap(PLAYER *x, PLAYER *y);
void bubble_sort(PLAYER data[], int n);
void AddRanking(int center_X,int center_Y,float avetime,char *name);
void showOption(int center_X,int center_Y);

/* PLAY TIME */
int mesureTime = 3;

int main(int argc,char **argv){
    int ch,i=0;
    int max_X,max_Y;
    int center_X,center_Y;
    char name[20];
    double time,aveTime;
    MEVENT event;

    if (argc > 1){
        fprintf(stderr,"オプション引数は対応しておりません\n");
        exit(1);
    }

    /* 環境変数から言語を設定 */
    setlocale(LC_ALL, "");

    /* スクリーンの作製 */
    initscr();

    /* スクリーン設定 */
    cbreak();
    curs_set(0);
    noecho();
    /* マウスを利用 */
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS,NULL);

    do{
        mvprintw(0,0,"Change Screen size 114x32");
        refresh();
    }while(getmaxx(stdscr) != 114);

    do{
        mvprintw(0,0,"Change Screen size 114x32");
        refresh();
    }while(getmaxy(stdscr) != 32);

    getmaxyx(stdscr,max_Y,max_X);
    center_X = max_X / 2;
    center_Y = max_Y / 2;

    SetTitle(center_X,center_Y);
    sleep(3);
    start:
        ShowOpt(center_X,center_Y);

    while(1){
        getmouse(&event);
        ch = getch();
        if (ch == 0x1b){
            end:
                break;
        }else{
            switch (event.bstate)
            {
                case BUTTON1_PRESSED:
                    /* START PLAY */
                    if ((event.y >= 6 && event.y <= 8) && (event.x>=49 && event.x <=65)){

                        clear();

                        mvprintw(center_Y,center_X-7,"ARE YOU READY?\n");
                        mvprintw(center_Y+1,center_X-19,"IF YOU'RE OK: PRESS Y OR NOT PRESS:N\n");
                        refresh();

                        if (getch() == 'n'){
                            clear();
                            goto start;
                        }else{
                            clear();

                            for(i = 0;i<mesureTime;i++){
                                mvprintw(getmaxy(stdscr)-1,center_X-(24/2),"GAME IS ALREADY BEGIN...");
                                refresh();

                                time = CountTime(center_X,center_Y);
                                aveTime += time;
                                sleep(2);
                            }

                            aveTime = aveTime / 3;

                            clear();
                            mvprintw(center_Y-2,center_X-7,"::: RESULT :::");
                            mvprintw(center_Y,center_X-12,"AVERAGE_TIME:%fsec",aveTime);

                            /* GET NAME AND SAVE TEXT */
                            if (mesureTime == 1){
                                mesureTime = 3;
                            }else{
                                echo();
                                nocbreak();

                                mvprintw(center_Y+1,0,"TYPE YOUR NAME NO MORE THAN 10 CHARACTERS:");
                                refresh();
                                scanw("%s",name);
                                refresh();
                                sleep(3);

                                noecho();
                                cbreak();

                                AddRanking(center_X,center_Y,aveTime,name);
                            }
                        }
                        
                        clear();
                        ShowOpt(center_X,center_Y);
                        refresh();
                    }else if((event.y >= 10 && event.y <= 12) && (event.x >= 47 && event.x <= 66)){
                        /* SHOW RANKING */
                        while(ch != 'q'){
                            ShowRanking(center_X,center_Y);
                            ch = getch();
                        }
                        clear();
                        ShowOpt(center_X,center_Y);
                        refresh();
                    }else if((event.y >= 14 && event.y <= 16) && (event.x >= 50 && event.x <= 64)){
                        /* OPTION */
                        clear();
                        mvprintw(0,0,"OPTION:");
                        showOption(center_X,center_Y);
                        mvprintw(getmaxy(stdscr)-1,center_X-(32/2),"=== EXIT OPTION IS PUSH 'q' ===");
                        while(ch != 'q'){
                            ch = getch();
                            if (ch == 'd'){
                                mesureTime = 1;
                                mvprintw(center_Y+10,center_X-(41/2),"DEBUG MODE IS SELECTED. LET'S PLAY GAME!");
                                refresh();
                            }
                        }
                        clear();
                        ShowOpt(center_X,center_Y);
                        refresh();

                    }else if((event.y >= 18 && event.y <= 20) && (event.x >= 48 && event.x <= 66)){
                        /* DESCRIPTION */
                        ShowDescription(center_X,center_Y);
                        while(ch != 'q'){
                            ch = getch();
                        }
                        clear();
                        ShowOpt(center_X,center_Y);
                        refresh();
                    }else if((event.y >= 22 && event.y <= 24) && (event.x >= 51 && event.x <= 62)){
                        /* QUIT */
                        clear();
                        mvprintw(center_Y,center_X-(17/2),"QUIT IS CLICKED.\n");
                        mvprintw(center_Y+1,center_X-(11/2),"See You...\n");
                        refresh();
                        sleep(3);
                        goto end;
                    }
                break;
            default:
                break;
            }
        }
    }
    clear();
    refresh();
    nocbreak();
    endwin();
    return 0;
}
/* already checked */
void SetTitle(int center_X,int center_Y){
    clear();

    mvprintw(0,center_X-center_X,"      ___           ___           ___                       ___           ___           ___           ___     \n");
    mvprintw(1,0,"     /  /\\         /  /\\         /  /\\                     /  /\\         /__/|         /  /\\         /  /\\    \n");
    mvprintw(2,0,"    /  /::\\       /  /:/_       /  /:/_                   /  /:/_       |  |:|        /  /:/_       /  /:/_   \n");
    mvprintw(3,0,"   /  /:/\\:\\     /  /:/ /\\     /  /:/ /\\  ___     ___    /  /:/ /\\      |  |:|       /  /:/ /\\     /  /:/ /\\  \n");
    mvprintw(4,0,"  /  /:/~/:/    /  /:/ /:/_   /  /:/ /:/ /__/\\   /  /\\  /  /:/ /:/_   __|__|:|      /  /:/ /:/_   /  /:/ /::\\ \n");
    mvprintw(5,0," /__/:/ /:/___ /__/:/ /:/ /\\ /__/:/ /:/  \\  \\:\\ /  /:/ /__/:/ /:/ /\\ /__/::::\\____ /__/:/ /:/ /\\ /__/:/ /:/\\:\\\n");
    mvprintw(6,0," \\  \\:\\/:::::/ \\  \\:\\/:/ /:/ \\  \\:\\/:/    \\  \\:\\  /:/  \\  \\:\\/:/ /:/    ~\\~~\\::::/ \\  \\:\\/:/ /:/ \\  \\:\\/:/~/:/\n");
    mvprintw(7,0,"  \\  \\::/~~~~   \\  \\::/ /:/   \\  \\::/      \\  \\:\\/:/    \\  \\::/ /:/      |~~|:|~~   \\  \\::/ /:/   \\  \\::/ /:/ \n");
    mvprintw(8,0,"   \\  \\:\\        \\  \\:\\/:/     \\  \\:\\       \\  \\::/      \\  \\:\\/:/       |  |:|      \\  \\:\\/:/     \\__\\/ /:/  \n");
    mvprintw(9,0,"    \\  \\:\\        \\  \\::/       \\  \\:\\       \\__\\/        \\  \\::/        |  |:|       \\  \\::/        /__/:/   \n");
    mvprintw(10,0,"     \\__\\/         \\__\\/         \\__\\/                     \\__\\/         |__|/         \\__\\/         \\__\\/    \n");

    mvprintw(center_Y,18,"      ___           ___           ___           ___           ___     \n");
    mvprintw(center_Y+1,18,"     /\\  \\         /\\__\\         /\\  \\         /\\  \\         /\\__\\    \n");
    mvprintw(center_Y+2,18,"    /::\\  \\       /:/  /        /::\\  \\       /::\\  \\       /:/  /    \n");
    mvprintw(center_Y+3,18,"   /:/\\:\\  \\     /:/__/        /:/\\:\\  \\     /:/\\:\\  \\     /:/__/     \n");
    mvprintw(center_Y+4,18,"  /:/  \\:\\  \\   /::\\  \\ ___   /::\\~\\:\\  \\   /:/  \\:\\  \\   /::\\__\\____ \n");
    mvprintw(center_Y+5,18," /:/__/ \\:\\__\\ /:/\\:\\  /\\__\\ /:/\\:\\ \\:\\__\\ /:/__/ \\:\\__\\ /:/\\:::::\\__\\\n");
    mvprintw(center_Y+6,18," \\:\\  \\  \\/__/ \\/__\\:\\/:/  / \\ \\~\\:\\ \\/__/ \\:\\  \\  \\/__/ \\/_|:|~~|~   \n");
    mvprintw(center_Y+7,18,"  \\:\\  \\            \\::/  /   \\:\\ \\:\\__\\    \\:\\  \\          |:|  |    \n");
    mvprintw(center_Y+8,18,"   \\:\\  \\           /:/  /     \\:\\ \\/__/     \\:\\  \\         |:|  |    \n");
    mvprintw(center_Y+9,18,"    \\:\\__\\         /:/  /       \\:\\__\\        \\:\\__\\        |:|  |    \n");
    mvprintw(center_Y+10,18,"     \\/__/         \\/__/         \\/__/         \\/__/         \\|__|    \n");
    refresh();
}

void ShowRanking(int center_X,int center_Y){
    int i=0,max;
    PLAYER data[100];
    FILE *fp;

    fp = fopen("ranking.txt","r");

    if (fp==NULL){
        mvprintw(1,1,"error:failed reading file.\n");
        refresh();
        sleep(1);
    }else{
        
        while(fscanf(fp,"%s %f\n",data[i].name,&data[i].time)!=EOF){
            i++;
            if (i>10){
                break;
            }
        }
    }

    bubble_sort(data,i);

    max = i;
    fclose(fp);

    clear();

    mvprintw(0,0,"SHOW RANKING:");
    mvprintw(getmaxy(stdscr)-1,center_X-(23/2),"=== EXIT OPTION IS PUSH 'q' ===");
    sleep(1);

    for(i=0;i<max;i++){
        mvprintw(center_Y+i-4,center_X-10,"%d. %20s %.5f\n",i+1,data[i].name,data[i].time);
    }
    refresh();
}

void ShowOpt(int center_X,int center_Y){
    clear();
    mvprintw(7,center_X-(17/2),"--- PLAY GAME ---");
    mvprintw(11,center_X-(20/2),"--- SHOW RANKING ---");
    mvprintw(15,center_X-(14/2),"--- OPTION ---");
    mvprintw(19,center_X-(19/2),"--- DESCRIPTION ---");
    mvprintw(23,center_X-(12/2),"--- QUIT ---");
    mvprintw(30,center_X-(51/2),"=== PRESS THE MOUSE LEFT BUTTON ON THE OPTION. ===");

    refresh();
}

void DrawCircle(int center_X,int center_Y){
    clear();
    mvprintw(center_Y-3,center_X-10,"       ###########");
    mvprintw(center_Y-2,center_X-10,"     ###############");
    mvprintw(center_Y-1,center_X-10,"    #################");
    mvprintw(center_Y,center_X-10,"   ###################");
    mvprintw(center_Y+1,center_X-10,"   ###################");
    mvprintw(center_Y+2,center_X-10,"    #################");
    mvprintw(center_Y+3,center_X-10,"     ###############");
    mvprintw(center_Y+4,center_X-10,"       ###########");
    refresh();
}

int  MakeRandomTime(void){

    int r_num;
    /* 乱数値を初期化 */
    srand(time(NULL));
    r_num = rand()%10 + 1;

    return r_num;
}

void swap(PLAYER *x, PLAYER *y) {
  PLAYER tmp = *x;
  *x = *y;
  *y = tmp;
}

void bubble_sort(PLAYER data[], int n) {
  int i, j;
  for (i = 0; i < n; i++){
    for (j = n-1; j > i; j--){
      if (data[j-1].time > data[j].time) {
	    swap(&data[j], &data[j-1]);
      }
    }
  }
}

double CountTime(int center_X,int center_Y){
    int num,ch;
    int flag=0;
    /* 時間計測用変数 */
    clock_t start_clock, end_clock;
    double time;

    mousemask(ALL_MOUSE_EVENTS,NULL);
    num = MakeRandomTime();

    sleep(num);

    start_clock = clock();
    DrawCircle(center_X,center_Y);

    while(1){
        ch = getch();
        if (BUTTON1_CLICKED){
            break;
        }
        printf("%d,%c\n",flag,ch);
        flag++;
    }

    end_clock = clock();
    clear();
    refresh();

    time = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;

    return time*10;
}

void AddRanking(int center_X,int center_Y,float avetime,char *name){
    FILE *fp;

    fp = fopen("ranking.txt","a");

    if (fp==NULL){
        mvprintw(1,1,"error:failed reading file.\n");
        refresh();
        sleep(1);
        exit(1);
    }else{
        fprintf(fp,"%s %f\n",name,avetime);
    }
    fclose(fp);
}

void ShowDescription(int center_X,int center_Y){
    clear();
    mvprintw(0,0,"DESCRIPTION:\n\n    THIS GAME IS MEASURING YOUR REFLEXES.\n\n    IF YOU CLICK 'PLAY GAME' THE GAME WILL START SOON.\n\n    AFTER START THE GAME, YOU NEED TO CLICK CIRCLE APPEAR RANDOM TIME IN 3 TIMES.\n\n    IF YOU SEE A '￥' ON THE TITLE SCREEN, CHANGE THE FONT TO 'CONSOLAS' ON THE C1-BOYD TERMINAL.\n\n");
    refresh();
}

void showOption(int center_X,int center_Y){
    clear();
    mvprintw(center_Y,center_X-(47/2),"IF YOU WANT TO PLAY DEBUG MODE, PRESS 'D' KEY.");
    
    refresh();
}