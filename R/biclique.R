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
biclique <- function(filename)  #may change this function name to bic.profile
{
    # Get profile raw data
    profile.raw = .Call("R_biclique", as.character(filename))
    # the number of elements in profile raw data
    nelems = profile.raw[1]

    # paste profile data into profile
    profile = paste("|Right Vertex|\t|Left Vertex|\tNumber\n")

   	for (i in seq(1, nelems-9, 3)){
   		profile = paste(profile, profile.raw[i+1], "\t", profile.raw[i+2], "\t", profile.raw[i+3], "\n", sep="")
   	}

   	profile = paste(profile, "\n", sep="")
    profile = paste(profile, "Number of left vertices     : ", profile.raw[nelems-7], "\n", sep="")
    profile = paste(profile, "Number of right vertices    : ", profile.raw[nelems-6], "\n", sep="")
    profile = paste(profile, "Number of edges             : ", profile.raw[nelems-5], "\n", sep="")
    profile = paste(profile, "Number of bicliques         : ", profile.raw[nelems-4], "\n", sep="")
    profile = paste(profile, "Size of edge max biclique   : ", "(", profile.raw[nelems-3], ",", profile.raw[nelems-2], ")", "\n", sep="")
    profile = paste(profile, "Size of vertex max biclique : ", "(", profile.raw[nelems-1], ",", profile.raw[nelems], ")", "\n", sep="")

    # print profile
    cat(profile)
}
