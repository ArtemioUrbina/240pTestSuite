#include <yaul.h>
#include <stdio.h>
#include <stdlib.h>
#include "svin.h"

#define UNUSED(x) (void)(x)

//int lwram_free=1024*1024;
_svin_alloc_type svin_allocations[100];
uint8_t svin_allocations_number=0;

void *
_svin_alloc_lwram(int size, int address)
{
		bool bBusy = false;
		int address_cached = address & 0xFFFFFFF;
        int allocs_address_cached;
        int allocs_size;
        if (-1 == address)
        {
                //TODO: address not provided, trying any avaiable space
                assert(0); //auto static allocation not implemented yet
                return NULL;
        }
        else
        {
			//check if requested region is not overlapped with exisiting allocations
			for (int i=0; i<svin_allocations_number; i++)
			{
				//two range intersects if either start of the 1 is within the range of the second, or vice versa
				//comparing only cached addresses
				allocs_address_cached = svin_allocations[i].address & 0xFFFFFFF;
				allocs_size = svin_allocations[i].size;
				if ( (allocs_address_cached >= address_cached) && (allocs_address_cached <= address_cached+size) )
					bBusy = true;
				else if ( (address_cached >= allocs_address_cached) && (address_cached <= allocs_address_cached+allocs_size) )
					bBusy = true;
			}
			if ( (false == bBusy) && (svin_allocations_number<100) )
			{
				//valid range, alocating
				svin_allocations[svin_allocations_number].address = address;
				svin_allocations[svin_allocations_number].size = size;
				return (void *)address;
			}
			else
			{
				assert(0);//static allocation failed
                return NULL;
			}
        }
        return NULL;
}

void
_svin_free_lwram(int address)
{
	//look up the allocated data
    for (int i=0; i<svin_allocations_number; i++)
			{
				if ( (svin_allocations[i].address == (uint32_t)address) )
				{
					//found, deleting
					for (int j=i;j<svin_allocations_number;j++)
						svin_allocations[j] = svin_allocations[j+1];
					svin_allocations_number--;
					i = svin_allocations_number; //stop search 
				}
			}
}