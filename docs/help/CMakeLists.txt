cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

find_package(Doxygen)
if (DOXYGEN_FOUND)

    find_program(QT_QCOLLECTIONGENERATOR_EXECUTABLE "qcollectiongenerator" PATHS
        "${CMAKE_PREFIX_PATH}/bin/"
        "${CMAKE_PREFIX_PATH}/tools/qt5-tools/"
        NO_SYSTEM_ENVIRONMENT_PATH
    )

    find_program(QT_QHELPGENERATOR_EXECUTABLE "qhelpgenerator" PATHS
        "${CMAKE_PREFIX_PATH}/bin/"
        "${CMAKE_PREFIX_PATH}/tools/qt5-tools/"
        NO_SYSTEM_ENVIRONMENT_PATH
    )

    if(QT_QCOLLECTIONGENERATOR_EXECUTABLE)
        message(STATUS "qcollectiongenerator found: ${QT_QCOLLECTIONGENERATOR_EXECUTABLE}")
    else()
        message(FATAL_ERROR "qcollectiongenerator executable not found")
    endif()

    # Qt Help Framework
    # .qhp: Qt Help Project (XML)
    # .qch: Qt Compiled Help (Binary)
    # .qhcp: Qt Help Collection Project (XML)
    # .qhc: Qt Help Collection (Binary)
    # The pipeline is: QHP -> QCH -> QHCP -> QHC

    # Qt Compiled Help file
    set(QCH_FILE "gravitate-manual.qch")

    # set input and output files
    set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
    set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    # request to configure the file
    configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY)
    message("Doxygen build started")


    set(PAGE_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/docs/index.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/inspection.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/fragment.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/feature-recognition.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/geometric-characterization.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/groups.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/overview.md"
        "${CMAKE_CURRENT_LIST_DIR}/docs/reassembly.md"
    )

    set(ADDITIONAL_SOURCES
        "manual_footer.html"
        "manual_header.html"
        "help_manual.css"
        )

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( user_manual ALL
        DEPENDS ${DOXYFILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/docs"
        COMMENT "Generating User manual documentation with Doxygen"
        SOURCES
            ${ADDITIONAL_SOURCES}
            ${PAGE_SOURCES}
        VERBATIM
    )

    # Generation of the QHelpCollection

    set(COLLECTION_PROJECT_IN "${CMAKE_CURRENT_LIST_DIR}/desktop-help.qhcp.in")
    set(COLLECTION_PROJECT "${CMAKE_CURRENT_BINARY_DIR}/desktop-help.qhcp")
    set(QHC_FILE "${CMAKE_CURRENT_BINARY_DIR}/desktop-help.qhc")

    configure_file(${COLLECTION_PROJECT_IN} ${COLLECTION_PROJECT} @ONLY)

    add_custom_command(
        TARGET user_manual
        POST_BUILD
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMAND ${QT_QCOLLECTIONGENERATOR_EXECUTABLE} ${COLLECTION_PROJECT} -o ${QHC_FILE}
        COMMENT "Compiling user manual..."
    )

else (DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the user manual")
endif (DOXYGEN_FOUND)
