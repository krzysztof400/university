#include<stdio.h>

void white(int w, char guess[5], char board[1296][5]);
void red(int r, char guess[5], char board[1296][5]);
void gen(char board[1296][5]);


int main()
{
	char board[1296][5];
	char guess[5];
	gen(board);
	int w=0;
	int r=0;
	int i=0;
	while(r<4 && i<1296)
	{
		while(board[i][0] == '\0')
		{
			i++;
		}
		if(i==1296)
		{
			printf("Oszukujesz \n");
			continue;
		}
		for(int j=0; j<4; j++)
		{
			guess[j] = board[i][j];
			printf("%c ", guess[j]);
		}
		printf("\n");
		printf("Czerwone: ");
		scanf("%d", &r);
		printf("   Biale: ");
		scanf("%d", &w);
		printf("\n");

		red(r, guess, board);
		white(w, guess, board);
		board[i][0]='\0';

	}
	if(r==4)
	{
		printf("Wygrałem\n");
	}
	return 0;
}

void white(int w, char guess[5], char board[1296][5])
{
	int l;
	int ind[4];
	for(int i=0; i<1296; i++)
	{
		if(board[i][0]!='\0')
		{
			l=0;
			ind[0]=1;
			ind[1]=1;
			ind[2]=1;
			ind[3]=1;
			for(int j=0; j<4; j++)
			{
				if(guess[j]!=board[i][j])
				{
					for(int k=0; k<4; k++)
					{
						if(board[i][j]==guess[k] && board[i][k]!=guess[k] && ind[k]==1)
						{
							l++;
							ind[k]=0;
							k=4;
						}
					}
				}
			}
			if(l!=w)
			{
				board[i][0]='\0';
			}
		}
	}
}

void red(int r, char guess[5], char board[1296][5])
{
	int l;
	for(int i=0; i<1296; i++)
	{
		if(board[i][0]!='\0')
		{
			l=0;
			for(int j=0; j<4; j++)
			{
				if(board[i][j]==guess[j])
				{
					l++;
				}
			}
			if(l!=r)
			{
				board[i][0] = '\0';
			}
		}
	}
}

void gen(char board[1296][5])
{
	for(int i=1; i<7; i++)
	{
		for(int j=1; j<7; j++)
		{
			for(int k=1; k<7; k++)
			{
			
				for(int l=1; l<7; l++)
				{
					board[(l-1)+(k-1)*6+(j-1)*6*6+(i-1)*6*6*6][0] = '0' + i;
					board[(l-1)+(k-1)*6+(j-1)*6*6+(i-1)*6*6*6][1] = '0' + j;
					board[(l-1)+(k-1)*6+(j-1)*6*6+(i-1)*6*6*6][2] = '0' + k;
					board[(l-1)+(k-1)*6+(j-1)*6*6+(i-1)*6*6*6][3] = '0' + l;
				}
			}
		}
	}
}
