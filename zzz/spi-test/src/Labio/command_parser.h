
/*
 * command_parser.h
 *
 * Created: 19.03.2019 14:56:36
 *  Author: ssorensen
 */

#ifndef IDE3466_PROTOTYPE_TESTING_H_INCLUDED
#define IDE3466_PROTOTYPE_TESTING_H_INCLUDED

#define COMMAND_MAX_LENGTH 22 //bytes (including the initiating '$' and terminating "0" bytes. CR+LF can be in addition to this)

int8_t get_num_unexecuted_commands();
int8_t execute_next_command();

/*
*  \brief Function that adds a command to the command-buffer.
*  \param[in] pointer to the char array that holds the command
*  \return error codes (not defined at the time of writing)
*/
int8_t add_command_to_buffer(char *command);

/*
*  \brief
*  \return The number of open slots in the command buffer. If the buffer is full, this will return 0
*/
int8_t get_open_slots_in_buffer();

/**
 * \brief Copies the next command in the command buffer over to the output_buffer.
 * \param[out] output_buffer char array that can hold the returned command
 * \return int8_t unspecified error code
 */
int8_t read_next_command(char *output_buffer);

/*
*  \brief Unimplemented placeholder for command validation. This should be run before the command is placed into the command buffer
*  \param[out] uint8_t* pointer to the char array containing the command that is to be validated
*  \return int8_t error code
*/
int8_t validate_command(char *command);


#endif // IDE3466_PROTOTYPE_TESTING_H_INCLUDED