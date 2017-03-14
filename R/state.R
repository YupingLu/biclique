# custom environment for biclique

init_state <- function(envir = .GlobalEnv)
{
    if (!exists(".bienv", envir = envir))
        envir$.bienv <- new.env(parent = envir)
  
    envir$.bienv$nofbi <- c()    # number of each biclique
    envir$.bienv$noflr <- c()    # number of left and right vertices in each biclique
    envir$.bienv$cnt <- 1        # index for variable nofbi

    invisible()
}