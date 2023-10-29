#include "includes/fdf.h"
// test push

typedef struct s_data
{
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_data;

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void draw_square(void *mlx, int center, int range, int color)
{
	int x_start;
	int x_end;
	int y_start;
	int y_end;
	int temp;

	y_start = center - range;
	y_end = center + range;
	temp = y_start;

	while (y_start <= y_end)
	{
		x_start = center - range;
		x_end = center + range;
		while (x_start <= x_end)
		{
			if (y_start <= y_end && x_start <= x_end)
				my_mlx_pixel_put(mlx, x_start, y_start,
								 color);
			x_start++;
		}
		y_start++;
	}
	return;
}

void draw_circle(void *img, int center, int radius, int color)
{
	int x;
	int y;

	y = center - radius;
	while (y <= center + radius)
	{
		x = center - radius;
		while (x <= center + radius)
		{
			if (radius * radius >=
				(center - x) * (center - x) + (center - y) * (center - y))
				my_mlx_pixel_put(
					img, x, y,
					color);
			x++;
		}
		y++;
	}
	y = center - 30;
	while (y <= center)
	{
		x = center - radius;
		while (x <= center + radius)
		{
			if (x % 2 == 0)
				my_mlx_pixel_put(img, x, y, color);
			x++;
		}
		y++;
	}
}

typedef struct s_vars
{
	void *mlx;
	void *win;
} t_vars;

int win_close(int keycode, t_vars *vars)
{
	if (keycode == 53)
		mlx_destroy_window(vars->mlx, vars->win);
	return (0);
}

int main(int ac, char *av[])
{
	void *mlx;
	t_data img;
	t_vars vars;
	int	x;
	int	y;

	vars.mlx = mlx_init();
	img.bits_per_pixel= 10;
	vars.win = mlx_new_window(vars.mlx, 1640, 1080, "fdf"); // max 1680, 1020
	img.img = mlx_new_image(vars.mlx, 1000, 1000);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								 &img.endian);
	// draw_square(&img, 550, 300, get_rgb(255, 0, 0));
	draw_circle(&img, 400, 400, 0x0000FF);
	draw_square(&img,550, 300, 0xFF0000);
	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_hook(vars.win, 2, 0, win_close, &vars);
	mlx_loop(vars.mlx); 
}

