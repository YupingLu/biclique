#' @title biclique
#'
#' @description 
#' Biclique enumeration
#' 
#' @param filename input file name
#' 
#' @examples
#' bic("example1.el")
#'
#' @export
bic <- function(filename)
{
    .Call("R_biclique", 
          as.character(filename))
    invisible()
}
