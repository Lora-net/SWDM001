/**
 * \file
 *
 * \brief Definitions of sx_comp_t samples
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

#include "sx126x_comp_samples.h"

const sx_comp_t sx_comp_sample_sx1261_xtal =
    {
        .radio_data = {
            .name = "sx1261",
            .sx_comp_flags = SX_COMP_RADIO_DATA_SX_COMP_FLAGS_NO_TCXO,
            .radio_flags = SX_COMP_RADIO_DATA_SX126X_COMP_FLAGS_SX1261,
            .cal_img_freq1_in_mhz = 863,
            .cal_img_freq2_in_mhz = 870,
        },
        .driver = SX126X_COMP_DRV_INSTANTIATE,
};

const sx_comp_t sx_comp_sample_sx1261_tcxo =
    {
        .radio_data = {
            .name = "sx1261",
            .sx_comp_flags = SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO,
            .radio_tcxo_voltage_in_radio_units = 2,
            .init_tcxo_startup_time_in_radio_units = 64, // 64000 per second
            .radio_flags = SX_COMP_RADIO_DATA_SX126X_COMP_FLAGS_SX1261,
            .cal_img_freq1_in_mhz = 863,
            .cal_img_freq2_in_mhz = 870,
        },
        .driver = SX126X_COMP_DRV_INSTANTIATE,
};

const sx_comp_t sx_comp_sample_sx1262_xtal =
    {
        .radio_data = {
            .name = "sx1262",
            .sx_comp_flags = SX_COMP_RADIO_DATA_SX_COMP_FLAGS_NO_TCXO,
            .radio_flags = SX_COMP_RADIO_DATA_SX126X_COMP_FLAGS_SX1262,
            .cal_img_freq1_in_mhz = 863,
            .cal_img_freq2_in_mhz = 870,
        },
        .driver = SX126X_COMP_DRV_INSTANTIATE,
};

const sx_comp_t sx_comp_sample_sx1262_tcxo =
    {
        .radio_data = {
            .name = "sx1262",
            .sx_comp_flags = SX_COMP_RADIO_DATA_SX_COMP_FLAGS_TCXO,
            .radio_tcxo_voltage_in_radio_units = 2,
            .init_tcxo_startup_time_in_radio_units = 64, // 64000 per second
            .radio_flags = SX_COMP_RADIO_DATA_SX126X_COMP_FLAGS_SX1262,
            .cal_img_freq1_in_mhz = 863,
            .cal_img_freq2_in_mhz = 870,
        },
        .driver = SX126X_COMP_DRV_INSTANTIATE,
};

const sx_comp_t sx_comp_sample_sx126x_dongle =
    {
        .radio_data = {
            .name = "sx126x",
        },
        .driver = SX126X_COMP_DRV_INSTANTIATE,
};
