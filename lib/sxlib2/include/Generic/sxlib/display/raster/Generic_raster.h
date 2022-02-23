/**
 * \file
 *
 * \brief Generic interface header for raster-based displays
 */

/**
 * \file
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SXLIB_INCLUDE_GENERIC_SXLIB_GENERIC_RASTER_GENERIC_RASTER_H_
#define SXLIB_INCLUDE_GENERIC_SXLIB_GENERIC_RASTER_GENERIC_RASTER_H_

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

// Opaque raster-device instance
typedef struct sxlib_generic_raster_s sxlib_generic_raster_t;

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * If CONFIG_SMTC_DEMO_ENABLE_DISPLAY is not set, various draw operations, below, will be replaced with NOP
 */

#ifdef CONFIG_SMTC_DEMO_ENABLE_DISPLAY
#define SXLIB_GENERIC_RASTER_INIT_SCREEN( dev, rgb888 ) sxlib_generic_raster_init_screen( dev, rgb888 )
#define SXLIB_GENERIC_RASTER_CLEAR_SCREEN( dev, rgb888 ) sxlib_generic_raster_clear_screen( dev, rgb888 )
#define SXLIB_GENERIC_RASTER_DRAW_STRING( dev, fg_rgb888, bg_rgb888, x, y, string ) \
    sxlib_generic_raster_draw_string( dev, fg_rgb888, bg_rgb888, x, y, string )
#define SXLIB_GENERIC_RASTER_DRAW_RECTANGLE( dev, x, y, width, height, rgb888 ) \
    sxlib_generic_raster_draw_rectangle( dev, x, y, width, height, rgb888 )
#else
#define SXLIB_GENERIC_RASTER_INIT_SCREEN( dev, rgb888 ) \
    do                                                  \
    {                                                   \
    } while( 0 )
#define SXLIB_GENERIC_RASTER_CLEAR_SCREEN( dev, rgb888 ) \
    do                                                   \
    {                                                    \
    } while( 0 )
#define SXLIB_GENERIC_RASTER_DRAW_STRING( dev, fg_rgb888, bg_rgb888, x, y, string ) \
    do                                                                              \
    {                                                                               \
    } while( 0 )
#define SXLIB_GENERIC_RASTER_DRAW_RECTANGLE( dev, x, y, width, height, rgb888 ) \
    do                                                                          \
    {                                                                           \
    } while( 0 )
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION PROTOTYPES ----------------------------------------------
 */

/**
 * @brief Initialize screen
 *
 * @param dev Raster device
 * @param rgb888 The desired screen color
 */
void sxlib_generic_raster_init_screen( const sxlib_generic_raster_t* dev, uint32_t rgb888 );

/**
 * @brief Clear screen
 *
 * @param dev Raster device
 * @param rgb888 The desired screen color
 */
void sxlib_generic_raster_clear_screen( const sxlib_generic_raster_t* dev, uint32_t rgb888 );

/**
 * @brief Draw ASCII string
 *
 * @param dev Raster device
 * @param fg_rgb888 The desired text foreground color
 * @param bg_rgb888 The desired text background color
 * @param x Text x coordinate
 * @param y Text y coordinate
 * @param string The string to display
 */
void sxlib_generic_raster_draw_string( const sxlib_generic_raster_t* dev, uint32_t fg_rgb888, uint32_t bg_rgb888, int x,
                                       int y, const char* string );

/**
 * @brief Draw hexadecimal data as a string
 *
 * @param dev Raster device
 * @param fg_rgb888 The desired text foreground color
 * @param bg_rgb888 The desired text background color
 * @param x Text x coordinate
 * @param y Text y coordinate
 * @param data Data buffer
 * @param len Length of data buffer, in bytes
 */
void sxlib_generic_raster_draw_hex( const sxlib_generic_raster_t* dev, uint32_t fg_rgb888, uint32_t bg_rgb888, int x,
                                    int y, const uint8_t* data, unsigned int len );

/**
 * @brief Draw a filled rectangle
 *
 * @param dev Raster device
 * @param x Rectangle top-left x coordinate
 * @param y Rectangle top-left y coordinate
 * @param width Rectangle width
 * @param height Rectangle height
 * @param rgb888 The desired rectangle color
 */
