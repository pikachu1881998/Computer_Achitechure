
#include <iostream>
#include <math.h>
#include "branchsim.h"
#include "gshare.h"
#include "perceptron.h"
#include "yeh_patt.h"

// Don't modify this line -- Its to make the compiler happy
branch_predictor_base::~branch_predictor_base() {}


// ******* Student Code starts here *******

// Gshare Branch Predictor
uint64_t stage =0;
void gshare::init_predictor(branchsim_conf *sim_conf)
{
    /*
     * todo initialize the ghr and pattern table for gshapre
     * initilize the vector with 01 for all the available indexes
     * define ghr in gshare.h use uint64, default value will be 0
     * create a new members in gshare.h to hold the configuration values the G, N, P
     * create another variable to hold the predicted value of the branch
     * create an tag vector and initiclize it with 0
     */
    stage=sim_conf->N;
    G = sim_conf->G;
    N = sim_conf->N;
    P = sim_conf->P;
    GHR = 0;
    recent_branch = false;
    numIndex = 0;
    for(uint64_t i =0; i<G; i++){
        numIndex = (numIndex<<1)+(uint64_t)1;
    }

    table = (pattern*) malloc((numIndex+(uint64_t)1) * (uint64_t)sizeof(pattern));
    for(uint64_t i =0 ; i<=numIndex; i++){
        table[i].counter = 1;
        table[i].btag = 0;
    }
}
uint64_t gshare::mask(branch * branch) {
    uint64_t temp =0;
    uint64_t mask = 0;

    for(uint64_t i =0; i<G; i++){
        temp = (temp<<1)+(unsigned)1;
    }
    uint64_t PC = (temp) & (branch->ip>>2);
    mask = (GHR) ^ PC;
    return mask;
}

bool gshare::predict(branch *branch, branchsim_stats *sim_stats) {
    /*
     * todo do the Gshare algorithm
     * compute
     * use the ghr in the private and mask the pc's bit 2 to the size of G+1
     * access that counter
     * and based on the value predict
     * update the value of the member to teh predicted to hold recent predicted value based on the counter
     * check the tag for the branch if match then dont do any thing, e
     * */
    sim_stats->total_instructions = branch->inst_num;
    sim_stats->num_branch_instructions++;

    uint64_t index = 0;
    index = mask(branch);
    uint64_t tag = (branch->ip)>>(G+2);
    if (table[index].btag != tag) { //TODO have to upadte the tag correctly its not the correct tag
        sim_stats->num_tag_conflicts++;
        table[index].btag = tag;
    }
    if (table[index].counter > 1) {
        if(branch->is_taken)
            sim_stats->num_branches_correctly_predicted++;
        else
            sim_stats->num_branches_miss_predicted++;
        recent_branch = TAKEN;
        return TAKEN;
    } else {
        if(!branch->is_taken)
            sim_stats->num_branches_correctly_predicted++;
        else
            sim_stats->num_branches_miss_predicted++;
        recent_branch = NOT_TAKEN;
        return NOT_TAKEN;
    }

}

void gshare::update_predictor(branch *branch)
{
    /*
     * todo update the counter and the GHR
     * use the ghr in the private and mask the pc's bit 2 to the size of G+1
     *
     * access the counter
     * update the counter based on the stored predicted branch
     * update the ghr, by  shifteing it by one and masking it with the highest value and than adding one bit based on the predicted branch
     *
     */
    uint64_t  index = mask(branch);
    if(branch->is_taken){
        GHR = ((GHR<<1) | (uint64_t)1) & numIndex;
        if (table[index].counter<3)
            table[index].counter = ((table[index].counter) & (uint64_t)3) + (uint64_t)1;

    } else {
        GHR = (GHR << 1) & numIndex;
        if (table[index].counter>0) {
            table[index].counter = ((table[index].counter) & (uint64_t)3) - (uint64_t) 1;
        }
    }

}

gshare::~gshare() {
    free(table);
}
// Yeh-Patt Branch Predictor


void yeh_patt::init_predictor(branchsim_conf *sim_conf) {
    /*
     * todo initilize the histry table it contains 2^G entris of shift GHR registers, which are p bit wide
     * todo initilize the pattern table, it caontains 2^p counters  each of them are initializeed at 1 (weakly not taken)
     * create a copy of G,P,N for later use when we update the tables
     * create a vector to hold the tags
     * create a variable to hold the recent prediction
     * create a variable to hold the mask value for the entire tables to make sure this
     */
    G = sim_conf->G;
    N = sim_conf->N;
    P = sim_conf->P;
    recent_branch = false;
    numHist = static_cast<uint64_t>(std::pow(2, G))-1;
    numPat = static_cast<uint64_t>(std::pow(2, P))-1;


    history = (Htable*)malloc((numHist+1) * sizeof(Htable));
    table = (patTable*)malloc((numPat+1) * sizeof(patTable));
    for(uint64_t i =0; i<=numHist; i++){
        history[i].btag =0;
        history[i].GHR = 0;
    }
    for(uint64_t i = 0; i<=numPat;i++){
        table[i].counter = (uint64_t)1;
    }
}

