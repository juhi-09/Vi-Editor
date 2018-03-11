



#include<stdio.h>
#include<errno.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include<termios.h>
#include <signal.h>
#include<ctype.h>
#include<string.h>
//*************************************************    #DEFINES  *****************************************
 

#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))
int cur_i;
int cur_j;
int change;                        //FILE IS NOT CHANGED INITIALLY
int defalt;                        // BY DEFAULT FILE OPENS IN NORMAL MODE 

// *****************************************defining editor interface*************************************
struct editorconfig
{
int cx,cy;
int winrows;
int wincols;
struct termios t;
} edit;

int getwinsize(int *rows,int* cols);
void initeditor();



void non_canon();            // SETTING TO NON_CANNONICAL MODE      

void end_canon();             // DISABLING NON_CANNONICAL MODE


                         //HANDLING THE NAVIGATION IN NORMAL MODE
void nav(char c);
void control_nav();

void tab_handle();
void normal_mode(char c);

                               //COMMAND MODE
void comm_exit(int change);
                              //INSERT MODE
void insert();

                            // EMPTY FILE HANDLER 
void empty_file(char c );

                                    // CLEARING SCREEN AND PRINTING THE FILE
int print_file(char * f);
