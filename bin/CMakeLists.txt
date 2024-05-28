add_executable(${PROJECT_NAME} main.cpp ../lib/Camera.cpp ../lib/Matrix.cpp ../lib/Vec3.cpp ../lib/Model.cpp ../lib/Vec2.cpp ../lib/Quat.cpp)

target_link_libraries(${PROJECT_NAME} luminar_controller)
target_link_libraries(${PROJECT_NAME} luminar_view)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})