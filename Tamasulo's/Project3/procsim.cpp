
#include <cstdio>
#include <cinttypes>
#include <iostream>
#include <deque>
#include <cmath>

#include "procsim.h"

uint64_t GLOBAL_CLOCK = 0;
bool RAISE_EXCEPTION = false;
bool ALL_DONE = false;

// You may define any global variables here

// We suggest using std::deque for the dispatch queue since instructions are both added and removed
// from it. Using an std::vector is really inefficient for popping from the front
// todo have to create strucks for rab, rat, pregfile
// todo create struct for the two queues Dqueue for dispatch and may be vector for scheduling
// also a vector for rob, it might be easy to use Array for rat so that we can index it easyly
//
// also need a helper function to travel trough the whole


typedef struct physical_register{
    uint64_t ready;
    uint64_t preg_number;
    bool free;
}preg;

typedef struct Function_Units{
    instruction Function_inst;
    int64_t dest_preg;
    uint64_t tag;// curent cycle
    uint64_t free;
    //todo
    // type of function unit
    // latency
    // actual time spent (what clock cycle of the executin we are in
}FU;

struct FU_cache{
    FU * blocks;   // this will be initilized based on latencey
};

typedef struct schedule_cell{
    //todo
    // need variable to hold what type of fu we are using
    // need a varable for the destination preg
    // need a avriables for two source pregs
    instruction SChedule_inst;
    uint64_t FU_type;
    uint64_t tag;
    int64_t src[2];
    uint64_t src1_ready;
    uint64_t src2_ready;
    int64_t dest_preg;
    uint64_t  fired = false;
    uint64_t complete =false;
}schedule_cell;


typedef struct ROB{
    // todo
    //  Areg
    //  preg
    //  privious Preg for this Areg
    // ready bit
    // value may be?
    instruction  Rob_inst;
    int64_t areg;
    uint64_t tag;
    uint64_t privious_preg;
    uint64_t ready = false;

    //not sure about the value


}rob;


/**
 * Subroutine for initializing the processor. You many add and initialize any global or heap
 * variables as needed.
 *
 * @param conf Pointer to the configuration. Read only in this function
 */

std::deque<schedule_cell> scheduleQueue;
std::deque<instruction> dispatchQueue;
int64_t rat[32];
std::deque<rob> reorderBuffer;
std::deque<instruction> retired;
preg * Physical;    // array for physical register (easy to iterate through)
FU * add;
FU * store;// adder or branch
FU_cache * MUl;   // multipy
FU_cache * LSB;   // load/store



// this is just to make sure that we dont though first 0-31 Pregs unintentionally

// simulation configuration just in case
// delete it if not needed
uint64_t fetchRate;
uint64_t numPregs;
uint64_t numAdder;
uint64_t numMul;
uint64_t numLSB;
uint64_t Robsize;
uint64_t Icycle; //
uint64_t maxDispatch;
uint64_t schedueSize;
uint64_t tag=0;

uint64_t instruction_num=0;


