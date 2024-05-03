#include <stddef.h>
#include <stdint.h>
// #include <stdio.h>
// #include <bits/libc-header-start.h>

uint32_t num_exits = 0; //variable for number of exits monitoring
char str1[100],str2[100] ; //strings for hypercall numExitsByType()
int count = 0;
// char *str = "Helloworld";

static void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

static void outl(uint16_t port, uint32_t value){
	asm("outl %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

void HC_print8bit(uint8_t val)
{
	outb(0xE9, val); //call outl assembly on port and pass the value val
}

void HC_print32bit(uint32_t val)
{
	// Fill in here
	outl(0x9D, val);		// Remove this
}

uint32_t HC_numExits()
{
	// Fill in here
	uint32_t *ptr = &num_exits; //take address of num_exits
	outl(0xEE,(uint32_t)(uintptr_t)ptr); //cast ptr value(absolute address of num_exits) to uint32_t and pass it to outl
	return num_exits;	// Remove this
}

void HC_printStr(char *str)
{
	outl(0xFF,(uint32_t)(uintptr_t)str); //cast str pointer's absolute value to uint32_t 
}

char *HC_numExitsByType()
{
	if(count == 0){ //this is for first call of this hypercall
	outl(0x3D,(uint32_t)(uintptr_t)str1);
		count++;
		return str1;
	}
	else{ //this is for second call of this hypercall	
		outl(0x3D,(uint32_t)(uintptr_t)str2);
		return str2;
	}
}

uint32_t HC_gvaToHva(uint32_t gva)
{
	outl(0x4D,(uint32_t)(uintptr_t)&gva);
	return gva;	
}

void HC_gvafromaddress(int *gva)
{
	outl(0x4A,(uintptr_t)gva);
	// return gva;	
}

void matmul(unsigned long long mat1[65536], unsigned long long mat2[65536])
{
    unsigned long long rslt[65536];
    for (int i = 0; i < 65536; i+=1024) {
        for (int j = 0; j < 65536; j+=1024) {
            rslt[i] = 0;
            for (int k = 0; k < 65536; k+=1024) {
                rslt[i] = (mat1[i]++) * (mat2[k]--);
				rslt[i]++;
            }
        }
    }
}

void run_sim(){
}

void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;
	// int a;
	// printf("The address of a is %p",&a);
	for (p = "Hello 695!\n"; *p; ++p)
		HC_print8bit(*p);

	/*---Your submission will fail the testcases if you modify this section---*/
	HC_print32bit(4294967295);

	uint32_t num_exits_a, num_exits_b;
	num_exits_a = HC_numExits();

	//this code is written to test the dirty pages
	// int i,/*c[65536][65535],b[65535][65535]*/count;
	unsigned long long b[65536], c[65536];
	// int a[5][5] = {{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
	// int b[5][5] = {{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};

	matmul(c,b);
	// for(i = 0; i < 65536; i+=1024){
	//  	c[i] = i;
	// 	if(c[i] < 0)
	// 		count++;
	// }
	// HC_gvafromaddress(&b[1]);
	// HC_gvafromaddress(&c[0]);
	char *str = "CS695 Assignment 2\n";

	num_exits_b = HC_numExits();
	HC_printStr(str);

	// for(i = 0; i < 65536; i+=1024){
	// 	b[i] = i + 1;
	// 	if(b[i] == 0)
	// 		count++;
	// }

	HC_print32bit(num_exits_a);
	HC_print32bit(num_exits_b);

	char *firststr = HC_numExitsByType();
	uint32_t hva;
	// hva = HC_gvaToHva(8192);  //0x2000
	// *(long *) 0x100000 = 52;  //writing at memory address 1MB
	hva = HC_gvaToHva(12288); //0x3000
	// hva = HC_gvaToHva(16384); //0x4000
	// hva = HC_gvaToHva(16392); //0x4008
	*(long *) 0x9000 = 62;  //writing at memory address 1MB
	HC_print32bit(hva);
	// hva = HC_gvaToHva(4294967295);
	// HC_print32bit(hva);
	*(long *) 0x10000 = 72;  //writing at memory address 1MB
	char *secondstr = HC_numExitsByType();

	HC_printStr(firststr);
	HC_printStr(secondstr);
	/*------------------------------------------------------------------------*/
	*(long *) 0x100000 = 42;  //writing at memory address 1MB

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
