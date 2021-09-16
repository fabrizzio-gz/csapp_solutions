int main () {
  /* A */
  x != 0;
  /* B */
  x != -1;
  /* C */
  x & 0ff != 0;
  /* D */
  /* shift most significant byte to beginning */
  int y = x >> ((sizeof(int) - 1)<<3);
  /* Mask to get only first byte */
  y & 0xff != 0xff
   
    
  return 0;
}
