# - Find OpenCV SDK
# Find the native OpenCV headers and libraries.
#
#  OPENCV_INCLUDE_DIRS - where to find header files
#  OPENCV_LIBRARIES    - List of libraries when using OpenCV.
#  OPENCV_BIN_DIR      - Path to binaries needed for using OpenCV, i.e. DLL-files
#  OPENCV_FOUND        - True if OpenCV found.


SET(OPENCV_ROOT_DIR
    "${OPENCV_ROOT_DIR}"
    CACHE
    PATH
    "Root directory to search for OpenCV")

# Look for the header file.
FIND_PATH(OPENCV_INCLUDE_DIRS NAMES opencv2/opencv.hpp HINTS 
	${OPENCV_ROOT_DIR}/build/include/ )

	# Determine architecture
IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
	IF(MSVC)
		SET(_OPENCV_LIB_ARCH "x64")
	ENDIF()
ELSE()
	IF(MSVC)
		SET(_OPENCV_LIB_ARCH "x86")
	ENDIF()
ENDIF()

MARK_AS_ADVANCED(_OPENCV_LIB_ARCH)

# Determine the compiler version for Visual Studio
IF (MSVC)
	# Visual Studio 2010
	IF(MSVC10)
		SET(_OPENCV_MSVC_DIR "vc10")
	ENDIF()
	# Visual Studio 2012
	IF(MSVC11)
		SET(_OPENCV_MSVC_DIR "vc11")
	ENDIF()
	# Visual Studio 2013
	IF(MSVC12)
		SET(_OPENCV_MSVC_DIR "vc12")
	ENDIF()
ENDIF()

# Append "d" to debug libs on windows platform
IF (WIN32)
	SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()

# Look for the library.
FIND_LIBRARY(OPENCV_LIBRARY NAMES opencv_world300 HINTS ${OPENCV_ROOT_DIR} ${OPENCV_ROOT_DIR}/build/${_OPENCV_LIB_ARCH}/${_OPENCV_MSVC_DIR}/lib)
FIND_LIBRARY(OPENCV_LIBRARY_DEBUG NAMES opencv_world300${CMAKE_DEBUG_POSTFIX} HINTS ${OPENCV_ROOT_DIR} ${OPENCV_ROOT_DIR}/build/${_OPENCV_LIB_ARCH}/${_OPENCV_MSVC_DIR}/lib)

MARK_AS_ADVANCED(OPENCV_LIBRARY)
MARK_AS_ADVANCED(OPENCV_LIBRARY_DEBUG)

SET(OPENCV_LIBRARIES optimized ${OPENCV_LIBRARY} debug ${OPENCV_LIBRARY_DEBUG})

# Find binaries i.e. DLL path
IF (MSVC)
	FIND_PATH(OPENCV_BIN_DIR opencv_world300.dll HINTS ${OPENCV_ROOT_DIR} ${OPENCV_ROOT_DIR}/build/${_OPENCV_LIB_ARCH}/${_OPENCV_MSVC_DIR}/bin)
	MARK_AS_ADVANCED(OPENCV_BIN_DIR)
ENDIF(MSVC)

# handle the QUIETLY and REQUIRED arguments and set OPENCV_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenCVSDK DEFAULT_MSG OPENCV_LIBRARIES OPENCV_INCLUDE_DIRS)

MARK_AS_ADVANCED(OPENCV_LIBRARIES OPENCV_INCLUDE_DIRS)
