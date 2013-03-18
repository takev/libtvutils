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
#include <tvutils/random.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

__thread uint64_t    tvu_random_data[TVU_RANDOM_DATA_SIZE];
__thread uint64_t    tvu_random_count = 0;

void tvu_random_load(void)
{
    int fd;
    int r;

    if ((fd = open("/dev/urandom", O_RDONLY)) == -1) {
        perror("Could not open '/dev/urandom'.");
        abort();
    }

    if ((r = read(fd, tvu_random_data, TVU_RANDOM_DATA_BYTE_SIZE)) == -1) {
        perror("Could not read from '/dev/urandom'.");
        abort();
    }

    if (r != TVU_RANDOM_DATA_BYTE_SIZE) {
        fprintf(stderr, "Could not read 0x8000 bytes from '/dev/urandom'.\n");
        abort();
    }

    if (close(fd) == -1) {
        perror("Could not close '/dev/urandom'.");
        abort();
    }
}

