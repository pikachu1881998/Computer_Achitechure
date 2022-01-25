#ifndef GSHARE_H
#define GSHARE_H

#include "branchsim.h"

// Gshare predictor definition
typedef struct patternTable{
    uint64_t counter;
    uint64_t btag;
}pattern;
class gshare : public branch_predictor_base
{

private:

    // define the vector for counter
    // define the vector for tag
    // or create an struct to hold both togather
    pattern * table;
    bool recent_branch;  // this is for the stat to figure out if the misprediction or
    uint64_t GHR;
    uint64_t G; // History Registers are G bits wide
    uint64_t P; // Pattern / Perceptron table addressing bits
    uint64_t N;
    uint64_t numIndex;


public:
    void init_predictor(branchsim_conf *sim_conf);
    
    // Return the prediction ({taken/not-taken}, target-address)
    bool predict(branch *branch, branchsim_stats *sim_stats);
    
    // Update the branch predictor state
    void update_predictor(branch *branch);
    uint64_t mask(branch * branch);

    // Cleanup any initialized memory here
    ~gshare();
};

#endif
