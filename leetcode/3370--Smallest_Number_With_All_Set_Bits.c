int smallestNumber(int n) {
  int p = 0;
  while(!(n&(0x8000>>p))) // find first valid bit
  {
    p++;
  }
  return (0xffff>>p); // the answer is all bits 1 remove p counts
}
