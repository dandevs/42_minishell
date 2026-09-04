*This project has been created as part of the 42 curriculum by danimend, mathvall.*

# DESCRIPTION

## Introduction

The minishell program has the goal to reproduce the behaviour of the shell, which is a program that allows us to type in instructions that will be interpreted by the kernel. Ours is pretty simple, and borrows a lot of informations and external programs that are defined within the shell scope. It can be viewed as a "mod" to the already existing shell rather than a real one. Our implementation of minishell is also different from the original one, which can be explained by the enourmous complexity shell has to face and the enormous responsibility it has, as millions (if not billions) of users, software, and even OS use it. We do not folllow the POSIX norm in this project.

        The programs go through the following steps :

1. First, we get the line that will be studied as one or multiple instruction(s) ;
2. We then go through a parsing stage that has the goal to create the appropriate data structure for the next parts of the program, from the line that we got.
        2. 1. First we assign semantic roles, called tokens, to portions of the line. If it fails, the appropriate error is written to fd nb 2 and we go back to step 1 to ask for a new line.
        2. 2. Next we build a tree data structure that will help us navigate appropriatly through our tokens.
3. If needed, we resolve heredocs situations : we get input from the user and fill the dedicated spaces in our tree.
4. We then build our instruction in the appropriate format (execve(char **args)) and do variable substitution;
5. Finally, we use our interpreter to go through our tree data structure and execute each instruction, and apply redirections when needed.

The "real" shell does not follow these steps in this exact order, and also provides additionnal features that we will not be covering, such as unique tokens for certain symbols (examples : ";", "||", "&&" etc...) associated with extra steps.

## 1. Getting the line / getting the instruction

We get the line that we will be analysing and executing if possible, by using the function readline. It can cause leaks that we don't treat a special way. We also build a history by using add_history if : our line is different from the previous one, and we are not receiving the line from a pipe or redirection (giving a line at minishell's launch).

## 2. Parsing the line

### 2.1. Assigning token value / semantic roles to portion of the line

At this point the line is a flat array of characters: the line *ls -la | grep "foo bar" > out.txt* is just a bunch of words with no special meanings. The lexer / parsing unit cuts it into meaningful units (tokens) and gives each one a semantic role.

We walk the line from left to right and decide what to do based on the current character:

- Whitespace outside quotes ends the current token and is discarded.
- Metacharacters (|, <, >) are tokens on their own. Since some can be doubled to mean something else (>>, <<), we always look at the next character before choosing the type.
- Quotes produce no token: they open a context / a "mode" in which whitespace and metacharacters lose their meaning. This is why "foo bar" stays one word and "|" is not a pipe.
- Anything else is considered a word token.

The result is a list of tokens, each carrying its raw content and its type. Being cuttable is not being correct, though: *| ls* and *ls >* tokenize fine but mean nothing. Therefore, we check the order of the tokens : no pipe at the start or the end, a redirection must be followed by a word, no unclosed quote, etc... On failure, the error is written to fd nb 2, our tokens are freed, and we go back to step 1.

### 2.2. Building the tree data structure

Our token list is correct, but it is linear, and execution is not. In cmd1 | cmd2 | cmd3 the commands run simultaneously, and the redirections of cmd2 concern only cmd2. A flat list cannot express that hierarchy : a tree can.

We build it by looking for pipes first : if we find one, then we have a Pipe node. Else, we have a Command node.

In a Command node, we have fields for : one *tokens*, which is an abstract structure that groups the portion of the line we are talking about and its token value; redirections if there are ; and finally *args* twhich contains our instruction in the right format for execution.

A Pipe node divides itself into two new tree node, a left and a right child. The left one contains all tokens from the beggining to the pipe, and the right contains everything from the pipe to the end. We then repeat the same process : is this node a Command node or Pipe node, etc...

The tree is our final representation of our data.

## 3. Heredoc resolution

A heredoc (<< EOF) is the only redirection whose content does not exist anywhere yet : it has to be typed by the user. So before any command can run, we traverse the tree once more, pipe nodes forwarding the work to their children, and for every command leaf we scan its redirections looking for a DLESSER token.

For each one found:

Get the delimiter (hd_delimiter) : the word following <<. Whether it was quoted or not is recorded, because it decides if the body will be expanded later - *<< EOF* expands $VAR inside the body, *<< "EOF"* does not. The quotes will then be removed from the delimiter itself, since we compare against the bare word.
Get the body (hd_body) : we prompt the user line by line until a line matches the delimiter exactly, and store the accumulated content directly in the hd_body field of the redirection node. We will detail more and explain the execution of this step in paragraph 5 of this document.

File field of redirection is set to NULL. This redirection node no longer designates something to open, the input is registered in the tree, and the executor will "feed" it to the command instead of calling open.

Traversing left before right matters here : heredocs must be filled in the order they appear on the line, so that *cat << A | cat << B* prompts for A first.

## 4. Args building and expansion

Before anything can be executed, each args field of Command nodes must be turned into what execve expects: a NULL-terminated char **args. This is done by a traversal of the tree : Pipe nodes simply forward the work to their two children, and all the real work happens on the leaves / Command nodes.

For each Command node :

Redirection check: if the target of a redirection expanded to zero or several words, the filename is undefined and we report an ambiguous redirect. Quotes are then removed from the remaining redirection targets.
Argument list: the token range of the command is converted into a list, cleaned (quote removal, and dropping the words that expansion reduced to nothing), then flattened into the final char **args stored in the node.
Expansion: $VAR and $? are replaced by their values. We then study our context / mod (quotes study), and if we find a var that expanded to several words that is not quoted, we word split, resulting in additional args.

Any failure aborts the whole traversal and the command is not executed. We go back to step 1, and eventually write the error on file descriptor number 2.

## 5. Runtime (Interpreter)

### Core

The interpreter checks what kind of node we are, a command? Run it! Is it a "|" pipe token? Then we create two new fd's with the pipe() function, and use recursion to traverse the tree, passing down information such as which fd's to use, we modify a "context" struct holding more information, such as the shell instance itself, the ENV, and so forth.

By running each command, we fill up the context, we know which PID's are running, which fd's are available, the final step is to waitpid() each running pid created by execve(). While waiting, we have updated signals so that we are able to catch it at that PID level (instead of letting it bubble into the actual minishell, crashing minishell itself), from there we are able to tell if it was a crash, a broken pipe, and so forth, and print the correct message.

### Heredocs

Heredoc tokens are already created from the tokenization/parser stage. The interpreter essentially just catches signals, eg SIGINT for when the user hits ctrl+c and exits. Heredoc uses a file based fd so that the input stream can be any size, otherwise using a pipe() would incur a 64kb maximum size.

### Redirections

The redirections also relatively simple, as the tokenizer has done most of the work and presents us with information, we just iterate through the list of redirections on the line, operations based on the token.

### Running commands

On running commands, we check name first and see if any builtins are hit. If not, we continue on traverse() and using an execve + use the context for ENV. We search through all of our paths to find the correct file, then spawn it in a new child, setting up the correct signals as to override and not crash the parent, and as well listen to when events occur.


# INSTRUCTIONS

At the root of the folder you cloned, type in the terminal the following :

make

./minishell

This should launch the program. After typing make, press enter, and then type the next instruction.

You can now type in the minishell any command, and see what you get !

# RESOURCES

Bash manual : https://www.gnu.org/software/bash/manual/bash.html
