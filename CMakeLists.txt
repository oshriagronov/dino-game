cmake_minimum_required(VERSION 3.21)

project(dino_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
  set(CMAKE_INSTALL_RPATH "@loader_path")
elseif(UNIX)
  set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

function(require_first_target out_var)
  foreach(candidate IN LISTS ARGN)
    if(TARGET "${candidate}")
      set(${out_var} "${candidate}" PARENT_SCOPE)
      return()
    endif()
  endforeach()

  message(FATAL_ERROR "None of the expected targets were found: ${ARGN}")
endfunction()

require_first_target(SDL2_CORE_TARGET
  SDL2::SDL2
  SDL2::SDL2-static
)

require_first_target(SDL2_IMAGE_TARGET
  SDL2_image::SDL2_image
  SDL2_image::SDL2_image-static
)

require_first_target(SDL2_TTF_TARGET
  SDL2_ttf::SDL2_ttf
  SDL2_ttf::SDL2_ttf-static
)

if(WIN32)
  require_first_target(SDL2_MAIN_TARGET
    SDL2::SDL2main
    SDL2::SDL2main-static
  )
endif()

add_executable(dino-game
  src/Cactus.cpp
  src/Dino.cpp
  src/Game.cpp
  src/Track.cpp
  src/main.cpp
)

target_include_directories(dino-game PRIVATE include)

target_link_libraries(dino-game PRIVATE
  ${SDL2_CORE_TARGET}
  ${SDL2_IMAGE_TARGET}
  ${SDL2_TTF_TARGET}
)

if(WIN32)
  target_link_libraries(dino-game PRIVATE ${SDL2_MAIN_TARGET})
endif()

if(APPLE)
  set_target_properties(dino-game PROPERTIES MACOSX_BUNDLE TRUE)
endif()

install(TARGETS dino-game
  RUNTIME DESTINATION .
  BUNDLE DESTINATION .
)

if(APPLE)
  install(
    DIRECTORY assets
    DESTINATION dino-game.app/Contents/Resources
    PATTERN ".DS_Store" EXCLUDE
  )
else()
  install(
    DIRECTORY assets
    DESTINATION .
    PATTERN ".DS_Store" EXCLUDE
  )
endif()

install(CODE [[
  if(APPLE)
    set(exe "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/dino-game.app/Contents/MacOS/dino-game")
    set(dest "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/dino-game.app/Contents/MacOS")
  else()
    set(exe "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/dino-game${CMAKE_EXECUTABLE_SUFFIX}")
    set(dest "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
  endif()

  if(EXISTS "${exe}")
    file(GET_RUNTIME_DEPENDENCIES
      EXECUTABLES "${exe}"
      RESOLVED_DEPENDENCIES_VAR deps
      UNRESOLVED_DEPENDENCIES_VAR unresolved
      PRE_EXCLUDE_REGEXES
        "api-ms-win-.*"
        "ext-ms-win-.*"
      POST_EXCLUDE_REGEXES
        "^/System/Library/"
        "^/usr/lib/"
        "^/lib/"
    )

    foreach(dep IN LISTS deps)
      file(INSTALL DESTINATION "${dest}" TYPE SHARED_LIBRARY FILES "${dep}")
    endforeach()

    if(unresolved)
      message(WARNING "Unresolved runtime dependencies: ${unresolved}")
    endif()
  else()
    message(WARNING "Executable not found at install time: ${exe}")
  endif()
]])
