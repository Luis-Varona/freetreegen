#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "layout.h"
#include "generator.h"

FreeTree *generate_successor(FreeTree *tree);
FreeTree **generate_all_free_trees(int order);

#endif
