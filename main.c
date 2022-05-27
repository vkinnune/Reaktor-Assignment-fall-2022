#include "poetry.h"
#include <stdio.h>

#define READ_SIZE 64

void	crash(char *str)
{
    printf("%s\n", str);
    exit(-1);
}

char	*reading(int fd)
{
	char	*buf = 0;
	size_t	read_bytes = 0;
	size_t	text_size = 0;
	size_t	allocated = READ_SIZE;
	buf = (char *)malloc(sizeof(char) * READ_SIZE);
	while (1)
	{
		read_bytes = read(fd, buf + text_size, READ_SIZE);
		if (read_bytes == 0) //EOF
			break ;
		if (read_bytes == -1)
			crash("Error");
		text_size += read_bytes;
		if (text_size + READ_SIZE > allocated)
		{
			allocated += READ_SIZE;
			buf = realloc(buf, allocated);
		}
	}
	return (buf);
}

int	main(int argc, char **argv)
{
	char	*buf;
	int	fd;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		buf = reading(fd);
		if (buf == 0)
			crash("Error");
	//	printf("%s\n", buf);
		parsing();
		/*
		magic();
		print_out();
		*/
	}
	else
		printf("No input\n");
	return (0);
}

