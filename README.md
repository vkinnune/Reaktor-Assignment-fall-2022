# Reaktor-Assignment-fall-2022

This project was a medium difficulty parsing problem. Our task was to do a small program that takes a poetry.lock file as input and showcases some key information in an HTML user interface.

The program has two parts. Parsing the input and printing out the output. Given I only had two days to complete the project, for the parsing, I made a simplification that the package order was already alphabetically ordered.

I stored the data about a single package in the following struct and made an array of structs.
```
typedef struct s_package {
	char	*name;
	char	*description;
	bool	optional;
	size_t	*dependencies;
	size_t	dependencies_size;
	size_t	*extras;
	size_t	extras_size;
}	t`_package;
```

## Usage
Run `make`.

And then with `./poetry-parser poetry.lock > index.html`.

Open the html on your web browser.

The solution is hosted on my Heroku: https://reaktor-assignment-fall-2022.herokuapp.com/
