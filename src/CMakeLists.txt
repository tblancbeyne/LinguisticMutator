file(GLOB libLangMutSrc "lib/*.cpp")

add_library(libLangMut ${libLangMutSrc})

file(GLOB langMutExec "executables/*.cpp")

foreach(executable ${langMutExec})
    get_filename_component(execName ${executable} NAME_WE)

    add_executable(${execName} ${executable})

    target_link_libraries(${execName} libLangMut)
endforeach(executable)
