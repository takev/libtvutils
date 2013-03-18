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
#ifndef TVU_RINGBUFFER_H
#define TVU_RINGBUFFER_H

#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <tvutils/number.h>
#include <tvutils/buffer.h>

#define TVU_RINGPACKET_FREE         0x00000000
#define TVU_RINGPACKET_ALLOC        0x40000000
#define TVU_RINGPACKET_DATA         0x80000000
#define TVU_RINGPACKET_LAST         0xc0000000
#define TVU_RINGPACKET_SIZE_MASK    0x3fffffff

/** The ring buffer.
 * Allocation of the ring buffer is up to the user.
 * This struct is a incomplete type with a data field which extends to
 * the amount that was allocated. Use tvu_ring_init() to initialize this
 * structure.
 *
 * This ring buffer uses 64 bit integers for the head and tail indices. It is assumed
 * that these integers are so large that it would be unfeasable to add so much data
 * in the buffer that it would overflow. If it would overflow nasty things would happen.
 */
typedef struct {
    uint64_t    head;   //< Index of the producer in the data.
    uint64_t    tail;   //< Index of the consumer in the data.
    uint64_t    free;   //< Index of where the data is free to be used by the producer again.
    uint64_t    size;   //< The number of bytes the array data is.
    uint8_t     data[]; //< The data array.
} tvu_ringbuffer_t;

/** A packet in the ring buffer.
 * A packet is always aligned to a 32 bit word boundary.
 * The size_and_flags fields should only be accessed by
 * the utility function operating on this packet.
 * The data is completely user defined.
 */
typedef struct {
    uint32_t    size_and_flags;
    uint8_t     data[];
} tvu_ringpacket_t;

/** Return the size of the header of a ringpacket.
 * @returns size of a tvu_ringpacket_t header.
 */
static inline size_t tvu_ringpacket_hdrsize(void)
{
    return offsetof(tvu_ringpacket_t, data);
}

/** Finished producing the packet.
 * This function must be called when you are finished writing data into the packet,
 * after calling tvu_ring_head().
 * It will mark the packet as data carrying which will allow the tail to advance.
 *
 * @param self  The packet.
 */
static inline void tvu_ringpacket_markdata(tvu_ringpacket_t volatile *self)
{
    tvu_atomic_write_barrier();
    self->size_and_flags = (self->size_and_flags & TVU_RINGPACKET_SIZE_MASK) | TVU_RINGPACKET_DATA;
}

/** Finished consuming the packet.
 * This function must be called when you are finished reading data from the packet,
 * after calling tvu_ring_tail().
 * It will mark the packet as data carrying which will allow the free to advance.
 *
 * @param self  The packet.
 */
static inline void tvu_ringpacket_markfree(tvu_ringpacket_t volatile *self)
{
    tvu_atomic_write_barrier();
    self->size_and_flags = (self->size_and_flags & TVU_RINGPACKET_SIZE_MASK) | TVU_RINGPACKET_FREE;
}

/** Create free space in the ring buffer.
 * This function is called internally to mark a region at the end of the ring buffer as
 * free.
 *
 * @param packet_size   The size of the packet including header, must be rounded up to a multiple
 *                      of the header size.
 */
static inline void tvu_ringpacket_setlast(tvu_ringpacket_t volatile *self, size_t packet_size)
{
    tvu_atomic_write_barrier();
    self->size_and_flags = (packet_size - tvu_ringpacket_hdrsize()) | TVU_RINGPACKET_LAST;
}

/** Create a freshly allocated packet on the ring buffer.
 * This function is called internally to create a new packet.
 */
static inline void tvu_ringpacket_setalloc(tvu_ringpacket_t volatile *self, size_t size)
{
    tvu_atomic_write_barrier();
    self->size_and_flags = size | TVU_RINGPACKET_ALLOC;
}

/** Check if this packet is free.
 * Used internal to determine of the free index can be advanced.
 */
static inline bool tvu_ringpacket_isfree(tvu_ringpacket_t const volatile *self)
{
    bool isfree = (self->size_and_flags & 0xff000000) == TVU_RINGPACKET_FREE;
    tvu_atomic_read_barrier();
    return isfree;
}

/** Check if this packet can be consumed.
 * Used internally to determine if the producer is finished with this packet; and
 * advance the tail index.
 */
