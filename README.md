# Socket Programming & HTTP Connection

###How to run code

- **Part1 - C code ** 
  - *Server* 
    - `gcc server.c`
    - `./a.out`  `[PORT]`
    - eg. ./a.out 8080
  - *Client*
    - `gcc client.c`

    - `./a.out` `[IP] [PORT] [filename present in server directory]`

    - eg.  ./a.out 127.0.0.1 8080  file1.txt

      ​


- **Part2 - Python code - Persistent & Non Persistent Both**
  - Server

    - server.py num < input files  **(num = 2   for persistent,  num = 1  for non persistent)**

  -   Client

      -   client.py num < input files   **(num = 2   for  persistent,  num = 1 for non persistent)**

          ****

- **Part2  - Persistent & Non Persistent Multiple Files**

  - Server

    - `gcc server_persistent.c or gcc server_nonpersistent.c`
    - `./a.out`  `[PORT]`
    - eg. ./a.out 8080
   - Client
        - `gcc client_persistent.c or gcc server_nonpersistent.c`
        - `./a.out` `[IP] [PORT] < Input files`  
        - ./a.out 127.0.0.1 8080 <file-name> 

  ​

### Definition 

1. **Socket** - A network socket is an internal endpoint for sending or receiving data at a single node in a computer network. Sockets are an API that most operating systems provide to be able to talk with the network.

2. **HTTP Connection** - HTTP is an application protocol. It basically means that HTTP itself 
   can't be used to transport information to/from a remote end point. Instead it relies on an underlying protocol which in HTTP's case is TCP.

3. **Socket Programming** - Socket programming is a way of connecting two nodes on a network to 
   communicate with each other. One socket(node) listens on a particular 
   port at an IP, while other socket reaches out to the other to form a 
   connection. Server forms the listener socket while client reaches out to
    the server.

   ​

   ​		**State diagram for server and client model**

   ![http://cdncontribute.geeksforgeeks.org/wp-content/uploads/Socket-Programming-in-C-C-.jpg](http://cdncontribute.geeksforgeeks.org/wp-content/uploads/Socket-Programming-in-C-C-.jpg)

### Implementation in C

1.  **Server** 

   1. **Socket creation:** `int sockfd = socket(domain, type, protocol)`

      1. **sockfd:** socket descriptor, an integer (like a file-handle)

      2. **domain:** integer, communication domain e.g., AF_INET (IPv4 protocol) , AF_INET6 (IPv6 protocol)

      3. **type:** communication type <u>SOCK_STREAM: TCP(reliable, connection oriented)</u> <u>     SOCK_DGRAM: UDP(unreliable, connectionless)</u> 

      4. **protocol: **Protocol value for Internet Protocol(IP), 
         which is 0. This is the same number which appears on protocol field in 
         the IP header of a packet.(man protocols for more details)

         ​

   2. **Setsockopt:** `int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);` 

      This helps in manipulating options for the socket referred by the file  descriptor sockfd. This is completely optional, but it helps in reuse of address and port. Prevents error such as: “address already in use”.

      ​

   3. **Bind:** `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`

   4. **Listen: ** `int listen(int sockfd, int backlog);`

   5. **Accept: ** `int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`

      ​

2.  **Client**

   1.  **Socket connection:** Exactly same as that of server’s socket creation.

   2.  **Connect:** `int connect(int sockfd, const struct sockaddr *addr,  socklen_t addrlen);`

      The connect() system call connects the socket referred to by the file descriptor sockfd to the address specified by addr. Server’s address and  port is specified in addr.

      ​

## Time Analysis

**Persistent Connection** -  It does not require connection setup again and again. Multiple objects can use connection.

**Non Persistent** -   It requires connection setup again and again for each object to send.

Time taken by persistent or non persistent depends on size of files to be downloaded, system configuration and various other factors.

**Time Persistent**

​		`real	0m0.156s`

​		`user	0m0.020s`

​		`sys	0m0.084s`



**Time Non Persistent**

​		`real	0m0.133s`

​		`user	0m0.044s`

​		`sys	0m0.036s`



*The time taken in user mode is more for persistent as compared to non persistent as expected but* 

<u>the server in non persistent send more data in first time as compared to persistent in which data is send at</u> 

<u>later more</u> *is the main reson for sys timing hence real timing.*



