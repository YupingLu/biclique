#' @title biclique
#'
#' @description 
#' Biclique enumeration
#' 
#' @param filename input file name
#' 
#' @examples
#' biclique("example1.el")
#'
#' @export
biclique <- function(filename)
{
    .Call("R_biclique", 
          as.character(filename))
    invisible()
}
