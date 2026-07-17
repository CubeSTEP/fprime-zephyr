# Zephyr::ZephyrADCDriver

Zephyr ADC Driver for the fprime-zephyr OSAL layer providing access to the Zephyr ADC driver APIs. 
Currently only supports 12 bit resolution and single channel sampling.

## Configuration

```Cpp
#define FPRIME_ADC_NODE DT_PATH(zephyr_user)
static const struct adc_dt_spec adc_configuration_storage =
    ADC_DT_SPEC_GET_BY_NAME(FPRIME_ADC_NODE, potentiometer);
const struct adc_dt_spec* adc_configuration = &adc_configuration_storage;

void configureTopology(){
    adcDriver.configure(adc_configuration);
};
```
<!-- If the component requires configuration at initialization, document here -->
