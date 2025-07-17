# SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

include(ECMAddAppIcon)

find_package(OpenSSL REQUIRED)

add_executable(ruqola main.cpp)
add_executable(e2ekeytool 
    ${CMAKE_SOURCE_DIR}/src/apps/e2ekeytool.cpp
    ${CMAKE_SOURCE_DIR}/src/core/encryption/encryptionutils.cpp
    ${CMAKE_SOURCE_DIR}/src/core/encryption/ruqolaencryptiondebug.cpp)

target_include_directories(e2ekeytool PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/encryption
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/connection
    ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt
)

target_link_libraries(ruqola
    libruqolawidgets
    librocketchatrestapi-qt
    KF6::Crash
    KF6::XmlGui
    KF6::IconThemes
)

target_link_libraries(e2ekeytool
    librocketchatrestapi-qt
    libruqolawidgets
    KF6::Crash
    Qt::Core
    Qt::Network
    OpenSSL::Crypto
)

if(USE_DBUS)
    target_link_libraries(ruqola KF6::DBusAddons)
    target_link_libraries(e2ekeytool KF6::DBusAddons)
else()
    target_link_libraries(ruqola KDAB::kdsingleapplication)
    target_link_libraries(e2ekeytool KDAB::kdsingleapplication)
    if (NOT WIN32 AND NOT APPLE)
        target_link_libraries(ruqola Qt::GuiPrivate)
    endif()
endif()

file(GLOB_RECURSE RUQOLA_ICONS "${PROJECT_SOURCE_DIR}/src/icons/*-apps-ruqola.png")
ecm_add_app_icon(appIcons ICONS "${RUQOLA_ICONS}")
target_sources(ruqola PRIVATE ${appIcons})

install(TARGETS ruqola ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(TARGETS e2ekeytool ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
