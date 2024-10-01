# Line Editor Project

## Project Overview

This project is a basic **line editor** designed to read from and write to files, perform string operations, manage a list of strings, and enable search operations. The line editor uses **cursor** which will point to a word in file. Users can use this cursor to perform CRUD operations on file. If you make a mistake, don't worry! this line editor has your back as it also allows UNDO/REDO operations.

## Features

1. **Command-Line File Handling**:
   - Open or create files based on the provided command-line arguments.
   - Files can be opened from the current directory or a specified directory.
   - Purely Terminal based, no need for any GUI's
   - File operations supported:
     - Open an existing file or create a new one if it does not exist.
     - Read and write modes for file access.

2. **Buffer for Text Management**:
   - The line editor uses a buffer to store up to 25 lines of text.
   - Each line is dynamically stored in memory to allow easy manipulation of the text.
   - Supports Dynamic buffer which allows to read file having more than 25 lines at once
   - The buffer design supports future operations like line insertion, updating, and deletion.

3. **Reading and Writing to Files**:
   - Read contents of a file line by line and store them in the buffer.
   - Print the contents of the buffer.
   - Write back the contents of the buffer to the file.

4. **Search Functionality**:
   - Allows for searching a word in buffer, if found moves cursor to first occurence of that word.
   - A composite variable(structure) called "cursor" holds the search result (line and position).
   - Moves cursor at beginning if word is not found and displays message of not found.

5. **CRUD Operations**:
   - **Create**:
     - Insert a new line at a specific position (cursor position).
     - Insert a word at the cursor position within a line.
   - **Read**:
     - Display the contents of a specific line.
     - Display the entire buffer line by line.
   - **Update**:
     - Replace a searched word with another word.
     - Modify a line starting from the cursor position.
   - **Delete**:
     - Remove a specific line from the buffer.
     - Remove a searched word from a line.

## Usage

1. **Command-Line Arguments**:
   - One argument: Create a new file named `file.txt` in the current directory with `w+` mode.
     ```bash
     ./editor
     ```
   - Two arguments: Open the specified filename in `r+` mode if it exists; otherwise, create it in `w+` mode.
     ```bash
     ./editor filename
     ```
   - Three arguments: Open the file in the specified directory in `r+` mode if it exists; otherwise, create it in `w+` mode.
     ```bash
     ./editor filename directoryname
     ```
   - More than three arguments: Display an error message.

2. **Editing Files**:
   *Once file is successfully loaded, you can perform operations like*
      1. Print File
      2. Search
      3. Move Cursor
      4. Insert Line
      5. insert Word
      6. Update Word
      7. Delete Line
      8. Delete Word
      9. Undo
      10. Redo
      11. Save
      12. Exit
   
      **Key Note**

      > **Exit** doesn't save the file, so save it before exiting.  
      > **Deleting a line** might lead to a lost cursor, so you might need to set it before executing any other operation.


    
4. **Search**:
   - Use the search functionality to find a word in the buffer. If found, the cursor stores the line number and position and the word is <mark>highlighted</mark>
   - If the word is not found, the cursor is not moved!.

5. **CRUD Operations**:
   - **Create**:
     - Insert a new line before or after the cursor.
     - Insert a new word before or after the cursor.
   - **Read**:
     - Prints the content of entire buffer.
   - **Update**:
     - Replace a word or modify a line from the cursor position.
   - **Delete**:
     - Remove a specific line or a word where cursor points.

## Project folders and Files

- **src**: This folder contains the source of the project
  - include: contains all the header files used in project
  - main.c: main file of c
  - crud.c: functional/logic file, contains all CRUD functions with other utils
  - io.c: contain some custom build print functions
- **bin**: compiled source code
  - linuxEditor: binary of editor for linux OS
  - winEditor: binary of editor for windows OS
- **README.md**: Project description and instructions.

## How to Build

1. Just clone the repo or download just src folder
2. If on windows, comment the '#define LINUX' in common.h which is present in 'include' directory
2. Run the following command
  ```bash
  gcc io.c crud.c main.c  -o editor
  ```

## How to Run

Just Run 
```bash
./editor
```
