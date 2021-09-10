/* Solution not tested */

/* Globals */
int readcnt = 0;
int writing = 0;
sem_t mutex, w; /* Initialized to 1 */
sem_t r;        /* Initialized to 0 */

void reader(void) {
  while (1) {
    P(&mutex);
    readcnt++;
    if (writing) {
      P(&r);
      writing = 0;
    }
    else if (readcnt == 1) 
     P(&w);
    V(&mutex);

    /* Critical section */
    /* Reading happens */

    P(&mutex);
    readcnt--;
    if (readcnt == 0) 
      V(&w);
    V(&mutex);
  }
}

void writer(void) {
  while (1) {
    P(&w);
    writing = 1;
    
    /* Critical section */
    /* Writing happens */

    if (readcnt > 0)
      V(&r);
    else
      V(&w);
  }
}
