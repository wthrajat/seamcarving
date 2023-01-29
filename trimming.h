/* ------------------------------------------------------------------------- *
 * Interface for slimming an image.
 * ------------------------------------------------------------------------- */

#ifndef _SLIMMING_H_
#define _SLIMMING_H_
// to be _TRIMMING_H_

#include <stddef.h>
//#include "PNM.h"

/* ------------------------------------------------------------------------- *
 * Reduce the width of a PNM image to `image->width-k`.
 *
 * The PNM image must later be deleted by calling freePNM().
 *
 * PARAMETERS
 * image        Pointer to a PNM image
 * k            The number of pixels to be removed (along the width axis)
 *
 * RETURN
 * image        Pointer to a new PNM image
 * NULL         if an error occured
 * ------------------------------------------------------------------------- */
PNMImage* reduceImageWidth(const PNMImage* image, size_t k);

#endif // _SLIMMING_H_