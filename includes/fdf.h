#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include "get_next_line.h"
# include "mlx.h"

char	**ft_split(char const *s, char c);
char	*ft_atoi(char const *s);
char	*get_next_line(int fd);

#endif
