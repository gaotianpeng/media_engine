#ifndef _TIM_INT_C_H_
#define _TIM_INT_C_H_
#include "tim_comm_c.h"

extern "C" 
{
	TIM_DECL void	TIMIntInit();
	TIM_DECL void	TIMIntUninit();	
	TIM_DECL void	TIMRequest(const char* cmd, const char* req, uint32_t req_len, TIMRequestCB *callback);//Ä¬ÈÏ5Ãë³¬Ê±
	TIM_DECL void	TIMRequestWithCustomTimeout(const char* cmd, const char* req, uint32_t req_len, TIMRequestCB *callback, uint64_t timeout, bool resend);
	TIM_DECL uint64_t TIMTinyID();

}
#endif