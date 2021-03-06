% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/biclique.R
\name{bi.format}
\alias{bi.format}
\title{Add number of vertices and edges to the input file}
\usage{
bi.format(filename, filetype = 0)
}
\arguments{
\item{filename}{Input file name}

\item{filetype}{Input file format <default = 0>. 0-edge list, 1-binary matrix.}
}
\description{
This funtion will calculate the number of vertices and edges and add them to the head of the input file.
For edge list file, three entries will be added. And they are: the number of left vertices, the number of right vertices and the number of edges.
For binary matrix file, two entries will be added. And they are: the number of left vertices and the number of right vertices.
The original input file will be changed.
If your input file already have those entries, pleast don't run this command. Otherwise, you'll get error results after running bi.clique.
Please also note that no comment characters are allowed in the input file.
}
\examples{
dir <- tempdir()
file.copy(system.file("extdata", "example2.el", package = "biclique"), dir)
file.copy(system.file("extdata", "example5.bmat", package = "biclique"), dir)
bi.format(file.path(dir, "example2.el"))
bi.format(file.path(dir, "example5.bmat"), 1)
}
