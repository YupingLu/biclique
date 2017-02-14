#' @title biclique
#'
#' @description 
#' Biclique enumeration
#' 
#' @param filename input file name
#' @param lleast 1
#' @param rleast 1
#' @param version 1
#' @param getclique 1
#' @param filetype 0
#' 
#' @examples
#' bicliques = biclique("example1.el")
#'
#' @export
biclique <- function(filename, lleast = 1, rleast = 1, version = 1, getclique = 1, filetype = 0)  #may change this function name to bic.profile
{
    # parameters for R_biclique
    isdegree = 0

    # Get raw data list
    data.raw = .Call("R_biclique", as.character(filename), as.integer(lleast), as.integer(rleast), as.integer(isdegree),
                                   as.integer(version), as.integer(getclique), as.integer(filetype))

    # Get profile raw data
    profile.raw = data.raw[[3]]
    # the number of elements in profile raw data
    nelems = profile.raw[1]

    # paste profile data into profile
    profile = paste("biclique\tNumber\n")

   	for (i in seq(1, nelems-9, 3)){
        profile = paste(profile, "K", profile.raw[i+2], ",", profile.raw[i+1], "\t\t", profile.raw[i+3], "\n", sep="")
   	}

   	profile = paste(profile, "\n", sep="")
    profile = paste(profile, "Number of left vertices     : ", profile.raw[nelems-7], "\n", sep="")
    profile = paste(profile, "Number of right vertices    : ", profile.raw[nelems-6], "\n", sep="")
    profile = paste(profile, "Number of edges             : ", profile.raw[nelems-5], "\n", sep="")
    profile = paste(profile, "Number of bicliques         : ", profile.raw[nelems-4], "\n", sep="")
    profile = paste(profile, "Maximum edge biclique       : ", "K", profile.raw[nelems-2], ",", profile.raw[nelems-3], "\n", sep="")
    profile = paste(profile, "Maximum vertex biclique     : ", "K", profile.raw[nelems], ",", profile.raw[nelems-1], "\n", sep="")

    # print profile
    on.exit(cat(profile))

    # unlist biclique lists
    bi = list()
    for(i in 1:profile.raw[nelems-4]){
        temp1 = unlist(data.raw[[1]][[i]])
        temp2 = unlist(data.raw[[2]][[i]])
        bi[[i]] = c(temp2, temp1)
    }

    # returned are the bicliques
    invisible(bi)
}


#' @title bi.degree
#'
#' @description 
#' Get the degree list
#' 
#' @param filename input file name
#' @param filetype 0
#' 
#' @examples
#' degreelist = bi.degree("example1.el")
#'
#' @export
bi.degree <- function(filename, filetype = 0)
{
    # parameters for R_biclique
    lleast = 1
    rleast = 1
    isdegree = 1
    version = 1 
    getclique = 1

    degreelist = .Call("R_biclique", as.character(filename), as.integer(lleast), as.integer(rleast), as.integer(isdegree),
                                     as.integer(version), as.integer(getclique), as.integer(filetype))

    degree = unlist(degreelist)
    w = data.frame(degree)

    # returned are the biclique degree data.frame
    invisible(w)
}
