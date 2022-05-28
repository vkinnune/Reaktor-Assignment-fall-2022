#include "poetry_parser.h"

void	crash(char *str)
{
    printf("%s\n", str);
    exit(-1);
}

size_t	clear_duplicates(size_t **real_dependencies, size_t *dependencies, size_t *extras, size_t dependencies_size, size_t extras_size)
{
	size_t	j;
	size_t	real_size = dependencies_size;

	*real_dependencies = (size_t *)malloc(sizeof(size_t) * (dependencies_size + extras_size));
	memcpy(*real_dependencies, dependencies, sizeof(size_t) * dependencies_size);
	for (size_t i = 0; i != extras_size; i++)
	{
		for (j = 0; j != real_size; j++)
			if (extras[i] == (*real_dependencies)[j])
				break ;
		if (j == real_size)
		{
			(*real_dependencies)[real_size] = extras[i];
			real_size++;
		}
	}
	return (real_size);
}

size_t	add_reverse_dependencies(size_t reverse_index, size_t **reverse_dependencies, size_t reverse_size)
{
	size_t i;

	for (i = 0; i != reverse_size; i++)
		if ((*reverse_dependencies)[i] == reverse_index)
			break ;
	if (i == reverse_size)
	{
		*reverse_dependencies = realloc(*reverse_dependencies, sizeof(size_t) * (reverse_size + 1));
		(*reverse_dependencies)[reverse_size] = reverse_index;
		reverse_size++;
	}
	return (reverse_size);
}

size_t	get_reverse_dependencies(size_t **reverse_dependencies, t_package *packages, size_t downloaded_packages, size_t current_index)
{
	size_t	reverse_size = 0;

	for (size_t i = 0; i != downloaded_packages; i++)
	{
		for (size_t j = 0; j != packages[i].dependencies_size; j++)
			if (packages[i].dependencies[j] == current_index)
				reverse_size = add_reverse_dependencies(i, reverse_dependencies, reverse_size);
	}
	for (size_t i = 0; i != downloaded_packages; i++)
	{
		for (size_t j = 0; j != packages[i].extras_size; j++)
			if (packages[i].extras[j] == current_index)
				reverse_size = add_reverse_dependencies(i, reverse_dependencies, reverse_size);
	}
	return (reverse_size);
}

void	print_out(t_package *packages, size_t downloaded_packages, size_t *index_page)
{
	printf("<html><style> a:active { color: red } a { color: blue } body { font-family: Arial; display: flex } div { display: flex; min-width: 10vh } #content { display: flex; flex-wrap: wrap } #sidebar { max-height: 100vh; overflow-y: auto; margin: 10px 10px 10px 10px; min-width: 35vh; position: -webkit-sticky; position: sticky; top: 0; align-self: flex-start; } #text { padding-bottom: 10px; border-bottom: 1px solid black; width: 100%%; flex-direction: column } h2 { } p { margin: initial; } </style><body>");
	{ //generate clickable index in alphabetical order
		printf("<div id=\"sidebar\">");
		printf("<p>");
		for (size_t i = 0; i != downloaded_packages; i++)
		{
			printf("<a href=\"#%s\">%s</a>", packages[index_page[i]].name, packages[index_page[i]].name);
			if (i != downloaded_packages - 1)
				printf(", ");

		}
		printf("</p></div>");
	}
	{ //generate the packages
		printf("<div id=\"content\">");
		for (size_t i = 0; i != downloaded_packages; i++)
		{
			size_t *dependencies = packages[index_page[i]].dependencies;
			size_t *extras = packages[index_page[i]].extras;
			size_t dependencies_size = packages[index_page[i]].dependencies_size;
			size_t extras_size = packages[index_page[i]].extras_size;
			size_t *reverse_dependencies = 0;
			size_t reverse_size = 0;
			size_t *real_dependencies = 0;
			size_t real_size = 0;
			printf("<div id=\"text\"><h2 id=\"%s\">%s</h2><p>%s</p>", packages[index_page[i]].name, packages[index_page[i]].name, packages[index_page[i]].description);
			real_size = clear_duplicates(&real_dependencies, dependencies, extras, dependencies_size, extras_size);
			if (real_size > 0)
			{
				printf("<h4>Dependencies</h4><p>");
				for (size_t j = 0; j != real_size; j++)
				{
					if (real_dependencies[j] < downloaded_packages)
						printf("<a href=\"#%s\">%s</a>, ", packages[real_dependencies[j]].name, packages[real_dependencies[j]].name);
					else
						printf("%s, ", packages[real_dependencies[j]].name);
				}
				printf("</p>");
			}
			reverse_size = get_reverse_dependencies(&reverse_dependencies, packages, downloaded_packages, index_page[i]);
			if (reverse_size)
			{
				printf("<h4>Reverse Dependencies</h4><p>");
				for (size_t j = 0; j != reverse_size; j++)
					printf("<a href=\"#%s\">%s</a>, ", packages[reverse_dependencies[j]].name, packages[reverse_dependencies[j]].name);
				printf("</p>");

			}
			printf("</div>");
		}
		printf("</div>");
	}
	printf("</body></html>");
}

void	create_index_page(size_t *index_page, size_t downloaded_packages)
{
	size_t	i;

	for (i = 0; i != downloaded_packages; i++)
	{
		index_page[i] = i;
	}
}

/*
   Parsing fills this following struct.
   Package struct includes:
	char	*name;
	char	*description;
	bool	optional;
	size_t	*dependencies;
	size_t	dependencies_size;
	size_t	*extras;
	size_t	extras_size;
   */

int	main(int argc, char **argv)
{
	t_package	*packages;
	char		*buf = 0;
	int		fd;
	size_t		package_count = 0;
	size_t		*index_page;
	size_t		downloaded_packages;

	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		buf = reading(fd);
		if (buf == 0)
			crash("Error");
		packages = allocate_packages(buf, &package_count);
		index_page =  (size_t *)malloc(sizeof(size_t) * package_count);
		downloaded_packages = package_count;
		create_index_page(index_page, downloaded_packages);
		parsing(buf, &package_count, &packages);
		print_out(packages, downloaded_packages, index_page);
	}
	else
		printf("poetry-parser: missing input file\n");
	return (0);
}

