#
# Building CommonUi lib
#


set( SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/subtab_frame.cpp
    )


add_library( CommonUi STATIC ${SRC_FILES} )
install( TARGETS CommonUi DESTINATION lib )

target_include_directories( CommonUi PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_libraries( CommonUi PUBLIC
    Borealis
    switch::libnx
    -L/opt/devkitpro/portlibs/switch/lib
    -L/opt/devkitpro/libnx/lib
    ${ZLIB_LIBRARIES}
    ${FREETYPE_LIBRARIES}
#    -lglfw3 -lEGL -lglad -lglapi -ldrm_nouveau -lm -lnx
    )
