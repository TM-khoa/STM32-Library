#ifndef _PID_H_
#define _PID_H_
#include "main.h"
#ifdef CONFIG_USE_PID
#include "math.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct OutVal{
    double curr;
    double pre;
    double P;// set
    double I;// set
    double I_P;// previous intergration
    double D;
    double DF;//fitered derivative
    double DF_P;//previous fitered derivative
}OutVal;

typedef struct Encoder{

}Encoder;

typedef struct PID_Param{
    uint32_t setVal;
    uint32_t deltaT;
    double currVal;
    double e;
    double e_p;
    double kp;
    double ki;
    double kd;
    OutVal *u;
    Encoder *enc;
}PID_Param;


#define uI_LLim 10
#define uI_HLim -10
#define u_LLim 10
#define u_HLim -10

#define setVal  pid->setVal
#define currVal pid->currVal
#define deltaT  pid->deltaT
#define e       pid->e
#define e_p     pid->e_p
#define kp      pid->kp
#define ki      pid->ki
#define kd      pid->kd
#define uCurr   pid->u->curr
#define uPre    pid->u->pre
#define uP      pid->u->P
#define uI      pid->u->I
#define uI_P    pid->u->I_P
#define uD      pid->u->D
#define uDF     pid->u->DF
#define uDF_P   pid->u->DF_P

int gunCalPID(PID_Param *pid);
#endif
#endif
