#ifndef RESET_H
#define RESET_H

void EiCp_initiateReset(void);
void EiCp_Reset(int flag);
void EiCp_setResetStart(void);
void EiCp_ResetStart(void);

void EiCp_initiateResetUser(size_t *p_ENV_bot, size_t *p_lut_bot, size_t *p_pp_bot, size_t *p_xalloc_bot); //maks
void EiCp_ResetUser(size_t u_ENV_bot, size_t u_lut_bot, size_t u_pp_bot, size_t u_xalloc_bot); //maks

#endif
