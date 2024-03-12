#include "agents.h"
#include <stdio.h>
#include <math.h>

double distance(struct agent a1, struct agent a2) {
    return sqrt(pow(a1.x - a2.x, 2) + pow(a1.y - a2.y, 2));
}
