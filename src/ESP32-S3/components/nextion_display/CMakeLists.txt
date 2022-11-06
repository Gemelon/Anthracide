set(component_srcs "nextion_display.c"
                  )

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES data_store)
