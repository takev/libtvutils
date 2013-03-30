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
#include <stdbool.h>
#include <stdio.h>
#include <tvutils/types.h>
#include <tvutils/string.h>
#include <tvutils/bigint.h>

/** RFC 3526 Defines this Diffie hellman group.
utf8_t tvu_diffie_hellman_2048_group_s =
    "FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1 29024E08 8A67CC74"
    "020BBEA6 3B139B22 514A0879 8E3404DD EF9519B3 CD3A431B 302B0A6D F25F1437"
    "4FE1356D 6D51C245 E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED"
    "EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D C2007CB8 A163BF05"
    "98DA4836 1C55D39A 69163FA8 FD24CF5F 83655D23 DCA3AD96 1C62F356 208552BB"
    "9ED52907 7096966D 670C354E 4ABC9804 F1746C08 CA18217C 32905E46 2E36CE3B"
    "E39E772C 180E8603 9B2783A2 EC07A28F B5C55DF0 6F4C52C9 DE2BCBF6 95581718"
    "3995497C EA956AE5 15D22618 98FA0510 15728E5A 8AACAA68 FFFFFFFF FFFFFFFF";

bigint2048_t tvu_diffie_hellman_2048_group;
utf8_t tvu_diffie_hellman_2048_group_reciprocal =
    "1"
    "00000000 00000000 36F0255D DE973DCB 4703CE7E 2E815197 A6DB0F58 8448B611"
    "64CFCAC5 F1872E51 B1F9FBB5 BF16FBE7 9689FC09 03A801E3 D4802FB8 D329550D"
    "C8C9D3D9 22EECE9A 5475DB33 DB7B83BB 5C0E13D1 68049BBC 86C58176 47B088D1"
    "7AA5CC40 E0203558 8EDB2DE1 89934137 19FC258D 79BC217A C4B8739C BEA038AA"
    "A88D0D2F 78A77A8A 6FC7FAA8 B2BDCA9B E7502D2F 5F6A7B65 F5E4F07A B8B286E4"
    "1115F024 A6E976BD 2BCE3E51 90B891AB BF2331E9 C94DE91F BE857437 0494A354"
    "EAC9BE0B 31EB3185 40E4069D 556E9DD0 9D5D89D7 DE4A75C8 8BB49316 C106E4E0"
    "14B636E6 0FEBC292 E6249105 F5B195FE 906EEF7D 26C90A17 477122CE 125FB665";

