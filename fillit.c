
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

struct		s_point  
{
	int				x;
	int				y;
}			s_point;

typedef	struct		s_list
{
	struct	s_point	t[4];
	char			c;
	struct s_list	*next;
}					t_tetr;

struct				s_square
{
	char			**mas;
	int				size;
}					s_square;


int validate(char *buf)
{
	int i;
	int lattice;
	int point;

	i = -1;
	point = 0;
	lattice = 0;
	while (++i < 21)
	{
		if (((i + 1) % 5 != 0) && (buf[i] != '.') && (buf[i] != '#') && (i != 20))
			return (-10);
		else if ((((i + 1) % 5 == 0) && (buf[i] != '\n')) || ((i == 20) && (buf[i] != '\n')))
			return (-11);
		if ('.' == buf[i])
			++point;
		else if ('#' == buf[i])
		{
			if (!(((0 == i) && (('#' == buf[i + 1]) || ('#' == buf[i + 5]))) ||
				(((i > 0) && (i < 4)) && (('#' == buf[i - 1]) || ('#' == buf[i + 1]) || ('#' == buf[i + 5]))) ||
				(((i > 4) && (i < 14)) && (('#' == buf[i -1]) || ('#' == buf[i - 5]) || ('#' == buf[i + 1]) || ('#' == buf[i + 5]))) ||
				(((i > 14) && (i < 18)) && (('#' == buf[i - 1]) || ('#' == buf[i - 5]) || ('#' == buf[i + 1]))) ||
				((18 == i) && (('#' == buf[i - 1]) || ('#' == buf[i - 5])))))
				return (-12);
			++lattice;
		}
	}
	if ((point != 12) || (lattice != 4))
		return (-13);
	return (1);
}

int record_tetr(char *buf, t_tetr *tetr, int count)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (i < 19)
	{
		if ('#' == buf[i])
		{
			tetr->t[j].x = i % 5;
			tetr->t[j].y = i / 5;
			++j;
		}
		++i;
	}
	i = 0;
	j = 3;
	k = 3;
	while (i < 4)
	{
		j = (j < tetr->t[i].x) ? j : tetr->t[i].x;
		k = (k < tetr->t[i].y) ? k : tetr->t[i].y;
		++i;
	}
	i = 0;
	while (i < 4)
	{
		tetr->t[i].x -= j;
		tetr->t[i].y -= k;
		++i;
	}
	tetr->c = 'A' + count;
	return (1);
}

int check_file(char *file_name, t_tetr *tetr)
{
	int		fd;
	char	buf[22];
	int		count;
	int		ret;

	if ((fd = open(file_name, O_RDONLY)) == -1)
		return (-1);
	
	count = 0;
	while (21 == (ret = read(fd, buf, 21)))
		if (1 != validate(buf))
			return (validate(buf));
		else 
		{
			if (0 == count)
			{
				record_tetr(buf, tetr, count);
				tetr->next = NULL;
			}
			else
			{
				if (!(tetr->next = malloc(sizeof(t_tetr))))
					return (-2);
				tetr = tetr->next;
				record_tetr(buf, tetr, count);
				tetr->next = NULL;
			}
			++count;
		}
	if ((count > 25) || (close(fd) == -1))
		return (-3);
	return (1);
}
/*
int fit(struct s_square square, t_tetr *tetr, int i, int j)
{
	return (1);
}

int set(struct s_square square, t_tetr *tetr, int i, int j)
{
	return (1);
}

int del(struct s_square square, t_tetr *tetr, int i, int j)
{
	return (1);
}

int recur(struct s_square square, t_tetr *tetr)
{
	int	i;
	int j;

	if (NULL == tetr)
		return (1);
	i = 0;
	while (i < square.size)
	{
		j = 0;
		while (j < square.size)
		{
			if (fit(square, tetr, i, j))
			{
				set(square, tetr, i, j);
				if (recur(square, tetr->next))
					return (1);
				else
					del(square, tetr, i, j)
			}
			++j;
		}
		if ((i + 1 == square.size) && (j + 1 == square.size) && (tetr != tetr_1))
			return (0);
		else if ((i + 1 == square.size) && (j + 1 == square.size) && (tetr == tetr_1))
		{
			square.size++;
			return (recur(square, tetr));
		}
		++i;
	}
}
*/

int print_tetr(t_tetr *tetr)
{
	int	i;
	int	j;

	j = 0;
	while (tetr)
	{
		i = -1;
		printf("tetr %d \n", j);
		while (++i < 4)
			printf("x%d = %d, y%d = %d \n", i, tetr->t[i].x, i, tetr->t[i].y);
		{
		++j;
	}
	return (1);
}

int main(int ac, char **av)
{
	t_tetr	*tetr;
	int		i;

	i = ac;	
	if (!(tetr = malloc(sizeof(t_tetr))))
		return (-1);
	tetr->next = NULL;
	printf("%d finish", check_file(av[1], tetr));
	print_tetr(tetr);
	return (1);
}
