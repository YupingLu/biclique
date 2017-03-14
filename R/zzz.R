# Hooks for Name Space events

# triggered by library()
.onLoad <- function(libname, pkgname)
{
    # Init biclique environment
    init_state()
} 