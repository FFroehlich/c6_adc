#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"

namespace esphome {
namespace c6_adc {

class C6ADCSensor : public sensor::Sensor, public PollingComponent
{
public:
  /// Update ADC values
  void update() override;
  /// Setup ADC
  void setup() override;
  void set_channel(adc_channel_t channel) {
    this->channel_ = channel;
  }
  void set_attenuation(adc_atten_t atten) {
    this->attenuation_ = atten;
  }
protected:
    adc_cali_handle_t cali_handle;
    bool ready_{false};
    adc_channel_t channel_{ADC_CHANNEL_0};
    adc_atten_t attenuation_{ADC_ATTEN_DB_12};
};

}
}
