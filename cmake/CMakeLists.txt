# Project: Tesseract-OCR
# License: Apache 2.0
# Project URL: http://code.google.com/p/tesseract-ocr/
# CMAKE files by: zdenop@gmail.com

# cp contrib/CMakeLists.txt ./
# mkdir build && cd build
# cmake .. -G "MinGW Makefiles"
# mingw32-make VERBOSE=1

PROJECT (tesseract-ocr)
SET (TESSERACT_SOVERSION 3)
SET (VERSION_SERIES 0)
SET (VERSION_MAJOR 2)
SET (VERSION_MINOR 0)
SET (PROJECT_VERSION ${TESSERACT_SOVERSION}.${VERSION_SERIES}${VERSION_MAJOR})
MESSAGE ("Creating tesseract-ocr ${PROJECT_VERSION}...")

CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
#Pretty colors
SET (CMAKE_COLOR_MAKEFILE ON)
#Dont force verbose
SET (CMAKE_VERBOSE_MAKEFILE OFF)
#Include current dir
SET (CMAKE_INCLUDE_CURRENT_DIR TRUE)
SET (CPACK_GENERATOR "TBZ2")
INCLUDE (CPack)

# Check if platform is 64 bit
if (NOT APPLE)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4)
		set (64_BIT_PLATFORM 0)
	else ()
		set (64_BIT_PLATFORM 1)
	endif ()
endif ()

# Set default compile flags for GCC
#INCLUDE (CheckCXXCompilerFlag)
#check_cxx_compiler_flag (-Wextra HAS_EXTRA)
#if (HAS_EXTRA)
	#MESSAGE ("-Wextra works")
	#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
#else (HAS_EXTRA)
	#MESSAGE ("-Wextra DO NOT works")
#endif (HAS_EXTRA)
#check_cxx_compiler_flag (-mthreads HAS_mthreads)
#if (HAS_mthreads)
	#MESSAGE ("-mthreads works")
	#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads")
#else (HAS_mthreads)
	#MESSAGE ("-mthreads DO NOT works")
#endif (HAS_mthreads)

# Profiler configuration for GCC
if (CMAKE_COMPILER_IS_GNUCXX )
	MESSAGE (STATUS "GCC detected, adding compile flags")
 set (CMAKE_BUILD_TYPE profiling)
 set (CMAKE_CXX_FLAGS_PROFILING "-O2 -DNDEBUG -pg -g -fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls")

 # After creating the new build type,
 # we clear the default back to empty
 set (CMAKE_BUILD_TYPE )
endif (CMAKE_COMPILER_IS_GNUCXX)


find_path (LEPT_INC
		NAMES
			allheaders.h
		PATHS
			/usr/include/leptonica/
			/usr/local/include/leptonica/
			/opt/include/leptonica/
			/opt/local/include/leptonica/
	)

find_library (LEPT_LIB_DIR
	NAMES
		lept
	PATHS
		/usr/lib
		/usr/lib64
		/usr/local/lib
		/usr/local/lib64
		/opt/lib
		/opt/lib64
		/opt/local/lib
		/opt/local/lib64
	)
if (LEPT_LIB_DIR)
	SET (LEPT_LIB lept)
endif (LEPT_LIB_DIR)

if (WIN32)
	ADD_DEFINITIONS (-DWIN32 -D__MSW32__ -D_CONSOLE)
	#ADD_DEFINITIONS (-DWIN32 -DMINGW32 -D__MSW32__ -D_CONSOLE)
	SET (LEPT_INC "vs2008/include/leptonica")
	SET (LEPT_LIB_DIR "vs2008/lib/")
	LINK_DIRECTORIES (${CMAKE_CURRENT_SOURCE_DIR}/${LEPT_LIB_DIR})
	SET (INSTALL_TARGETS_DEFAULT_ARGS
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib)
endif (WIN32)

if (WIN32)
	INCLUDE_DIRECTORIES (vs2008/port)
	FILE (GLOB port_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/vs2008/port/strtok_r.cpp)
endif (WIN32)

