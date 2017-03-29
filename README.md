# biclique
An R package to compute bicliques

Install the package.
> devtools::install_github("YupingLu/biclique")

The input file should be tab delimited. This package supports edgelist and matrix format.
Load the package
> library(biclique)

Add the number of vertices and edges to the original input graph. If your input file already has these values, you don't have to run this command.
> bi.format("example1.el")  #edgelist file

OR
> bi.format("example2.bmat", 1) #matrix file

Compute the degree of each vertex. This command is optional.
> degreelist = bi.degree("example1.el")

Compute the bicliques
> bicliques = bi.clique("example1.el")

Print the bicliques
> bi.print()
