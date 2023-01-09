# MiniHttp - SAND

![](/img/mini.png)

This project is using only rapidJson as extern library

Will check to move todos to issues. I don't like it like that anymore
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
  - Summarize how kqueue, kevent works
  - Check if handle_read is working okay?!
  - Add PHP support -> Thanks to mujtaba
    - This would introduce more complexity and I cant handle it
  - HttpRequest_t SimpleHttpServer_t::handle_read fix that ugly methode wtf
  - Code:
    - Layout
      - Public methodes
      - Public members
      - Private methodes
      - Private members


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

# Optimize Server with Threads

- Adding separate thread for listen and for event handling
    - The listener thread should only listen for incoming connections and add them to the kqueue
    - Added. The listener thread works in own thread and adds new connections to the kqueue.
    - Added. Worker threads have each their own kqueue and operate on this kqueue. The listener thread adds connections
      to each of the workers kqueue
- Splitting start server into multiple functions
    - Done. Start server function has been split in multiple smaller functions

# Using Kqueue

- How to use it
- What is it
- How does it work?


# Current Status

![](/img/Benchmark-Version2.png)

# Old Versions

![](/img/FirstWorkingVersion.png)
