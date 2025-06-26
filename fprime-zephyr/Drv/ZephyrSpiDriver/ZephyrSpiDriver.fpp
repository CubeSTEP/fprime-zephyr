# Assumes the standard structure where fprime and fprime-zephyr are parallel directories
include "../../../../fprime/Drv/Interfaces/Spi.fpp"

module Zephyr {
    @ SPI Driver for Zephyr
    passive component ZephyrSpiDriver {
        import Drv.Spi
    }
}