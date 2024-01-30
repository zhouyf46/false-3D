#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#define D_map_wide 16
#define D_map_high 16
#define D_screen_wide 80
#define D_screen_high 23
#define D_view 45.0
#define D_sight_distance_max 20.0
#define D_P (3.14159/180)

char c_map[D_map_high][D_map_wide],c_screen[D_screen_high][D_screen_wide];

struct 
{
	float f_x,f_y,f_toward;
} str_player;

void v_initializa(void);
void v_display(void);
void v_move(char c_toward);

void v_initializa(void)
{
	// initializa map
	FILE *F_map_file;
	int i_x,i_y;

	memset(c_map,' ',sizeof(c_map));

	if (!(F_map_file = fopen("map.txt","r")))
	{
		printf("you haven't initializan before\n");
		exit(1);
	}

	for (i_y = 0;i_y < D_map_high;i_y++)
	{
		for (i_x = 0;i_x < D_map_wide;i_x++)
		{
			c_map[i_y][i_x] = getc(F_map_file);
		}
		while (getc(F_map_file) != '\n');
	}
	fclose(F_map_file);

	// initializa screen
	memset(c_screen,' ',sizeof(c_screen));

	// initializa player
	str_player.f_x = D_map_wide/2;
	str_player.f_y = D_map_high/2;
	str_player.f_toward = 0.0;
}
void v_display(void)
{
	int i_x,i_y,i_sight_x,i_sight_y,i_ceiling,i_floor;
	float f_distance,f_toward;

	for (i_x = 0;i_x < D_screen_wide;i_x++)
	{
		f_distance = 0.0;
		f_toward = str_player.f_toward - D_view/2 + i_x * D_view/D_screen_wide;
		
		while (1)
		{
			f_distance += 0.1;
			i_sight_x = (int)(str_player.f_x + sinf(f_toward * D_P) * f_distance);
			i_sight_y = (int)(str_player.f_y - cosf(f_toward * D_P) * f_distance);

			if (i_sight_x < 0 || i_sight_x > D_map_wide-1 || i_sight_y < 0 || i_sight_y > D_map_high-1 || f_distance > D_sight_distance_max)
			{
				i_ceiling = D_screen_high/2+1;
				i_floor = D_screen_high-i_ceiling;
				break;
			}
			else if (c_map[i_sight_y][i_sight_x] == '#')
			{
				i_ceiling = (int)((D_screen_high-D_screen_high/f_distance)/2);
				i_floor = D_screen_high-i_ceiling;
				break;
			}
		}

		for (i_y = 0;i_y < D_screen_high;i_y++)
		{
			if (i_y < i_ceiling)
			{
				c_screen[i_y][i_x] = ' ';
			}
			else if (i_y <= i_floor)
			{
				c_screen[i_y][i_x] = '#';
			}
			else
			{
				c_screen[i_y][i_x] = ' ';
			}
		}
	}
	
	for (i_y = 0;i_y < D_map_high && i_y < D_screen_high;i_y++)
	{
		for (i_x = 0;i_x < D_map_wide && i_x < D_screen_wide;i_x++)
		{
			c_screen[i_y][i_x] = c_map[i_y][i_x];
		}
	}
	c_screen[(int)str_player.f_y][(int)str_player.f_x] = 'P';

	for (i_y = 0;i_y < D_screen_high;i_y++)
	{
		for (i_x = 0;i_x < D_screen_wide;i_x++)
		{
			putchar(c_screen[i_y][i_x]);
		}
		putchar('\n');
	}

	fflush(stdout);
}
void v_move(char c_toward)
{
	switch (c_toward)
	{
		case 'w':
			if (c_map[(int)(str_player.f_y - cosf(str_player.f_toward * D_P) * 0.1)][(int)(str_player.f_x + sinf(str_player.f_toward * D_P) * 0.1)] != '#')
			{
				str_player.f_x += sinf(str_player.f_toward * D_P) * 0.1;
				str_player.f_y += -cosf(str_player.f_toward * D_P) * 0.1;
			}
			break;
		case 's':
			if (c_map[(int)(str_player.f_y + cosf(str_player.f_toward * D_P) * 0.1)][(int)(str_player.f_x - sinf(str_player.f_toward * D_P) * 0.1)] != '#')
			{
				str_player.f_x -= sinf(str_player.f_toward * D_P) * 0.1;
				str_player.f_y -= -cosf(str_player.f_toward * D_P) * 0.1;
			}
			break;
		case 'a':
			if (c_map[(int)(str_player.f_y - cosf((str_player.f_toward-90) * D_P) * 0.05)][(int)(str_player.f_x + sinf((str_player.f_toward-90) * D_P) * 0.05)] != '#')
			{
				str_player.f_x += sinf((str_player.f_toward-90) * D_P) * 0.05;
				str_player.f_y += -cosf((str_player.f_toward-90) * D_P) * 0.05;
			}
			break;
		case 'd':
			if (c_map[(int)(str_player.f_y - cosf((str_player.f_toward+90) * D_P) * 0.05)][(int)(str_player.f_x + sinf((str_player.f_toward+90) * D_P) * 0.05)] != '#')
			{
				str_player.f_x += sinf((str_player.f_toward+90) * D_P) * 0.05;
				str_player.f_y += -cosf((str_player.f_toward+90) * D_P) * 0.05;
			}
			break;
		case 'q':
			str_player.f_toward -= 1.8;
			break;
		case 'e':
			str_player.f_toward += 1.8;
			break;
	}
}

int main()
{
	v_initializa();

	while (1)
	{
		v_display();
		v_move(getchar());
	}

	return 0;
}