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

static inline uint32_t inb(uint16_t port) {
  uint32_t ret;
  asm("in %1, %0" : "=a"(ret) : "Nd"(port) : "memory" );
  return ret;
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

void HC_gvafromaddress_ul(unsigned long long *gva)
{
	outl(0xFE,(uintptr_t)gva);
	// return gva;	
}

void HC_getRandom(uint32_t *x){
	*x = inb(0x88);
}

void HC_gvafromaddress(int *gva)
{
	outl(0x4A,(uintptr_t)gva);
	// return gva;	
}

void matmul_intarray(int *mat1, int *mat2, int size_i)
{
    int rslt[size_i];
	// HC_gvafromaddress(&rslt[0]);
	// HC_gvafromaddress(&rslt[size_i-1]);
    for (int i = 0; i < size_i; i+=1024) {
        for (int j = 0; j < size_i; j+=1024) {
            rslt[i] = 0;
            for (int k = 0; k < size_i; k+=1024) {
                rslt[i] = (mat1[i]++) * (mat2[k]--);
				rslt[i]++;
            }
        }
    }
}

void matmul_doublearray(double *mat1, double *mat2, int size_d)
{
    double rslt[size_d];
    for (int i = 0; i < size_d; i+=512) {
        for (int j = 0; j < size_d; j+=512) {
            rslt[i] = 0;
            for (int k = 0; k < size_d; k+=512) {
                rslt[i] = (mat1[i]++) * (mat2[k]--);
				rslt[i]++;
            }
        }
    }
}

void matmul_longarray(unsigned long long *mat1, unsigned long long *mat2, int size_ul)
{
    unsigned long long rslt[size_ul];
    for (int i = 0; i < size_ul; i+=512) {
        for (int j = 0; j < size_ul; j+=512) {
            rslt[i] = 0;
            for (int k = 0; k < size_ul; k+=512) {
                rslt[i] = (mat1[i]++) * (mat2[k]--);
				rslt[i]++;
            }
        }
    }
}



void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {

	// int a;
	// printf("The address of a is %p",&a);
	// const char *p;
	// for (p = "Hello 695!\n"; *p; ++p)
	// 	HC_print8bit(*p);

	/*---Your submission will fail the testcases if you modify this section---*/
	HC_print32bit(4294967295);

	uint32_t num_exits_a, num_exits_b;
	num_exits_a = HC_numExits();


	//this code is written to test the dirty pages
	//setup for 1GB page
	int size_i = 131072;
	int size_ul = 16384;
	// int size_d = 8;
	
	//setup for 2MB page
	// int size_i = 131072;
	// int size_ul = 8192;

	int b_i[size_i], c_i[size_i]; 
	unsigned long long b_u[size_ul], c_u[size_ul]; 
	// double b_d[size_d], c_d[size_d];
	
	// HC_gvafromaddress(&b_i[0]);
	// HC_gvafromaddress(&b_i[size_i-1]);
	// HC_gvafromaddress(&c_i[0]);
	// HC_gvafromaddress(&c_i[size_i-1]);
	// HC_gvafromaddress_ul(&b_u[0]);
	// HC_gvafromaddress_ul(&b_u[size_ul-1]);
	// HC_gvafromaddress_ul(&c_u[0]);
	// HC_gvafromaddress_ul(&c_u[size_ul-1]);

	char *str = "CS695 Assignment 2\n";
	HC_printStr(str);

	num_exits_b = HC_numExits();

	// for(int i = 0; i < 65536; i+=1024){
	// 	b_i[i] = i + 1;
	// 	if(b_i[i] == 6)
	// 		count++;
	// }

	HC_print32bit(num_exits_a);
	HC_print32bit(num_exits_b);
	
	uint32_t num_loop;
	HC_getRandom(&num_loop);
	
	// //setup for 1GB VM
	for(uint32_t i = 0; i<num_loop; i++){
		*(long *)(uintptr_t)(0x300000+i*0x1000) = 100 * i;
	}//this code dirties random number of pages from address 0x300000
	
	//setup for 2MB page
	// for(uint32_t i = 0; i<num_loop; i++){
	// 	*(long *)(uintptr_t)(0x24000+i*0x1000) = 100 * i;
	// }//this code dirties random number of pages from address 0x300000


	
	// //setup for 2MB page
	// *(long *) 0x20000 = 72;  //writing at random memory address
	// *(long *) 0x21000 = 62;  //writing at random memory address
	// *(long *) 0x22000 = 52;  //writing at random memory address
	// *(long *) 0x23000 = 52;  //writing at random memory address

	// //setup for 1GB page
	*(long *) 0x100000 = 72;  //writing at random memory address
	*(long *) 0x101000 = 62;  //writing at random memory address
	*(long *) 0x102000 = 52;  //writing at random memory address
	*(long *) 0x103000 = 52;  //writing at random memory address

	char *firststr = HC_numExitsByType();
	uint32_t hva;

	matmul_intarray(&b_i[0],&c_i[0],size_i); //this simulates a code similar to matrix multiplication for two int arrays
	matmul_longarray(&b_u[0],&c_u[0],size_ul); //this simulates a code similar to matrix multiplication for two unsigned long long arrays
	// matmul_doublearray(&b_d[0],&c_d[0],size_d); //this simulates a code similar to matrix multiplication for two double arrays
	
	// hva = HC_gvaToHva(8192);  //0x2000
	hva = HC_gvaToHva(12288); //0x3000
	// hva = HC_gvaToHva(65536); //0x4000
	// hva = HC_gvaToHva(16392); //0x4008
	
	HC_print32bit(hva);
	// hva = HC_gvaToHva(4294967295);
	// HC_print32bit(hva);
	char *secondstr = HC_numExitsByType();

	HC_printStr(firststr);
	HC_printStr(secondstr);
	/*------------------------------------------------------------------------*/
	*(long *) 0x1000 = 42;  //writing at memory address 1MB

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
