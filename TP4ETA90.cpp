#include <stdlib.h>
#include <conio.h>

float zab=1.0;
int pole[81][51],indred,flgnozap,brt=1;

struct top4etype {
	int x,y; // koordinati na top4eto
   int dir;  // teky6ta posoka na top4eto
} top4e[12];

struct bndrytype {
	int x,y;
   int vtr;
} bndry[4000];

struct redtraektorytype {
	int x,y;
} redtr[4000];

int if_hit_how_change_dir(int x,int y,int dir);      // prototype

void nadpis(int xnach,int y) {
    int i,j,x;
   int letx[5][2]={0,-1,1,2,3,4,5,-1,6,7},leto[5][6];
   for (i=1;i<80;i++)
      for (j=1;j<50;j++)
      	pole[i][j]==0;

   for (x=xnach;x<80;x+=44)
   for (i=0;i<5;i++)
      for (j=0;j<2;j++)
         if (letx[i][j]!=-1) {
      		pole[x-i][y-letx[i][j]]=1;
            pole[x+i][y-letx[i][j]]=1;
            pole[x-i][y+letx[i][j]]=1;
            pole[x+i][y+letx[i][j]]=1;
      	}
    for (i=0;i<5;i++)
      for (j=0;j<6;j++)
         leto[i][j]=-1;
    leto[0][0]=7;leto[1][0]=7;leto[2][0]=7;leto[3][0]=6;
    for (i=5;i>-1;i--)
    		leto[4][i]=i;

     x=xnach+12;
     for (i=0;i<5;i++)
      for (j=0;j<6;j++)
         if (leto[i][j]!=-1) {
      		pole[x-i][y-leto[i][j]]=1;
            pole[x+i][y-leto[i][j]]=1;
            pole[x-i][y+leto[i][j]]=1;
            pole[x+i][y+leto[i][j]]=1;
      	}
     x=xnach+24;
     for (i=0;i<5;i++)
      for (j=0;j<2;j++)
         if (letx[i][j]!=-1) {
      		pole[x-i][y-letx[i][j]]=1;
            pole[x+i][y+letx[i][j]]=1;
      	}

   for (i=y-7;i<y+8;i++) {
   		pole[39][i]=1;
         pole[47][i]=1;
         pole[53][i]=1;
     }

    for (i=1;i<80;i++)
      for (j=1;j<50;j++)
      	if (pole[i][j]==1) {
         	gotoxy(i,j);
            cprintf("%c",219);
         }

}

void logo(void) {
   textbackground(MAGENTA);
   clrscr();
   textcolor(LIGHTGREEN);
	nadpis(19,25);
   int xt=28, yt=21, post=1;
   textcolor(15);
   do {
   	if (kbhit() && getch()==' ') break;
      if (post==1) {xt++;yt++;}
      if (post==2) {xt++;yt--;}
      if (post==3) {xt--;yt--;}
      if (post==4) {xt--;yt++;}

      post=if_hit_how_change_dir(xt,yt,post);

      gotoxy(xt,yt);
      cprintf("*");
      for(float z=-2000000;z<2000000;z+=zab);
      gotoxy(xt,yt);
      cprintf(" ");

   }
   while (1);

}

void Top4eInit(void) {
	int i;
   randomize();
   for (i=0;i<brt;i++) {
   	top4e[i].x=rand()%78+2;
      top4e[i].y=rand()%45+2;
      top4e[i].dir=rand()%4+1;
   }
}

int UdarInRed(int x,int y) {
    int imaudar=0;
    if (pole[x][y+1]==2) imaudar=1;
    if (pole[x][y-1]==2) imaudar=1;
    if (pole[x-1][y+1]==2) imaudar=1;
    if (pole[x+1][y+1]==2) imaudar=1;
    if (pole[x-1][y-1]==2) imaudar=1;
    if (pole[x+1][y-1]==2) imaudar=1;
    if (pole[x-1][y]==2) imaudar=1;
    if (pole[x+1][y]==2) imaudar=1;
    return imaudar;
}

