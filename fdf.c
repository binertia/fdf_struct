#include "includes/fdf.h"
#include <sys/fcntl.h>

typedef struct s_img
{
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;

	// custom
	int	box_size;
	int	row;
	int col;
	int	init_x;
	int x;
	int y;
} t_img;

void my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

typedef struct	s_data
{
	int	x;
	int	y;
	int	z;
}	t_data;

typedef	struct	Node
{
	char	**row;
	struct Node	*next;
}	t_node;

typedef struct s_wd
{
	void	*mlx;
	void	*win;
}	t_wd;
//list func
t_node	*ft_new_t_node(char	**s)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	new -> row = s;
	new -> next = NULL;
	return (new);
}

void	ft_new_list_add_back(t_node **head, char **content)
{
	t_node *temp;

	if (!*head)
		*head = ft_new_t_node(content);
	else
	{
		temp = *head;
		while (temp -> next)
			temp = temp -> next;
		temp -> next = ft_new_t_node(content);
	}
}

// input func
int	is_input_valid(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

// int	is_map_valid(char **s)
// {
// }

// 2. calc function
void	calc_canvas(t_img img, t_node *map)
{
	int	x;
	int	y;
	int	box_size;

	y = 0;
	x = 0;
	box_size = 1;
	while (map->row[x] && map->row[x][0] != '\n')
		x++;
	while (map)
	{
		map = map->next;
		y++;
	}
	while (y * box_size < 600)
		box_size++;
	box_size -= 1;
	while (x * box_size > 1200)
		box_size--; 
	img.box_size = box_size;
}

int	ft_not_valid_input(char *s)
{
	while (*s)
	{
		if (*s == ' ' || *s == '\n' ||(*s >= '0' && *s <= '9'))
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
	return (*s);
}

int	ft_is_space(char c)
{
	if (c == ' ' || c == '\n' || c >='\t' && c <= '\r')
		return (1);
	return (0);
}
int	skip_space(char *s)
{
	int	i;
	i = 0;
	while (s[i] && ft_is_space(s[i]))
		i++;
	return (i);
}

int	get_count(char *s)
{
	int	i;

	i = 0;
	s += skip_space(s);
	while (*s)
	{
		if (ft_is_space(*s))
		{
			s += skip_space(s);
			i++;
		}
		else
			s++;
	}
	return (i);
}

#define INIT_X 800
#define INIT_Y 300

int	ft_not_valid_map(t_img *img, t_node *node)
{
	int	x;
	int	y;
	int	i;

	x = -1;
	y = 0;
	while (node)
	{
		i = 0;
		while (node->row[i] && node->row[i][0] != '\n')
			i++;
		if (x == -1)
			x = i;
		else if (x != i)
			return (0);
		node = node->next;
		y++;
	}
	img->row = x;
	img->col = y;

	if (x > y)
		img->init_x = INIT_X - (((double)(x - y) / (x + y)) * 1600) ;
	else
		img->init_x = INIT_X + (((double)(y - x) / (x + y)) * 1600) ;
	return (0);
}

void	draw_to(t_img *ref ,int end_x, int end_y)
{
	int	i;
	int	j;
	int dif_x;
	int	dif_y;

	dif_y = end_y - ref->y;
	dif_x = end_x - ref->x;
	i = 0;
	while (i != end_y)
	{
		j = 0;
		while (j != end_x)
		{
			if ((double)dif_x/dif_y == (double)j/i)
				my_mlx_pixel_put(ref->img, ref->x + j,ref->y + i, 0xFF0000);
			j++;
		}
		i++;
	}
	ref->x = end_x;
	ref->y = end_y;
}

int	draw_canvas(t_img *img,t_node *node)
{
	int	i;
	int	j;
	int	x;
	int	y;

	i = 0;
	y = INIT_Y;
	while (node)
	{
		j = 0;
		x = img->init_x - (img->box_size * i);
		img-> x = -1;
		img-> y = -1;
		while (node->row[j])
		{
			img->next_x = x + (img->box_size * j);
			img->next_y = 
			ft_atoi(node->row[j]);
		}
		i++;
	}
	i = 0;
	return (0);
}

int	main(int ac, char *av[])
{
	int		fd;
	int		count;
	char	*line;
	t_node	*head;
	t_img	img;
	t_wd	var;

	line = NULL;
	head = NULL;
	if (ac == 2)
	{
		//0.setup static win and init mlx
		var.mlx = mlx_init();
		var.win = mlx_new_window(var.mlx, 1640, 1080, "fdf"); // max 1680, 1020
		img.img = mlx_new_image(var.mlx, 1640, 1080);
		img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								 &img.endian);
		//1 .read file >> get_map >>
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
			return (2);
		line = get_next_line(fd);
		count = get_count(line);
		if (!count)
			return (2);
		while (line)
		{
			if (ft_not_valid_input(line) && count != get_count(line))
				return (2);
			ft_new_list_add_back(&head, ft_split(line, ' '));
			line = get_next_line(fd);
		}
		//1.2 validate and get x & y in img;
		if (ft_not_valid_map(&img, head))
			return (2);
		//2.get_map_size >> calc_drawing_pos (& isometric ?) // set dynamic size of boxsize
		calc_canvas(img, head);
		if (!draw_canvas(&img, head))
			return (2);
		// --- start x = 200; y == 300
		// while (get_next_line(fd, &line) != -1)
		// 	ft_new_list_add_back(head ,ft_split(line, ' '));
		close(fd);
		//put draw do loop TODO: add another keypress hook
		mlx_put_image_to_window(var.mlx, var.win, img.img, 0, 0);
		mlx_loop(var.mlx); 

	}
	return (0);
}
