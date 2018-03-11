

#include "os_final1.h"


                                                         // defining editor interface

int getwinsize(int *rows,int* cols)
{
struct winsize w;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)==-1 ||w.ws_col==0)
    return -1;
   else
    {
   *rows=w.ws_row;
   *cols=w.ws_col;
   return 0;
   }
}


void initeditor()
{
edit.cx=1;
edit.cy=1;
getwinsize(&(edit.winrows),&(edit.wincols));
}



                                                   // SETTING TO NON_CANNONICAL MODE 
void non_canon()
{
tcgetattr(STDIN_FILENO,&edit.t);
struct termios in=edit.t;
in.c_lflag &=~(ECHO | ICANON | ISIG);
in.c_lflag &=~(IXON);
in.c_oflag &=~(OPOST);
tcsetattr(STDIN_FILENO,TCSAFLUSH,&in);
}


                                                    // DISABLING NON_CANNONICAL MODE
void end_canon()
{
tcsetattr(STDIN_FILENO,TCSAFLUSH,&edit.t);
} 

                                                   //HANDLING THE NAVIGATION IN NORMAL MODE 

void nav(char c)
{
 
  switch(c)
   {

  case'k': if(edit.cx>1 && edit.cx<=cur_i+2)
             edit.cx--;
            break;
  case 'l': if(edit.cy<edit.wincols)
            edit.cy++;
            else if(edit.cy==edit.wincols)
            {
             edit.cy=1; 
             if(edit.cx<cur_i)
             edit.cx++;
             }
            break;
 case'h': if(edit.cy>1)
           edit.cy--;
          else if(edit.cy==1 && edit.cx!=1)
           {
            edit.cx--;
            edit.cy=edit.wincols;
           }
            break;
 case'j': if(edit.cx<=cur_i+2)
          edit.cx++;
          break;

  }
}



                                        // HANDLING THE NAVIGATION WITH CONTROL KEYS IN NORMAL MODE 
void control_nav()
{
   char s[2];
   read(STDIN_FILENO,&s[0],1); 
   read(STDIN_FILENO,&s[1],1);
        if(s[0]=='[')
           {
             switch(s[1])
               {
                   case'A': nav ('k');
                             break;
                   case 'B': nav('j');
                              break;
                   case 'C': nav('l');
                              break;
                   case 'D': nav('h');
                              break;
                }
           }
}



                                         // HANDLING TAB CASE 
void tab_handle()
{
 edit.cy=((edit.cy/4)+1)*4;
 gotoxy(edit.cx,edit.cy);
 fflush(stdout);
}


                                          //NORMAL MODE 
void normal_mode(char c)
{ 
     switch(c)
     {
     case 'h':
     case 'j':
     case 'k':
     case 'l':   nav(c);
                 gotoxy(edit.cx,edit.cy);
                 fflush(stdout);
                 break;
                      //*******************************************CONTROL KEYS NAVIGATION*****************************
     case '\x1b': control_nav();
                  gotoxy(edit.cx,edit.cy);
                  fflush(stdout);
                  break;
                       //***********************REPLACE A CHARACTER **************************
     case 'r':    read(0,&c,1);
                  end_canon();
                  printf("%c",c);
                  fflush(stdout);
                  non_canon();
                  change=1;
                  break;
                       //*************************GOING TO FIRST LINE OF THE FILE *********************************
     case 'g':     read(0,&c,1);
                  if(c=='g')
                   edit.cx=1;
                   edit.cy=1;
                  gotoxy(edit.cx,edit.cy);
                  fflush(stdout);
                  break;
                        //**************************GOING TO LAST LINE OF THE FILE **********************************
     case 'G':     edit.cx=cur_i;
                   edit.cy=1;
                   gotoxy(edit.cx,edit.cy);
                   fflush(stdout);
                   break;
    case '\x09':   tab_handle();
                   break;

    case'\x0d':    edit.cy=1;
                   edit.cx++;
                   gotoxy(edit.cx,edit.cy);
                   fflush(stdout);
                   break;
     }

}



                                             //HANDLING THE FILE IN COMMAND MODE 
void comm_exit(int change)
{
    char s[2];
     end_canon();
     gotoxy(edit.winrows-1,1);
     fflush(stdout);
    
    printf("%c",':');
    fflush(stdout);
    end_canon();
     scanf("%s",s); 
    printf("%s",s);
    fflush(stdout);
    sleep(2);
    
     if(s[0]=='q' && change ==1 && s[1]!='!')
     {

      printf("%s","file is modified do you want to exit!!");
      fflush(stdout);
      sleep(3);
     }
     else if(s[0]=='w' && s[1]=='q')
     {

        printf("saving your file");
        fflush(stdout);
        sleep(3);
     }
  if(s[0]!='\x1b')    
  {
     printf("\033[2J\033[1;1H");
     fflush(stdout);
     gotoxy(1,1);
     fflush(stdout);
     end_canon();
     exit(0);
  }
}


                                                    //HANDLING INSERT MODE

void insert()
 {    
      char c;
      
      defalt=0;
      end_canon();
      fflush(stdin);
      read(0,&c,1);
      fflush(stdin);
      while(c!='\x1b' && c!=':')
       {
        end_canon();
        printf("%c",c);
        fflush(stdout);
        change=1;
        if(c=='\n' || cur_j==edit.wincols)
         {
           ++cur_i;
           cur_j=0;
           printf("%c",'\n');
           fflush(stdout);
         }
        else
          {
            printf("%c",c);
            fflush(stdout);
            cur_j++;
            if(cur_j==edit.wincols)
            ++cur_i;
          }
        fflush(stdout);
        non_canon();
        read(0,&c,1);
        fflush(stdin);
         defalt=1;
        }
      if(c=='\x1b')
         defalt=1;

 
      if(c==':')
        {  
          defalt=0; 
          end_canon();
          comm_exit(change);
        }
      
  }

// EMPTY FILE HANDLER 

void empty_file(char c )
{
   switch(c)
        {     
       case'\x09':      tab_handle();
                        break; 

       case ':'  :
                     comm_exit(1); 
                     break;
       case 'i':    insert();
                      break;

       default:
                       edit.cx=1;
                       edit.cy=1;
                       gotoxy(1,1);
                       non_canon();
                       read(0,&c,1);
                       fflush(stdin);
                       while(c!='i' && c!=':')
                        {
                          normal_mode(c);
                          read(0,&c,1);
                          fflush(stdin);
                        }
                       if(c==':')
                       comm_exit(1); 
                       if(c=='i')
                        insert();
                       break;
                       
         }
}

                                    // CLEARING SCREEN AND PRINTING THE FILE
int print_file(char * f)
{

printf("\033[2J\033[1;1H");
fflush(stdout);
FILE *fp;
char c;
fp=fopen(f,"r");
if(fp)
{  
    c = fgetc(fp);
    while (c != EOF)
    {
        printf ("%c", c);
        fflush(stdin);
          cur_j++;
        c = fgetc(fp);
        if(c=='\n' && c!=EOF)
        { 
         ++cur_i; 
         cur_j=0;
        }
    }
}
else                                 //********************************** HANDLING AN EMPTY FILE CASE *********************************
  {
   while(1)
    {non_canon();
     read(0,&c,1);
     empty_file(c);     
     }
   }
}


