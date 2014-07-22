PROJECT(kdevhga)
INCLUDE_DIRECTORIES(.)

SET (kdevhga_SOURCES
	gchromoh.cpp
	ghga.cpp
	ginsth.cpp
	gnodeinfos.cpp
	kdevhga.cpp
	khgaview.cpp
	kheuristic.cpp
	kprjview.cpp
	main.cpp
   qtreeinfos.cpp
	kappoptions.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

KDE4_HANDLE_AUTOMOC(${kdevhga_SOURCES})

KDE4_ADD_UI_FILES(kdevhga_SOURCES
	kappoptions.ui
	kprjview.ui
	qtreeinfos.ui
	kheuristic.ui
	khgaview.ui
)

KDE4_ADD_EXECUTABLE(kdevhga ${kdevhga_SOURCES})
TARGET_LINK_LIBRARIES(kdevhga roptimization rcore rcorekde rmath roptimizationkde ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS})


########### install files ###############
install(TARGETS kdevhga ${INSTALL_TARGETS_DEFAULT_ARGS})
install( FILES kdevhga.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install(FILES kdevhgaui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kdevhga )
install(FILES kdevhgaui.rc  DESTINATION  /usr/share/kde4/apps/kdevhga )
kde4_install_icons(${ICON_INSTALL_DIR})