void zapylvane2() {     //namirane na granicite
  int x,y,index,pos,i,flag;
  
   x=redtr[0].x;y=redtr[0].y;pos=1;index=-1;

   do   {

      flag=0;
   	if (flag==0 && pos==1 && pole[x][y]!=0 && pole[x][y+1]!=0 && pole[x+1][y+1]==0) {pos=2;flag=1;}
      if (flag==0 && pos==1 && pole[x][y]!=0 && pole[x][y-1]!=0 && pole[x-1][y-1]==0) {pos=4;flag=1;}
      if (pos==1 && pole[x][y]!=0 && pole[x][y-1]==0) {
      	index++;
         bndry[index].x=x;
         bndry[index].y=y;
         bndry[index].vtr=4;
         flag=1;
      }
      if (flag==0 && pos==2 && pole[x][y]!=0 && pole[x+1][y]!=0 && pole[x+1][y-1]==0) {pos=1;flag=1;}
      if (flag==0 && pos==2 && pole[x][y]!=0 && pole[x-1][y]!=0 && pole[x][y+1]==0) {pos=3;flag=1;}
      if (pos==2 && pole[x][y]!=0 && pole[x+1][y]==0) {
      	index++;
         bndry[index].x=x;
         bndry[index].y=y;
         bndry[index].vtr=1;
         flag=1;
      }
      if (flag==0 && pos==3 && pole[x][y]!=0 && pole[x][y+1]!=0 && pole[x+1][y+1]==0) {pos=2;flag=1;}
      if (flag==0 && pos==3 && pole[x][y]!=0 && pole[x][y-1]!=0 && pole[x-1][y]==0) {pos=4;flag=1;}
      if (pos==3 && pole[x][y]!=0 && pole[x][y+1]==0) {
      	index++;
         bndry[index].x=x;
         bndry[index].y=y;
         bndry[index].vtr=2;
      }


      if (flag==0 && pos==4 && pole[x][y]!=0 && pole[x-1][y]!=0 && pole[x-1][y+1]==0) {pos=3;flag=1;}
      if (flag==0 && pos==4 && pole[x][y]!=0 && pole[x+1][y]!=0 && pole[x][y-1]==0) {pos=1;flag=1;}
      if (pos==4 && pole[x][y]!=0 && pole[x-1][y]==0) {
      	index++;
         bndry[index].x=x;
         bndry[index].y=y;
         bndry[index].vtr=3;
         flag=1;
      }

      if (pos==1) x++;
      if (pos==2) y++;
      if (pos==3) x--;
      if (pos==4) y--;

      if  (index>1)
         if ((bndry[index-1].x==bndry[index-2].x) && (bndry[index].x!=bndry[index-1].x)) {
            bndry[index-1].vtr=bndry[index-2].vtr;
            for(double z=-20000;z<200000;z+=0.24);
         }

      if (index>=indred && bndry[index].x==bndry[0].x && bndry[index].y==bndry[0].y) break;
      int stop=0;
      for (i=0;i<index;i++)
      	if (bndry[i].x==x && bndry[i].y==y) {stop=1;break;}
      if (index>=indred && stop==1) break;

      for(double z=-20000;z<20000;z+=0.24);
   }
   while (1);

   textcolor(WHITE);
   for (i=0;i<=index; i++) {
      pole[bndry[i].x][bndry[i].y]=bndry[i].vtr+2;
      //gotoxy(bndry[i].x,bndry[i].y);
      //cprintf("%d",bndry[i].vtr+2);
   }
   	for(double z=-2000000;z<2000000;z+=0.24);

   int start;

  textcolor(RED);
  flgnozap=0;
  for(y=1;y<50;y++)  {

      start=0;

      for(x=1;x<80;x++)  {
         if (pole[x][y]==3 && start==0) { start=1; continue;}
         if (pole[x][y]==5 && start==1) { start=0; }
         if (start==1) {
         	pole[x][y]=7; //gotoxy(x,y); cprintf("7");
         }
         for(double z=-20000;z<20000;z++);

         for (i=0;i<brt;i++) {
         		if (pole[top4e[i].x][top4e[i].y]==7) {
                   flgnozap=1;
               }
         }

   	}

  }

}

void init_pole(char walcol,char frcol);

