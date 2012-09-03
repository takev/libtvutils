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
#include <tvutils/compiler.h>
#include <tvutils/atomic.h>
#include <tvutils/ringbuffer.h>

void tvu_ring_advance_free(tvu_ringbuffer_t *self)
{
    uint64_t            old_free;
    uint64_t            old_tail;
    uint64_t            new_free;
    tvu_ringpacket_t    *packet;

    for (;;) {
        do {
            old_free = tvu_atomic_read_u64(&self->free);
            old_tail = tvu_atomic_read_u64(&self->tail);

            if (old_free >= old_tail) {
                goto last;
            }

            packet = (tvu_ringpacket_t *)&self->data[old_free];
            if (!tvu_ringpacket_isfree(packet)) {
                goto last;
            }

            new_free = old_free + tvu_ringpacket_pktsize(packet);

        } while (!tvu_atomic_cas_u64(&self->free, old_free, new_free));
    }
last:
    return;
}

tvu_ringpacket_t *tvu_ring_head(tvu_ringbuffer_t *self, size_t size)
{
    uint64_t            old_head;
    uint64_t            new_head;
    uint64_t            old_free;
    size_t              padded_size = tvu_round_up_u64(size, tvu_ringpacket_hdrsize()) + tvu_ringpacket_hdrsize();
    size_t              size_at_end;
    bool                need_empty_packet_at_end;
    tvu_ringpacket_t    *packet;

    tvu_ring_advance_free(self);

    do {
        old_head = tvu_atomic_read_u64(&self->head);
        old_free = tvu_atomic_read_u64(&self->free);

        // Calculate where the new head is supposed to be.
        size_at_end = self->size - (old_head % self->size);

        need_empty_packet_at_end = padded_size > size_at_end;
        new_head = old_head + padded_size + (need_empty_packet_at_end ? size_at_end : 0);

        // Check if the head is progressed beyond the tail.
        if (unlikely(new_head > (old_free + self->size))) {
            errno = EAGAIN;
            return NULL;
        }
    } while (!tvu_atomic_cas_u64(&self->head, old_head, new_head));

    // Add a special end packet to the end of the ring.
    if (need_empty_packet_at_end) {
        packet = (tvu_ringpacket_t *)&self->data[old_head % self->size];
        tvu_ringpacket_setlast(packet, size_at_end);
        old_head+= size_at_end;
    }

    // Start the actual packet.
    packet = (tvu_ringpacket_t *)&self->data[old_head % self->size];
    tvu_ringpacket_setalloc(packet, size);
    return packet;
}

tvu_ringpacket_t *tvu_ring_tail(tvu_ringbuffer_t *self)
{
    uint64_t            old_tail;
    uint64_t            new_tail;
    uint64_t            old_head;
    tvu_ringpacket_t    *packet;

    tvu_ring_advance_free(self);

again:
    do {
        old_tail = tvu_atomic_read_u64(&self->tail);
        old_head = tvu_atomic_read_u64(&self->head);

        if (old_head <= old_tail) {
            errno = EAGAIN;
            return NULL;
        }

        packet = (tvu_ringpacket_t *)&self->data[old_tail % self->size];
        if (!tvu_ringpacket_isdata_or_last(packet)) {
            errno = EAGAIN;
            return NULL;
        }

        new_tail = old_tail + tvu_ringpacket_pktsize(packet);

    } while (!tvu_atomic_cas_u64(&self->tail, old_tail, new_tail));

    if (unlikely(tvu_ringpacket_islast(packet))) {
        tvu_ringpacket_markfree(packet);
        goto again;
    }

    tvu_ring_advance_free(self);

    return packet;
}

