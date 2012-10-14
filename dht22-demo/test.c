
#include<stdio.h>

main()
{
    int i=0;
    do{
       i++;
       printf("Hello World %d\n",0x80);
       printf("Hello World %d\n",0xFF & 0x80);
       printf("Hello World %d\n",0X00 & 0x80);
       printf("Hello World %d\n",~ 0X00 & 0x80);
       printf("Hello World %d\n",~ 0X00 & 0x80);
       printf("Hello World %d\n",~ (0X00 & 0x80));
       if (i>10) break;
    } while (!((0xFF & 0x80)!=0));

}
