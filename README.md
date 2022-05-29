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
}	t_package;
```
Firstly I calculated the amount of downloaded packages. A definition of a downloaded package is that there is a *package* section. With that I allocated correct amount of data. Then I parsed the *package* sections and filled the data in the structs. After that I parsed all the *package.dependencies* and *package.extras* and filled the integer arrays. I use integer arrays because as I read the packages I just add the new package to the struct array and increase the index. So I can just store of the indexes of the struct array in the int arrays. If there was a new package that was not already found in the struct array I just allocate more memory and increase the index. I know the downloaded package amount so everything above that are optional packages. 

## Usage
Run `make`.

And then with `./poetry-parser poetry.lock > index.html`.

Open the html on your web browser.

The solution is hosted on my Heroku: https://reaktor-assignment-fall-2022.herokuapp.com/
