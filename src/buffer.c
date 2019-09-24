#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

/*
 * Gap Buffer is a simple character array with an empty space
 * between two chunks of characters that acts as a cursor and
 * allows for fast inserts and deletes. A Gap Buffer (say of size 6)
 * when initialized will have the following structure:
 *
 * +---+---+---+---+---+---+
 * | X | X | X | X | X | X |
 * +---+---+---+---+---+---+
 *
 * X's represents the locations occupied by the cursor, which during
 * initialization is the whole buffer.
 */
buffer* init(int size)
{
    buffer *buf = malloc(sizeof(buffer));
    buf->size = size;
    buf->arr = (char*)calloc((size_t)buf->size, sizeof(char));
    /* initially the cursor spreads over the entirity of the buffer */
    buf->gap_size = size;
    buf->cursor_start = &buf->arr[0];
    buf->cursor_end = &buf->arr[buf->size - 1];

    return buf;
}

/* insert a gap twice the size of the previous gap at the cursor. */
static void resize(buffer *buf)
{
    int *res = realloc(buf->arr, (buf->gap_size * 2) * sizeof(char));
    if(res) {
        buf->gap_size *= 2;
        buf->size += buf->gap_size;
        buf->cursor_start++;
        buf->cursor_end = &buf->arr[(buf->size - 1)];
        return;
    } else {
        fprintf(stderr, "ERROR: realloc failed");
        return;
    }
}

/* moves the cursor one location forward and copies
 * character at the beginning of the rear end to the
 * end of the front end of the buffer.
 */
void move_cursor_forward(buffer *buf)
{
    char *pointer = buf->cursor_end +1;
    char temp = *buf->cursor_start;
    *buf->cursor_start = *pointer;
    *pointer = temp;
    buf->cursor_start++;
    buf->cursor_end++;
}

/* moves the cursor one location backward and copies
 * character at the end of the front end to the
 * beginning of the rear end of the buffer.
 */
void move_cursor_backward(buffer *buf)
{
    char *pointer = buf->cursor_start -1;
    char temp = *buf->cursor_end;
    *buf->cursor_end = *pointer;
    *pointer = temp;
    buf->cursor_start--;
    buf->cursor_end--;
}


/* inserts a string at the start pos of the cursor. */
void insert_str(buffer *buf, char *str)
{
    size_t length = strlen(str);
    for(int i=0; i<length; i++) {
        *buf->cursor_start = str[i];
        buf->cursor_start++;
        if(buf->cursor_start > buf->cursor_end) {
            resize(buf);
        }
    }
}

/* deletes the character array and other contents of the buffer struct */
void delete_buffer(buffer *buf)
{
    free(buf->arr);
    free(buf);
}