main(){
char i,j,xi,yi,ch,posi,fff,frcol=BLUE,walcol=GREEN,lives=5,colori=GREEN,level=1;
float percent=6.48;
int brzapylvane;
long score=0;
textmode(C4350);

    _setcursortype(_NOCURSOR);
    logo();

   clrscr();

    init_pole(walcol,frcol);

     Top4eInit();
     xi=40;yi=1;posi=0;
     fff=0;
       do{
	  if (kbhit()) {
	  ch=getch();
	  if (ch=='q' || ch=='Q') break;
	  if (ch=='m' || ch=='M')
     		if ((fff==1 && posi!=2) || fff==0) posi=4;
	  if (ch=='j' || ch=='J')
     		if ((fff==1 && posi!=1) || fff==0) posi=3;
	  if (ch=='k' || ch=='K')
     		if ((fff==1 && posi!=3) || fff==0) posi=1;
	  if (ch=='i' || ch=='I')
     		if ((fff==1 && posi!=4) || fff==0) posi=2;
     if (ch=='L' || ch=='l')  posi=0;  
	  //if (ch=='5') posi=0;
       }
       if (pole[xi][yi]==0 && fff==0) {    // imame napuskane na bezopasnoto pole
       			fff=1;colori=RED;
               gotoxy(xi,yi);
       			textcolor(colori);
               cprintf("%c",219);
               indred=0;
               redtr[indred].x=xi;
               redtr[indred].y=yi;
       }

       if (pole[xi][yi]==1 && fff==1) {   // imame otnovo vry6tane vbezopasnoto pole
		posi=0;fff=0;ungetch(0);

      for (i=0;i<brt;i++) {
       textcolor(YELLOW);
       textbackground(frcol);
			gotoxy(top4e[i].x,top4e[i].y);
			cprintf("*");
     	}
      zapylvane2();              // zapylvane
      //do { if (kbhit() && getch()==' ') break;} while (1);
      colori=GREEN;
      textcolor(colori);

      if (flgnozap==0)
		 for (i=1;i<81;i++)
		   for (j=1;j<50;j++) {
		   	if (pole[i][j]>=2) {
         		pole[i][j]=1;
            	gotoxy(i,j);

            	cprintf("%c",219);
         	}
         	//if (pole[i][j]==1) brzapylvane++;
         }
       else  { // flgnozap==1;
         textcolor(GREEN);
         for (i=0;i<indred+1;i++) {
               pole[redtr[i].x][redtr[i].y]=1;
         		gotoxy(redtr[i].x,redtr[i].y);
         		cprintf("%c",219);
         }
         textcolor(BLUE);
         for (i=1;i<81;i++)
		   for (j=1;j<50;j++)    {
		   	if (pole[i][j]==7) {
            		pole[i][j]=0;
                  gotoxy(i,j);
         			cprintf("%c",219);
            }
            if (pole[i][j]>2 && pole[i][j]<7) {
            		pole[i][j]=1;
            }
         }   
        }
       //percent=(float)brzapylvane*10/392;
	}
     brzapylvane=0;
     for (i=1;i<81;i++)
		   for (j=1;j<50;j++)
            if (pole[i][j]==1) brzapylvane++;

     percent=(float)brzapylvane*10/392;

     if (percent>=75) {
         score=score+brzapylvane;
         gotoxy(52,50);
			cprintf("\%%=%2.2f  ",percent);
         gotoxy(63,50);
			cprintf("score=%07d  ",score);
     		for(double z=-20000000;z<20000000;z+=0.5);
     		brt++;
         init_pole(walcol,frcol);
         percent=6.48;
         Top4eInit();
         level++;
         xi=40;yi=1;posi=0;

     }

	  if (fff==1) {
     		pole[xi][yi]=2;
         indred++;
         redtr[indred].x=xi;
         redtr[indred].y=yi;
     }
	  
	  switch (posi) {
		case 1: xi++;break;
		case 2: yi--;break;
		case 3: xi--;break;
		case 4: yi++;break;
	  }
	  if (xi==0) {xi++; posi=0;}
	  if (yi==0) {yi++; posi=0;}
	  if (xi==81) {xi=80; posi=0;}
	  if (yi==50) {yi=49; posi=0;}


       if (lives==0) break;
       gotoxy(xi,yi);
       textbackground(colori);
       textcolor(BLACK);

       cprintf("%c",219);

     for (i=0;i<brt;i++) {
       textcolor(YELLOW);
       textbackground(frcol);
			gotoxy(top4e[i].x,top4e[i].y);
			cprintf("*");
      top4e[i].dir=if_hit_how_change_dir(top4e[i].x,top4e[i].y,top4e[i].dir);
     }


       textcolor(15);

	for(float z=-2000000;z<2000000;z+=zab);

   for (i=0;i<brt;i++) {
       if (UdarInRed(top4e[i].x,top4e[i].y)==1) {
         textcolor(BLUE);
       	for(double z=-2000000;z<5000000;z+=0.24);
	    	for (i=0;i<indred;i++) {
		   		pole[redtr[i].x][redtr[i].y]=0;
               gotoxy(redtr[i].x,redtr[i].y);
               cprintf("%c",219);
         }
        	gotoxy(xi,yi);
         cprintf("%c",219);
	    	xi=40;yi=1;posi=0;ungetch(0);lives--;
         fff=0;
         colori=GREEN;
       }

   }

		 gotoxy(xi,yi);
       textbackground(colori);
       cprintf(" ");
       textbackground(frcol);

       for (i=0;i<brt;i++) {
       	gotoxy(top4e[i].x,top4e[i].y);
			cprintf(" ");
     }

    for (i=0;i<brt;i++) {
       if(top4e[i].dir==1) {top4e[i].x++;top4e[i].y++;}
       if(top4e[i].dir==2) {top4e[i].x++;top4e[i].y--;}
       if(top4e[i].dir==3) {top4e[i].x--;top4e[i].y--;}
       if(top4e[i].dir==4) {top4e[i].x--;top4e[i].y++;}
    }

       // next 4 rows are for printing info
	textcolor(YELLOW);
	textbackground(MAGENTA);
	gotoxy(27,50);
	cprintf("level=%2d  lives=%2d  ",level,lives);
   gotoxy(52,50);
	cprintf("\%%= %2.2f  ",percent);
   gotoxy(63,50);
	cprintf("score=%07d  ",score+brzapylvane);
	}
	while(1);

	gotoxy(35,25);
	cprintf("GAME OVER!");
	ungetch(0);
	do {
	}
	while (getch()==0);
} 										// krai na maina

