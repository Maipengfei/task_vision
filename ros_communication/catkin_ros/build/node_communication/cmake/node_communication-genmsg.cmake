# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "node_communication: 1 messages, 0 services")

set(MSG_I_FLAGS "-Inode_communication:/home/mds/catkin_ros/src/node_communication/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(node_communication_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/mds/catkin_ros/src/node_communication/msg/person.msg" NAME_WE)
add_custom_target(_node_communication_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "node_communication" "/home/mds/catkin_ros/src/node_communication/msg/person.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(node_communication
  "/home/mds/catkin_ros/src/node_communication/msg/person.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_communication
)

### Generating Services

### Generating Module File
_generate_module_cpp(node_communication
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_communication
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(node_communication_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(node_communication_generate_messages node_communication_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/mds/catkin_ros/src/node_communication/msg/person.msg" NAME_WE)
add_dependencies(node_communication_generate_messages_cpp _node_communication_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_communication_gencpp)
add_dependencies(node_communication_gencpp node_communication_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_communication_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(node_communication
  "/home/mds/catkin_ros/src/node_communication/msg/person.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/node_communication
)

### Generating Services

### Generating Module File
_generate_module_eus(node_communication
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/node_communication
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(node_communication_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(node_communication_generate_messages node_communication_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/mds/catkin_ros/src/node_communication/msg/person.msg" NAME_WE)
add_dependencies(node_communication_generate_messages_eus _node_communication_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_communication_geneus)
add_dependencies(node_communication_geneus node_communication_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_communication_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(node_communication
  "/home/mds/catkin_ros/src/node_communication/msg/person.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_communication
)

### Generating Services

### Generating Module File
_generate_module_lisp(node_communication
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_communication
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(node_communication_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(node_communication_generate_messages node_communication_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/mds/catkin_ros/src/node_communication/msg/person.msg" NAME_WE)
add_dependencies(node_communication_generate_messages_lisp _node_communication_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_communication_genlisp)
add_dependencies(node_communication_genlisp node_communication_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_communication_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(node_communication
  "/home/mds/catkin_ros/src/node_communication/msg/person.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/node_communication
)

### Generating Services

### Generating Module File
_generate_module_nodejs(node_communication
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/node_communication
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(node_communication_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(node_communication_generate_messages node_communication_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/mds/catkin_ros/src/node_communication/msg/person.msg" NAME_WE)
add_dependencies(node_communication_generate_messages_nodejs _node_communication_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_communication_gennodejs)
add_dependencies(node_communication_gennodejs node_communication_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_communication_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(node_communication
  "/home/mds/catkin_ros/src/node_communication/msg/person.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_communication
)

### Generating Services

### Generating Module File
_generate_module_py(node_communication
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_communication
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(node_communication_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(node_communication_generate_messages node_communication_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/mds/catkin_ros/src/node_communication/msg/person.msg" NAME_WE)
add_dependencies(node_communication_generate_messages_py _node_communication_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(node_communication_genpy)
add_dependencies(node_communication_genpy node_communication_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS node_communication_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_communication)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/node_communication
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(node_communication_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/node_communication)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/node_communication
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(node_communication_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_communication)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/node_communication
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(node_communication_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/node_communication)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/node_communication
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(node_communication_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_communication)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_communication\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/node_communication
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(node_communication_generate_messages_py std_msgs_generate_messages_py)
endif()
