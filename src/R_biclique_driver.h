/* 
 * R wrapper of Biclique Enumerator
 *
 * Author: Yuping Lu, yupinglu@utk.edu
 * Created: September 2016
 *
 * Copyright 2016-2017
 * Department of Computer Science, University of Tennessee, Knoxville
 *
 * Last modified: September 2016
 *
 */

#ifndef __R_BICLIQUE_DRIVER__
#define __R_BICLIQUE_DRIVER__

#include <string.h>
#include "bit.h"
#include "bigraph.h"
#include "utility.h"
#include "biclique.h"

void maximal_biclique(char *fn, BiGraph *G);

#endif