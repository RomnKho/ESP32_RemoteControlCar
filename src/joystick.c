/**
 * @file    joystick.c
 * @brief   Implementation file for reading a joystick using One-shot Mode Driver ADC and Calibration driver
 * @author  Roman Khomenko
 **/

#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define     ADC1_CHAN0      ADC_CHANNEL_4       // GPIO 32
#define     ADC1_CHAN1      ADC_CHANNEL_5       // GPIO 33
#define     ADC1_BITWIDTH   ADC_BITWIDTH_12
#define     ADC1_UNIT       ADC_UNIT_1
#define     ADC1_ATTEN      ADC_ATTEN_DB_12 

static bool b_chan0_cali;
static bool b_chan1_cali;

static const uint8_t  no_samples = 64;

static adc_oneshot_unit_handle_t    adc1_handle             = NULL;
static adc_cali_handle_t            adc_cali_chan0_handle   = NULL;
static adc_cali_handle_t            adc_cali_chan1_handle   = NULL;

static const char *TAG = "joystick.c";

static bool adc_calibration_init();

void joystick_init(void)
{
    // ADC1 init
    adc_oneshot_unit_init_cfg_t adc1_init = {
        .unit_id    = ADC1_UNIT,                // set ADC1
        .ulp_mode   = ADC_ULP_MODE_DISABLE,     // Normal mode - not low power mode
        .clk_src    = ADC_RTC_CLK_SRC_DEFAULT   // Default clk source
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_init, &adc1_handle));

    // ADC1 config
    adc_oneshot_chan_cfg_t adc1_chan_config = {
        .atten = ADC1_ATTEN,                    // The input voltage of ADC will be attenuated extending the range of measurement by about 12 dB
        .bitwidth = ADC1_BITWIDTH               // 12 bit res.         
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN0, &adc1_chan_config));    // x
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC1_CHAN1, &adc1_chan_config));    // y

    // ADC1 Calibration init
    b_chan0_cali = adc_calibration_init(ADC1_UNIT, ADC1_CHAN0, ADC1_ATTEN, &adc_cali_chan0_handle);
    b_chan1_cali = adc_calibration_init(ADC1_UNIT, ADC1_CHAN1, ADC1_ATTEN, &adc_cali_chan1_handle);
}

void joystick_adc_read(int16_t *x_read, int16_t *y_read, int *x_mV, int *y_mV) 
{
    if(b_chan0_cali == false || b_chan1_cali == false)
    {
        ESP_LOGI(TAG, "Calibration wrong");
        return;
    }

    int chan0_raw_read = 0;
    int chan1_raw_read = 0;

    int chan0_mV = 0;
    int chan1_mV = 0;

    uint32_t chan0_input = 0;
    uint32_t chan1_input = 0;    

    for(int i = 0; i < no_samples; i++)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN0, &chan0_raw_read));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC1_CHAN1, &chan1_raw_read));

        chan0_input += chan0_raw_read;
        chan1_input += chan1_raw_read;
    }

    chan0_input /= no_samples;
    chan1_input /= no_samples;

    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_chan0_handle, chan0_input, &chan0_mV));
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_chan1_handle, chan1_input, &chan1_mV));

    *x_read = chan0_input * 100 / 4095;
    *y_read = chan1_input * 100 / 4095;
    *x_mV   = chan0_mV;
    *y_mV   = chan1_mV;
}

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

    // ESP32 WROVER only supports line fitting
    if (!calibrated) 
    {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");

        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC1_BITWIDTH
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}