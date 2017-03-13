#' @title Biclique enumeration
#'
#' @description
#' This function will compute the bicliques and output the statistics of these bicliques.
#' If you want to get bicliques above a threshold, you can change the values of lleast and rleast.
#' The input file should be tab delimited with number of vertices and edges at the head of the input file.
#' If your input file does not have these values, you can use function bi.format to add these values to it.
#' This package supports edgelist and binary matrix file format.
#' Two versions of algorithms are implemented in this function, you can choose either one to get bicliques.
#'
#' @param filename Input file name
#' @param lleast Least number of left partite <default = 1>
#' @param rleast Least number of right partite <default = 1>
#' @param version Algorithm version <default = 1> [1|2]
#' @param filetype Input file format <default = 0>. 0-edge list, 1-binary matrix.
#'
#' @examples
#' \dontrun{
#' bicliques = bi.clique("example1.el")
#' bicliques = bi.clique("example1.el", 3, 2)
#' bicliques = bi.clique("example4.bmat")
#' }
#'
#' @export
bi.clique <- function(filename, lleast = 1, rleast = 1, version = 1, filetype = 0)
{
    # parameters for R_biclique
    isdegree = 0
    # Get bicliques <default = 1>. If you set it to 0. you'll only get the statistics without bicliques.
    getclique = 1

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

#' @title Get the degree list
#'
#' @description
#' This function will output the degree of each vertex.
#'
#' @param filename Input file name
#' @param filetype Input file format <default = 0>. 0-edge list, 1-binary matrix.
#'
#' @examples
#' \dontrun{
#' degreelist = bi.degree("example1.el")
#' degreelist = bi.degree("example4.bmat", 1)
#' }
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

#' @title Print the bicliques
#'
#' @description
#' You can pass results from function bi.clique to this function to visualize the bicliques.
#'
#' @param bi bicliques
#' @param text default is 0. If you want to show labels, change it to 1.
#'
#' @examples
#' \dontrun{
#' bi.print(bicliques)
#' }
#'
#' @importFrom graphics barplot mtext
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

#' @title Add number of vertices and edges to the input file
#'
#' @description
#' This funtion will calculate the number of vertices and edges and add them to the head of the input file.
#' The original input file will be changed.
#'
#' @param filename Input file name
#' @param filetype Input file format <default = 0>. 0-edge list, 1-binary matrix.
#'
#' @examples
#' \dontrun{
#' bi.format("example2.el")
#' bi.format("example5.bmat", 1)
#' }
#'
#' @importFrom utils read.table write.table
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
