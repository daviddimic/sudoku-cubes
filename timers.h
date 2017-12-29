#ifndef TIMER_H_
#define  TIMER_H_

#define TIMER_ROTATE_WAIT 50
#define TIMER_ROTATE_ID 0

extern int timer_rotate_active;
extern unsigned char wsad_key;

/* tajmer za okretanje kocke */
void on_timer_wsad(int value);

#endif /*TIMER_H_*/
