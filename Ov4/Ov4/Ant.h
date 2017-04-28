#pragma once
#include"scheduleBuilder.h"
#include <vector>

void ants(Jobs* jobs);
float stateTransitionRule(float phi_ij, float eta_ij, Jobs* tabu);

