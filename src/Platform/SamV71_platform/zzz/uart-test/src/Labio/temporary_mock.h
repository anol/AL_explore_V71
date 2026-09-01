//
// Created by anolsen on 16.09.2019.
//

#ifndef UART_TEST_TEMPORARY_MOCK_H
#define UART_TEST_TEMPORARY_MOCK_H


class temporary_mock {

public:
    static void IDE3466_spi_init(int i);

    static void hri_ide3466_read_CNT_reg(uint32_t *pInt, int su);

    static void util_IDE3466_set_all_CNTs_to_zero();

    static void util_NORM_dump_readout_buffers();
};


#endif //UART_TEST_TEMPORARY_MOCK_H
