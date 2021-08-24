/* 
 * Allocator that uses only a header for allocated blocks.
 * Free blocks still use both a header and a footer.
 * Minimum block size: 2 words (1 word hdr + 1 word free).
 * Showing only functions with changes relative to mm.c.
 */

/* Pack a size and allocated bit into a word of the header.
 * Use 2nd low order bit to store allocated state of prev block.
 * Not required in the footer.
 */
#define PREV_ALLOCATED 1<<1 /* 2nd low order bit = 1 */
#define PREV_FREE 0

#define PACK(size, alloc, prev_alloc)  ((size) | (alloc) | (prev_alloc))
#define GET_PREVALLOC(P) (GET(p) & 0x2)

static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);

int mm_init(void) 
{
  /* Create the initial empty heap */
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) //line:vm:mm:begininit
    return -1;
  PUT(heap_listp, 0);                          /* Alignment padding */
  /* Prologue and epilogue are the same as initial allocator implementation */
  PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1, PREV_FREE)); /* Prologue header */ 
  PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1, PREV_FREE)); /* Prologue footer */ 
  PUT(heap_listp + (3*WSIZE), PACK(0, 1, PREV_FREE));     /* Epilogue header */
  heap_listp += (2*WSIZE);                     //line:vm:mm:endinit  
  /* $end mminit */

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
    return -1;
  return 0;
}


/*
 * mm_malloc - Allocate a block with at least size bytes of payload 
 */
/* $begin mmmalloc */
void *mm_malloc(size_t size) 
{
  size_t asize;      /* Adjusted block size */
  size_t extendsize; /* Amount to extend heap if no fit */
  char *bp;      

  /* $end mmmalloc */
  if (heap_listp == 0){
    mm_init();
  }
  /* $begin mmmalloc */
  /* Ignore spurious requests */
  if (size == 0)
    return NULL;

  /* Adjust block size to include header and maintain DWORD alignment */
  if (size <= WSIZE)                                         
    asize = DSIZE; /* Minimum block size*/                                       
  else
    /* Adjust to next higher multiple of 8 (DSIZE) */
    asize = DSIZE * ((size + (DSIZE) + (WSIZE - 1)) / DSIZE);

  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {  //line:vm:mm:findfitcall
    place(bp, asize);                  //line:vm:mm:findfitplace
    return bp;
  }

  /* No fit found. Get more memory and place the block */
  extendsize = MAX(asize,CHUNKSIZE);                 //line:vm:mm:growheap1
  if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
    return NULL;                                  //line:vm:mm:growheap2
  place(bp, asize);                                 //line:vm:mm:growheap3
  return bp;
} 
/* $end mmmalloc */

/* 
 * mm_free - Free a block 
 */
/* $begin mmfree */
void mm_free(void *bp)
{
  /* $end mmfree */
  if (bp == 0) 
    return;
  
  /* $begin mmfree */
  size_t size = GET_SIZE(HDRP(bp));
  /* $end mmfree */
  if (heap_listp == 0){
    mm_init();
  }
  /* $begin mmfree */
  
  /* The PREV_ALLOC bit is preserved */
  int prev_alloc = GET_PREVALLOC(HDRP(bp));
  PUT(HDRP(bp), PACK(size, 0, prev_alloc));
  PUT(FTRP(bp), PACK(size, 0, prev_alloc));
  /* Update next block PREV_ALLOC bit */
  bp = NEXT_BLKP(bp);
  int next_alloc = GET_ALLOC(HDRP(bp));
  int next_size = GET_SIZE(HDRP(bp));
  PUT(HDRP(bp), PACK(next_size, next_alloc, PREV_FREE));
  coalesce(bp);
}

/* $end mmfree */
/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
/* $begin mmfree */
static void *coalesce(void *bp) 
{
  size_t prev_alloc = GET_PREVALLOC(HDRP(bp));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {            /* Case 1 */
    return bp;
  }

  else if (prev_alloc && !next_alloc) {      /* Case 2 */
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0, PREV_ALLOCATED));
    PUT(FTRP(bp), PACK(size,0, PREV_ALLOCATED));
  }

  else if (!prev_alloc && next_alloc) {      /* Case 3 */
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0, PREV_ALLOCATED));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0, PREV_ALLOCATED));
    bp = PREV_BLKP(bp);
  }

  else {                                     /* Case 4 */
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
      GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0, PREV_ALLOCATED));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0, PREV_ALLOCATED));
    bp = PREV_BLKP(bp);
  }

  return bp;
}
/* $end mmfree */

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
/* $begin mmextendheap */
static void *extend_heap(size_t words) 
{
  char *bp;
  size_t size;

  /* Allocate an even number of words to maintain alignment */
  size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; //line:vm:mm:beginextend
  if ((long)(bp = mem_sbrk(size)) == -1)  
    return NULL;                                        //line:vm:mm:endextend

  /* Initialize free block header/footer and the epilogue header */
  int prev_alloc = GET_PREVALLOC(HDRP(bp));
  PUT(HDRP(bp), PACK(size, 0, prev_alloc));         /* Free block header */   //line:vm:mm:freeblockhdr
  PUT(FTRP(bp), PACK(size, 0, prev_alloc));         /* Free block footer */   //line:vm:mm:freeblockftr
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1, PREV_FREE)); /* New epilogue header */ //line:vm:mm:newepihdr

  /* Coalesce if the previous block was free */
  return (!prev_alloc) && coalesce(bp);
}
/* $end mmextendheap */

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 * While allocating, PREV_ALLOC bit is also set for both current and
 * next block.
 */
/* $begin mmplace */
/* $begin mmplace-proto */
static void place(void *bp, size_t asize)
/* $end mmplace-proto */
{
  size_t csize = GET_SIZE(HDRP(bp));
  /* Preserve prev_alloc field in header */
  int prev_alloc = GET_PREVALLOC(HDRP(bp));

  /* Split with fragments of at least DSIZE */
  if ((csize - asize) >= (DSIZE)) { 
    PUT(HDRP(bp), PACK(asize, 1, prev_alloc));
    /* Split block */
    bp = NEXT_BLKP(bp);
    /* Set PREV_ALLOC bit in fragment */
    PUT(HDRP(bp), PACK(csize-asize, 0, PREV_ALLOCATED));
    PUT(FTRP(bp), PACK(csize-asize, 0, PREV_ALLOCATED));
  }
  else { 
    PUT(HDRP(bp), PACK(csize, 1, prev_alloc));
    bp = NEXT_BLKP(bp);
    /* update next block hdr and ftr PREV_ALLOC bit */
    int next_alloc = GET_ALLOC(HDRP(bp));
    int next_size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(next_size, next_alloc, PREV_ALLOCATED));
    /* No need to update PREV_ALLOC bit in next block ftr */
  }
}
/* $end mmplace */

