# C++ Technical Test Readme File
Hours taken to code/plan/revise: 19 hours

## Application Design 

It is built as a console application for faster development. 
Composed of a the main class and a class named 'PhoneMessagesRecordParser' which contains
the entire functionality of the app. It can be run via the command line with an extra
parameter to change the name of the input file it will process.

The program is written in a way that the reader will be able to know what each method is supposed to do
by just reading its name.

Public methods are constructed in a way that Unit-testing will be doable this also results into
an added flexibility on the program's input areas.

It is also coded in a way that re-arranging the order of inputs and outputs will not
necessitate big and breaking code changes. Re-arranging is only a matter of reassigning numbers
on designated Enums (namely 'InputFields' and 'OutputFields').
Most methods are virtual, making it easily extensible for further improvements.

As for the output file, the phone numbers are uniformly formatted into pure numbers without spaces and other symbols.

As the application reads each line from the input file, it saves only the
relevant data needed to produce the expected output thus eliminating the need to save each
line of data from the input file. There is also a post-processing function for operations
that can only be performed once the whole input file has been processed.


Thank you for this opportunity!
