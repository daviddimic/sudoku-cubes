#ifndef TIMERS_H_
#define TIMERS_H_

/* parametri tajmera rotacije */
#define TIMER_ROTATE_WAIT 50
#define TIMER_ROTATE_ID 0

/* parametri tajmera spinovanja */
#define TIMER_SPIN_WAIT 20
#define TIMER_SPIN_ID 2

/* parametri tajmera kretanja */
#define TIMER_MOVE_WAIT 20
#define TIMER_MOVE_ID 1

/* makro koji prverava da li je X izmedju A i B*/
#define IN(X, A, B) (((X) >= (A)) && ((X) <= (B)))
#define EPS 0.2

/* gravitaciona konstanta i broj PI - za kretanje kocke */
#define G 9.81
#define pi 3.141592654
/* pocetna brzina bacanja za spin */
#define v0spin 7

/* globalne promenljive */
extern int timer_rotate_active;
extern int timer_move_active;
extern int timer_spin_active;
extern unsigned char wsad_key;


/* tajmer za okretanje kocke */
void on_timer_wsad(int value);
/* tajmer za spinovanje kocke oko y-ose */
void spin_timer(int value);
/* tajmer za kretanje kocke */
void on_timer_move(int value);

#endif /*TIMERS_H_*/