uint64_t yeh_patt::mask(branch *branch) {

    uint64_t PC = (numHist) & (branch->ip>>2);
    return PC;
}

bool yeh_patt::predict(branch *branch, branchsim_stats *sim_stats)
{
    /*
     * todo do the yeahpatt
     * calculate the index by shifting the pc by 2 bits and than masking it with the numIndex(max number of entries )
     * based on the index access the ghr (just have to make sure GHR is only p bits long)
     * based on the GHR value access the counter at that value
     * based on that value predict and update the stats
     */
    sim_stats->total_instructions = branch->inst_num;
    sim_stats->num_branch_instructions++;

    uint64_t index = mask(branch);
    uint64_t counter = history[index].GHR;

    uint64_t tag = (branch->ip)>>(G+2);
    if (history[index].btag != tag) { //TODO have to upadte the tag correctly its not the correct tag
        sim_stats->num_tag_conflicts++;
        history[index].btag = tag;
    }

    if(table[counter].counter>1){
        if(branch->is_taken)
            sim_stats->num_branches_correctly_predicted++;
        else
            sim_stats->num_branches_miss_predicted++;
        return TAKEN;
    }else{
        if(!branch->is_taken)
            sim_stats->num_branches_correctly_predicted++;
        else
            sim_stats->num_branches_miss_predicted++;
        return NOT_TAKEN;
    }

}

void yeh_patt::update_predictor(branch *branch)
{
    /*
     * todo update the History table and pattern table
     * calculate the index same way we calculated fot eh predict
     * based on the value of the original branch (taken or not tanken) update the pattern table first and then the GHR for that index
     */
    uint64_t  index = mask(branch);
    uint64_t counter = history[index].GHR;
    if(branch->is_taken){
        if (table[counter].counter<3)
            table[counter].counter = ((table[counter].counter) & (uint64_t)3) + (uint64_t)1;
        history[index].GHR = ((history[index].GHR<<1) | (uint64_t)1) & numPat;
    } else {
        if (table[counter].counter>0) {
            table[counter].counter = ((table[counter].counter) & (uint64_t)3) - (uint64_t) 1;
        }
        history[index].GHR = ((history[index].GHR<<1)) & numPat;
    }
}

yeh_patt::~yeh_patt()
{
//free any resources allocated dynamically on memory
free(history);
free(table);
}



// Perceptron Branch Predictor


void perceptron_predictor::init_predictor(branchsim_conf *sim_conf)
{
    //todo alot of stuff that may mor may not make sense
    /* a GHR Gbit long
     * have to create a perciptron table 2^p number of perceptron
     * have to make sure that each perciptron holds G+1 waites
     * have to initialize the tags in the perceptron table
     */
    G = sim_conf->G;
    N = sim_conf->N;
    P = sim_conf->P;
    threshold = static_cast<int>(1.93 * (int) G + 14);
    recent_branch = -1;
    GHR = (hist *) malloc((G)*sizeof(hist));
    for(int i =0; i<(int)G; i++){
        GHR[i].GHR = -1;
    }
    numperce = static_cast<uint64_t>(std::pow(2, P) - 1);
    table = (neutron *) malloc((numperce+1) * sizeof(neutron));
    for (uint64_t i = 0; i <= numperce; i++) {
        table[i].weights = (theta *) malloc((G+1 )* sizeof(theta));
        table[i].tag = 0;
        for (int j = 0; j <=(int)G; j++) {
            table[i].weights[j].theta = 0;
        }

    }

}

