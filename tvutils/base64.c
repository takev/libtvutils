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
#include <string.h>
#include <tvutils/base64.h>

utf8_t tvu_base64_enc_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
utf8_t tvu_base64_dec_table[128];

utf8_t tvu_base85_enc_table[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";
utf8_t tvu_base85_dec_table[128];


void tvu_base_init(void)
{
    int     i;
    utf8_t  c;

    memset(tvu_base64_dec_table, 0xff, 128);
    memset(tvu_base85_dec_table, 0xff, 128);

    for (i = 0; i < 64; i++) {
        c = tvu_base64_enc_table[i];
        tvu_base64_dec_table[(int)c] = i;
    }
    for (i = 0; i < 85; i++) {
        c = tvu_base85_enc_table[i];
        tvu_base85_dec_table[(int)c] = i;
    }
}

