#include "tools.h"

/**
 * @brief Returns the index of num (more or less tolerance) 
 *        in the array arr, of a given size.
 * 
 * @param num 
 * @param tolerance 
 * @param arr 
 * @param size 
 * @return int 
 */
int indexOf(int num, int tolerance, int16_t *arr, int size)
{
    for(int i=0;i<size;i++)
    {
        int delta = abs(arr[i] - num);

        if(delta <= tolerance)
            return i;

    }
    return -1;
}