/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "shijingcheng",              /* Team name */

    "shijingcheng",     /* First member full name */
    "1090737321@qq.com",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel* src, pixel* dst)
{
    int i, j;
    for (i = 0; i < dim; i += 32)
        for (j = 0; j < dim; j++) {
            int dstnum = RIDX(dim - 1 - j, i, dim);
            int srcnum = RIDX(i, j, dim);
            dst[dstnum] = src[srcnum];
            dst[dstnum + 1] = src[srcnum + dim];
            dst[dstnum + 2] = src[srcnum + dim * 2];
            dst[dstnum + 3] = src[srcnum + dim * 2 + dim];
            dst[dstnum + 4] = src[srcnum + dim * 4];
            dst[dstnum + 5] = src[srcnum + dim * 4 + dim];
            dst[dstnum + 6] = src[srcnum + dim * 4 + dim * 2];
            dst[dstnum + 7] = src[srcnum + dim * 4 + dim * 2 + dim];
            dst[dstnum + 8] = src[srcnum + dim * 8];
            dst[dstnum + 9] = src[srcnum + dim * 8 + dim];
            dst[dstnum + 10] = src[srcnum + dim * 8 + dim * 2];
            dst[dstnum + 11] = src[srcnum + dim * 8 + dim * 2 + dim];
            dst[dstnum + 12] = src[srcnum + dim * 8 + dim * 4];
            dst[dstnum + 13] = src[srcnum + dim * 8 + dim * 4 + dim];
            dst[dstnum + 14] = src[srcnum + dim * 8 + dim * 4 + dim * 2];
            dst[dstnum + 15] = src[srcnum + dim * 8 + dim * 4 + dim * 2 + dim];
            dst[dstnum + 16] = src[srcnum + dim * 16];
            dst[dstnum + 17] = src[srcnum + dim * 16 + dim];
            dst[dstnum + 18] = src[srcnum + dim * 16 + dim * 2];
            dst[dstnum + 19] = src[srcnum + dim * 16 + dim * 2 + dim];
            dst[dstnum + 20] = src[srcnum + dim * 16 + dim * 4];
            dst[dstnum + 21] = src[srcnum + dim * 16 + dim * 4 + dim];
            dst[dstnum + 22] = src[srcnum + dim * 16 + dim * 4 + dim * 2];
            dst[dstnum + 23] = src[srcnum + dim * 16 + dim * 4 + dim * 2 + dim];
            dst[dstnum + 24] = src[srcnum + dim * 16 + dim * 8];
            dst[dstnum + 25] = src[srcnum + dim * 16 + dim * 8 + dim];
            dst[dstnum + 26] = src[srcnum + dim * 16 + dim * 8 + dim * 2];
            dst[dstnum + 27] = src[srcnum + dim * 16 + dim * 8 + dim * 2 + dim];
            dst[dstnum + 28] = src[srcnum + dim * 16 + dim * 8 + dim * 4];
            dst[dstnum + 29] = src[srcnum + dim * 16 + dim * 8 + dim * 4 + dim];
            dst[dstnum + 30] = src[srcnum + dim * 16 + dim * 8 + dim * 4 + dim * 2];
            dst[dstnum + 31] = src[srcnum + dim * 16 + dim * 8 + dim * 4 + dim * 2 + dim];
        }

}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}
static pixel avgnew(int dim, int i, int j, pixel* src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;
    initialize_pixel_sum(&sum);
    int maxi = i - 1;
    int maxj = j - 1;
    int mini = i + 1;
    int minj = j + 1;
    for (ii = maxi; ii <= mini; ii++)
        for (jj = maxj; jj <= minj; jj++) {
            pixel p=src[RIDX(ii, jj, dim)];
            accumulate_sum(&sum, p);
        }

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}
static pixel avgborder(int dim, int i, int j, pixel* src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;
    initialize_pixel_sum(&sum);
    int maxi = max(i - 1, 0);
    int maxj = max(j - 1, 0);
    int mini = min(i + 1, dim - 1);
    int minj = min(j + 1, dim - 1);
    for (ii = maxi; ii <= mini; ii++)
        for (jj = maxj; jj <= minj; jj++) {
            pixel p = src[RIDX(ii, jj, dim)];
            accumulate_sum(&sum, p);
        }

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}
/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (j = 0; j < dim; j++) {
        dst[RIDX(0, j, dim)] = avgborder(dim, 0, j, src);
    }
    for (j = 0; j < dim; j++) {
        dst[RIDX(dim-1, j, dim)] = avgborder(dim, dim-1, j, src);
    }
    for (i = 1; i < dim-1; i++) {
        dst[RIDX(i, 0, dim)] = avgborder(dim, i, 0, src);
    }
    for (i = 1; i < dim-1; i++) {
        dst[RIDX(i, dim-1, dim)] = avgborder(dim, i, dim-1, src);
    }
    for (i = 1;i < dim-1; i++)
        for (j = 1; j < dim-1; j++){ 
            dst[RIDX(i, j, dim)] = avgnew(dim, i, j, src);
        }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

