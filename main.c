#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char *argv[])
{
  int read_value;

  uint16_t logical_addr, i, memo_pos;
  uint8_t pg_number, offset, frame, data_read;

  double pg_error_rate, tlb_match_rate;
  FILE *addr_file;

  if (argc < 2)
  {
    printf("usage: ./proj2.out addresses.txt\n");
    return -1;
  }

  addr_file = fopen(argv[1], "r");

  if (addr_file == NULL)
  {
    printf("error opening file\n");
    return -1;
  }

  backstore_memo_file = fopen("BACKING_STORE.bin", "rb");

  if (backstore_memo_file == NULL)
  {
    printf("error opening file\n");
    return -1;
  }

  do
  {
    while (fscanf(addr_file, "%d", &read_value) != EOF)
    {
      memo_access++;
      
      printf("read value: %d\n", read_value);
      
      logical_addr = 0xFFFF & read_value;
      pg_number = (0xFF00 & logical_addr) >> 8;
      offset = 0x00FF & logical_addr;
      
      printf("\nlogical addr: %u \npage number: %u \noffset: %u\n", logical_addr, pg_number, offset);
      
      frame = check_tlb_table(pg_number);
      
      printf("frame number found: %u\n", frame);
      
      memo_pos = frame * 256 + offset;
      
      printf("memory position found: %u\n", memo_pos);
      
      data_read = read_physical_memo(memo_pos);
      
      printf("data read: %u\n", data_read);
      printf("\n");
    }

    printf("page error rate: %.2lf%\n", calc_pg_error_rate(pg_err, memo_access));
    printf("tlb match rate: %.2lf%\n", calc_tbl_match_rate(tlb_match, memo_access));

    fclose(addr_file);
    fclose(backstore_memo_file);
    
    return 0;
  } while (addr_file != NULL && backstore_memo_file != NULL);
}
