# Assumes the standard structure where fprime and fprime-zephyr are parallel directories
# include "../../../../fprime/Drv/Interfaces/I2c.fpp"

module Zephyr {
    @ I²C driver for the fprime-zephyr OSAL layer that bridges F´ components with Zephyr's I²C API.
    passive component ZephyrI2CDriver {
        import Drv.I2c
    }
}