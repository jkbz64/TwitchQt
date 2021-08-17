cmake_minimum_required(VERSION 3.14)

project(TwitchQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TwitchQt INTERFACE)
add_library(TwitchQt::TwitchQt ALIAS TwitchQt)

target_sources(TwitchQt
    INTERFACE
        TwitchQt/json.hpp
        TwitchQt/twitchapi.hpp
        TwitchQt/twitchapi.inl
        TwitchQt/twitchbadge.hpp
        TwitchQt/twitchbadgereply.hpp
        TwitchQt/twitchemote.hpp
        TwitchQt/twitchemotereply.hpp
        TwitchQt/twitchgame.hpp
        TwitchQt/twitchgamereply.hpp
        TwitchQt/twitchreply.hpp
        TwitchQt/twitchstream.hpp
        TwitchQt/twitchstreamreply.hpp
        TwitchQt/twitchuser.hpp
        TwitchQt/twitchuserreply.hpp
        TwitchQt/json/json.hpp)

target_include_directories(TwitchQt
    INTERFACE
        $<BUILD_INTERFACE:${TwitchQt_SOURCE_DIR}/TwitchQt>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/TwitchQt>)
