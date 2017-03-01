# biclique
An R package to compute bicliques

Download the package to your computer and go to the directory where the package is downloaded.
Install the package

> R CMD INSTALL biclique

The input file should be tab tab delimited. This package supports edgelist and matrix format.
Load the package
> library(biclique)

Add the number of vertices and edges to the original input graph. If your input file already has these values, you don't have to run this command.
> bi.format("example1.el")  #edgelist
OR
> bi.format("example2.bmat") #matrix

Compute the degree of each vertex. This command is optional.
> b = bi.degree("example1.el")

Compute the bicliques
> bi = biclique("example1.el")

Print the bicliques
> bi.print(bi)
