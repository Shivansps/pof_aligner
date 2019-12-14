#ifndef VIEWERS_H_INCLUDED
#define VIEWERS_H_INCLUDED

#include "misc.h"

/*Returns 0 if data is aligned, or the amount of detected alignment problems*/

int view_bsp_data(ubyte *bytes, unsigned int size_int, int version);
int view_bsp_sortnorm(ubyte *bytes, int count, int show);
int view_pof(ubyte *pof_bytes, int total_size);
int view_txtr(ubyte *bytes);
int view_fuel(ubyte *bytes, int version);
int view_dock(ubyte *bytes, int show);
int view_path(ubyte *bytes);
int view_bsp_defpoints(ubyte *bytes);
int view_spcl(ubyte *bytes);
int view_pinf(ubyte *bytes, unsigned int chunk_size);
int view_glow(ubyte *bytes);
int view_sldc(ubyte *bytes);
int view_slc2(ubyte *bytes);

#endif // VIEWERS_H_INCLUDED
