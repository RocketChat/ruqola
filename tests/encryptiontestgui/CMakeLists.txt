# SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>, 2025 Andro Ranogajec <ranogaet@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause
find_package(OpenSSL REQUIRED)

add_executable(encryptiontestgui)
target_sources(encryptiontestgui PRIVATE encryptiontestgui.h encryptiontestgui.cpp)
target_link_libraries(encryptiontestgui
    Qt::Widgets
    Qt::Gui
    libruqolacore
)
set_target_properties(encryptiontestgui PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)

add_executable(encryptiontestcli
    ${CMAKE_CURRENT_SOURCE_DIR}/encryptiontestcli.cpp
)
target_include_directories(encryptiontestcli PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core/encryption
    ${CMAKE_BINARY_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/connection
    ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt
    ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt/e2e
)
target_link_libraries(encryptiontestcli
    librocketchatrestapi-qt
    libruqolawidgets
    KF6::Crash
    Qt::Network
    OpenSSL::Crypto
)
