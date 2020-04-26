cmake_minimum_required (VERSION 3.0)

set(LIBTDD_UTESTS_DIRECTORY "../../tdd_utests")

add_subdirectory(${LIBTDD_UTESTS_DIRECTORY} tdd_utests_build)

add_executable(test_bits test_bits.c)
add_executable(test_stream_serialize test_stream_serialize.c)
add_executable(test_net_messages test_net_messages.c)
add_executable(test_packets test_packets.c)
add_executable(test_sending_thread test_sending_thread.c)

add_dependencies(test_bits test_bits)
add_dependencies(test_stream_serialize test_stream_serialize)
add_dependencies(test_net_messages test_net_messages)
add_dependencies(test_packets test_packets)
add_dependencies(test_sending_thread test_sending_thread)

target_link_libraries(test_bits tdd_network tdd_utests)
target_link_libraries(test_stream_serialize tdd_network tdd_utests)
target_link_libraries(test_net_messages tdd_network tdd_utests)
target_link_libraries(test_packets tdd_network tdd_utests)
target_link_libraries(test_sending_thread tdd_network tdd_utests)

target_include_directories(tdd_network PUBLIC ../src)
target_include_directories(tdd_utests PUBLIC ${LIBTDD_UTESTS_DIRECTORY})

add_test(bits test_bits)
add_test(stream_serialize test_stream_serialize)
add_test(net_messages test_net_messages)
add_test(packets test_packets)
add_test(sending_thread test_sending_thread)