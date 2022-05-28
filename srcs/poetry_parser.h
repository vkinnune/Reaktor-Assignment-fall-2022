#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

typedef struct s_package {
	char	*name;
	char	*description;
	bool	optional;
	size_t	*dependencies;
	size_t	dependencies_size;
	size_t	*extras;
	size_t	extras_size;
}	t_package;

enum	e_package_content
{
	name,
	version,
	description,
	category,
	optional,
	python_version
};

char		*reading(int fd);
t_package	*allocate_packages(char *buf, size_t *package_count);
void		parsing(char *buf, size_t *package_count, t_package **packages);
void		crash(char *str);

