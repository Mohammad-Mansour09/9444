/* Copyright (c) 2019, 2020, Daniel Kasza
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "banner.h"
#include "util.h"

#define KERNEL_START  0x80200000
#define DVTREE_START (KERNEL_START + (16*1024*1024))
#define DVTREE_OURS  (0x40)
#define DVTREE_SIZE  (32*1024)

#define HTIF_BASE_ADDR 0x40008000

void printc(char c) {
    *(volatile uint32_t*)(HTIF_BASE_ADDR + 0) = c;
    *(volatile uint32_t*)(HTIF_BASE_ADDR + 4) = 0x01010000;
}

int main() {
    printstr(banner);

    /* Copy the DTB closer to the kernel. */
    void *dtb_addr = (void*)DVTREE_START;
    memcpy(
        dtb_addr,
        (void*)DVTREE_OURS,
        DVTREE_SIZE
    );

    start_supervisor(KERNEL_START, (uint64_t)dtb_addr);

    for(;;);

    return 1;
}
