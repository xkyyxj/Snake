#include "process.h"

void make_init(){
	struct tss init_tss;
	init_tss.cs = 0x0008;
	init_tss.ds = 0x0010;
}

void set_tss_des(struct tss *input){

}
