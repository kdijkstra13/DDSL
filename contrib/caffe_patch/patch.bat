copy CaffeTargets.cmake ..\caffe\build\CaffeTargets.cmake
copy layer_factory.cpp ..\caffe\src\caffe\layer_factory.cpp
copy solver_factory.cpp ..\caffe\src\caffe\solver_factory.cpp
move ..\caffe\build\libraries\lib\libopenblas.dll.a ..\caffe\build\libraries\lib\libopenblas.dll.a.lib
move ..\caffe\build\libraries\lib\libopenblas.a ..\caffe\build\libraries\lib\libopenblas.a.lib
pause