/* libtvutils - Library of utilities.
 * Copyright (C) 2012  Take Vos <take.vos@vosgames.nl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; of version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TVU_STRING_H
#define TVU_STRING_H

/** Count how many times a character is in a string.
 * @param haystack  The string to find character is.
 * @param needle    The character to find in the string.
 * @returns         The number of needles found.
 */
static inline unsigned int tvu_count_character(char const * restrict haystack, char needle)
{
    unsigned int    count = 0;
    char            c;
   
    while ((c = *haystack++) != 0) {
        count+= (c == needle);
    }

    return count;
}


#endif
