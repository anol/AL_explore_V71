# AstraLEO Explore SAMV71 Peripherals, etc.

Using FreeRTOS (FreeRTOSv202604.01-LTS), copied selected code into this project.

## Wiring

Aggregation graphs of the two concrete objects wired together in `main()`
(`V71_EK_hello_world.cpp`):

- [`doc/hello_world_aggregation.svg`](doc/hello_world_aggregation.svg) — what `Hello_world` (the application) owns and references
- [`doc/v71_ek_board_aggregation.svg`](doc/v71_ek_board_aggregation.svg) — what `V71_EK_board` (the board) owns and references

Graphviz source (larger/editable version): [`doc/hello_world_wiring_A4.dot`](doc/hello_world_wiring_A4.dot)


