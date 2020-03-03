Maximal Biclique Enumeration in Bipartite Graphs
================================================

<https://cran.r-project.org/package=biclique>

[![Build Status](https://travis-ci.org/YupingLu/biclique.svg?branch=master)](https://travis-ci.org/YupingLu/biclique)
[![Downloads](https://cranlogs.r-pkg.org/badges/biclique?color=brightgreen)](https://www.r-pkg.org/pkg/biclique)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/YupingLu/biclique/issues)
[![License](https://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](https://www.gnu.org/licenses/gpl-2.0.html)
[![CRAN](https://www.r-pkg.org/badges/version/biclique)](https://cran.r-project.org/package=biclique)


Description
-----------
A tool for enumerating maximal complete bipartite graphs. The input file should be tab delimited. This package supports edgelist and binary matrix format. You can try the example files in the inst directory. The output are maximal complete bipartite graphs.

Installation
------------
The stable, CRAN-ready version can be installed with:

    install.packages("biclique")

The latest version can be obtained via:

    devtools::install_github("YupingLu/biclique")

Examples
--------
Load the package
    
    library(biclique)

Add the number of vertices and edges to the original input graph. If your input file already has these values, you don't have to run this command.
    
    bi.format("example2.el")       #edgelist format
    bi.format("example5.bmat", 1)  #binary matrix format

Input: edgelist format.

    degreelist = bi.degree("example1.el")  #Compute the degree of each vertex
    bicliques = bi.clique("example1.el")   #Compute the bicliques
    bi.print()                             #Print the bicliques

You can also choose the threshold size for computed bicliques.
    
    bicliques = bi.clique("example1.el", 3, 2)  #bicliques larger or equal to K3,2

Input: binary matrix format.

    degreelist = bi.degree("example4.bmat", filetype = 1)  #Compute the degree of each vertex
    bicliques = bi.clique("example4.bmat", filetype = 1)   #Compute the bicliques
    bi.print()                                             #Print the bicliques

For more details, please check the documentation.

Ask Questions 
-------------
* Please use [biclique/issues](https://github.com/YupingLu/biclique/issues) for how to use biclique and reporting bugs

License
-------
© Contributors, 2020. Licensed under an [GPL-2.0](https://github.com/YupingLu/biclique/blob/master/LICENSE) license.

Reference Paper
---------------
Lu, Y., Phillips, C.A. & Langston, M.A. Biclique: an R package for maximal biclique enumeration in bipartite graphs. 
BMC Res Notes 13, 88 (2020). https://doi.org/10.1186/s13104-020-04955-0
