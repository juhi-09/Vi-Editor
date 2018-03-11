
#include "os_final1.h"


                                            //  THE MAIN FUNCTION 
int main(int agrc,char* argv[])
{

cur_i=1;;
cur_j=1;
change=0;                        //FILE IS NOT CHANGED INITIALLY
 defalt=1;  

char c;
initeditor();
print_file(argv[1]);
gotoxy(edit.cx,edit.cy);
fflush(stdout);
while(1)
{
  non_canon();
  read(0,&c,1);
  fflush(stdin);
    if(c =='\x1b' || defalt )
       comm_exit(change);
         normal_mode(c);                //********************NORMAL MODE() ****************

   if(c=='i')                 // ************************INSERT MODE **********************
   insert();

    if(c==':')  
     comm_exit(change);               //*************************COMMAND MODE*******************
}
end_canon();
printf("\033[2J\033[1;1H");
gotoxy(1,1);

return 0;
}