void setup_proc(const procsim_conf *conf)
{
   // std::cout<< "inside the setup function";
    GLOBAL_CLOCK = 0;
    RAISE_EXCEPTION = false;
    ALL_DONE = false;

    // Student code should go below this line
    //todo need dispach queue
    //todo need seqduling queue
    //todo need rob
    //todo needs rat
    //todo pregfile
    // todo need fus based on the J, K, L values and their attributes
    // Note - The schedule queue can hold a maximum of 2 * (J + K + L) instructions

    /*
     * have to init first 32 pregs as aregs
     */
    fetchRate = conf->F;
    numPregs = conf->P;
    numAdder = conf->J;
    numMul = conf->K;
    numLSB = (conf->L) ;
    Robsize = conf->R;
    Icycle = conf->I;

    schedueSize = 2*(numAdder + numMul + numLSB);

    Physical  = (physical_register*) malloc((conf->P)*sizeof(physical_register));

    for(uint64_t i= 0; i< conf->P ; i++){
        Physical[i].free = true;
        Physical[i].preg_number = i;
        Physical[i].ready = 0;
    }
    //first 32 pregs are Aregs
    // so have to initilize them
    for(uint64_t i = 0 ; i< 32; i++)
    {
        Physical[i].ready = 1;
        Physical[i].free = false;
        rat[i] = Physical[i].preg_number; //this is just to make sure there is no grabage value in it.
    }


    add = (FU *) malloc((numAdder)*sizeof(Function_Units));
    for(uint64_t i =0; i<numAdder;i++){
        add[i].tag = 0;
        add[i].free = true;
    }
    //std::cout<< "free adder"<< numAdder << std::endl;
    MUl = (FU_cache *) malloc((numMul) * sizeof(FU_cache));
    LSB = (FU_cache*) malloc((numLSB)*sizeof(FU_cache));
    store = (FU*)malloc(sizeof(FU));

    for(uint64_t i=0; i<numMul; i++){
        MUl[i].blocks = (FU*)malloc(3 * sizeof(FU));
        for(uint64_t j=0; j<3; j++){
            MUl[i].blocks[j].tag = 0;
            MUl[i].blocks[j].free = true;
        }
    }
    for(uint64_t i=0; i<numLSB; i++){
        LSB[i].blocks = (FU*)malloc(2 * sizeof(FU));
        for(uint64_t j=0; j<2; j++){
            LSB[i].blocks[j].tag = 0;
            LSB[i].blocks[j].free = true;
        }
    }

}

// Write helper methods for each superscalar operation here

uint64_t fetch_done;

