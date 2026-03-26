# COL106 - Long Assignment 2: SocialNet Simulator

This project is an implementation of **SocialNet**, a command-line simulator for a social network's backend services, submitted for the COL106: Data Structures and Algorithms assignment.

The primary goal is to simulate the management of users, friendships, and content. The core data structures—a **Graph** to represent the network and an **AVL Tree** for storing posts—were built from scratch.

## System Architecture

The simulator's backend relies on three core data structures:

* **Graph:** An unweighted, undirected graph represents the social network. Each **user** is a **vertex** in the graph, and a **friendship** is represented as an **edge** between two vertices.
* **AVL Tree:** Each `user` object contains its own self-balancing AVL Tree. This tree is used to store all posts created by the user.
    * **Sorting:** The key for each node in the AVL Tree is the `time_t` timestamp from when the post was created. This ensures that an in-order traversal of the tree retrieves posts in chronological order.
* **Hash Map (`std::unordered_map`):** As permitted by the assignment, the C++ `unordered_map` is used to map string usernames to their corresponding user IDs (or graph vertices). This provides an efficient O(1) average-time lookup for finding users.

## Prerequisites

To compile and run this project, you will need:
* A C++ compiler that supports C++11 or newer (e.g., g++).
* A standard shell environment (e.g., bash).

## How to Compile and Run

1.  **Compile:** Navigate to the directory containing the source code (e.g., `social_net.cpp`) and run the following command in your terminal:
    ```bash
    g++ social_net.cpp -o social_net
    ```

2.  **Run:** To start the program, execute the compiled file:
    ```bash
    ./social_net
    ```
3.  **Using the Program:** The program will wait for you to input commands. All commands are read from standard input (`stdin`).

## Command Reference

The program accepts the following commands. All commands and arguments are **case-insensitive**.

| Command | Syntax | Description |
| :--- | :--- | :--- |
| `ADD_USER` | `ADD_USER <username>` | Adds a new, unique user to the network. |
| `ADD_FRIEND` | `ADD_FRIEND <username1> <username2>` | Establishes a bidirectional friendship. |
| `LIST_FRIENDS` | `LIST_FRIENDS <username>` | Prints an alphabetically sorted list of a user's friends. |
| `SUGGEST_FRIENDS` | `SUGGEST_FRIENDS <username> <N>` | Recommends up to `N` new friends. |
| `DEGREES_OF_SEPARATION` | `DEGREES_OF_SEPARATION <username1> <username2>` | Finds the shortest friendship path between two users. |
| `ADD_POST` | `ADD_POST <username> "<post_content>"` | Adds a new post to a user's timeline. |
| `OUTPUT_POSTS` | `OUTPUT_POSTS <username> <N>` | Outputs the user's `N` most recent posts. |
| `EXIT` | `EXIT` | Terminates the simulator. |

---

## Command Details and Error Handling

The program performs robust error handling for all commands. For a command to execute successfully, the following conditions must be met.

* **General**:
    * The command must be one of the known commands.
    * Usernames and post content are **case-insensitive** and are internally converted to lowercase. For example, `ADD_USER "Alice"` and `ADD_USER "alice"` refer to the same user.
    * It's assumed that usernames do not contain spaces.

* **`ADD_USER <username>`**
    * **Condition:** Must have exactly one argument.
    * **Condition:** A user with the same username must not already exist.

* **`ADD_FRIEND <username1> <username2>`**
    * **Condition:** Must have exactly two arguments.
    * **Condition:** Both `username1` and `username2` must exist in the network.
    * **Condition:** A user cannot be friends with themselves.
    * **Note:** If the users are already friends, the command will print a message and do nothing.

* **`LIST_FRIENDS <username>`**
    * **Condition:** Must have exactly one argument.
    * **Condition:** The specified user must exist.
    * **Output:** Prints a list of the user's friends, one per line, sorted **alphabetically**.

* **`SUGGEST_FRIENDS <username> <N>`**
    * **Condition:** Must have exactly two arguments.
    * **Condition:** The specified user must exist.
    * **Condition:** `<N>` must be a valid, non-negative integer. If `N=0`, the command outputs nothing.
    * **Logic:** This command finds all "friends of friends" who are not currently friends with the user.
    * **Ranking:** Suggestions are ranked based on the number of **mutual friends (descending)**. Any ties are broken by **alphabetical order** of the usernames.

* **`DEGREES_OF_SEPARATION <username1> <username2>`**
    * **Condition:** Must have exactly two arguments.
    * **Condition:** Both users must exist.
    * **Logic:** Implements a **Breadth-First Search (BFS)** starting from `username1` to find the shortest path to `username2`.
    * **Output:**
        * `0` if `username1` and `username2` are the same.
        * `-1` if no friendship path exists between the users.
        * A positive integer (e.g., `2`) representing the shortest path length.

* **`ADD_POST <username> "<post_content>"`**
    * **Condition:** The specified user must exist.
    * **Crucial Syntax:** The `<post_content>` **must be enclosed in double quotation marks** (e.g., `"Hello world"`). The program parses this exact format and will show an error if the quotes are missing or mismatched.
    * **Logic:** The post content (converted to lowercase) is added to the user's AVL Tree. The tree node is keyed by the system's current `time(0)` timestamp.
    * **Note:** The post content can be empty (e.g., `ADD_POST Alice ""`).

* **`OUTPUT_POSTS <username> <N>`**
    * **Condition:** Must have exactly two arguments.
    * **Condition:** The specified user must exist.
    * **Condition:** `<N>` must be a valid integer.
    * **Logic:** This command prints the user's `N` most recent posts. Because posts are stored in an AVL Tree sorted by time, this is achieved by performing an in-order traversal and printing the last `N` elements.
    * **Special Case:** If `N = -1`, all posts by the user will be printed in reverse chronological order.
    * **Edge Case:** If the user has fewer than `N` posts, all available posts are printed.

---

## Example Usage

```bash
> ADD_USER Alice
> ADD_USER Bob
> ADD_USER Charlie
> ADD_FRIEND Alice Bob
> ADD_FRIEND Bob Charlie

> LIST_FRIENDS Bob
alice
charlie

> DEGREES_OF_SEPARATION Alice Charlie
2

> SUGGEST_FRIENDS Alice 1
charlie

> ADD_POST Alice "Hello world! This is my first post."
> ADD_POST Alice "This is a second post, added later."

> OUTPUT_POSTS Alice -1
this is a second post, added later.
hello world! this is my first post.

> OUTPUT_POSTS Alice 1
this is a second post, added later.

> EXIT