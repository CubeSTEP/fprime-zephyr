# Assumes the standard structure where fprime and fprime-zephyr are parallel directories
# include "../../../../fprime/Drv/Interfaces/Gpio.fpp"

module Zephyr {
    @ Zephyr GPIO Driver
    passive component ZephyrGpioDriver {  
              
        import Drv.Gpio

    }
}