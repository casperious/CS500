This is Project 1 of Operating Systems.

The objective is to read data from an input file, convert it into binary, with parity bit encoding.

Then to pass the encoded frame, with added SYN and length bits, through a pipe to the consumer.

Decode the frames into their original form, checking for parity, and capitalizing all lower case alphabets.

Then to re-encode and send the capitalized frame through a pipe back to the producer.

The final result should be saved in data.done

An error module has been added, to flip a random bit in the Third frame.

To Run this program, navigate to the folder in your directory, and type ./producer in your shell.
