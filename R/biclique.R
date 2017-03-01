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
        bi[[i]] = list(temp2, temp1)
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

#' @title bi.print
#'
#' @description
#' print the bicliques
#'
#' @param bi bicliques
#' @param text default is 0. If you want to show labels, change it to 1
#'
#' @examples
#' bi.print(biclique)
#'
#' @export
bi.print <- function(bi, text = 0)
{
    res = c()
    cnt = 1
    for(b in bi){
        res[cnt] = length(b[[1]])
        cnt = cnt+1
        res[cnt] = length(b[[2]])
        cnt = cnt+1
    }
    mat = matrix(res, nrow=2, byrow=TRUE)
    mp = barplot(mat, main = "Bicliques", beside=T)
    if(text == 1) mtext(side=1, at=mp, text=mat, line=1)
}

#' @title bi.format
#'
#' @description
#' add number of vertices and edges to the input file. The original input file will be changed
#'
#' @param filename input file name
#' @param filetype 0, 1 is bmat
#'
#' @examples
#' bi.format(filename)
#'
#' @export
bi.format <- function(filename, filetype = 0)
{
    # read file
    temp.df = read.table(filename, sep="\t")
    if(filetype == 0) {
        # count the number of left vertices
        left = length(unique(temp.df$V1))
        # count the number of left vertices
        right = length(unique(temp.df$V2))
        # count the number of edges
        edges = length(temp.df$V1)

        # write data to the input file
        cat(left, file=filename)
        cat("\t", file=filename, append=TRUE)
        cat(right, file=filename, append=TRUE)
        cat("\t", file=filename, append=TRUE)
        cat(edges, file=filename, append=TRUE)
        cat("\n", file=filename, append=TRUE)
        write.table(temp.df, filename, append=TRUE, quote = FALSE, sep="\t", row.names = FALSE, col.names = FALSE)
    }else {
        # count the number of left vertices
        left = length(unique(row.names(temp.df)))
        # count the number of left vertices
        right = length(unique(colnames(temp.df)))

        # write data to the input file
        cat(left, file=filename)
        cat("\t", file=filename, append=TRUE)
        cat(right, file=filename, append=TRUE)
        cat("\n", file=filename, append=TRUE)
        suppressWarnings(write.table(temp.df, filename, append=TRUE, quote = FALSE, sep="\t", row.names = TRUE, col.names = TRUE))
    }
}
