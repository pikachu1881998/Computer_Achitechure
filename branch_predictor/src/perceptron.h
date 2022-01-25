#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "branchsim.h"
#include <vector>
typedef struct weights{
    int theta;
}theta;
// Perceptron predictor definition
typedef struct perceptron{
    theta * weights;
    uint64_t tag;
}neutron;
typedef struct hisrtoryReg{
    int GHR;
}hist;
class perceptron_predictor : public branch_predictor_base
{
private:
    neutron * table;
    hist * GHR;
    uint64_t G; // History Registers are G bits wide
    uint64_t P; // Pattern / Perceptron table addressing bits
    uint64_t N;
    int threshold;
    int recent_branch;
    uint64_t numperce;


public:
    void init_predictor(branchsim_conf *sim_conf);
    
    // Return the prediction 
    bool predict(branch *branch, branchsim_stats *sim_stats);
    
    // Update the branch predictor state
    void update_predictor(branch *branch);

    // Cleanup any initialized memory here
    uint64_t mask(branch*branch1);
    ~perceptron_predictor();
};


#endif
