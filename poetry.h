#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

typedef struct s_package {
	char	*name;
	size_t	version;
	char	*description;
	char	*category;
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