void sxlib_generic_raster_draw_rectangle( const sxlib_generic_raster_t* dev, int x, int y, int width, int height,
                                          uint32_t rgb888 );

/**
 * @brief Send raw bitmap data to the display driver
 *
 * @param dev Raster device
 * @param x Region top-left x coordinate
 * @param y Region top-left y coordinate
 * @param width Region width
 * @param height Region height
 * @param bytes_per_pixel The number of bytes stored per pixel in the data buffer
 * @param data Data buffer
 */
void sxlib_generic_raster_draw_bitmap( const sxlib_generic_raster_t* dev, int x, int y, int width, int height,
                                       unsigned int bytes_per_pixel, uint8_t* data );

/**
 * @brief Draw a filled rectangle
 *
 * @param dev Raster device
 * @param rgb888 The desired rectangle color
 * @param x Rectangle top-left x coordinate
 * @param y Rectangle top-left y coordinate
 * @param width Rectangle width
 * @param height Rectangle height
 * @param x_thickness Border thickness of vertical lines
 * @param y_thickness Border thickness of horizontal lines
 */
void sxlib_generic_raster_draw_frame( const sxlib_generic_raster_t* dev, uint32_t rgb888, int x, int y, int width,
                                      int height, int x_thickness, int y_thickness );

/**
 * @brief Draw a string inside a frame
 *
 * @param dev Raster device
 * @param fg_rgb888 Text foreground color
 * @param bg_rgb888 Text background color
 * @param border_rgb888 Border color
 * @param x X coordinate
 * @param y Y coordinate
 * @param thickness Border thickness
 * @param x_margin Horizontal distance separating the border from the text
 * @param y_margin Vertical distance separating the border from the text
 * @param x_align_text If true, x coordinate is used to position the text, not the frame
 * @param y_align_text If true, y coordinate is used to position the text, not the frame
 * @param string String to display
 */
void sxlib_generic_raster_draw_boxed_string( const sxlib_generic_raster_t* dev, uint32_t fg_rgb888, uint32_t bg_rgb888,
                                             uint32_t border_rgb888, int x, int y, int thickness, int x_margin,
                                             int y_margin, bool x_align_text, bool y_align_text, const char* string );

/**
 * @brief Convert 24-bit RGB888 color to 16-bit RGB565 color
 *
 * @param rgb888 RGB565-formatted color
 *
 * @return 24-bit RGB888-formatted color
 */
static inline uint16_t sxlib_generic_raster_rgb888_to_rgb565( uint32_t rgb888 )
{
    uint32_t r8 = ( rgb888 >> 16 ) & 0xFF;
    uint32_t g8 = ( rgb888 >> 8 ) & 0xFF;
    uint32_t b8 = ( rgb888 >> 0 ) & 0xFF;

    uint32_t r5 = r8 >> 3;
    uint32_t g6 = g8 >> 2;
    uint32_t b5 = b8 >> 3;

    return ( r5 << 11 ) | ( g6 << 5 ) | b5;
}

/**
 * @brief Create a packet-count string from a value
 *
 * @param value A packet count value
 * @param buffer A buffer of at least length 10 where the result will be stored
 */
static inline void sxlib_generic_raster_format_pkt_count( char* buffer, uint32_t value )
{
    sprintf( buffer, "%7" PRIu32, ( ( value ) &0x7FFFFF ) );
}

/**
 * @brief Create a packet-error-rate percentage string from a value and reference
 *
 * @param value The total number of lost packets
 * @param reference The total number of transmitted packets
 * @param buffer A buffer of at least length 10 where the result will be stored
 */
static inline void sxlib_generic_raster_format_per( char* buffer, uint32_t value, uint32_t reference )
{
    const unsigned int int_part  = ( ( ( uint64_t ) value * 100000 ) / reference ) / 1000;
    const unsigned int frac_part = ( ( ( uint64_t ) value * 100000 ) / reference ) % 1000;
    sprintf( buffer, "%3u.%03u", int_part, frac_part );
}

/**
 * @brief Create a zero prefixed 8-digit integral string from a value
 *
 * @param value The integral value to convert
 * @param buffer A buffer of at least length 10 where the result will be stored
 */
