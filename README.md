# http

# Serving static files

- Added this

# Add remaining http methods -> post,delete,put

# Fix recv and send function to send whole data safe!

# Optimize Server with Threads

- Adding Seperate thread for listen and for event handling
    - The listener thread should only listen for incoming connections and add them to the kqueue
    - Added. The listener thread works in a own thread and adds new connections to the kqueue.
    - Added. Worker threads have each their own kqueue and operate on this kqueue. The listener thread adds connections
      to each of the workers kqueues
- Splitting start server into multple functions
    - Done. Start server function has been split in multiple smaller functions

# Check how do determine a reasonable buffer size

# Check get_in_addr function

- For ip network format to string
- Probably inet_ntop function is wrongly used
- Make Ip to string conversion work with sockaddr_storage so we can later handle IPv4 and IPv6

# Using Kqueue

- How to use it
- What is it
- How does it work?

# TODO:

- Adding Unit Tests
- Give the parser some love to handle post and so on

# Current Status

![](/img/Benchmark-Version2.png)

# Old Versions

![](/img/FirstWorkingVersion.png)
