#include "poetry_parser.h"

#define READ_SIZE 64

char	*reading(int fd)
{
	int	read_bytes = 0;
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
			buf = realloc(buf, sizeof(char) * allocated);
		}
	}
	buf[read_bytes + text_size] = 0;
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
	bzero(packages, sizeof(t_package) * package_counter);
	*package_count = package_counter;
	return (packages);
}

char	*parse_package(char *p, size_t *type, size_t order_index)
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
			if (order_index != i)
				crash("Error");
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

void	format_data(char *p, size_t type, t_package *packages)
{
	char	*save_p;
	int	i;

	switch (type)
	{
		case name:
			p++;
			save_p = p;
			p = strchr(p, '"');
			i = p - save_p;
			packages->name = (char *)malloc(sizeof(char) * (i + 1));
			strncpy(packages->name, save_p, i);
			packages->name[i] = 0;
			break;
		case description:
			p++;
			save_p = p;
			p = strchr(p, '"');
			i = p - save_p;
			packages->description = (char *)malloc(sizeof(char) * (i + 1));
			strncpy(packages->description, save_p, i);
			packages->description[i] = 0;
			break;
		case optional:
			if (!memcmp("false", p, 5))
				packages->optional = false;
			else if (!memcmp("true", p, 4))
				packages->optional = true;
			else
				crash("Error");
			break;
	}
}

int	search_package(char *name, size_t len, t_package *packages, size_t package_count)
{
	for (size_t i = 0; i != package_count; i++)
	{
		if (!memcmp(name, packages[i].name, len))
			return (i);
	}
	return (-1);
}

t_package	*add_package(t_package *packages, size_t package_count, size_t len, char *name)
{
	packages = realloc(packages, sizeof(t_package) * (package_count + 1));
	packages[package_count].name = (char *)malloc(sizeof(char) * (len + 1));
	strncpy(packages[package_count].name, name, len);
	packages[package_count].name[len] = 0;
	return (packages);
}

char	*parse_extras(char *p, t_package **packages, size_t package_index, size_t *package_count)
{
	char	*save_p;
	char	*temp_p;
	int	index;
	size_t	len;

	p = strchr(p, '"');
	p++;
	save_p = p;
	temp_p = strchr(p, '"');
	p = memchr(save_p, ' ', temp_p - save_p);
	if (!p)
		p = temp_p;
	len = p - save_p;
	index = search_package(save_p, len, *packages, *package_count);
	if (index == -1)
	{
		*packages = add_package(*packages, *package_count, len, save_p);
		index = *package_count;
		(*package_count)++;
	}
	(*packages)[package_index].extras = realloc((*packages)[package_index].extras, sizeof(size_t) * ((*packages)[package_index].extras_size + 1));
	(*packages)[package_index].extras[(*packages)[package_index].extras_size] = (size_t)index;
	(*packages)[package_index].extras_size++;
	return (temp_p + 1);
}

char	*parse_dependencies(char *p, t_package **packages, size_t package_index, size_t *package_count)
{
	char	*save_p;
	char	*temp_p;
	int	index;
	size_t	len;

	save_p = p;
	temp_p = strchr(p, '\n');
	p = memchr(save_p, ' ', temp_p - save_p);
	if (!p)
		p = temp_p;
	len = p - save_p;
	index = search_package(save_p, len, *packages, *package_count);
	if (index == -1)
	{
		*packages = add_package(*packages, *package_count, len, save_p);
		index = *package_count;
		(*package_count)++;
	}
	(*packages)[package_index].dependencies = realloc((*packages)[package_index].dependencies, sizeof(size_t) * ((*packages)[package_index].dependencies_size + 1));
	(*packages)[package_index].dependencies[(*packages)[package_index].dependencies_size] = (size_t)index;
	(*packages)[package_index].dependencies_size++;
	return (temp_p);
}

void	parsing(char *buf, size_t *package_count, t_package **packages)
{
	char	*p = buf;
	size_t	package_index = 0;
	size_t	type = 0;

	while (*p != 0)
	{
		if (!memcmp("[[package]]", p, 11))
		{
			for (int i = 0; i != 6; i++)
			{
				p = parse_package(p, &type, i);
				format_data(p, type, &(*packages)[package_index]);
			}
			package_index++;
		}
		p++;
	}
	p = buf;
	package_index = 0;
	type = 0;
	while (*p != 0)
	{
		if (!memcmp("[[package]]", p, 11))
			package_index++;
		else if (!memcmp("[package.extras]", p, 16))
		{
			while (*p != 0)
			{
				if (!memcmp("\n\n", p, 2))
					break ;
				p = parse_extras(p, packages, (package_index - 1), package_count);
				p++;
			}
		}
		else if (!memcmp("[package.dependencies]", p, 22))
		{
			p = strchr(p, '\n');
			p++;
			while (*p != 0)
			{
				if (*p == '\n')
					break ;
				p = parse_dependencies(p, packages, (package_index - 1), package_count);
				p++;
			}
		}
		p++;
	}
}