static void fetch(procsim_stats *stats)
{
    // Sample for how instructions are read from the trace
    //todo put the instruction into dispatch queue
    // have to parse the stuff and update the overall intrcint to put it into the Dqueue
    // have to check if there is an exception then have to send the I instruction;
    std::cout<<"Fetch" << std::endl;
    std::cout << "Clocl CYCYL = " << GLOBAL_CLOCK <<std::endl;

        for (uint64_t i = 0; i < fetchRate; i++) {
            {
                instruction *new_inst = new instruction;
                if (read_instruction(new_inst)) {
//                    fetch_done = false;
                    instruction_num++;
                    new_inst->fetch_cycle = GLOBAL_CLOCK;
                    new_inst->inst_number = instruction_num;
                    std::cout<< "inside the fetch instruction = " << new_inst->inst_number <<std::endl;
                    dispatchQueue.push_back(*new_inst);
                } else
                    fetch_done =true;
            }
        }

}
bool interept_dispacthed = false;
uint64_t dispQ_len = 0;
uint64_t dipatch_done;
static void dispatch(procsim_stats *stats)
{
    // todo dispatch as many instruction as possible into schedule queue
    // needs to update pregs, rats and rob based on the lecture noted
    // Each ROB entry must store the Areg number and the previous Preg for that Areg. and have to store more values as needed

    std::cout << "Clocl CYCYL = " << GLOBAL_CLOCK <<std::endl;
    bool rob_found = false;

    maxDispatch = 0;

    bool Preg_found = false;
    dispQ_len = dispQ_len + dispatchQueue.size();
    if(dispatchQueue.empty()){
        dipatch_done =true;
    }else{
        dipatch_done = false;
    }
    if(dispatchQueue.size() > maxDispatch){
        maxDispatch = dispatchQueue.size();
    }


    if(!RAISE_EXCEPTION){
        interept_dispacthed = false;
        while((scheduleQueue.size() < schedueSize) && (!dispatchQueue.empty()) && (reorderBuffer.size() < Robsize)){  // have to stall after interupt is being dipached

            instruction inst = dispatchQueue.front();
            schedule_cell temp_sch;
            rob temp_rob;
            inst.dispatch_cycle = GLOBAL_CLOCK;
            temp_sch.SChedule_inst = inst;

            if(inst.opcode == 2 || inst.opcode == 6 || inst.opcode == 7){
                temp_sch.FU_type = 0;
                if(inst.opcode == 6){
                    stats->branch_instructions++;
                }else if(inst.opcode==7){
                    stats->num_exceptions++;
                }
            }
            else if(inst.opcode==4 || inst.opcode == 5)
            {
                temp_sch.FU_type = 2;
                if(inst.opcode==4){
                    stats->load_instructions++;
                }
                else{
                    stats->store_instructions++;
                }
            }
            else if(inst.opcode == 3){
                temp_sch.FU_type = 1;
            }
            else{
                continue; // this is if we encounter any nops than just break the loop;
            }

//            if(inst.dest_reg!=-1){
//                for(uint64_t k=32; k<numPregs; k++){
//                    if(Physical[k].free){
//                        Preg_found = true;
//                        rat[inst.dest_reg] = Physical[k].preg_number;
//                        Physical[k].ready = false;
//                        Physical[k].free = false;
//                        temp_sch.dest_preg = Physical[k].preg_number;
//                        break;
//                    }
//                }
//
//                if(!Preg_found){
//                    std::cout<< "in dipatch assigning the preg found  = " <<Preg_found <<std::endl;
//                    continue; // if preg is not foud then stall or just keep continueing until found
//                }
//            } // what to  do is the destination register is -1



            for(uint64_t i = 0; i< 2 ;i++){
                rob_found = false;
                if (inst.src_reg[i] == -1){
                    if(i == 0)
                        temp_sch.src1_ready = true;
                    if(i == 1)
                        temp_sch.src2_ready = true;
                } else{
                    for(uint64_t j = 0; j< reorderBuffer.size(); j++) {
                        if (reorderBuffer[j].areg == inst.src_reg[i]) {

                            if (reorderBuffer[j].ready) {
                                rob_found = true;
                                //std::cout<< "in dipatch assigning the preg for the src if rob is ready  = " << Physical[rat[reorderBuffer[j].areg]].preg_number <<std::endl;
                                temp_sch.src[i] = Physical[rat[reorderBuffer[j].areg]].preg_number;
                                if (i == 0)
                                    temp_sch.src1_ready = true;
                                if (i == 1)
                                    temp_sch.src2_ready = true;
                                break;
                            } else {
                                //std::cout<< "in dipatch  assigning the preg for the src if rob is  not ready  = " << reorderBuffer[j].privious_preg <<std::endl;
                                temp_sch.src[i] =  reorderBuffer[j].privious_preg; // is this correct?
                                if (i == 0)
                                    temp_sch.src1_ready = false;
                                if (i == 1)
                                    temp_sch.src2_ready =false;

                                break;
                            }
                        }
                    }
                    if(!rob_found){
                        //std::cout<< "in dipatch  assigning the preg for the src if not found in rob  = " << Physical[rat[inst.src_reg[i]]].preg_number <<std::endl;
                        temp_sch.src[i] = Physical[rat[inst.src_reg[i]]].preg_number;
                        if(i==0)
                            temp_sch.src1_ready = Physical[rat[inst.src_reg[i]]].ready;
                        if(i==1)
                            temp_sch.src2_ready = Physical[rat[inst.src_reg[i]]].ready;
                    }
                }
            }

            temp_rob.areg = inst.dest_reg;

            if(temp_rob.areg != -1){
                temp_rob.privious_preg = rat[inst.dest_reg] ;
            }

            if(inst.dest_reg!=-1){
                for(uint64_t k=32; k<numPregs; k++){
                    if(Physical[k].free){
                        Preg_found = true;
                        rat[inst.dest_reg] = Physical[k].preg_number;
                        Physical[k].ready = false;
                        Physical[k].free = false;

                        temp_sch.dest_preg = Physical[k].preg_number;
                        break;
                    }
                }

                if(!Preg_found){
                    std::cout<< "in dipatch assigning the preg found  = " <<Preg_found <<std::endl;
                    continue; // if preg is not foud then stall or just keep continueing until found
                }
            }
            temp_rob.tag =tag;
            temp_sch.fired = false;
            temp_sch.tag = tag;
            std::cout<< "inside the dispatch instruction = " << temp_sch.SChedule_inst.inst_number<<" with opcede = "<<temp_sch.SChedule_inst.opcode
                     <<"with dest_preg = "<< temp_sch.dest_preg<< " and src_preg for src0 = "<< temp_sch.src[0]<<" and src_preg for src1 = "<<temp_sch.src[1]
                     <<std::endl;
            scheduleQueue.push_back(temp_sch);
            temp_rob.ready = false;
            temp_rob.Rob_inst = inst;
            reorderBuffer.push_back(temp_rob);
            tag++;


            dispatchQueue.pop_front(); // is this need to be done here or in the clean up?
        }
    }else if(!interept_dispacthed && dispatchQueue.front().opcode == 7){
        schedule_cell temp_sch;
        rob temp_rob;
        interept_dispacthed = true;
        instruction temp = dispatchQueue.front();
        temp.dispatch_cycle = GLOBAL_CLOCK;
        temp_sch.SChedule_inst = temp;
        temp_sch.tag = tag;
        temp_sch.dest_preg = -1;
        temp_sch.src[0] = -1;
        temp_sch.src[1] = -1;
        temp_sch.src1_ready = true;
        temp_sch.src2_ready = true;
        temp_sch.FU_type = 0;
        temp_sch.fired = false;
        temp_sch.complete = false;
        temp_sch.SChedule_inst.dispatch_cycle = GLOBAL_CLOCK;
        scheduleQueue.push_back(temp_sch);
        temp_rob.areg = temp.dest_reg;
        temp_rob.ready = false;
        temp_rob.tag = tag;
        temp_rob.Rob_inst = temp;
        reorderBuffer.push_back((temp_rob));
        tag++;
        dispatchQueue.pop_front();
    }
}
uint64_t schedule_done = false;
static void schedule(procsim_stats *stats) {
    // todo have to fire any available instruction as they come in(from the trace if add and mul both are ready and
    //  if add is the first one to arrive than fire it first)
    // dont throw the fired instruction here throw it in execute after the last cycle.
    // follow the latency table for the how much it will take to finish the instruction that is in fu
    std::cout<< "inside the schedule q with size =" << scheduleQueue.size()<< std::endl;
    std::cout << "Clocl CYCYL = " << GLOBAL_CLOCK <<std::endl;
    for (auto i = scheduleQueue.begin(); i != scheduleQueue.end() ; /*NOTE: no incrementation of the iterator here*/) {
        if (i->complete) {
            std::cout << "inside the schedule q completing instruction  = " << i->SChedule_inst.inst_number
                      << std::endl;
            i = scheduleQueue.erase(i);
        }// erase returns the next iterator
        else
            ++i; // otherwise increment it by yourself
    }
    schedule_done = scheduleQueue.empty();
    for (uint64_t i = 0; i < scheduleQueue.size(); i++) {
        schedule_cell temp_cel = scheduleQueue[i];
        // if both of the register are ready and if the instruction is not fired before than fire the instruction
        //get the needed FU that is available for the instruction
        // update the state of the instruction
        // fire as much as possible
         // is this what we have to do
        //std::cout<< "inside the schedule q with instruction =" << scheduleQueue[i].SChedule_inst.inst_number<< std::endl;

        std::cout<< "inside the schdule instruction = " << temp_cel.SChedule_inst.inst_number<<" with opcode = "<<temp_cel.SChedule_inst.opcode
                 <<"with dest_preg = "<< temp_cel.dest_preg<< " and src_preg for src0 = "<< temp_cel.src[0]<<" which ready"<<temp_cel.src1_ready
                 <<" and src_preg for src1 = "<<temp_cel.src[1] << " which is ready" << temp_cel.src2_ready
                 <<std::endl;
        if ((temp_cel.src1_ready) && temp_cel.src2_ready && temp_cel.fired == 0) {
            scheduleQueue[i].SChedule_inst.schedule_cycle = GLOBAL_CLOCK;
            if (temp_cel.FU_type == 0) {

                for (uint64_t j = 0; j < numAdder; j++) {
                    std::cout<< "finding the free adder j = "<< j <<std::endl;
                    if (add[j].free) {
                        add[j].free = false;
                        add[j].Function_inst = temp_cel.SChedule_inst;
                        add[j].dest_preg = temp_cel.dest_preg;
                        add[j].tag = temp_cel.tag;
                        scheduleQueue[i].fired = true;
                        break;
                    }
                }
            } else if (temp_cel.FU_type == 1) {
                for (uint64_t j = 0; j < numMul; j++) {
                    std::cout<< "finding the free Mul = "<<j<< std::endl;
                    if (MUl[j].blocks[0].free) {
                        MUl[j].blocks[0].free = false;
                        MUl[j].blocks[0].Function_inst = temp_cel.SChedule_inst;
                        MUl[j].blocks[0].dest_preg = temp_cel.dest_preg;
                        MUl[j].blocks[0].tag = temp_cel.tag;
                        scheduleQueue[i].fired = true;
                        break;
                    }
                }
            } else if (temp_cel.FU_type == 2) {
                std::cout<< "finding the free load or store opcode"<< temp_cel.SChedule_inst.opcode<< std::endl;
                if (temp_cel.SChedule_inst.opcode == 4) {

                    for (uint64_t j = 0; j < numLSB; j++) {
                        std::cout<< "finding the free load = "<< j << std::endl;
                        if (LSB[j].blocks[0].free) {
                            LSB[j].blocks[0].free = false;
                            LSB[j].blocks[0].Function_inst = temp_cel.SChedule_inst;
                            LSB[j].blocks[0].dest_preg = temp_cel.dest_preg;
                            LSB[j].blocks[0].tag = temp_cel.tag;
                            scheduleQueue[i].fired = true;
                            break;
                        }
                    }
                } else if (temp_cel.SChedule_inst.opcode == 5) {
                    if (store->free) {
                        std::cout<< "finding the free store"<< std::endl;
                        store->free = false;
                        store->Function_inst = temp_cel.SChedule_inst;
                        store->dest_preg = temp_cel.dest_preg;
                        store->tag = temp_cel.tag;
                        scheduleQueue[i].fired = true;
                    }
                }
            }
        }else if(!temp_cel.src1_ready){
            temp_cel.src1_ready = Physical[temp_cel.src[0]].ready;
        } else if (!temp_cel.src2_ready){
            temp_cel.src2_ready = Physical[temp_cel.src[1]].ready;
        }


        /*
         * i think i have to cheak if any of the instruction is complete if yes than remove them from the RS but not sure how to do erase in dqueue
         */
            // how to do erase in schedul queue

    }

}

