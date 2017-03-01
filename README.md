# biclique
An R package to compute bicliques

Download the package to your computer and go to the directory where the package is downloaded.
If it is a zip file. Use zip command to unzip it.
> unzip biclique-master.zip

If the file directory is not biclique, rename it.
> mv biclique-master biclique

Install the package
> R CMD INSTALL biclique

The input file should be tab delimited. This package supports edgelist and matrix format.
Load the package
> library(biclique)

Add the number of vertices and edges to the original input graph. If your input file already has these values, you don't have to run this command.
> bi.format("example1.el")  #edgelist file

OR
> bi.format("example2.bmat") #matrix file

Compute the degree of each vertex. This command is optional.
> b = bi.degree("biclique/inst/example1.el")

Compute the bicliques
> bi = biclique("biclique/inst/example1.el")

Print the bicliques
> bi.print(bi)
