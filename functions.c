


#include "timer.h"



unsigned char lower(unsigned char n) { //To get the lower four bits of a member (units digit)
    return n&0x0F ;
}

unsigned char upper(unsigned char n) { //To get the upper four bits of a member (tens digit)
    return (n&0xF0)>>4 ;
}
void  bcd_inc(unsigned char *n) { //To increment a BCD number (if it exceeds 9 add 6)

   (*n)++;
   if(lower(*n) >= 0xA)
        (*n)+=6;
}

void time_inc(struct Time *t) { //To increment my time structure
	//First : increment the seconds
    bcd_inc( &(t->seconds) ) ;

    //Second : if seconds reach 60, clear them and increment minutes
    if(t->seconds ==0x60) {
        t->seconds = 0 ;
        bcd_inc(&(t->minutes));
    }

    //Third : if minutes reach 60, clear them and increment hours
    if(t->minutes ==0x60) {
        t->minutes = 0 ;
        bcd_inc(&(t->hours));
    }

    //Finally : if the hours exceed 99 , overflow occurs
    if(t->hours > 0x99) {
        t->hours = 0 ;
    }


}

