#include "includes/fdf.h"
#include "includes/mlx.h"
#include <math.h>
#include <sys/fcntl.h>

// setup
typedef struct s_img
{
	// default
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

	// custom
	int		row;
	int		col;
	int		box_size;
	int		init_x;
	int		init_y;
}	t_img;

// first part == input
typedef struct s_var
{
	void	*win;
	void	*mlx;
}	t_var;

typedef struct s_node
{
	int **num_arr;
	struct s_node	*next;
}	t_node;
// status :: not use yet
// int	ft_is_space(char c)
// {
// 	if (c == ' ' || (c >= '\t' && c <= '\r'))
// 		return (1);
// 	return (0);
// }


int	check_valid_input(char *s)
{
	while (*s)
	{
		if (*s == ' ' || (*s >= '0' && *s <= '9') || *s == '\n')
			s++;
		else if (*s == '-')
		{
			if (s[1] && (s[1] >= '0' && s[1] <= '9'))
				s++;
			else
				break;
		}
		else
			break;
	}
	if (*s)
		return (0);
	return (1);
}

void	*ft_new_list(int **content)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	new->num_arr = content;
	return (new);
}


void	ft_new_list_add_back(int **content, t_node **node)
{
	t_node	*temp;
	if (*node == NULL)
	{
		*node = ft_new_list(content);
		return ;
	}
	temp = *node;
	while (temp -> next)
		temp = temp -> next;
	temp->next = ft_new_list(content);
}

int	ft_make_map(char **arr, t_node **node)
{
	int	i;
	int	j;
	int	max;
	int	**num_arr;

	printf("ft_make_map\n");
	i = 0;
	j = 0;
	while (arr[i])
		i++;
	num_arr = (int **)malloc(sizeof(int *) * (i + 1));
	while (i > j)
	{
		// if (arr[i][0] == 0)
		// {
		// 	printf("hello bith\n");
		// 	return (0);
		// }
		num_arr[j] = (int *)malloc(sizeof(int));
		num_arr[j][0] = ft_atoi(arr[j]);
		printf("==%i==\n", num_arr[j][0]);
		j++;
	}
	num_arr[j] = NULL;
	printf("j == %i\n", j);
	ft_new_list_add_back(num_arr, node);

	//:HACK:free logic
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
	//
	
	return (j);
}

void	add_init(t_img **img, int row, int col)
{
	(*img)->row = row + 1;
	(*img)->col = col;
}

int	get_input(char *s, t_node **head, t_img *img, int fd)
{
	int		checker;
	int		num;
	int		count;
	char	*temp;

	if (fd == -1)
		return (0);
	checker = 0;
	count = 0;
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp)
			break;
		if (!check_valid_input(temp))
			return (0); // need to free :TODO:
		num = ft_make_map(ft_split(temp, ' '), head);
		if (checker == 0)
			checker = num;
		if (num != checker)
		{
			printf("error ja");
			return (0); // need to fre?? :TODO:
		}
		free(temp);
		count++;
	}
	//close(fd);
	add_init(&img, num, count);
	printf("hello\n");
	return (1);
}

// calc init_point :BUG:

// void	calc_init_draw(t_img *img)
// {
// 	int	box_size;
// 	int	temp;
//
// 	box_size = 1;
// 	while (600 > (img->col + img->row) * (double)box_size / 2)
// 		box_size++;
// 	box_size -= 2;
// 	img->box_size = box_size;
// 	img->init_x = 800 + (int)(img->col * ((double)box_size / 2) * sqrt(3) - img->row * ((double)box_size/2) * sqrt(3));
// 	img->init_y = 300;
// 	printf("%i\n" ,img->init_x);
// 	printf("%i\n" ,img->row);
// 	printf("%i\n" ,img->col);
// 	printf("%i\n" ,img->box_size);
// }
//
// // for put
 void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