static inline void sxlib_generic_raster_format_uint32_t( char* buffer, uint32_t value )
{
    sprintf( buffer, "%08" PRIx32, value );
}

/**
 * @brief Create a 4-digit signed integral string from a value
 *
 * @param value The signed integral value to convert
 * @param buffer A buffer of at least length 10 where the result will be stored
 */
static inline void sxlib_generic_raster_format_rssi_snr( char* buffer, int16_t value )
{
    sprintf( buffer, "   %4d", ( value ) );
}

/**
 * @brief Display a packet-count from a value
 *
 * @param display Raster device
 * @param fg_rgb888 24-bit RGB foreground color
 * @param bg_rgb888 24-bit RGB background color
 * @param buffer A buffer of at least length 10 where the result will be stored
 * @param x Display x coordinate
 * @param y Display y coordinate
 * @param value A packet count value
 */
static inline void display_raster_format_pkt_count( sxlib_generic_raster_t* display, uint32_t fg_rgb888,
                                                    uint32_t bg_rgb888, char* buffer, int x, int y, uint32_t value )
{
    sxlib_generic_raster_format_pkt_count( buffer, value );
    SXLIB_GENERIC_RASTER_DRAW_STRING( display, fg_rgb888, bg_rgb888, x, y, buffer );
}

/**
 * @brief Display a packet-error-rate percentage string from a value and reference
 *
 * @param display Raster device
 * @param fg_rgb888 24-bit RGB foreground color
 * @param bg_rgb888 24-bit RGB background color
 * @param buffer A buffer of at least length 10 where the result will be stored
 * @param x Display x coordinate
 * @param y Display y coordinate
 * @param value The total number of lost packets
 * @param ref The total number of transmitted packets
 */
static inline void display_raster_format_per( sxlib_generic_raster_t* display, uint32_t fg_rgb888, uint32_t bg_rgb888,
                                              char* buffer, int x, int y, uint32_t value, uint32_t ref )
{
    sxlib_generic_raster_format_per( buffer, value, ref );
    SXLIB_GENERIC_RASTER_DRAW_STRING( display, fg_rgb888, bg_rgb888, x, y, buffer );
}

/**
 * @brief Display a zero prefixed 8-digit integral string from a value
 *
 * @param display Raster device
 * @param fg_rgb888 24-bit RGB foreground color
 * @param bg_rgb888 24-bit RGB background color
 * @param buffer A buffer of at least length 10 where the result will be stored
 * @param x Display x coordinate
 * @param y Display y coordinate
 * @param value The integral value to convert
 */
static inline void display_raster_format_uint32_t( sxlib_generic_raster_t* display, uint32_t fg_rgb888,
                                                   uint32_t bg_rgb888, char* buffer, int x, int y, uint32_t value )
{
    sxlib_generic_raster_format_uint32_t( buffer, value );
    // TODO: shift others to the rigth to avoid this left shift by 8.
    SXLIB_GENERIC_RASTER_DRAW_STRING( display, fg_rgb888, bg_rgb888, x - 8, y, buffer );
}

/**
 * @brief Display a 4-digit signed integral string from a value
 *
 * @param display Raster device
 * @param fg_rgb888 24-bit RGB foreground color
 * @param bg_rgb888 24-bit RGB background color
 * @param buffer A buffer of at least length 10 where the result will be stored
 * @param x Display x coordinate
 * @param y Display y coordinate
 * @param value The signed integral value to convert
 */
static inline void display_raster_format_rssi_snr( sxlib_generic_raster_t* display, uint32_t fg_rgb888,
                                                   uint32_t bg_rgb888, char* buffer, int x, int y, int16_t value )
{
    sxlib_generic_raster_format_rssi_snr( buffer, value );
    SXLIB_GENERIC_RASTER_DRAW_STRING( display, fg_rgb888, bg_rgb888, x, y, buffer );
}

#ifdef __cplusplus
}
#endif

#endif  // SXLIB_INCLUDE_GENERIC_SXLIB_GENERIC_RASTER_GENERIC_RASTER_H_

/* --- EOF ------------------------------------------------------------------ */
