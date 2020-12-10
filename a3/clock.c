/*
 * This code is provided solely for the personal and private use of students
 * taking the CSC369H course at the University of Toronto. Copying for purposes
 * other than this use is expressly prohibited. All forms of distribution of
 * this code, including but not limited to public repositories on GitHub,
 * GitLab, Bitbucket, or any other online platform, whether as given or with
 * any changes, are expressly prohibited.
 *
 * Authors: Andrew Peterson, Karen Reid
 *
 * All of the files in this directory and all subdirectories are:
 * Copyright (c) 2019, 2020 Karen Reid
 */

#include "pagetable.h"
#include "sim.h"

int clock_hand;

/* Page to evict is chosen using the CLOCK algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int clock_evict(void)
{
	//TODO
	while (coremap[clock_hand].pte->frame & PG_REF) { //while clock points to entry with ref bit 1
		coremap[clock_hand].pte->frame &= ~PG_REF; // set ref to 0
		clock_hand = (clock_hand + 1) % memsize; // loops around the clock 
	}
	return clock_hand;
}

/* This function is called on each access to a page to update any information
 * needed by the CLOCK algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p)
{
	p->frame |= PG_REF; // set ref bit to one
}

/* Initialize any data structures needed for this replacement algorithm. */
void clock_init(void)
{
	clock_hand = 0;
}

/* Cleanup any data structures created in clock_init(). */
void clock_cleanup(void)
{
	//TODO
}