IF (UNIX)
	find_library (PTHREAD_LIBRARY
		NAMES
			pthread
		PATHS
			/usr/lib
			/usr/lib64
			/usr/local/lib
			/usr/local/lib64
			/opt/lib
			/opt/lib64
			/opt/local/lib
			/opt/local/lib64
	)
	if (PTHREAD_LIBRARY) # Dostros other than Ubuntu/Mint haven't qmobilityglobal dependices
		SET (TESSERACT_LINK_FLAGS "${TESSERACT_LINK_FLAGS} -lpthread")
	endif (PTHREAD_LIBRARY)
endif (UNIX)

# compiler depended part...
if (MSVC90) # stdint.h and inttypes.h for VC++20008
	INCLUDE_DIRECTORIES (vs2008/include)
endif (MSVC90)

if (MSVC)
	SET (TESSERACT_LINK_FLAGS "${TESSERACT_LINK_FLAGS} /LIBPATH:${LEPT_LIB_DIR} /NOLOGO /SUBSYSTEM:CONSOLE /DYNAMICBASE /NXCOMPAT /ERRORREPORT:PROMPT") # /NODEFAULTLIB:library /LTCG /INCREMENTAL:NO 
	MESSAGE ("************** MSVC detected **********")
	SET (LEPT_LIB liblept-static-mtdll.lib zlib-static-mtdll.lib libpng-static-mtdll.lib libjpeg-static-mtdll.lib libtiff-static-mtdll.lib giflib-static-mtdll.lib)
	FILE (GLOB port_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/vs2008/port/gettimeofday.cpp)
else (MINGW32)
	MESSAGE ("************** MINGW32 detected **********")
	SET (TESSERACT_LINK_FLAGS "${TESSERACT_LINK_FLAGS} -L${LEPT_LIB_DIR}")
	if (WIN32)
		SET (LEPT_LIB lept168) # it is not possible to link static library created by Visual Studio to mingw...
	endif (WIN32)
endif (MSVC)

ADD_DEFINITIONS (-DNDEBUG -DUSE_STD_NAMESPACE -DHAVE_LIBLEPT)
ADD_DEFINITIONS (-D__BLOB_T_DEFINED)
ADD_DEFINITIONS (-DVERSION="${PROJECT_VERSION}")

INCLUDE_DIRECTORIES (. api ccmain ccstruct ccutil classify cube cutil dict
	image neural_networks/runtime textord training viewer wordrec ${LEPT_INC})

