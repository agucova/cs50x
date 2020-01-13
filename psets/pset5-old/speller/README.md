# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

The largest word in the dictionary, which gives us our length constant. More generally:
> An invented long word said to mean a lung disease caused by inhaling very fine ash and sand dust.

## According to its man page, what does `getrusage` do?

Collect runtime statistics for processes or thread:
> Return resource usage statistics for the calling process,  which is the sum of resources used by all threads in the process.

## Per that same man page, how many members are in a variable of type `struct rusage`?

There are 16 first level members (there are structs inside)

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Because that allows us to check if the pointer is empty, that is, if getrusage returned something. But more importantly, using an already stored variable is more memory efficient than copying it from scratch.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.
The program starts by opening the file in a read mode using `fopen` in a FILE* variable, which if successful continues to a for loop with three arguments:

> for (int c = fgetc(file); c != EOF; c = fgetc(file))

**fgetc()'s** man page describes:
> **fgetc**() reads the next character from _stream_ and returns it as an _unsigned_ char cast to an _int_, or **EOF** on end of file or error.

Which let's us translate the loop into three steps:
 - Initialize c with the a character in the stream, which in this case is the first one. (*fgetc*)
 - Loop until c equals the End of File signal (*EOF*).
 - Every iteration in the loop, load a new character into c. (*fgetc*)

![Imgur](https://i.imgur.com/LN1N2my.png)

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?
Reading character by character allows us to more explicitly distinguish separate words, since there are lots of exceptions where a word is not clearly cut by two spaces, instead having notations such as dots, semicolons, etc. This allows us to process the word while it's being stored, instead of modifying the array directly, while also allowing us to discard anything else, like numbers or special notation being added in the text.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?
Given that these are parameters that don't need any change during execution, by example, the words have no need to be modified only to be checked, and the dictionary filename doesn't either. This allows, to my knowledge, for the compiler to optimize them in memory.
