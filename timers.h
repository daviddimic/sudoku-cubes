#ifndef TIMERS_H_
#define TIMERS_H_

/* parametri tajmera rotacije */
#define TIMER_ROTATE_WAIT 50
#define TIMER_ROTATE_ID 0

/* parametri tajmera kretanja */
#define TIMER_MOVE_WAIT 20
#define TIMER_MOVE_ID 1

/* parametri tajmera spinovanja */
#define TIMER_SPIN_WAIT 20
#define TIMER_SPIN_ID 2

/* parametri tajmera skakanja */
#define TIMER_JUMP_WAIT 10
#define TIMER_JUMP_ID 3

/* gravitaciona konstanta - za kretanje kocke, spinovanje i skakanje */
#define G 9.81
/* pocetna brzina bacanja za spin */
#define v0spin 7
/* pocetna brzina skoka */
#define v0jump 9

/* globalne promenljive */
extern int timer_rotate_active;
extern int timer_move_active;
extern int timer_spin_active;
extern int timer_jump_active;
extern unsigned char wsad_key;


/* tajmer za okretanje kocke */
void on_timer_wsad(int value);
/* tajmer za spinovanje kocke oko y-ose */
void spin_timer(int value);
/* tajmer za kretanje kocke */
void on_timer_move(int value);
/* tajmer za skakanje kamere */
void on_timer_jump(int value);

#endif /*TIMERS_H_*/
