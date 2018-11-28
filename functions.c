#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int check_tlb_initialization()
{
  int first_are_zeroes = (tlb_pg_number[0] == 0) && (tlb_frame[0] == 0);
  int seconds_are_zeroes = (tlb_pg_number[1] == 0) && (tlb_frame[1] == 0);

  if (first_are_zeroes == 1 && seconds_are_zeroes == 1) return 0;
  else return 1;
}

void update_tlb(uint8_t pg_number, uint8_t frame)
{
  // init tlb
  if ((frame == 0) && (last_updated_tlb == 0))
  {
    tlb_pg_number[0] = pg_number;
    tlb_frame[0] = frame;
    
    printf("tlb updated:\nposition %u\npage number: %u\nframe number: %u\n", last_updated_tlb, tlb_pg_number[0], tlb_frame[0]);
    
    last_updated_tlb++;
  }

  else
  {
    tlb_pg_number[last_updated_tlb] = pg_number;
    tlb_frame[last_updated_tlb] = frame;
    
    printf("tlb updated:\nposition %u\npage Number: %u\nframe Number: %u\n", last_updated_tlb, tlb_pg_number[last_updated_tlb], tlb_frame[last_updated_tlb]);
    
    last_updated_tlb = (last_updated_tlb + 1) % 16;
  }
}

uint8_t read_physical_memo(uint16_t memo_pos)
{
  return (uint8_t)physical_memo[memo_pos];
}

void update_physical_memo(uint8_t pg_number, uint8_t frame)
{
  uint16_t i = 0;
  fseek(backstore_memo_file, pg_number * 256, SEEK_SET);
  
  while(i < 256)
  {
    fread(&physical_memo[frame * 256 + i], 1, 1, backstore_memo_file);
    i++;
  }
}

uint8_t check_pg_table(uint8_t pg_number)
{
  uint8_t i, j;

  for (i = 0; i < 256; i++)
  {
    // no frames, must init tlb
    if ((i == pg_number) && (num_used_frames == 0))
    {
      printf("page error: page not initialized in memory\n");
      printf("updating physical memory with a new frame...\n");

      update_physical_memo(pg_number, num_used_frames);

      printf("updated physical memory!\n");

      pg_table[i] = num_used_frames;
      num_used_frames++;

      printf("updating the tlb with the new page number...\n");

      update_tlb(pg_number, pg_table[i]);

      printf("finished\n");
      return pg_table[i];
    }
    // no frame for current search
    else if (i == pg_number && (pg_table[i] == 0))
    {
      pg_err++;

      printf("page not initialized in memory yet. Page error generated.\n");
      printf("updating physical memory with the new frame...\n");

      update_physical_memo(pg_number, num_used_frames);

      printf("updated physical memory\n");

      pg_table[i] = num_used_frames;
      num_used_frames++;

      printf("updating tlb with the new page...\n");
      
      update_tlb(pg_number, pg_table[i]);
      
      printf("finished\n");
      return pg_table[i];
    }

    // all good, found frame
    else if (i == pg_number)
    {
      printf("Page found in memory... yay!\n");
      printf("updating the tlb with the new page number...\n");
      
      update_tlb(pg_number, pg_table[i]);

      printf("finished\n");
      return pg_table[i];
    }
  }
}

uint8_t check_tlb_table(uint8_t pg_number)
{
  printf("verifying page number in tlb...\n");

  if (check_tlb_initialization())
  {
    uint8_t i;
    
    for (i = 0; i < 16; i++)
    {
      if (tlb_pg_number[i] == pg_number)
      {
        tlb_match++;

        printf("found frame %u in TLB.\n", tlb_frame[i]);
        
        return tlb_frame[i];
      }
    }
  }

  printf("page number not found! Opening the page table...\n");
  return check_pg_table(pg_number);
}

double calc_rate(double number, unsigned int nmbr_access)
{
  double result = 0;

  result = (number / ((double) nmbr_access)) * 100;

  return result;
}

