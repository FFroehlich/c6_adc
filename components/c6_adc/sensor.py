import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import UNIT_VOLT, DEVICE_CLASS_VOLTAGE, STATE_CLASS_MEASUREMENT
from esphome.const import CONF_NUMBER, CONF_PIN, CONF_ATTENUATION, CONF_ID

ATTENUATION_MODES = {
    "0db": cg.global_ns.ADC_ATTEN_DB_0,
    "2.5db": cg.global_ns.ADC_ATTEN_DB_2_5,
    "6db": cg.global_ns.ADC_ATTEN_DB_6,
    "12db": cg.global_ns.ADC_ATTEN_DB_12,
}

adc_channel_t = cg.global_ns.enum("adc_channel_t")
ADC_PIN_TO_CHANNEL = {
        0: adc_channel_t.ADC_CHANNEL_0,
        1: adc_channel_t.ADC_CHANNEL_1,
        2: adc_channel_t.ADC_CHANNEL_2,
        3: adc_channel_t.ADC_CHANNEL_3,
        4: adc_channel_t.ADC_CHANNEL_4,
        5: adc_channel_t.ADC_CHANNEL_5,
        6: adc_channel_t.ADC_CHANNEL_6,
}

def validate_adc_pin(value):
    conf = pins.internal_gpio_input_pin_schema(value)
    value = conf[CONF_NUMBER]
    if value not in ADC_PIN_TO_CHANNEL:
        raise cv.Invalid(f"Pin {value} is invalid for ADC")
    return conf

c6_adc_ns = cg.esphome_ns.namespace("c6_adc")

C6ADCSensor = c6_adc_ns.class_("C6ADCSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(
        C6ADCSensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=3,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
        )
    .extend(
        {
            cv.Required(CONF_PIN): validate_adc_pin,
            cv.Optional(CONF_ATTENUATION): cv.All()
        })
    .extend(cv.polling_component_schema("5s")),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    if attenuation := config.get(CONF_ATTENUATION):
        cg.add(var.set_attenuation(attenuation))
    pin_num = config[CONF_PIN][CONF_NUMBER]
    chan = ADC_PIN_TO_CHANNEL[pin_num]
    cg.add(var.set_channel(chan))
