#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>

int height=58, width=26,foodx,foody,score=0,choice;
const int U=1,D=2,R=3,L=4;
FILE *fp;

typedef struct Snake
{
	int x,y;
	struct Snake * suiv;
}snake;

void red()
{
	printf("\x1b[31m");
}
void blue()
{
	printf("\033[34m");
}
void yellow()
{
	printf("\033[33m");
}
void purple()
{
	printf("\033[35m");
}
void reset()
{
	printf("\033[0m");
}

void pos(int x,int y)
{
	COORD pos;
	HANDLE output;
	pos.X=x;
	pos.Y=y;
	output=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void drawmap()
{
	int i;
	purple();
	for(i=0;i<height;i+=2)
	{
		pos(i,0);
		printf(" - ");
		pos(i,width);
		printf(" - ");
	}
	for(i=1;i<width;i++)
	{
		pos(0,i);
		printf("| ");
		pos(height-2,i);
		printf("  |");
	}	
	reset();
}

void affiche_snake(snake* tete)
{
	pos(tete->x,tete->y);
	yellow();
	printf("o");
	reset();
	snake* p=tete->suiv;
	blue();
	while(p!=NULL)
	{
		pos(p->x,p->y);
		printf("o");
		p=p->suiv;
	}
	reset();
}

void input()
{
	if (GetAsyncKeyState(VK_UP) && choice!=D)
	choice=U;
	else
	{
		if (GetAsyncKeyState(VK_DOWN) && choice!=U)
			choice= D;
		else
		{
			if (GetAsyncKeyState(VK_LEFT) && choice!=R)
				choice= L;
			else
			{
				if (GetAsyncKeyState(VK_RIGHT) && choice!=L)
					choice= R;
			}
		}
	}
}

snake* create_snake()
{
	snake* tete=(snake*)malloc(sizeof(snake));
	tete->x=26;
	tete->y=10;
	snake* p=tete;
	int i;
	for(i=0;i<3;i++)
	{
		p->suiv=(snake*)malloc(sizeof(snake));
		p->suiv->x=p->x -1;
		p->suiv->y=p->y;
		p=p->suiv;
	}
	p->suiv=NULL;
	return tete;
}
void food()
{
	foodx=(rand()+9) % 58+1;
	foody=(rand()+9) % 26+1;
}

void drawfood()
{
	pos(foodx,foody);
	printf("*");
}

int ate(snake* tete)
{
	if(tete->x==foodx && tete->y==foody)
	return 1;
	else 
	return 0;
}

void drawscore()
{
	pos(62,10);
	red();
	printf("SCORE : %d",score);
	reset();
}

void move(snake* tete, int choice)
{
	input();
	Sleep(100);
	system("cls");
	int x=tete->x, y=tete->y;
	switch(choice)
	{
		case U: tete->y-=1;
		break;
		case D: tete->y+=1;
		break;
		case R: tete->x+=1;
		break;
		case L: tete->x-=1;
	}
	snake *cell =(snake*)malloc(sizeof(snake));
	cell->x=x;
	cell->y=y;
	cell->suiv=tete->suiv;
	tete->suiv=cell;
	snake* current=tete, *prec=NULL;
	while(current->suiv !=NULL)
	{
		prec=current;
		current=current->suiv;
	}
	prec->suiv=NULL;
	free(current);
	drawmap();
	drawfood();
	affiche_snake(tete);
}
/*int nbre_elements(snake* tete)
{
	snake*current=tete;
	int nb=0;
	while(current!=NULL)
	{
		nb++;
		current=current->suiv;
	}
	return nb;
}
void move(snake* tete,int choice)
{
	Sleep(100);
	system("cls");
	input();
	snake* newsnake=(snake*)malloc(sizeof(snake));
	newsnake->x=tete->x;
	newsnake->y=tete->y;
	switch(choice)
	{
		case U: tete->y-=1;
		break;
		case D: tete->y+=1;
		break;
		case R: tete->x+=1;
		break;
		case L: tete->x-=1;
	}
	int i;
	for(i=0;i<nbre_elements(tete);i++)
	{
		newsnake->suiv=(snake*)malloc(sizeof(snake));
		newsnake->suiv->x=newsnake->x -1;
		newsnake->suiv->y=newsnake->y;
		newsnake=newsnake->suiv;
	}
	newsnake->suiv=NULL;
	while(nbre_elements(tete))
	{
	snake* current=tete, *prec=NULL;
	while(current->suiv !=NULL)
	{
		prec=current;
		current=current->suiv;
	}
	prec->suiv=NULL;
	free(current);
	}
	free(tete);
	affiche_snake(newsnake);
}*/

void savegame()
{
	char nom[30];
	printf("Donner votre nom: ");
	scanf("%s",nom);
	fp = fopen("scoreboard","a");
    fprintf(fp, "%s : %d\n", nom, score);
    fclose(fp);
    printf("Game saved successfully.");
    system("cls");
}

void leadboard()
{
	char ch;
    system("cls");
    blue();
    printf("LISTE DES SCORES\n");
    reset();
    fp = fopen("scoreboard","r");
    if(fp==NULL){
      printf("Il n'ya pas de liste des scores.");
      return;
    }
    while((ch = fgetc(fp)) != EOF)
      printf("%c", ch);
}

void tail(snake* tete)
{
	snake* current=tete;
	while(current->suiv!=NULL)
	{
		current=current->suiv;
	}
	snake* cell=(snake*)malloc(sizeof(snake));
	cell->suiv=NULL;
	switch(choice)
	{
		case U:
		{
			cell->x=current->x;
			cell->y=current->y -1;
			break;
		}
		case D:
		{
			cell->x=current->x;
			cell->y=current->y +1;
			break;
		}
		case R:
		{
			cell->x=current->x +1;
			cell->y=current->y;
			break;
		}
		case L:
		{
			cell->x=current->x -1;
			cell->y=current->y;
			break;
		}
	}
	current->suiv=cell;
}

void game();
void home();
 
void gameover()
{
	system("cls");
	pos(26,5);
	yellow();
	printf("Game ");
	reset();
	red();
	printf("over!\n");
	reset();
	int c=2;
	while(c==2 || c==3)
	{
	pos(26,7);
	printf("(1)Try Again\n");
	pos(26,9);
	printf("(2)Save Game\n");
	pos(26,11);
	printf("(3)Leadboard\n");
	pos(26,13);
	printf("(4)Menu\n");
	pos(26,15);
	printf("(5)Exit Game\n");
	c=getch()-'0';
	switch(c)
	{
		case 5:
		{
			pos(26,17);
			printf("Exiting...");
			exit(0);
			break;
		}
		case 1:
		{
			system("cls");
			score=0;
			drawmap();
			game();
			break;
		}
		case 4:
		{
			system("cls");
			home();
			break;
		}
		case 3:
		{
			system("cls");
			leadboard();
			break;
		}
		case 2:
		{
			system("cls");
			savegame();
			break;
		}	
	}
	}
}

int collision(snake* tete)
{
	snake* p=tete->suiv;
	while(p!=NULL)
	{
		if(p->x==tete->x && p->y==tete->y)
		return 1;
		p=p->suiv;
	}
	return 0;
}

int alive(snake* tete)
{
	if(tete->x>=58 || tete->x<=0 || tete->y<=0 || tete->y>=26 || collision(tete))
	return 0;
	else
	return 1;
}

void game()
{
	choice=3;
	food();
	drawfood();
	snake *init=create_snake();
	affiche_snake(init);
	while(alive(init))
	{
		drawscore();
		move(init,choice);
		if(ate(init))
		{
			Beep(750, 10);
			score++ ;
			food();
			tail(init);
		}
	}
	gameover();
}

void home()
{
	pos(20,1);
	red();
	printf("******************** Welcome to the  mini snake game *********************\n");
	reset();
	pos(22,3);
	printf("(1) Instructions\n");
	pos(22,5);
	printf("(2) Play Game\n");
	pos(22,7);
	printf("(3) Quit Game");
	int c=getch()- '0';
    switch(c)
    {
    	case 1:
    	{
    		system("cls");
    		int x;
    		pos(26,1);
   			printf("************ Game instructions: *************\n");
   			pos(26,3);
   			printf("\n-> Use arrow keys to move the snake.\n\n-> You will be provided foods at the several coordinates of the screen which you have to eat. Everytime you eat a food the length of the snake will be increased by 1 element and thus the score.\n\n->Your snake die as you hit the wall or snake's body.\n\n-> Enjoy the game! \n");
			pos(26,15);
			printf("(1)Menu\n");
			pos(26,17);
   			printf("(2)Quit Game");	
   			x=getch()-'0';
   			if(x==1)
			{
				system("cls");
				home();
			}
			else
			{
				if(x==2)
				{
					pos(26,19);
					printf("Exiting..!");
					exit(0);
				}
			}
			break;
		}
    	case 2:
		{
			drawmap();
    		game();
    		break;
		}
		case 3:
		{
			pos(22,9);
			printf("\nExiting...");
			exit(0);	
		}
	}
}

int main ()
{
	srand(time(0));
	home();
	return 0;
}