static void execute(procsim_stats *stats)
{
    // todo update pregs,rs, rob when they are completed how though?
    // have oto check each array of fu's and see if its completed or not
    // if yes then update the RS in this cycle mark it as complete
    // update the rob and pregs readybits


    std::cout << "this is execute function" << std::endl;
    std::cout << "Clocl CYCYL = " << GLOBAL_CLOCK <<std::endl;
    for(uint64_t i = 0; i<numAdder ; i++) {
        //checking if it has spent enough time
        // if not then dont do any thing
        // if yes then do lot of stuff
        if (add[i].free == 0) {
            add[i].free = 1;
            for(uint64_t j = 0; j < reorderBuffer.size(); j++) {
                if (add[i].Function_inst.inst_number == reorderBuffer[j].Rob_inst.inst_number) {
                    std::cout << "this is execute function and tag match addintruction = "<< (add[i].Function_inst.inst_number)  << std::endl;
                    //reorderBuffer[j].Rob_inst = add[i].Function_inst;
                    reorderBuffer[j].Rob_inst.execute_cycle = GLOBAL_CLOCK;
                    reorderBuffer[j].ready = true;
                    std::cout<< "updating Areg = " << reorderBuffer[j].areg << "for instruction "<<reorderBuffer[j].Rob_inst.inst_number <<std::endl;
                    if(reorderBuffer[j].areg != -1) {
                        std::cout<< "updating preg = " << Physical[rat[reorderBuffer[j].areg]].preg_number << "for instruction "<<reorderBuffer[j].Rob_inst.inst_number <<std::endl;
                        Physical[rat[reorderBuffer[j].areg]].ready = true;
                    }
                }
            }

            for (uint64_t j = 0; j < scheduleQueue.size(); j++) {
                //finding if any instruction matches this instruction in schedule queue if yes than make its complete bit true so that we can erase it in schdue in next cycle
                if (add[i].Function_inst.inst_number == scheduleQueue[j].SChedule_inst.inst_number ){
                    scheduleQueue[j].complete = true;
                    scheduleQueue[j].SChedule_inst.completeed = true;
                }
                // checking if any other instruction is using this destination preg
                // if yes that make them ready too for each entry

                for (int k = 0; k < 2; k++) {
                    if (scheduleQueue[j].src[k] == add[i].dest_preg) {
                        if (k == 0)
                            scheduleQueue[j].src1_ready = true;
                        if (k == 1)
                            scheduleQueue[j].src2_ready = true;
                    }
                }
            }
                // updating the rob by making it ready
                // updating the prf based on the areg in the rob and making it ready
        std::cout<< "inside the schdule instruction = " << add[i].Function_inst.inst_number<<" with opcode = "<<add[i].Function_inst.opcode
                 <<"with dest_preg = "<< add[i].dest_preg
                 <<std::endl;

        }
    }
    // going threu all the multiplicator
    // checking the last one first if its free or not
    //if its free then move on to the next one to overwrite this one
    for(uint64_t i =0; i<numMul; i++){
        if (MUl[i].blocks[2].free == 0) {
            // going threu the whole rob queue to update the status of that rob to ready and update the preg for that areg of the instruction
            for(uint64_t j=0; j< reorderBuffer.size() ; j++){
                if(MUl[i].blocks[2].Function_inst.inst_number == reorderBuffer[j].Rob_inst.inst_number){
                    std::cout << "this is execute function and tag match MULinstrution = "<< (MUl[i].blocks[2].Function_inst.inst_number)  << std::endl;
                    reorderBuffer[j].ready = true;
                    //reorderBuffer[j].Rob_inst = MUl[i].blocks[2].Function_inst;
                    reorderBuffer[j].Rob_inst.execute_cycle = GLOBAL_CLOCK;
                    std::cout<< "updating Areg = " << reorderBuffer[j].areg << "for instruction "<<reorderBuffer[j].Rob_inst.opcode <<std::endl;
                    if(reorderBuffer[j].areg != -1) {
                        std::cout<< "updating preg = " << Physical[rat[reorderBuffer[j].areg]].preg_number << "for instruction "<<reorderBuffer[j].Rob_inst.inst_number <<std::endl;
                        Physical[rat[reorderBuffer[j].areg]].ready = true;
                    }
                }
            }
            // after reorder buffer we are going to the whole scheduling queue to make this instruction complete and to check if any other instruction needed this pregthan update that too
            for (uint64_t j = 0; j < scheduleQueue.size(); j++) {
                if (MUl[i].blocks[2].Function_inst.inst_number == scheduleQueue[j].SChedule_inst.inst_number) {
                    scheduleQueue[j].complete = true;
                    scheduleQueue[j].SChedule_inst.completeed = true;
                }
                for (int k = 0; k < 2; k++) {
                    if (scheduleQueue[j].src[k] == MUl[i].blocks[2].dest_preg) {
                        if (k == 0) {
                            std::cout<<"updating schedule Q's src0 = " << scheduleQueue[j].src[k]<<std::endl;
                            scheduleQueue[j].src1_ready = true;
                        }
                        if (k == 1) {
                            std::cout<<"updating schedule Q's src0 = " << scheduleQueue[j].src[k]<<std::endl;
                            scheduleQueue[j].src2_ready = true;
                        }
                    }
                }
            }
            MUl[i].blocks[2].free = true;
        }
        //this is just movinf the stuff in pipelin this one is doing 2cycle to third cycle
        if(MUl[i].blocks[2].free != 0){
            MUl[i].blocks[2].Function_inst = MUl[i].blocks[1].Function_inst;
            MUl[i].blocks[2].dest_preg = MUl[i].blocks[1].dest_preg;
            MUl[i].blocks[2].tag = MUl[i].blocks[1].tag;
            MUl[i].blocks[2].free = false;
            MUl[i].blocks[1].free = true;

        }
        // this one if first ot the second one
        if( MUl[i].blocks[1].free != 0){   // not sure if this is correct
            MUl[i].blocks[1].Function_inst = MUl[i].blocks[0].Function_inst;
            MUl[i].blocks[1].dest_preg = MUl[i].blocks[0].dest_preg;
            MUl[i].blocks[1].tag = MUl[i].blocks[0].tag;
            MUl[i].blocks[1].free = false;
            MUl[i].blocks[0].free = true;
        }// just making sure if the first block of the three mul is ready have to ask if this is correct though
    }

    for(uint64_t i=0;i< numLSB; i++){
        if(LSB[i].blocks[1].free == 0){
            // going threu the whole rob queue to update the status of that rob to ready and update the preg for that areg of the instruction
            for(uint64_t j=0; j< reorderBuffer.size() ; j++){
                if(LSB[i].blocks[1].tag == reorderBuffer[j].tag){
                   // reorderBuffer[j].Rob_inst = LSB[i].blocks[1].Function_inst;
                    reorderBuffer[j].Rob_inst.execute_cycle = GLOBAL_CLOCK;
                    reorderBuffer[j].ready = true;
                    std::cout<< "updating Areg = " << reorderBuffer[j].areg << "for instruction "<<reorderBuffer[j].Rob_inst.opcode <<std::endl;
                    if(reorderBuffer[j].areg != -1) {
                        std::cout<< "updating preg = " << Physical[rat[reorderBuffer[j].areg]].preg_number << "for instruction "<<reorderBuffer[j].Rob_inst.inst_number <<std::endl;
                        Physical[rat[reorderBuffer[j].areg]].ready = true;
                    }
                    std::cout << "this is execute function and tag match load instruction = "<< (LSB[i].blocks[1].Function_inst.inst_number)  << std::endl;
                } // do i ahve to erase it from this
            }
            // after reorder buffer we are going to the whole scheduling queue to make this instruction complete and to check if any other instruction needed this pregthan update that too
            for (uint64_t j = 0; j < scheduleQueue.size(); j++) {
                if (LSB[i].blocks[1].Function_inst.inst_number == scheduleQueue[j].SChedule_inst.inst_number) {
                    scheduleQueue[j].complete = true;
                    scheduleQueue[j].SChedule_inst.completeed = true;
                }

                    for (int k = 0; k < 2; k++) {
                        if (scheduleQueue[j].src[k] == LSB[i].blocks[1].dest_preg) {
                            if (k == 0)
                                scheduleQueue[j].src1_ready = true;
                            if (k == 1)
                                scheduleQueue[j].src2_ready = true;
                        }
                    }

                LSB[i].blocks[1].free = true;
            }
        }
        if( LSB[i].blocks[1].free != 0){
            LSB[i].blocks[1].Function_inst = LSB[i].blocks[0].Function_inst;
            LSB[i].blocks[1].dest_preg = LSB[i].blocks[0].dest_preg;
            LSB[i].blocks[1].tag = LSB[i].blocks[0].tag;
            LSB[i].blocks[1].free = false;
            LSB[i].blocks[0].free = true;
        }
    }
    if(store->free == 0) {
        {
            for (uint64_t j = 0; j < reorderBuffer.size(); j++) {
                if (store->Function_inst.inst_number == reorderBuffer[j].Rob_inst.inst_number) {
                    std::cout << "this is execute function and tag match store= "<< store->Function_inst.inst_number  << std::endl;
                    //reorderBuffer[j].Rob_inst = store->Function_inst;
                    reorderBuffer[j].Rob_inst.execute_cycle = GLOBAL_CLOCK;
                    reorderBuffer[j].ready = true;
                    std::cout<< "updating Areg = " << reorderBuffer[j].areg << "for instruction "<<reorderBuffer[j].Rob_inst.opcode <<std::endl;
                    if(reorderBuffer[j].areg != -1) {
                        std::cout<< "updating preg = " << Physical[rat[reorderBuffer[j].areg]].preg_number << "for instruction "<<reorderBuffer[j].Rob_inst.inst_number <<std::endl;
                        Physical[rat[reorderBuffer[j].areg]].ready = true;
                    }
                }
            }
            // after reorder buffer we are going to the whole scheduling queue to make this instruction complete and to check if any other instruction needed this pregthan update that too
            for (uint64_t j = 0; j < scheduleQueue.size(); j++) {
                if (store->Function_inst.inst_number == scheduleQueue[j].SChedule_inst.inst_number) {
                    scheduleQueue[j].complete = true;
                    scheduleQueue[j].SChedule_inst.completeed = true;
                }

                    for (int k = 0; k < 2; k++) {
                        if (scheduleQueue[j].src[k] == store->dest_preg) {
                            if (k == 0)
                                scheduleQueue[j].src1_ready = true;
                            if (k == 1)
                                scheduleQueue[j].src2_ready = true;
                        }
                    }

            }
            store->free = true;
        }
    }
}



