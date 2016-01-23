/*
 * Frame allocator.
 * Very, very fast allocator for temporary data freed at the end of each frame.
 */
#pragma once

void frame_alloc_create(size_t poolsz);
void frame_alloc_destroy();

void *frame_alloc(size_t bytes);
void frame_alloc_reset();