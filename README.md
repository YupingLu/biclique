# biclique
An R package to compute bicliques

### Install the package
> devtools::install_github("YupingLu/biclique")

The input file should be tab delimited. This package supports edgelist and matrix format. You can try the example files in the inst directory.

Load the package
> library(biclique)

Add the number of vertices and edges to the original input graph. If your input file already has these values, you don't have to run this command.
> bi.format("example2.el")  #edgelist format

OR
> bi.format("example5.bmat", 1) #binary matrix format

### edgelist format
Compute the degree of each vertex.
> degreelist = bi.degree("example1.el")

Compute the bicliques
> bicliques = bi.clique("example1.el")

Print the bicliques
> bi.print()

### binary matrix format
Compute the degree of each vertex.
> degreelist = bi.degree("example4.bmat", filetype = 1)

Compute the bicliques
> bicliques = bi.clique("example4.bmat", filetype = 1)

Print the bicliques
> bi.print()
