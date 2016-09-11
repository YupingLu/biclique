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
    # Get raw data list
    data.raw = .Call("R_biclique", as.character(filename))

    # Get profile raw data
    profile.raw = data.raw[[3]]
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
    on.exit(cat(profile))

    # unlist biclique lists
    bi = list()
    for(i in 1:profile.raw[nelems-4]){
      temp1 = unlist(data.raw[[1]][[i]])
      temp2 = unlist(data.raw[[2]][[i]])
      bi[[i]] = c(temp1, temp2)
    }

    # returned are the bicliques
    invisible(bi)
}
