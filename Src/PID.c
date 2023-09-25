#include "PID.h"
#ifdef CONFIG_USE_PID

#define e               pid->e
#define e_Pre           pid->e_Pre
#define deltaT          pid->deltaT
#define kP              pid->kP
#define uP              pid->uP
#define kI              pid->kI
#define uI              pid->uI
#define uI_Pre          pid->uI_Pre
#define kD              pid->kD
#define uD              pid->uD
#define uD_Fil          pid->uD_Fil
#define uD_FilPre 		pid->uD_FilPre
#define alpha   		pid->alpha
#define u				pid->u
#define u_AboveLimit 	pid->u_AboveLimit
#define u_BelowLimit 	pid->u_BelowLimit
#define uI_AboveLimit	pid->uI_AboveLimit
#define uI_BelowLimit	pid->uI_BelowLimit

/**
 * @brief Tính toán giá trị điện áp xuất ra bằng công thức PID
 * @param *pid con trỏ tới struct PID_Param
 * @param Target_set giá trị xác lập mong muốn
 * @param CurrVal_set giá trị feedback của hệ thống
 * @return
 */

void PID_Cal(PID_Param *pid,double Target_set,double CurrVal_set)
{
//-----------------------Input-------------------------//
	e = Target_set - CurrVal_set;

//-----------------------Propotion Term----------------//
	uP = kP*e;

//-----------------------Integral Term-----------------//
	uI = uI_Pre + kI*e*deltaT;
	uI = uI > uI_AboveLimit ? uI_AboveLimit : uI;
	uI = uI < uI_BelowLimit ? uI_BelowLimit : uI;

//-----------------------Derivative Term---------------//
	uD = kD*(e - e_Pre)/deltaT;
	uD_Fil = (1-alpha)*uD_FilPre+alpha*uD;

//-----------------------Previous Value----------------//
	e_Pre = e;
	uI_Pre = uI;
	uD_FilPre = uD_Fil;

//-----------------------Sum---------------------------//
	u = uP + uI + uD;
	u = u > u_AboveLimit ? u_AboveLimit : u;
	u = u < u_BelowLimit ? u_BelowLimit : u;
}

#endif
