#include <stdlib.h>
#include <time.h>

void set_seed_random_number(void);
double generate_random_number(void);
/*
* Sets a seed for the random Number Generator
*/
void set_seed_random_number(void) {
  srand(time(NULL));
}
/*
* Generates a random double between 0 and 1
* (with 2 digit precision)
*/
double generate_random_number(void) {
  int generated_number = rand() % 101;
  return (double)generated_number / 100;
}
