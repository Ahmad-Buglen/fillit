
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

typedef	struct		s_point  
{
	int				x;
	int				y;
}					s_point;

typedef	struct		s_list
{
	struct	s_point	t[4];
	char			c;
	struct	s_list	*next;
}					t_tetr;

typedef	struct		s_square
{
	char			**mas;
	int				size;
}					s_square;

void print_square(s_square square)
{
	int		i;
	
	i = -1;
	while (++i < square.size)
	{
		write(1, square.mas[i], square.size);
		write(1, "\n", 1);
	}
}

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
	return (count);
}

int fit(s_square square, t_tetr *tetr, int i, int j)
{
	int	n;

	n = 0;
	while (n < 4)
	{
		if (('.' != square.mas[i + tetr->t[n].y][j + tetr->t[n].x]) ||
				(i + tetr->t[n].y >= square.size) || (j + tetr->t[n].x >= square.size))
			return (-20);
		++n;
	}
	return (1);
}

void	set(s_square square, t_tetr *tetr, int i, int j)
{
	int	n;

	n = 0;
	while (n < 4)
	{
		square.mas[i + tetr->t[n].y][j + tetr->t[n].x] = tetr->c;
		++n;
	}
}

void	del(s_square square, t_tetr *tetr, int i, int j)
{
	int	n;

	n = 0;
	while (n < 4)
	{
		square.mas[i + tetr->t[n].y][j + tetr->t[n].x] = '.';
		++n;
	}
}

int recur(s_square square, t_tetr *tetr, t_tetr *tetr_1)
{
	int	i;
	int j;

	if (NULL == tetr)
	{		
	//	write(1, "\ntetr_end\n", 10);
		return (11);
	}
	i = 0;
	while (i < square.size)
	{
		j = 0;
		while (j < square.size)
		{
			if (1 == fit(square, tetr, i, j))
			{
				set(square, tetr, i, j);
	//			print_square(square);
	//			write(1, "\n", 1);
				if (recur(square, tetr->next, tetr_1) != 0)
					return (13);
				else
				{
					del(square, tetr, i, j);
	//				write(1, "\ndel\n", 5);
				}
			}
			++j;
		}
//		if ((i + 1 == square.size) && (j + 1 == square.size) && (tetr != tetr_1))
//		{
//			write(1, "\nfull\n", 6);
//			return (0);
//		}
//		else if ((i + 1 == square.size) && (j + 1 == square.size) && (tetr == tetr_1))
//		{
//			square.size++;
//			return (recur(square, tetr, tetr_1));
//		}
		++i;
	}
	if ((i == square.size) && (j  == square.size) && (tetr != tetr_1))
	{
	//	write(1, "\nfull\n", 6);
		return (0);
	}
	else if ((i == square.size) && (j == square.size) && (tetr == tetr_1))
	{
	//	write(1, "\n+size+\n", 8);
		square.size++;
		return (recur(square, tetr, tetr_1));
	}
	return (12);
}
//*/

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
		++j;
	}
	return (1);
}

void	free_square(char **mas, int i)
{
	int j;

	j = -1;
	while (++j < i)
	{
		free(mas[j]);
		mas[j] = NULL;
	}
	free(mas);
	mas = NULL;
}

char **generate_square()
{
	int		i;
	int		j;
	char	**mas;

	if (!(mas = (char **)malloc(sizeof(char *) * 14)))
		return (NULL);
	i = 0;
	while (i < 13)
	{
		if(!(mas[i] = (char *)malloc(sizeof(char *) * 14)))
		{
			free_square(mas, i);
			return (NULL);
		}
		j = -1;
		while (++j < 14)
			mas[i][j] = '.';
		mas[i][j] = '\0';
		++i;
	}
	mas[i] = NULL;
	return (mas);
}

int min_size(int n)
{
	int	i;

	i = 1;
	while ((i * i) < (n * 4))
		++i;
	return (i);
}

int main(int ac, char **av)
{
	t_tetr			*tetr;
	int				i;
	s_square		square;

	i = ac;	
	if (!(tetr = malloc(sizeof(t_tetr))))
		return (-1);
	tetr->next = NULL;
	i = check_file(av[1], tetr);
	square.size = min_size(i);
	square.mas = generate_square();
//	printf("\n %d ", recur(square, tetr, tetr));
	recur(square, tetr, tetr);
	print_square(square);
	//print_tetr(tetr);
	return (1);
}