uint64_t perceptron_predictor::mask(branch *branch) {

    uint64_t PC = (numperce) & (branch->ip>>2);
    return PC;
}
bool perceptron_predictor::predict(branch *branch, branchsim_stats *sim_stats) {
    // todo inplemmet the perceptron algot=rithm
    /*
     * updating the stats
     */
    sim_stats->total_instructions = branch->inst_num;
    sim_stats->num_branch_instructions++;
    // getting the index for the perceptron's weights
    uint64_t index = mask(branch);
    //checking for the tag miss
    uint64_t tag = (branch->ip)>>(P+2);
    if (table[index].tag != tag) { //TODO have to upadte the tag correctly its not the correct tag
        sim_stats->num_tag_conflicts++;
        table[index].tag = tag;
    }
    // calculating the rediction
    int pred = 0;
    pred += table[index].weights[0].theta ;
    for (int i = 0; i < (int)G; i++) {
        pred += table[index].weights[i+1].theta * GHR[i].GHR;
    }
    /*
     * based on the prediction return the values
     * update the stats
     */
    recent_branch = pred;
    if (pred >= 0) {
        if (branch->is_taken)
            sim_stats->num_branches_correctly_predicted++;
        else
            sim_stats->num_branches_miss_predicted++;
        return TAKEN;
    } else {
        if(!branch->is_taken)
            sim_stats->num_branches_correctly_predicted++;
        else
            sim_stats->num_branches_miss_predicted++;

        return NOT_TAKEN;
    }
}

void perceptron_predictor::update_predictor(branch *branch)
{
    /*
     * implementing the update for perceptron predictor
     */
    uint64_t index = mask(branch);                                                              //getting the value of the current branch's index form the tag
    int sign  = (branch->is_taken)?1:-1;                                                        // getting waht is the actual value of branch (taken or not taken )
    if((std::signbit(recent_branch)!=std::signbit(sign))||std::abs(recent_branch)<=threshold){  // based on the sign or the values we train the predictor
        if((table[index].weights[0].theta+sign) < (-threshold-1)) {                             // checking if theta is lower than the lowest value possible
            table[index].weights[0].theta = -threshold-1;                                       // if it is than make it the lowest possible value
        } else if( (table[index].weights[0].theta+sign) > threshold){                           // if not than check if it greater than biggest possible value of the theta
            table[index].weights[0].theta = threshold;                                          // if it is than make it the biggst possible value
        } else                                                                                  // if not thank just calculate the theta
            table[index].weights[0].theta += sign;

        for(int i =0; i< (int)G ;i++){                                                          // do the same for the rest of the theta
            if((table[index].weights[i+1].theta + sign*GHR[i].GHR) < (-threshold-1)) {
                table[index].weights[i+1].theta = -threshold-1;
            } else if( (table[index].weights[i+1].theta + sign*GHR[i].GHR) > threshold){
                table[index].weights[i+1].theta = threshold;
            } else
                table[index].weights[i+1].theta += sign*GHR[i].GHR;
        }
    }
    /*
     * updating the GHR based on the actual value of the branch
     */
    for(int i =0; i<(int)(G-1);i++){
        GHR[i].GHR = GHR[i+1].GHR;
    }
    GHR[G-1].GHR = sign;
}

perceptron_predictor::~perceptron_predictor()
{
    /*
     * freeing the perceptron table and GHR array for the percepton
     */
    free(GHR);
    for(int i =0; i<=(int)numperce; i++ )
        free(table[i].weights);
    free(table);
}




// Common Functions to update statistics and final computations, etc.

/**
 *  Function to update the branchsim stats per prediction
 *
 *  @param perdiction The prediction returned from the predictor's predict function
 *  @param branch Pointer to the branch that is being predicted -- contains actual behavior
 *  @param stats Pointer to the simulation statistics -- update in this function
*/
void branchsim_update_stats(bool prediction, branch *branch, branchsim_stats *sim_stats)
{
    // stats are being updated in the functions
}

/**
 *  Function to cleanup branchsim statistic computations such as prediction rate, etc.
 *
 *  @param stats Pointer to the simulation statistics -- update in this function
*/
void branchsim_cleanup_stats(branchsim_stats *sim_stats)
{
    /*
     * calculating the resto of the remainnig stats for the simulater
     */
    sim_stats->fraction_branch_instructions = (double) sim_stats->num_branch_instructions / (double)sim_stats->total_instructions;
    sim_stats->misses_per_kilo_instructions =
            ((sim_stats->num_branches_miss_predicted * (1000)) /
             (sim_stats->total_instructions)) ;
    sim_stats->prediction_accuracy = (double) sim_stats->num_branches_correctly_predicted /(double)sim_stats->num_branch_instructions;
    if(sim_stats->N>7){
        sim_stats->stalls_per_miss_predicted_branch = (sim_stats->N/2)-1;
    } else
        sim_stats->stalls_per_miss_predicted_branch =2;
    sim_stats->average_CPI = 1.0 + ((double)(sim_stats->stalls_per_miss_predicted_branch*sim_stats->num_branches_miss_predicted)/(double)sim_stats->total_instructions);
}
