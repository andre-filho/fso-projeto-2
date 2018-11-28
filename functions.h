#ifndef FUNCTIONS
#define FUNCTIONS

#include <stdint.h>

// global vars
FILE *backstore_memo_file;

unsigned int pg_err = 0;
unsigned int tlb_match = 0;
unsigned int memo_access = 0;

uint8_t num_used_frames = 0;
uint8_t last_updated_tlb = 0;

uint8_t tlb_pg_number[16] = {0};
uint8_t tlb_frame[16] = {0};
uint8_t pg_table[256] = {0};
uint8_t physical_memo[65536] = {0};

// functions declaration
int check_tlb_initialization();

void update_tlb(uint8_t frame, uint8_t pg_number);

uint8_t read_physical_memo(uint16_t memo_pos);

void update_physical_memo(uint8_t frame, uint8_t pg_number);

uint8_t check_pg_table(uint8_t pg_number);

uint8_t check_tlb_table(uint8_t pg_number);

double calc_rate(double number, unsigned int nmbr_access);

#endif
