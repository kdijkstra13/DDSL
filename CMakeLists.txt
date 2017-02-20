cmake_minimum_required(VERSION 2.6)

project(DDSL)

#Compiler flags
option(CLANG "Use the Clang compiler" OFF)
option(DDSL_EXT_LIB "Use external libraries" ON)
option(DDSL_EXT_LIB_PNG "PNG support" ON)
option(DDSL_EXT_LIB_SVM "SVM support" ON)
option(DDSL_EXT_LIB_AF "ArrayFire support" ON)
option(DDSL_EXT_LIB_CAFFE "Caffe support" ON)

#Projects
option(BUILD_DDSL_NO_EXT_LIB "Build vanilla DDSL" OFF)
option(BUILD_CUDA_GPU "Build with GPU support" ON)
option(BUILD_DDSL_TESTER "Build testing project for debugging DDSL" ON)
option(BUILD_LIB_DDSL "Build DDSL as a static library" OFF) #Depricated/unmaintained use header file code instead
option(BUILD_LIBPNG_DDSL "Build a library for PNG" ON)
option(BUILD_LIBSVM_DDSL "Build a library for SVM" ON)
option(BUILD_LIBAF_DDSL "Build a library for ArrayFire" ON)
option(BUILD_LIBCAFFE_DDSL "Build a library for Caffe" ON)

