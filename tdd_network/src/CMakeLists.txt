set(SOURCES bits.c bit_writer.c bit_reader.c write_stream.c read_stream.c measure_stream.c net_message.c packet.c sending_thread.c sending_service.c connection_manager.c tdd_network.h)

add_library(tdd_network ${SOURCES})

target_link_libraries(tdd_network tdd)