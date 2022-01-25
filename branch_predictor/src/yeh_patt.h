#ifndef YEH_PATT_H
#define YEH_PATT_H

#include "branchsim.h"
typedef struct histroyTable{
    uint64_t GHR;
    uint64_t btag;      // not sure where to put it here
}Htable;

typedef struct patterTable{
    uint64_t counter;
}patTable;
// Yeh-Patt predictor definition
class yeh_patt : public branch_predictor_base
{
private:
    Htable * history;
    patTable * table;
    uint64_t GHR;
    uint64_t G; // History Registers are G bits wide
    uint64_t P; // Pattern / Perceptron table addressing bits
    uint64_t N;
    bool recent_branch;
    uint64_t numHist;
    uint64_t numPat;
    // create a struck that will hold the counter
    // initialize the sim configuration
public:
    void init_predictor(branchsim_conf *sim_conf);
    uint64_t mask(branch*branch);
    // Return the prediction ({taken/not-taken}, target-address)
    bool predict(branch *branch, branchsim_stats *sim_stats);
    
    // Update the branch predictor state
    void update_predictor(branch *branch);

    // Cleanup any initialized memory here
    ~yeh_patt();
};

#endif
