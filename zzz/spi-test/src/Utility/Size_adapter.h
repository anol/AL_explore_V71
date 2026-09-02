//
// Created by anolsen on 19.09.2019.
//

#ifndef SPI_TEST_SIZE_ADAPTER_H
#define SPI_TEST_SIZE_ADAPTER_H


class Size_adapter {
public:
    static void store_byte(uint32_t *p_buffer, uint32_t size, uint32_t byte_index, uint8_t data);

    static uint8_t fetch_byte(const uint32_t *p_buffer, uint32_t size, uint32_t byte_index);
};


#endif //SPI_TEST_SIZE_ADAPTER_H
