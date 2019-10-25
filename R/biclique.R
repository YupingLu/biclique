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
#' @param left_least Least number of left partite <default = 1>
#' @param right_least Least number of right partite <default = 1>
#' @param version Algorithm version <default = 1> [1|2]
#' @param filetype Input file format <default = 0>. 0-edge list, 1-binary matrix.
#' @param getclique Get bicliques <default = 1>. If you set it to 0. you'll only get the statistics without bicliques.
#' @param envir biclique environment
#'
#' @examples
#' bicliques = bi.clique(system.file("extdata", "example1.el", package = "biclique"))
#' bicliques = bi.clique(system.file("extdata", "example1.el", package = "biclique"), 3, 2)
#' bicliques = bi.clique(system.file("extdata", "example4.bmat", package = "biclique"), filetype = 1)
#' # check a biclique
#' bicliques$biclique1
#'
#' @export
bi.clique <- function(filename, left_least = 1, right_least = 1, version = 1, filetype = 0, getclique = 1, envir = .GlobalEnv$.bienv)
{
    # reset global variables
    init_state()

    # parameters for R_biclique
    isdegree = 0
    # Get bicliques <default = 1>. If you set it to 0. you'll only get the statistics without bicliques.
    #getclique = 1

    # Get raw data list
    data.raw = .Call("R_biclique", as.character(filename), as.integer(left_least), as.integer(right_least), as.integer(isdegree),
                                   as.integer(version), as.integer(getclique), as.integer(filetype))

    # Get profile raw data
    profile.raw = data.raw[[3]]
    # the number of elements in profile raw data
    nelems = profile.raw[1]

    # paste profile data into profile
    profile = paste("Biclique\tNumber\n")

   	for (i in seq(1, nelems-9, 3)){

        profile = paste(profile, "K", profile.raw[i+2], ",", profile.raw[i+1], "\t\t", profile.raw[i+3], "\n", sep="")
        envir$nofbi[envir$cnt] <- profile.raw[i+3]
        envir$noflr[envir$cnt*2-1] <- profile.raw[i+2]
        envir$noflr[envir$cnt*2] <- profile.raw[i+1]
        envir$cnt <- envir$cnt + 1
   	}

   	#profile = paste(profile, "\n", sep="")
    #profile = paste(profile, "Number of left vertices     : ", profile.raw[nelems-7], "\n", sep="")
    #profile = paste(profile, "Number of right vertices    : ", profile.raw[nelems-6], "\n", sep="")
    #profile = paste(profile, "Number of edges             : ", profile.raw[nelems-5], "\n", sep="")
    profile = paste(profile, "Number of bicliques         : ", profile.raw[nelems-4], "\n", sep="")
    profile = paste(profile, "Vertex-maximum biclique     : ", "K", profile.raw[nelems], ",", profile.raw[nelems-1], "\n", sep="")
    profile = paste(profile, "Edge-maximum biclique       : ", "K", profile.raw[nelems-2], ",", profile.raw[nelems-3], "\n", sep="")
    

    # print profile
    on.exit(cat(profile))

    # unlist biclique lists
    bi = list()
    for(i in 1:profile.raw[nelems-4]){
        temp1 = unlist(data.raw[[1]][[i]])
        temp2 = unlist(data.raw[[2]][[i]])
        idx = paste("biclique", i, sep="")
        bi[[idx]] = list(left=temp2, right=temp1)
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
#' degreelist = bi.degree(system.file("extdata", "example1.el", package = "biclique")) 
#' degreelist = bi.degree(system.file("extdata", "example4.bmat", package = "biclique"), 1)
#' #get the vertex degree
#' degreelist['A']
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
    #w = data.frame(degree)

    # print edgelist
    #on.exit(print(w))

    # returned are the biclique degree data.frame
    #invisible(degreelist)
}

#' @title Print the bicliques
#'
#' @description
#' You can pass results from function bi.clique to this function to visualize the bicliques.
#'
#' @param envir biclique environment
#'
#' @examples
#' bicliques = bi.clique(system.file("extdata", "example1.el", package = "biclique"))
#' bi.print()
#'
#' @importFrom graphics barplot lines points text
#' @export
bi.print <- function(envir = .GlobalEnv$.bienv)
{
     # check if .GlobalEnv$.bienv$nofbi is null
    if(is.null(envir$nofbi))
        stop("No bicliques. Make sure you run bi.clique first.")

    ylim <- c(0, 1.1*max(envir$noflr))
    mat <- matrix(envir$noflr, nrow=2)
    colnames(mat) <- envir$nofbi
    rownames(mat) <- c("Left Partite","Right Partite")
    mat <- as.table(mat)
    xx <- barplot(mat, legend=T, beside=T, main='Bicliques', xlab="Number of each biclique", ylab="Number of vertices", ylim=ylim)
    text(x=xx, y=mat, label=mat, pos=3, cex=0.8, col="red")
    x1 = (xx[1,] + xx[2,])/2
    y1 = (max(envir$noflr)*0.6) / max(envir$nofbi)
    lines(x1, envir$nofbi*y1, col="orange")
    points(x1, envir$nofbi*y1, col="orange")
}

#' @title Add number of vertices and edges to the input file
#'
#' @description
#' This funtion will calculate the number of vertices and edges and add them to the head of the input file.
#' For edge list file, three entries will be added. And they are: the number of left vertices, the number of right vertices and the number of edges.
#' For binary matrix file, two entries will be added. And they are: the number of left vertices and the number of right vertices.
#' The original input file will be changed.
#' If your input file already have those entries, pleast don't run this command. Otherwise, you'll get error results after running bi.clique.
#' Please also note that no comment characters are allowed in the input file.
#'
#' @param filename Input file name
#' @param filetype Input file format <default = 0>. 0-edge list, 1-binary matrix.
#'
#' @examples
#' dir <- tempdir()
#' file.copy(system.file("extdata", "example2.el", package = "biclique"), dir)
#' file.copy(system.file("extdata", "example5.bmat", package = "biclique"), dir)
#' bi.format(file.path(dir, "example2.el"))
#' bi.format(file.path(dir, "example5.bmat"), 1)
#' @importFrom utils read.table write.table
#' @export
bi.format <- function(filename, filetype = 0)
{
    # check the head. if they alreay contain the necessary entries, quit.
    title.line = readLines(filename, n=1)
    titles = unlist(strsplit(title.line, "\t"))
    len = length(titles)
    
    if(!((filetype == 0 && len == 3) || (filetype == 1 && len == 2))) {
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
}
