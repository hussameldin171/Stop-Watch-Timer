/*
 * timer.h
 *
 *  Created on: Sep 17, 2021
 *      Author: User
 */

#ifndef TIMER_H_
#define TIMER_H_



struct Time{
    unsigned char seconds, minutes , hours;

};
unsigned char lower(unsigned char n);
unsigned char upper(unsigned char n);
void  bcd_inc(unsigned char *n);
void time_inc(struct Time *t);



#endif /* TIMER_H_ */
