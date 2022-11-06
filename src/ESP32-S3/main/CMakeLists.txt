idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES anthracide_led anthracide_i2c sigma_dsp udp_server ir_nec_decoder nextion_display)