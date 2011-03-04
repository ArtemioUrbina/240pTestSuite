#include <stdio.h>

// Calculate the palette values 
// for 1555 colorspace

int rndup(double n)
{
	int 	val;
	double frac;

	val = (int)n;
	frac = n - val;
	if(frac >= 0.0001)
		val += 1;

	return val;
}     

int main()
{
	double i = 0;
	double val = 0;

	printf("index\t16b hex\tpcnt\n");
	for(i = 0; i < 32; i++)
	{
		val = (i*255)/31;
		printf("%X\t%X\t%d%%\n", (int)i/2, rndup(val), rndup(i*100/31));
	}
	return 0;
}
