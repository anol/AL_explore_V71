//
// Created by anolsen on 12.09.2019.
//

#ifndef STATUS_CODE_H
#define STATUS_CODE_H

enum status_code {
    STATUS_OK = 0, //!< Success
    STATUS_ERR_BUSY = 0x19,
    STATUS_ERR_DENIED = 0x1C,
    STATUS_ERR_TIMEOUT = 0x12,
    ERR_IO_ERROR = -1, //!< I/O error
    ERR_FLUSHED = -2, //!< Request flushed from queue
    ERR_TIMEOUT = -3, //!< Operation timed out
    ERR_BAD_DATA = -4, //!< Data integrity check failed
    ERR_PROTOCOL = -5, //!< Protocol error
    ERR_UNSUPPORTED_DEV = -6, //!< Unsupported device
    ERR_NO_MEMORY = -7, //!< Insufficient memory
    ERR_INVALID_ARG = -8, //!< Invalid argument
    ERR_BAD_ADDRESS = -9, //!< Bad address
    ERR_BUSY = -10, //!< Resource is busy
    ERR_BAD_FORMAT = -11, //!< Data format not recognized
    ERR_NO_TIMER = -12, //!< No timer available
    ERR_TIMER_ALREADY_RUNNING = -13, //!< Timer already running
    ERR_TIMER_NOT_RUNNING = -14, //!< Timer not running
    ERR_ABORTED = -15, //!< Operation aborted by user
    ERR_INVALID_DATA = -1,
    ERR_NO_CHANGE = -2,
    ERR_SUSPEND = -5,
    ERR_IO = -6,
    ERR_REQ_FLUSHED = -7,
    ERR_NOT_FOUND = -10,
    ERR_BAD_FRQ = -16,
    ERR_DENIED = -17,
    ERR_ALREADY_INITIALIZED = -18,
    ERR_OVERFLOW = -19,
    ERR_NOT_INITIALIZED = -20,
    ERR_SAMPLERATE_UNAVAILABLE = -21,
    ERR_RESOLUTION_UNAVAILABLE = -22,
    ERR_BAUDRATE_UNAVAILABLE = -23,
    ERR_PACKET_COLLISION = -24,
    ERR_PIN_MUX_INVALID = -26,
    ERR_UNSUPPORTED_OP = -27,
    ERR_NO_RESOURCE = -28,
    ERR_NOT_READY = -29,
    ERR_FAILURE = -30,
    ERR_WRONG_LENGTH = -31,
    OPERATION_IN_PROGRESS = -128,
};

#endif //STATUS_CODE_H
