cmake_minimum_required(VERSION 2.8)

# find opencv
find_package(OpenCV REQUIRED)

include_directories(AFTER SYSTEM $ENV{G3X_PATH}/include)

set(CMAKE_C_FLAGS "-lm -L$ENV{lGL_PATH} -framework OpenGL -framework GLUT -L$ENV{G3X_PATH} -lg3x.6d.$ENV{PROCBIT}")

# or the automatic version
file(GLOB_RECURSE SRC_FILES *.c)

# for each sample file, make an exe
foreach (SRC_FILE ${SRC_FILES})
    get_filename_component(FILE ${SRC_FILE} NAME_WE)
    add_executable(${FILE} ${SRC_FILE})
    target_link_libraries(${FILE} ${OpenCV_LIBS})
endforeach ()

