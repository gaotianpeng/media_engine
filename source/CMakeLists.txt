add_subdirectory(UI)
add_subdirectory(mediaengine)
add_subdirectory(ffmepgtest)
add_subdirectory(test)
add_subdirectory(audio)
add_subdirectory(video)

SET_PROPERTY(TARGET test PROPERTY FOLDER "TEST")
