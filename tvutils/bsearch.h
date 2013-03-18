/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TVU_BSEARCH_H
#define TVU_BSEARCH_H

#include <sys/types.h>

static inline void *tvu_bsearch_entry(const void *base, size_t width, off_t i)
{
    return (char *)base + (width * i);
}

static inline void *tvu_bsearch_near(const void *key, const void *base, size_t nel, size_t width, int (*compar) (const void *, const void *))
{
    off_t   imin = 0;
    off_t   imax = nel - 1;
    off_t   imid;

    while (imin < imax) {
        imid = (imin + imax) / 2;

        if (compar(key, tvu_bsearch_entry(base, width, imid)) > 0) {
            imin = imid + 1;
        } else {
            imax = imid;
        }
    }

    if ((compar(key, tvu_bsearch_entry(base, width, imin)) < 0) & (imin > 0)) {
        // Adjust when the key was not found.
        imin--;
    }

    return tvu_bsearch_entry(base, width, imin);
}

#endif
