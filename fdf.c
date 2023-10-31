#include "includes/fdf.h"
#include <math.h>
#include <sys/fcntl.h>

// setup
typedef struct s_img
{
	void	*img;
	char	*addr;

	int		box_size;
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

	printf("ft_make_map");
	i = 0;
	j = 0;
	while (arr[i])
		i++;
	num_arr = (int **)malloc(sizeof(int *) * i);
	while (i != j)
	{
		if (!arr[j][0])
		{
			printf("error map in ft_make_map\n");
			return (0); //TODO: need to handle free malloc if error input
		}
		num_arr[j] = (int *)malloc(sizeof(int));
		num_arr[j][0] = ft_atoi(arr[j]);
		printf("==%i==\n", num_arr[j][0]);
		j++;
	}
	ft_new_list_add_back(num_arr, node);

	//:HACK:free logic
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	//
	
	return (j - 1);
}

int	get_input(char *s, t_node **head, t_img *img)
{
	int		fd;
	int		checker;
	int		num;
	char	*temp;

	temp = s;
	checker = 0;
	fd = open(s, O_RDONLY);
	if (fd == -1)
		return (0);
	while (1)
	{
		write(1,"\ninput\n", 5);
		if (temp != s)
		{
			printf("%s", temp);
			free(temp);
		}
		temp = get_next_line(fd);
		if (!temp)
			break;
		if (!check_valid_input(temp))
			return (0);
		num = ft_make_map(ft_split(temp, ' '), head);
		if (checker == 0)
			checker = num;
		if (num != checker)
		{
			printf("error for making map");
			return (0);
		}
	}
	return (1);
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
	int fd = open(av[1], O_RDONLY);
	char *s;
	int	 i = 0;
	if (!get_input(av[1], &map, &img))
		return (2);

	// teest :HACK:
	printf("\n______\n");
	t_node *temp = map;
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