static inline bool tvu_ringpacket_isdata_or_last(tvu_ringpacket_t const volatile *self)
{
    uint32_t flags = self->size_and_flags & 0xff000000;
    tvu_atomic_read_barrier();
    return (flags == TVU_RINGPACKET_DATA) | (flags == TVU_RINGPACKET_LAST);
}

/** Check if this is a filler packet at the end of the ring buffer.
 * Used internally to know if we need to skip over this packet.
 */
static inline bool tvu_ringpacket_islast(tvu_ringpacket_t const volatile *self)
{
    bool islast = (self->size_and_flags & 0xff000000) == TVU_RINGPACKET_LAST;
    tvu_atomic_read_barrier();
    return islast;
}

static inline size_t tvu_ringpacket_size(tvu_ringpacket_t const volatile *self)
{
    size_t size = self->size_and_flags & 0x00ffffff;
    tvu_atomic_read_barrier();
    return size;
}

static inline size_t tvu_ringpacket_pktsize(tvu_ringpacket_t const volatile *self)
{
    return tvu_round_up_u64(tvu_ringpacket_size(self), tvu_ringpacket_hdrsize()) + tvu_ringpacket_hdrsize();
}

/** Convert a packet into a buffer.
 * @param packet    A ringbuffer packet.
 * @returns         A buffer object.
 */
static inline tvu_buffer_t tvu_ringpacket_to_buffer(tvu_ringpacket_t *self)
{
    tvu_buffer_t buffer = {
        .data = self->data,
        .size = tvu_ringpacket_size(self),
        .offset = 0,
        .fd = TVU_BUFFER_UNKNOWN
    };

    return buffer;
}

static inline size_t tvu_ring_hdrsize(void)
{
    return offsetof(tvu_ringbuffer_t, data);
}

/** Check if the ring buffer is initialized.
 * @param self  A pointer to an (un)initialized ring buffer.
 * @param size  The size of memory allocated for the ring buffer;
 *              not the size of the ring buffer itself.
 * @return      true if initialized, false if not.
 */
static inline bool tvu_ring_is_initialized(tvu_ringbuffer_t *self, size_t size)
{
    return self->size == tvu_round_down_u64(size - tvu_ring_hdrsize(), tvu_ringpacket_hdrsize());
}

/** Initialize the ring buffer.
 * @param self  A pointer to an unitialized and zeroed out ring buffer.
 * @param size  The size of memory allocated for the ring buffer;
 *              not the size of the ring buffer itself.
 * @return      0 if ok, -1 on error.
 */
static inline int tvu_ring_init(tvu_ringbuffer_t *self, size_t size)
{
    if (size < tvu_ring_hdrsize()) {
        errno = EINVAL;
        return -1;
    }
    self->head = 0;
    self->tail = 0;
    self->free = 0;
    self->size = tvu_round_down_u64(size - tvu_ring_hdrsize(), tvu_ringpacket_hdrsize());
    return 0;
}

/** Print debug information for the ring buffer.
 * @param self      Ring buffer.
 */
static inline void tvu_ring_debug(tvu_ringbuffer_t *self)
{
    fprintf(stderr, "Ringbuffer %llx(%llu) H:%llu T:%llu F:%llu\n",
        (unsigned long long)self->data,
        (unsigned long long)self->size,
        (unsigned long long)self->head,
        (unsigned long long)self->tail,
        (unsigned long long)self->free
    );
}

/** Advance the free pointer.
 * After a tail is advanced it will take a while until it is actually
 * consumed. This function needs to be called to move the free pointer
 * once one or more of the packets are actually consumed.
 *
 * param self   The ring buffer.
 */
void tvu_ring_advance_free(tvu_ringbuffer_t *self);

/** Get a packet at the head.
 *
 * @param self  The ring buffer
 * @param size  Number of bytes to reserve.
 * @returns     Packet.
 */
tvu_ringpacket_t *tvu_ring_head(tvu_ringbuffer_t *self, size_t size);

/** Get the packet at the tail.
 *
 * @param self  The ring buffer
 * @returns     Packet at end of the ring.
 */
tvu_ringpacket_t *tvu_ring_tail(tvu_ringbuffer_t *self);


#endif
