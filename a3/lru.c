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


list_entry_t *first;
list_entry_t *last;

list_entry_t *entries;

void remove_from_list(list_entry_t *entry){
	if(entry->frame != -1){
		if(entry->prev){
				entry->prev->next = entry->next;
			} 
		if(entry->next){
			entry->next->prev = entry->prev;
		} else {
			last = entry->prev;
		}
		entry->frame = -1; // set frame in this entry to -1 to indicate not in the list
	}
}
/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int lru_evict(void)
{
	int lru_frame = last->frame;
	remove_from_list(last);
	return lru_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the LRU algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p)
{
	//TODO
	int frame_referenced = p->frame >> PAGE_SHIFT;
	struct frame *frame = &coremap[frame_referenced];
	
	remove_from_list(frame->entry); // removes if the entry is present


	//insert as head of list (most recently referenced)
	frame->entry->prev = NULL;
	frame->entry->next = first;
	frame->entry->frame = frame_referenced;

	//update first
	if(first){
		first->prev = frame->entry;
	}
	first = frame->entry;

	if(last == NULL) {
		last = first; // last is initially assigned to the very first element added to the list
	}

}

/* Initialize any data structures needed for this replacement algorithm. */
void lru_init(void)
{
	//TODO
	first = NULL;
	last = NULL;
	entries = malloc(sizeof(list_entry_t) * memsize);
	for(unsigned i = 0; i < memsize; i++){
		list_entry_t *entry = entries + i;
		coremap[i].entry = entry;
		entry->next = NULL;
		entry->prev = NULL;
		entry->frame = -1; //unassigned, not in the list
	}
}

/* Cleanup any data structures created in lru_init(). */
void lru_cleanup(void)
{
	free(entries);
}
