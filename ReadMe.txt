For examples see example1_gen.txt, example2_gen.txt, ..., example9_gen.txt.

Declare system: same as standard tchecker
Declare clock: format: <clock:clock_type:clock_name>
        clock_type must be one of the strings history, prophecy, normal, timer.
        clock_name can be any identifier except tmp

Declare event: format: <event:event_name>
        event_name can be any identifier
Declare event clock: format: <event:event_name:bit_representing_history_clock:bit_representing_prophecy_clock>
         bit_representing_history_clock can be 0 or 1 representing whether event history clock corresponding to the event is there in the system or not
         bit_representing_prophecy_clock can be 0 or 1 representing whether event prophecy clock corresponding to the event is there in the system or not
         if history (or prophecy) clock is defined for the event then these clocks can be used in guards with event_name"_h" (or event_name"_p")
         
Write a program: A program is a sequence of provided and do operations. It must start with provided: operations.
                    every provided and do operation end with ";"
                    
                    See examples for more details.
