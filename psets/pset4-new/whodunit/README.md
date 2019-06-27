# Questions

## What's `stdint.h`?

It's a header file with the purpose of enabling new type definitions for integers with defined lengths.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

The new unsigned integer definitions allow for bigger or fixed width integers which are limited with only positive numbers.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

A *BYTE* has just one byte, *DWORD* has 4 bytes, *LONG* too has 4 bytes and *WORD* 2 bytes.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The header signature, which in ASCII is "BM" for bitmap, presumably.

## What's the difference between `bfSize` and `biSize`?
*bfSize* stores the size of the entire bitmap while *biSize* only stores the size of the header.
TODO

## What does it mean if `biHeight` is negative?

It means the orientation of the bitmap image, regarding the order of its pixels, is top-down (as it would intuitively).

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

In the header, *biBitCount* specifies the color depth of uncompressed files.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

That might happen if fopen couldn't access the file, normally when the file doesn't exists, or if a permission error occurs.

## Why is the third argument to `fread` always `1` in our code?

Because we are reading only one item of the size specified at a time, like pixels (which are 3 bytes each).

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

`4 - (((3 * 3) % 4) % 4) = 3`, therefore three bytes of padding.
## What does `fseek` do?

It changes the current "cursor" on the file being read, allowing to change the position relative to its content.

## What is `SEEK_CUR`?

That is the position relative to which we will add an offset to set the new cursor position.

## Whodunit?

It's no mistery it was Colonel Mustard, with the candlestick in the library.