uint64_t rob_done=false;
static void state_update(procsim_stats *stats)
{
    // retireing the instructions and freeing the pregs for future use
    //go through the head of the rab queue checkif its ready if yest than erase it form the rob from the head to the first non ready instruction

    if(reorderBuffer.empty()){
        rob_done = true;
    }
    else{
        rob_done =false;
    }
    std::cout<< "inside the state update with rob isntruction = " << reorderBuffer.front().Rob_inst.inst_number << std::endl;
    std::cout << "Clocl CYCYL = " << GLOBAL_CLOCK <<std::endl;
    while(!reorderBuffer.empty() && reorderBuffer.front().ready) {
        stats->instructions_retired++;
        if (reorderBuffer.front().Rob_inst.opcode != 7) {
            if (reorderBuffer.front().Rob_inst.dest_reg != -1 && reorderBuffer.front().privious_preg >=32 ) {
                Physical[reorderBuffer.front().privious_preg].free = true;
            }
            retired.push_back(reorderBuffer.front().Rob_inst);
            reorderBuffer.pop_front();
        }
        else {
            RAISE_EXCEPTION = false;
            retired.push_back(reorderBuffer.front().Rob_inst);
            reorderBuffer.pop_front();
            stats->instructions_retired++;
            }
        }
}


/**
 * Subroutine that simulates the processor. The processor should fetch instructions as
 * appropriate, until all instructions have executed
 *
 * @param stats Pointer to the statistics structure
 * @param conf Pointer to the configuration. Read only in this function
 */
