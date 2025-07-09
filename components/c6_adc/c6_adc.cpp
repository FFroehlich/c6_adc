#include "esphome/core/log.h"
#include "c6_adc.h"

namespace esphome {
namespace c6_adc {

static const char *TAG = "c6_adc";

//Global handle for adc
static adc_oneshot_unit_handle_t adc_handle;
static bool is_handle_configured = false;


void C6ADCSensor::setup() {
    // This will be called by App.setup()
    esp_err_t err;
    ESP_LOGCONFIG(TAG, "Setting up ADC '%s'...", this->get_name().c_str());
    if (! is_handle_configured) {
        ESP_LOGCONFIG(TAG, "Initializing ADC...");
        adc_oneshot_unit_init_cfg_t init_config1 = {
            .unit_id = ADC_UNIT_1,
            .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
            };
        err = adc_oneshot_new_unit(&init_config1, &adc_handle);
        if (err != ESP_OK) {
            ESP_LOGCONFIG(TAG, "Failed to initialize ADC: %d", err);
            return;
        }
        is_handle_configured = true;
    }
    ESP_LOGCONFIG(TAG, "Failed to initialize ADC: %d", err);
    adc_oneshot_chan_cfg_t config = {
        .atten = attenuation_,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    err = adc_oneshot_config_channel(adc_handle, channel_, &config);
    if (err != ESP_OK) {
        ESP_LOGCONFIG(TAG, "Failed to configure ADC channel %d: %d", channel_, err);
        return;
    }
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .chan = channel_,
        .atten = attenuation_,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
    err = adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle);
    if (err != ESP_OK) {
        ESP_LOGCONFIG(TAG, "Failed to calibrate ADC channel %d: %d", channel_, err);
        return;
    }
    ready_ = true;
}
void C6ADCSensor::update() {
    this->publish_state(sample());
}
float C6ADCSensor::sample() {
    esp_err_t err;
    int adc_read;
    if (! ready_) {
        return 0.0;
    }
    int raw_read;
    adc_oneshot_read(adc_handle, channel_, &raw_read);
    ESP_LOGI(TAG, "Raw ADC: %d Calibrated: %d", raw_read, adc_read);
    err = adc_oneshot_get_calibrated_result(adc_handle, cali_handle, channel_, &adc_read);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read adc for channel %d: %d", channel_, err);
        return 0.0;
    }
    return adc_read / 1000.0;
}

}
}
