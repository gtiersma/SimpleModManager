# Building ModBrowser lib




set( SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameModBrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TabGroupBrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TabModBrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TabModOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TabModPlugins.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TabModPresets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ThumbnailPresetEditor.cpp
    )




add_library( FrameModBrowser STATIC ${SRC_FILES} )
install( TARGETS FrameModBrowser DESTINATION lib )

target_include_directories( FrameModBrowser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_libraries( FrameModBrowser PUBLIC
    Borealis
    CommonUi
    CoreExtension
    switch::libnx
    -L/opt/devkitpro/portlibs/switch/lib
    -L/opt/devkitpro/libnx/lib
    ${ZLIB_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    -lglfw3 -lEGL -lglad -lglapi -ldrm_nouveau -lm -lnx
    )