void run_proc(procsim_stats *stats, const procsim_conf *conf)
{
    do {
        state_update(stats);
        execute(stats);
        schedule(stats);
        dispatch(stats);
        fetch(stats);

        GLOBAL_CLOCK++; // Clock the processor

        // Raise an exception/interrupt every 'I' clock cycles
        // When the RAISE_EXCEPTION FLAG is raised -- an Interrupt instruction is added
        // to the front of the dispatch queue while the schedule queue and ROB are flushed
        // Execution resumes starting from the Interrupt Instruction. The flushed instructions
        // are re-executed
        if ( (GLOBAL_CLOCK % conf->I) == 0 ) {
            RAISE_EXCEPTION = true;

            instruction temp;
            temp.opcode = 7;
            temp.dest_reg = -1;
            temp.src_reg[0] = -1;
            temp.src_reg[1] = -1;
            temp.completeed = false;
            temp.inst_addr = tag;
            temp.fetch_cycle = GLOBAL_CLOCK;
            instruction_num++;
            temp.inst_number = instruction_num;
            std::cout<< "inside the fetch instruction sending interupt as instruction = " << temp.inst_number <<std::endl;
            dispatchQueue.push_back(temp);

        }
        stats->cycle_count = GLOBAL_CLOCK;
        if(fetch_done){
            ALL_DONE = true;
        }
        // Run the loop until all the instructions in the trace have retired
        // Feel free to replace the condition of the do-while loop
    } while (!ALL_DONE);
}

/**
 * Subroutine for cleaning up any outstanding instructions and calculating overall statistics
 * such as average IPC, average fire rate etc.
 *
 * @param stats Pointer to the statistics structure
 */
void complete_proc(procsim_stats *stats)
{
    stats->max_disp_queue_size = maxDispatch;
    stats->average_disp_queue_size = (double)dispQ_len /(double) stats->cycle_count;

    free(add);
    free(store);
    for(uint64_t i=0; i<numMul; i++){
        free(MUl[i].blocks);
    }
    free(MUl);
    for(uint64_t i = 0 ; i<numLSB;i++){
        free(LSB[i].blocks);
    }
    free(LSB);
    free(Physical);
}