// for drawing :BUG:
// typedef	struct s_arg
// {
// 	int	row;
// 	int	col;
// }	t_arg;
//
// typedef	struct	s_data
// {
// 	int	row;
// 	int	col;
// 	int	start_x;
// 	int	start_y;
// 	int	end_x;
// 	int	end_y;
// }				t_data;
//
// t_data	calc_pos(t_arg curr, t_img *img, int prev_dent, int dent)
// {
// 	t_data	res;
// 	double diff_x;
// 	double diff_y;
//
// 	diff_x = ((double)img->box_size / 2) * sqrt(3);
// 	diff_y = (double)img->box_size / 2;
// 	res.col = curr.col;
// 	res.row = curr.row;
// 	res.end_x = img->init_x + (curr.row * diff_x) - curr.col * diff_x;
// 	res.start_x = img->init_x + ((curr.row + 1) * diff_x) - curr.col * diff_x;
// 	res.end_y = img->init_y + (curr.row + curr.col + dent) * diff_y;
// 	res.start_y = img->init_y + (curr.row + curr.col + prev_dent - 1) * diff_y;
// 	printf("___%i \n %i\n ___\n",res.end_x, res.end_y );
// 	return (res);
// }
//
// #define FIXED_SCALE 16
//
// typedef	struct	s_calc
// {
// 	int	dif_x;
// 	int	dif_y;
// 	int	steps;
// 	int	x_inc;
// 	int	y_inc;
// }				t_calc;
//
// void	draw_point_to_point(t_img **img, t_data curr)
// {
// 	int		i;
// 	t_calc	c;
// 	int		x;
// 	int		y;
//
// 	c.dif_x = (curr.end_x - curr.start_x) << FIXED_SCALE;
// 	c.dif_y = (curr.end_y - curr.start_y) << FIXED_SCALE;
//     if (abs(c.dif_x) > abs(c.dif_y))
//         c.steps = abs(c.dif_x >> FIXED_SCALE);
//     else
//         c.steps = abs(c.dif_y >> FIXED_SCALE);
//     c.x_inc = c.dif_x / c.steps;
//     c.y_inc = c.dif_y / c.steps;
//     x = curr.start_x << FIXED_SCALE;
//     y = curr.start_y << FIXED_SCALE;
//     my_mlx_pixel_put(*img,  x >> FIXED_SCALE, y >> FIXED_SCALE, 0xFF0000); // Convert back to integer
// 	i = 0;
// 	while(i < c.steps)
// 	{
//         x += c.x_inc;
//         y += c.y_inc;
//         my_mlx_pixel_put(*img, x >> FIXED_SCALE, y >> FIXED_SCALE, 0xFF0000); // Convert back to integer
// 		i++;
//     }
// }
//
// void	draw_from_prev_row(t_img **img, int **curr_set,int **prev_set ,int current_col)
// {
// 	int	i = 0;
// 	while (prev_set[i])
// 	{
// 		draw_point_to_point(img, calc_pos((t_arg){i, current_col},*img, prev_set[i][0], curr_set[i][0]));
// 		i++;
// 	}
// }
//
// void	draw_map_to_img(t_node *head,t_img *img)
// {
// 	int	i;
// 	int	j;
// 	int	prev;
// 	int	**previous_set;
//
// 	previous_set = NULL;
// 	j = 0;
// 	while (head)
// 	{
// 		i = 0;
// 		while (head->num_arr[i])
// 		{
// 			if (i)
// 				draw_point_to_point(&img, calc_pos((t_arg){i, j}, img, prev, head->num_arr[i][0]));
// 			prev = head->num_arr[i][0];
// 			i++;
// 		}
// 		if (previous_set)
// 			draw_from_prev_row(&img, head->num_arr, previous_set, j);
// 		previous_set = head->num_arr;
// 		head = head -> next;
// 		j++;
// 	}
// }
//  ======================================


// debug drawing :BUG:


void draw_circle(t_img **img, int center, int radius, int color)
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
					*img, x, y, 0xFF0000);
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
				my_mlx_pixel_put(*img, x, y, 0x003412);
			x++;
		}
		y++;
	}
}

void	draw_d(t_img **img)
{
	draw_circle(img, 400, 400, 0xFF0000);
}

void	draw_c(t_img *img)
{
	draw_d(&img);
}
// win : 1600 - 1000
// half : 800 - 500
// usaged : width  while box_size < 600 
int	main(int ac, char *av[])
{
	t_img img;
	t_var var;
	t_node *map;

	map = NULL;
	if (ac != 2)
		return (0);
	//if (!get_input(av[1]))
	//	return (0);
	if (!get_input(av[1], &map, &img, open(av[1], O_RDONLY)))
		return (2);
	// === start mlx and img.img
	// default setup ::	
	// var.mlx = mlx_init();
	// var.win = mlx_new_window(var.mlx, 1640, 1080, "fdf"); // max 1680, 1020
	// img.img = mlx_new_image(var.mlx, 1630, 1070);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	// 							 &img.endian);
	// debugg :BUG:
//	draw_c(&img);
	
	// === calculate init x and init y;
	//calc_init_draw(&img);	
	// === drawing to img.img
	//	draw_map_to_img(map, &img);
	// === put drawing to win
//	mlx_put_image_to_window(var.mlx, var.win, img.img, 0, 0);
	
	// === set hook :TODO:
	//mlx_hook(var.win, 2, 0, win_close, &vars);
	// === set loop;
//	mlx_loop(var.mlx); 
	// usaged
	// teest :HACK:
	printf("\n%d", map->num_arr[0][0]);
	printf("\n______\n");
	t_node *temp = map;
	int	i;
	while (map && map->num_arr)
	{
		i = 0;
		while(map->num_arr[i])
		{
			printf("\n%d", map->num_arr[i][0]);
			i++;
		}
		map = map -> next;
		printf("\n3333333333");
	}

	//free :HACK:
	while (temp)
	{
		t_node *free_time = temp;
		temp = temp->next;
		int i = 0;
		while(free_time->num_arr[i])
		{
			free(free_time->num_arr[i]);
			i++;
		}
		free(free_time->num_arr[i]);
		free(free_time->num_arr);
		free(free_time);
		free_time = NULL;
	}
	//

	// test

	// while (1)
	// {
	// 	s = get_next_line(fd);
	// //	char **s_arr = ft_split(s, ' ');
	// //	int i = ft_atoi(s_arr[0]);
	// 	if (!s)
	// 		break;
	// 	printf("%s", s);
	// 	printf("%i\n", i);
	// 	i++;
	// }
	printf("-end-");
	return (0);
}
