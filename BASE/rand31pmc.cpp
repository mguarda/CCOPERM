#include "rand31pmc.h"

/////////////////////////////////////////
//
// Implementations of the methods.

// seedi()
//

//! Set the seed from a long unsigned integer.  If zero is used, then the seed will be set to 1.
void rand31dc::seedi(long unsigned int seedin)
{
  if (seedin == 0) seedin = 1;
  seed31 = seedin;
}

// ranlui()
//

//! Return next pseudo-random value as a long unsigned integer.
long unsigned int rand31dc::ranlui(void)  
{
  return nextrand();
}

// ranf()
//

//! Return next pseudo-random value as a floating point value.
float rand31dc::ranf(void)  
{
  return (nextrand() / 2147483647.0);
}
