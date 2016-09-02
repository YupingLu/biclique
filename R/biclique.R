#' @title biclique
#'
#' @description 
#' Biclique enumeration
#' 
#' @param filename input file name
#' 
#' @examples
#' bpls("attributes.bp")
#'
#' @export
bpclique <- function(filename)
{
    .Call("R_biclique", 
          as.character(filename))
    invisible()
}
