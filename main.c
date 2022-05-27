#include "poetry.h"
#include <stdio.h>
#include <string.h>

#define READ_SIZE 64

void	crash(char *str)
{
    printf("%s\n", str);
    exit(-1);
}

char	*reading(int fd)
{
	size_t	read_bytes = 0;
	size_t	text_size = 0;
	size_t	allocated = READ_SIZE;
	char	*buf = (char *)malloc(sizeof(char) * READ_SIZE);
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

t_package	*allocate_packages(char *buf, size_t *package_count)
{
	char		*p = buf;
	size_t		package_counter = 0;
	t_package	*packages = 0;
	while (*p != 0)
	{
		if (!memcmp("[[package]]", p, 11))
			package_counter++;
		p++;
	}
	packages = (t_package *)malloc(sizeof(t_package) * package_counter);
	*package_count = package_counter;
	return (packages);
}

char	*parse_package(char *p, size_t &type)
{
	char 	cmp[6][15] = {"name", "version", "description", "category", "optional", "python-version"};
	size_t	i;
	while (*p != 0)
	{
		for (i = 0; i != 6; i++)
			if (!memcmp(cmp[i], p, strlen(cmp[i])))
				break ;
		if (i < 6)
		{
			while (1)
			{
				if (!memcmp(" = ", p, 3))
					break ;
				p++;
			}
			p = p + 3;
			*type = i;
			return (p);
		}
		p++;
	}
	crash("Error");
	return (0);
}

void	format_data(char *p, size_t type)
{

}

void	parsing(char *buf, size_t package_count, t_package *packages)
{
	char	*p = buf;
	size_t	package_index = 0;
	size_t	type = 0;
	while (*p != 0)
	{
		if (!memcmp("[[package]]", p, 11))
		{
			p = parse_package(p, &type);
			format_data(p, type);
			package_index++;
		}
		else if (!memcmp("[package.extras]", p, 16))
		{

		}
		else if (!memcmp("[package.dependencies]", p, 22))
		{

		}
		p++;
	}
	//printf("%d\n", package_index);
}

/*
   Parsing fills this following struct.
   Package struct includes:
	char	*name;
	size_t	version;
	char	*description;
	char	*category;
	bool	optional;
   */

int	main(int argc, char **argv)
{
	char		*buf;
	int		fd;
	t_package	*packages;
	size_t		package_count = 0;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		buf = reading(fd);
		if (buf == 0)
			crash("Error");
		packages = allocate_packages(buf, &package_count);
		parsing(buf, package_count, packages);
		printf("%d\n", package_count);
		/*
		magic();
		print_out();
		*/
	}
	else
		printf("No input\n");
	return (0);
}

