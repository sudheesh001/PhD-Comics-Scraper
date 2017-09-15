# PhD Comics Scraper
An attempt to scrape PhD comics website and download the comics using C++. Using libcurl and standard C++ libraries.

![Image](http://phdcomics.com/comics/archive/phd0209.gif)

## Building

1. Clone the repository
    `git clone https://github.com/sudheesh001/PhD-Comics-Scraper PhDComics`
2. `cd PhDComics`
3. To build the code you need to have `libcurl` installed with `-lcurl` compilation flag.
4. `c++ main.cpp -lcurl -o phd` This will compile and generate a binary `./phd`. Without the `-o` flag, the default executable will be `./a.out`