option(BUILD_GCC_O3 "Build using -O3 optimization" OFF)
option(BUILD_GCC_Ofast "Build using OFast optimization" ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14 -march=native -fPIC -Wno-unused-result")
if (BUILD_GCC_O3)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	message("Added -O3")
endif()
if (BUILD_GCC_Ofast)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
	message("Added -Ofast")
endif()

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread -std=c++14 -march=native -Wall -fPIC -Wno-unused-function -Wno-unused-result" )
if (NOT DDSL_EXT_LIB)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDDSL_NO_EXT_LIB" )
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDDSL_NO_EXT_LIB" )
else()
	if (NOT DDSL_EXT_LIB_PNG)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDDSL_NO_EXT_LIB_PNG" )
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDDSL_NO_EXT_LIB_PNG" )
	else()
		#sudo apt-get libpng-dev
		find_package(ZLIB REQUIRED)
		find_package(PNG REQUIRED)
	endif()
	if (NOT DDSL_EXT_LIB_SVM)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDDSL_NO_EXT_LIB_SVM" )
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDDSL_NO_EXT_LIB_SVM" )
	endif()
	if (NOT DDSL_EXT_LIB_CAFFE)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDDSL_NO_EXT_LIB_CAFFE" )
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDDSL_NO_EXT_LIB_CAFFE" )
	else()
		#Setup: Optionally edit Caffe CMakeLists.txt
		#sudo apt-get install --no-install-recommends libboost-all-dev
		#sudo apt-get install libopencv-dev libatlas-dev libatlas-base-dev libsnappy-dev
		#sudo apt-get install libhdf5-dev libhdf5-serial-dev libleveldb-dev liblmdb-dev
		#sudo apt-get install libgflags-dev libgoogle-glog-dev protobuf-compiler libprotobuf-dev
		#sudo apt-get install libpython-dev
		#Install Cuda from http://docs.nvidia.com/cuda/index.html#axzz4YZYC9wB0 and https://developer.nvidia.com/cuda-downloads
		#Install NCCL
		#	git clone https://github.com/NVIDIA/nccl.git
		#	cd nccl
		#	make CUDA_HOME=/usr/local/cuda test
		#	sudo make PREFIX=/usr/local install
		#Install Caffe and DIGITS from GIT repo
		#	After pip install -r requirements.txt do pip install -U scikit-image
		#	Use multi-gpu
		#	cmake -DUSE_NCCL=1 ..
		set (Caffe_DIR ${CMAKE_CURRENT_LIST_DIR}/3rdparty/linux/caffe/build)
		find_package(OpenCV REQUIRED)
		find_package(Caffe REQUIRED)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_NCCL")
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DUSE_NCCL")
		if (NOT BUILD_CUDA_GPU)
			set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCPU_ONLY" )
			set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPU_ONLY" )
		endif()
	endif()
	if (NOT DDSL_EXT_LIB_AF)
		#sudo apt-get install libglfw3-dev libnnvm-dev
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDDSL_NO_EXT_LIB_AF")
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDDSL_NO_EXT_LIB_AF" )
	endif()
endif()

if (CLANG)
  set (CMAKE_C_COMPILER clang )
  set (CMAKE_CXX_COMPILER clang++)
  set (CMAKE_AR      "llvm-ar")
  set (CMAKE_LINKER  "llvm-ld")
  set (CMAKE_NM      "llvm-nm")
  set (CMAKE_OBJDUMP "llvm-objdump")
  set (CMAKE_RANLIB  "llvm-ranlib")
endif ()

if (BUILD_DDSL_NO_EXT_LIB)
	set (SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/DDSL_NO_EXT_LIB/main.cpp")
	set (EXE_NAME "ddsl_no_ext_lib")
	message("Make exe ${EXE_NAME}")
	include_directories (${CMAKE_CURRENT_LIST_DIR}/src)
	add_executable(${EXE_NAME} ${SRC_FILES})
endif()

if (BUILD_LIBPNG_DDSL)
	set (SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/libpngDDSL/pngDDSL.c")
	set (LIB_NAME "pngddsl")
	message("Make lib ${LIB_NAME}")
	include_directories (${CMAKE_CURRENT_LIST_DIR}/src)
	include_directories (${ZLIB_INCLUDE_DIRS})
	include_directories (${PNG_INCLUDE_DIRS})
	add_library(${LIB_NAME} STATIC ${SRC_FILES})
	target_link_libraries(${LIB_NAME} ${ZLIB_LIBRARIES})
	target_link_libraries(${LIB_NAME} ${PNG_LIBRARIES})
endif()

if (BUILD_LIBSVM_DDSL)
	set (SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/3rdparty/libsvm-3.21/svm.cpp")
	set (LIB_NAME "svmddsl")
	message("Make lib ${LIB_NAME}")
	include_directories (${CMAKE_CURRENT_LIST_DIR}/src)
	add_library(${LIB_NAME} STATIC ${SRC_FILES})
endif()

if (BUILD_LIBAF_DDSL)
	set (SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/libarrayfireDDSL/ArrayFireDDSL.cpp")
	set (LIB_NAME "afddsl")
	message("Make lib ${LIB_NAME}")
	include_directories (${CMAKE_CURRENT_LIST_DIR}/src)
	include_directories (/usr/local/arrayfire-3/include)
	add_library(${LIB_NAME} STATIC ${SRC_FILES})
	link_directories(/usr/local/arrayfire-3/lib)
	if (BUILD_CUDA_GPU)
		target_link_libraries(${LIB_NAME} afcuda)
	else()
		target_link_libraries(${LIB_NAME} afcpu)
	endif()
endif()

if (BUILD_DDSL_TESTER)
	set (SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/DDSL_TESTER/main.cpp")

	if (DDSL_EXT_LIB_CAFFE)
		list(APPEND SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/libcaffe/force_link.cpp")
	endif()

	set (EXE_NAME "ddsl_tester")
	message("Make exe ${EXE_NAME}")
	include_directories (${CMAKE_CURRENT_LIST_DIR}/src)
	add_executable(${EXE_NAME} ${SRC_FILES} )
	if (DDSL_EXT_LIB_PNG)
		include_directories (${CMAKE_CURRENT_LIST_DIR}/libpngDDSL)
		include_directories (${ZLIB_INCLUDE_DIRS})
		include_directories (${PNG_INCLUDE_DIRS})
		target_link_libraries(${EXE_NAME} pngddsl)
	endif()	
	if (DDSL_EXT_LIB_SVM)
		include_directories (${CMAKE_CURRENT_LIST_DIR}/3rdparty/libsvm-3.21)
		target_link_libraries(${EXE_NAME} svmddsl)
	endif()
	if (DDSL_EXT_LIB_AF)
		include_directories (${CMAKE_CURRENT_LIST_DIR}/libarrayfireDDSL)
		include_directories (/usr/local/arrayfire-3/include)
		target_link_libraries(${EXE_NAME} afddsl)
	endif()
	if (DDSL_EXT_LIB_CAFFE)
		include_directories(${Caffe_INCLUDE_DIRS})
		target_link_libraries(${EXE_NAME} ${Caffe_LIBRARIES})

		message("DIRS: ${Caffe_INCLUDE_DIRS}")
		message("LIBS: ${Caffe_LIBRARIES}")
	endif()
endif()

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

message("CXX FLAGS: ${CMAKE_CXX_FLAGS}")
message("CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")

message("EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message("EXE_LINKER_FLAGS_DEBUG: ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

message("SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")
message("SHARED_LINKER_FLAGS_DEBUG: ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