void init_pole(char walcol,char frcol) {
  int x,y;
  for(x=0;x<81;x++)
	for(y=0;y<51;y++)
		pole[x][y]=0;
  for(x=1;x<81;x++){
  pole[x][1]=1;
  pole[x][49]=1;
  }
  for(y=1;y<50;y++){
  pole[1][y]=1;
  pole[80][y]=1;}

    textbackground(frcol);
    for(x=1;x<81;x++)
    for(y=1;y<50;y++)
     if(pole[x][y]==1)	{
		textcolor(walcol);
		gotoxy(x,y);
		cprintf("%c",(char)219);
     }
     else  {
		textcolor(frcol);
		gotoxy(x,y);
		cprintf("%c",(char)219);
     }
}
int if_hit_how_change_dir(int x, int y,int dir) {
     int flag=0;
       if (flag==0 && dir==1 && pole[x][y+1]==1 && pole[x+1][y+1]==1 && pole[x+1][y]==1)
	       dir=3;
       if (flag==0 && dir==2 && pole[x][y-1]==1 && pole[x+1][y-1]==1 && pole[x+1][y]==1)
	       dir=4;
       if (flag==0 && dir==3 && pole[x][y-1]==1 && pole[x-1][y-1]==1 && pole[x-1][y]==1)
	       dir=1;
       if (flag==0 && dir==4 && pole[x-1][y]==1 && pole[x-1][y+1]==1 && pole[x][y+1]==1)
	       dir=2;

       if(flag==0 && dir==1 && pole[x][y+1]==1) {dir=2;flag=1;}
       if(flag==0 && dir==1 && pole[x+1][y]==1) {dir=4;flag=1;}
       if(flag==0 && dir==2 && pole[x+1][y]==1) {dir=3;flag=1;}
       if(flag==0 && dir==2 && pole[x][y-1]==1) {dir=1;flag=1;}
       if(flag==0 && dir==3 && pole[x][y-1]==1) {dir=4;flag=1;}
       if(flag==0 && dir==3 && pole[x-1][y]==1) {dir=2;flag=1;}
       if(flag==0 && dir==4 && pole[x-1][y]==1) {dir=1;flag=1;}
       if(flag==0 && dir==4 && pole[x][y+1]==1) {dir=3;flag=1;}

       if(flag==0 && dir==1 && pole[x+1][y+1]==1) {dir=3;flag=1;}
       if(flag==0 && dir==2 && pole[x+1][y-1]==1) {dir=4;flag=1;}
       if(flag==0 && dir==3 && pole[x-1][y-1]==1) {dir=1;flag=1;}
       if(flag==0 && dir==4 && pole[x-1][y+1]==1) {dir=2;flag=1;}
  return dir;

}

