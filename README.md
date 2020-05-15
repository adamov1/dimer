# dimer
> Generation of Random Lozenge Tilings

## Installation

- Clone this repository using `https://github.com/adamov1/dimer`
- SVG rendering depends on <a href="https://www.cairographics.org/" target="_blank">**Cairo**</a>.

### Setup

> install libcairo2-dev

```shell
$ sudo apt-get install libcairo2-dev
```

> clone repository

```shell
$ git clone https://github.com/adamov1/dimer
```

> once in repository, compile using Makefile

```shell
$ make
```

## Usage

> To generate SVG, run
```shell
$ ./tile <destination_file> <size> <linewidth> <arctic_circle> <a> <b> <c>
```
### Parameters
- `<destination_file>`: filename of output
- `<size>`: height of image (in px)
- `<linewidth>`: thickness of borders around tiles (set to be `0.0` if you want borderless)
- `<arctic_circle>`: set as `true` or `false` depending on whether you want to highlight the arctic circle
- `<a> <b> <c>`: determine the dimensions of the hexagon
