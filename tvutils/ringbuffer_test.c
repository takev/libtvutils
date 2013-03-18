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
#include <stdio.h>
#include <tvutils/tvutils.h>
#include <tvutils/ringbuffer.h>

char                buffer[32 + 32];
tvu_ringbuffer_t    *ring = (tvu_ringbuffer_t *)buffer;

int main(int argc __attribute__((unused)), char *argv[])
{
    TVU_UNITTEST_INIT
    tvu_init(argv[0]);

    tvu_ringpacket_t *packet;

    tvu_ring_init(ring, sizeof (buffer));
    //tvu_ring_debug(ring);

    packet = tvu_ring_head(ring, 6);
    TVU_UNITTEST_NOT_NULL(packet);
    strcpy((char *)packet->data, "hello");
    tvu_ringpacket_markdata(packet);
    //tvu_ring_debug(ring);

    packet = tvu_ring_head(ring, 6);
    TVU_UNITTEST_NOT_NULL(packet);
    strcpy((char *)packet->data, "world");
    tvu_ringpacket_markdata(packet);
    //tvu_ring_debug(ring);

    packet = tvu_ring_head(ring, 11);
    TVU_UNITTEST_NULL(packet);
    TVU_UNITTEST_CMP(errno, EAGAIN);
    // Wanted to add the string ringbuffer.
    //tvu_ring_debug(ring);

    packet = tvu_ring_tail(ring);
    TVU_UNITTEST_NOT_NULL(packet);
    TVU_UNITTEST_STRCMP((char *)packet->data, "hello");
    tvu_ringpacket_markfree(packet);
    //tvu_ring_debug(ring);

    packet = tvu_ring_head(ring, 11);
    TVU_UNITTEST_NULL(packet);
    TVU_UNITTEST_CMP(errno, EAGAIN);
    // Wanted to add the string ringbuffer.
    //tvu_ring_debug(ring);

    packet = tvu_ring_tail(ring);
    TVU_UNITTEST_NOT_NULL(packet);
    TVU_UNITTEST_STRCMP((char *)packet->data, "world");
    tvu_ringpacket_markfree(packet);
    //tvu_ring_debug(ring);

    packet = tvu_ring_tail(ring);
    TVU_UNITTEST_NULL(packet);
    TVU_UNITTEST_CMP(errno, EAGAIN);
    // Nothing to read here.
    //tvu_ring_debug(ring);

    packet = tvu_ring_head(ring, 11);
    TVU_UNITTEST_NOT_NULL(packet);
    strcpy((char *)packet->data, "ringbuffer");
    tvu_ringpacket_markdata(packet);
    //tvu_ring_debug(ring);

    packet = tvu_ring_tail(ring);
    TVU_UNITTEST_NOT_NULL(packet);
    TVU_UNITTEST_STRCMP((char *)packet->data, "ringbuffer");
    tvu_ringpacket_markfree(packet);
    //tvu_ring_debug(ring);

    return 0;
}
