# soduku_solver

[![DeepWiki](https://img.shields.io/badge/DeepWiki-hmgle%2Fsudoku__solver-blue.svg?logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACwAAAAyCAYAAAAnWDnqAAAAAXNSR0IArs4c6QAAA05JREFUaEPtmUtyEzEQhtWTQyQLHNak2AB7ZnyXZMEjXMGeK/AIi+QuHrMnbChYY7MIh8g01fJoopFb0uhhEqqcbWTp06/uv1saEDv4O3n3dV60RfP947Mm9/SQc0ICFQgzfc4CYZoTPAswgSJCCUJUnAAoRHOAUOcATwbmVLWdGoH//PB8mnKqScAhsD0kYP3j/Yt5LPQe2KvcXmGvRHcDnpxfL2zOYJ1mFwrryWTz0advv1Ut4CJgf5uhDuDj5eUcAUoahrdY/56ebRWeraTjMt/00Sh3UDtjgHtQNHwcRGOC98BJEAEymycmYcWwOprTgcB6VZ5JK5TAJ+fXGLBm3FDAmn6oPPjR4rKCAoJCal2eAiQp2x0vxTPB3ALO2CRkwmDy5WohzBDwSEFKRwPbknEggCPB/imwrycgxX2NzoMCHhPkDwqYMr9tRcP5qNrMZHkVnOjRMWwLCcr8ohBVb1OMjxLwGCvjTikrsBOiA6fNyCrm8V1rP93iVPpwaE+gO0SsWmPiXB+jikdf6SizrT5qKasx5j8ABbHpFTx+vFXp9EnYQmLx02h1QTTrl6eDqxLnGjporxl3NL3agEvXdT0WmEost648sQOYAeJS9Q7bfUVoMGnjo4AZdUMQku50McDcMWcBPvr0SzbTAFDfvJqwLzgxwATnCgnp4wDl6Aa+Ax283gghmj+vj7feE2KBBRMW3FzOpLOADl0Isb5587h/U4gGvkt5v60Z1VLG8BhYjbzRwyQZemwAd6cCR5/XFWLYZRIMpX39AR0tjaGGiGzLVyhse5C9RKC6ai42ppWPKiBagOvaYk8lO7DajerabOZP46Lby5wKjw1HCRx7p9sVMOWGzb/vA1hwiWc6jm3MvQDTogQkiqIhJV0nBQBTU+3okKCFDy9WwferkHjtxib7t3xIUQtHxnIwtx4mpg26/HfwVNVDb4oI9RHmx5WGelRVlrtiw43zboCLaxv46AZeB3IlTkwouebTr1y2NjSpHz68WNFjHvupy3q8TFn3Hos2IAk4Ju5dCo8B3wP7VPr/FGaKiG+T+v+TQqIrOqMTL1VdWV1DdmcbO8KXBz6esmYWYKPwDL5b5FA1a0hwapHiom0r/cKaoqr+27/XcrS5UwSMbQAAAABJRU5ErkJggg==)](https://deepwiki.com/hmgle/sudoku_solver)

Solve soduku via DLX.

## Building

```console
make
```

## Usage

Usage: `./sudoku_sover [-v] [inputfile]`

Example:

```console
$ ./sudoku_solver -v hardest_sudoku.txt
input sudoku:
+-------+-------+-------+
| 8 0 0 | 0 0 0 | 0 0 0 |
| 0 0 3 | 6 0 0 | 0 0 0 |
| 0 7 0 | 0 9 0 | 2 0 0 |
+-------+-------+-------+
| 0 5 0 | 0 0 7 | 0 0 0 |
| 0 0 0 | 0 4 5 | 7 0 0 |
| 0 0 0 | 1 0 0 | 0 3 0 |
+-------+-------+-------+
| 0 0 1 | 0 0 0 | 0 6 8 |
| 0 0 8 | 5 0 0 | 0 1 0 |
| 0 9 0 | 0 0 0 | 4 0 0 |
+-------+-------+-------+
dlx_search return 60:
+-------+-------+-------+
| 8 1 2 | 7 5 3 | 6 4 9 |
| 9 4 3 | 6 8 2 | 1 7 5 |
| 6 7 5 | 4 9 1 | 2 8 3 |
+-------+-------+-------+
| 1 5 4 | 2 3 7 | 8 9 6 |
| 3 6 9 | 8 4 5 | 7 2 1 |
| 2 8 7 | 1 6 9 | 5 3 4 |
+-------+-------+-------+
| 5 2 1 | 9 7 4 | 3 6 8 |
| 4 3 8 | 5 2 6 | 9 1 7 |
| 7 9 6 | 3 1 8 | 4 5 2 |
+-------+-------+-------+
```

It can accept one line description of the sudoku input like this:

> 800000000003600000070090200050007000000045700000100030001000068008500010090000400

```console
$ echo "8000000000036000000700902000500070000000457000001000300010000680085000100900004"|./sudoku_solver -v
input sudoku:
+-------+-------+-------+
| 8 0 0 | 0 0 0 | 0 0 0 |
| 0 0 3 | 6 0 0 | 0 0 0 |
| 0 7 0 | 0 9 0 | 2 0 0 |
+-------+-------+-------+
| 0 5 0 | 0 0 7 | 0 0 0 |
| 0 0 0 | 0 4 5 | 7 0 0 |
| 0 0 0 | 1 0 0 | 0 3 0 |
+-------+-------+-------+
| 0 0 1 | 0 0 0 | 0 6 8 |
| 0 0 8 | 5 0 0 | 0 1 0 |
| 0 9 0 | 0 0 0 | 4 0 0 |
+-------+-------+-------+
dlx_search return 60:
+-------+-------+-------+
| 8 1 2 | 7 5 3 | 6 4 9 |
| 9 4 3 | 6 8 2 | 1 7 5 |
| 6 7 5 | 4 9 1 | 2 8 3 |
+-------+-------+-------+
| 1 5 4 | 2 3 7 | 8 9 6 |
| 3 6 9 | 8 4 5 | 7 2 1 |
| 2 8 7 | 1 6 9 | 5 3 4 |
+-------+-------+-------+
| 5 2 1 | 9 7 4 | 3 6 8 |
| 4 3 8 | 5 2 6 | 9 1 7 |
| 7 9 6 | 3 1 8 | 4 5 2 |
+-------+-------+-------+
```

or the inputfile like this:

> 800000000  
> 003600001  
> 070090200  
> 050007000  
> 000045700  
> 000100030  
> 001000068  
> 008500010  
> 090000400
