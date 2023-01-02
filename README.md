# MiniHttp - SAND

![](/img/mini.png)

This project is using only rapidJson as extern library

# Todo:
  - Cleanup of code and simplifying of code
    - This is the next prio
  - Send json object back to client.
    - Add function to construct object.
  - Check reachable code
  - Add more functions to jsonParser_t so we can use it for more stuff within the server
    - Now we can extract bool, string, ints and arrays this should be fine for now
    - Additionally, I need to add to parse further nested objects.
  - Add remaining http methods
  - Improve parser
    - This is improved for now. Still not happy with it
  - Add Unit_tests --> This will probably never happen
  - Check get_in_addr function --> I forgot what I wanted to check here
  - Summarize how kqueue, kevent works
  - Simplify server_static_function / Make it easier to use
  - Check if handle_read is working okay?!
  - Add PHP support -> Thanks to mujtaba
  - Add dynamic routing stuff
  - HttpRequest_t SimpleHttpServer_t::handle_read fix that ugly methode wtf
  - Code:
    - Layout
      - Public methodes
      - Public members
      - Private methodes
      - Private members
  - I am only here for the new year commit


# Serving static files

- Added this
- Files in static folder are served
- Removed adding of headers in "serve_static_file" function, so we can use the setter function. Also using "
  buildResponseBody" is now possible

# Add remaining http methods -> post,delete,put
  - [x] GET
  - [x] POST
  - [ ] PUT
  - [ ] DELETE

# Fix recv and send function to send whole data safe!

  - Added, for valid requests/response
  - Need to add for paths which are not registered

# Optimize Server with Threads

- Adding separate thread for listen and for event handling
    - The listener thread should only listen for incoming connections and add them to the kqueue
    - Added. The listener thread works in own thread and adds new connections to the kqueue.
    - Added. Worker threads have each their own kqueue and operate on this kqueue. The listener thread adds connections
      to each of the workers kqueue
- Splitting start server into multiple functions
    - Done. Start server function has been split in multiple smaller functions

# Check get_in_addr function

- For ip network format to string
- Probably inet_ntop function is wrongly used
- Make Ip to string conversion work with sockaddr_storage, so we can later handle IPv4 and IPv6

# Using Kqueue

- How to use it
- What is it
- How does it work?


# Current Status

![](/img/Benchmark-Version2.png)

# Old Versions

![](/img/FirstWorkingVersion.png)
