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
}	t_package;

