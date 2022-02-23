/**
 * \file
 *
 * \brief Implementation of LED/LCD interface preparation code
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

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <sxlib/System/config/Rtos_config.h>
#include <sxlib/Gpio/Led/Generic_Led.h>
#include <sxlib/display/interface/Generic_interface.h>
#include <sxlib/display/raster/Generic_raster.h>

#include "prepare_interface.h"

#include "smtc_shield.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTION DEFINITIONS ---------------------------------------------
 */

void main_single_interface_init( sxlib_generic_interface_t* interface )
{
    *interface = ( sxlib_generic_interface_t ){ 0 };

    interface->led_interface.led_tx = ( struct sxlib_Gpio_Led_dev* ) &global_gpio_led_tx;
    interface->led_interface.led_rx = ( struct sxlib_Gpio_Led_dev* ) &global_gpio_led_rx;

#ifdef CONFIG_SMTC_DEMO_ENABLE_DISPLAY
    interface->display_interface.display   = &global_display;
    interface->display_interface.bg_rgb888 = 0x000000;
    interface->display_interface.fg_rgb888 = 0xFFFFFF;
    sxlib_generic_raster_init_screen( interface->display_interface.display, interface->display_interface.bg_rgb888 );
#endif
}

/* --- EOF ------------------------------------------------------------------ */