FILE (GLOB tesseract_LIB_SRCS
	api/baseapi.cpp
	ccmain/*.cpp
	ccstruct/*.cpp
	# ccutil/*.cpp # EXTRA_DIST = mfcpch.cpp scanutils.cpp scanutils.h
	ccutil/ambigs.cpp ccutil/basedir.cpp ccutil/bits16.cpp ccutil/bitvector.cpp
	ccutil/ccutil.cpp ccutil/clst.cpp ccutil/elst.cpp ccutil/elst2.cpp
	ccutil/errcode.cpp ccutil/globaloc.cpp ccutil/hashfn.cpp 
	ccutil/indexmapbidi.cpp ccutil/mainblk.cpp ccutil/memry.cpp
	ccutil/params.cpp ccutil/serialis.cpp ccutil/strngs.cpp
	ccutil/tessdatamanager.cpp ccutil/tprintf.cpp ccutil/unichar.cpp
	ccutil/unicharmap.cpp ccutil/unicharset.cpp ccutil/unicodes.cpp
	classify/*.cpp
	cube/*.cpp
	cutil/*.cpp
	dict/*.cpp
	image/*.cpp
	neural_networks/runtime/*.cpp
	textord/*.cpp
	viewer/*.cpp
	wordrec/*.cpp)

FILE (GLOB traning_LIB_SRCS training/tessopt.cpp training/commontraining.cpp)

FILE (GLOB tesseract_LIB_HEADERS api/*.h ccmain/*.h ccstruct/*.h
	ccutil/*.h classify/*.h cube/*.h cutil/*.h dict/*.h image/*.h
	neural_networks/runtime/*.h textord/*.h training/*.h viewer/*.h
	wordrec/*.h)

FILE (GLOB LANGUAGE_DATA
	tessdata/*.traineddata
	tessdata/*cube.*
	tessdata/configs/ambigs.train
	tessdata/configs/api_config
	tessdata/configs/bigram
	tessdata/configs/box.train
	tessdata/configs/box.train.stderr
	tessdata/configs/digits
	tessdata/configs/hocr
	tessdata/configs/inter
	tessdata/configs/kannada
	tessdata/configs/linebox
	tessdata/configs/logfile
	tessdata/configs/makebox
	tessdata/configs/rebox
	tessdata/configs/strokewidth
	tessdata/configs/unlv
	tessdata/tessconfigs/batch
	tessdata/tessconfigs/batch.nochop
	tessdata/tessconfigs/nobatch
	tessdata/tessconfigs/matdemo
	tessdata/tessconfigs/msdemo
	tessdata/tessconfigs/segdemo)

ADD_EXECUTABLE (tesseract api/tesseractmain.cpp)
SET_TARGET_PROPERTIES (tesseract PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
TARGET_LINK_LIBRARIES (tesseract libtesseract-static)
INSTALL (TARGETS tesseract RUNTIME DESTINATION bin)

ADD_LIBRARY (libtesseract-static STATIC ${tesseract_LIB_SRCS} ${port_LIB_SRCS})
ADD_LIBRARY (libtesseract-shared SHARED ${tesseract_LIB_SRCS} ${port_LIB_SRCS})
ADD_DEPENDENCIES (libtesseract-static ${tesseract_LIB_HEADERS})
ADD_DEPENDENCIES (libtesseract-shared ${tesseract_LIB_HEADERS})

IF (WIN32)
	TARGET_LINK_LIBRARIES (libtesseract-static ${LEPT_LIB} ws2_32)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES (libtesseract-static ${LEPT_LIB})
ENDIF (WIN32)
#INSTALL (TARGETS libtesseract-static ${INSTALL_TARGETS_DEFAULT_ARGS})

IF (WIN32)
	ADD_CUSTOM_COMMAND (TARGET libtesseract-shared PRE_BUILD COMMAND windres -i ${CMAKE_CURRENT_SOURCE_DIR}/contrib/tesseract.rc -o tesseract.o COMMENT "Windows DLL version")
	SET_TARGET_PROPERTIES (libtesseract-shared PROPERTIES
		LINK_FLAGS "tesseract.o"
		VERSION ${VERSION_MAJOR}${VERSION_MINOR}
		OUTPUT_NAME tesseract_${TESSERACT_SOVERSION}${VERSION_SERIES}${VERSION_MAJOR}
		COMPILE_FLAGS "-DMAKE_DLL")
	TARGET_LINK_LIBRARIES (libtesseract-shared ${LEPT_LIB} ws2_32)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES (libtesseract-shared ${LEPT_LIB})
ENDIF (WIN32)
INSTALL (TARGETS libtesseract-shared RUNTIME DESTINATION bin
									LIBRARY DESTINATION lib
									ARCHIVE DESTINATION lib)

ADD_EXECUTABLE (cntraining training/cntraining.cpp ${traning_LIB_SRCS})
SET_TARGET_PROPERTIES (cntraining PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (cntraining PROPERTIES OUTPUT_NAME "cntraining")
TARGET_LINK_LIBRARIES (cntraining libtesseract-static)
INSTALL (TARGETS cntraining RUNTIME DESTINATION bin)

ADD_EXECUTABLE (mftraining training/mftraining.cpp training/mergenf.cpp ${traning_LIB_SRCS})
SET_TARGET_PROPERTIES (mftraining PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (mftraining PROPERTIES OUTPUT_NAME "mftraining")
TARGET_LINK_LIBRARIES (mftraining libtesseract-static)
INSTALL (TARGETS mftraining RUNTIME DESTINATION bin)

ADD_EXECUTABLE (unicharset_extractor training/unicharset_extractor.cpp training/tessopt.cpp)
SET_TARGET_PROPERTIES (unicharset_extractor PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (unicharset_extractor PROPERTIES OUTPUT_NAME "unicharset_extractor")
TARGET_LINK_LIBRARIES (unicharset_extractor libtesseract-static)
INSTALL (TARGETS unicharset_extractor RUNTIME DESTINATION bin)

ADD_EXECUTABLE (wordlist2dawg training/wordlist2dawg.cpp training/tessopt.cpp)
SET_TARGET_PROPERTIES (wordlist2dawg PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (wordlist2dawg PROPERTIES OUTPUT_NAME "wordlist2dawg")
TARGET_LINK_LIBRARIES (wordlist2dawg libtesseract-static)
INSTALL (TARGETS wordlist2dawg RUNTIME DESTINATION bin)

ADD_EXECUTABLE (combine_tessdata training/combine_tessdata.cpp)
SET_TARGET_PROPERTIES (combine_tessdata PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (combine_tessdata PROPERTIES OUTPUT_NAME "combine_tessdata")
TARGET_LINK_LIBRARIES (combine_tessdata libtesseract-static)
INSTALL (TARGETS combine_tessdata RUNTIME DESTINATION bin)

ADD_EXECUTABLE (ambiguous_words training/ambiguous_words.cpp training/tessopt.cpp)
SET_TARGET_PROPERTIES (ambiguous_words PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (ambiguous_words PROPERTIES OUTPUT_NAME "ambiguous_words")
TARGET_LINK_LIBRARIES (ambiguous_words libtesseract-static)
INSTALL (TARGETS ambiguous_words RUNTIME DESTINATION bin)

ADD_EXECUTABLE (classifier_tester training/classifier_tester.cpp ${traning_LIB_SRCS})
SET_TARGET_PROPERTIES (classifier_tester PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (classifier_tester PROPERTIES OUTPUT_NAME "classifier_tester")
TARGET_LINK_LIBRARIES (classifier_tester libtesseract-static)
INSTALL (TARGETS classifier_tester RUNTIME DESTINATION bin)

ADD_EXECUTABLE (dawg2wordlist training/dawg2wordlist.cpp ${traning_LIB_SRCS})
SET_TARGET_PROPERTIES (dawg2wordlist PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (dawg2wordlist PROPERTIES OUTPUT_NAME "dawg2wordlist")
TARGET_LINK_LIBRARIES (dawg2wordlist libtesseract-static)
INSTALL (TARGETS dawg2wordlist RUNTIME DESTINATION bin)

ADD_EXECUTABLE (shapeclustering training/shapeclustering.cpp ${traning_LIB_SRCS})
SET_TARGET_PROPERTIES (shapeclustering PROPERTIES COMPILE_FLAGS "-DMAKE_EXE")
SET_TARGET_PROPERTIES (shapeclustering PROPERTIES OUTPUT_NAME "shapeclustering")
TARGET_LINK_LIBRARIES (shapeclustering libtesseract-static)
INSTALL (TARGETS shapeclustering RUNTIME DESTINATION bin)

SET_TARGET_PROPERTIES (libtesseract-static
	PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
	LINK_FLAGS "-L${LEPT_LIB_DIR}"
	PREFIX "lib"
	OUTPUT_NAME "tesseract")

SET_TARGET_PROPERTIES (tesseract cntraining mftraining unicharset_extractor wordlist2dawg combine_tessdata shapeclustering dawg2wordlist classifier_tester ambiguous_words
	PROPERTIES VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
	LINK_FLAGS ${TESSERACT_LINK_FLAGS}
	)

INSTALL (FILES ${LANGUAGE_DATA} DESTINATION tessdata)

###########################################################################
# uninstall target
###########################################################################
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/contrib/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
		IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# TODO:
# windows->static, unix->shared
# installed automatically to /usr or declare /usr/local!
# wrong instalation place of tessdata configs
# strange naming on linux (liblibtesseract-shared.so  should be libtesseract.so.3.0.2)
# install manpages, doc, licence
# install header files

# packaging - see nootka
# strip *.exe
# upx *.exe

#install (FILES ${tesseract_LIB_HEADERS} DESTINATION include)

#install (TARGETS tesseract_main ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

#install (FILES control.h cube_reco_context.h)
