# raylib-webrtc_bomberman
Clone of the multipayer Bomberman experience playable in a web browser over WebRTC. Written in C with [raylib](https://www.raylib.com/).

Watch it here: [https://www.youtube.com/watch?v=ccELsHNIhfQ&t=102s](https://www.youtube.com/watch?v=ccELsHNIhfQ&t=102s)

*This a WIP, still have quite a lot of things to do, see the "Roadmap" section at the end.*

## Introduction

The only dependency external dependency of this project is raylib. I wrote several libraries to help me along the way and I hope to reuse them for future projects. I regrouped everything in this repository. A lot of the code here is still a WIP, some things I'm aware I should change/fix but to be honest I kind of moved fast as I wanted to get something working as soon as possible, spending *too much* time trying to make things perfect is a flaw I have.

Everything is written in C99 C except for WebRTC related code because [emscripten does not provide a C API around WebRTC](https://emscripten.org/docs/porting/networking.html#webrtc-and-udp), more about that in the "Networking" section.

## Networking

This project started as a client-server approach with the server being authorative over the game. This idea has been abandoned because it adds a lot of complexity and in the end, I just wanted to get something working so I could play with family and friends and not worry about the cheating aspect. I still want to implement it for future projects though.

Despite all that, the game still uses a client-server approach with the server acting as a kind of proxy that forwards messages between clients, this is not a true peer-to-peer approach but it works well and I believe it has the advantage of removing the "host advantage" you have with pure peer-to-peer.

For communication, the game uses two WebRTC data channels, one *reliable* and one *unreliable ordered* (`maxRetransmits: 0, ordered: true`).
The reliable one is used for time-sensitive punctual actions like "I dropped a bomb at (x, y)" and for "control" messages like "round start" or "game over" or anything that would break the game if not received on the other end. The unreliable one is used for player positions and ping messages or any other more disposable messages. It's ok to lose a position message once in a while because we know the next one will be the most updated position anyway. The `ordered: true` is used so that any out of order message is simply dropped (we don't want to deal with an outdated player position).

Before any real WebRTC communication can happen, some [signaling](https://www.html5rocks.com/en/tutorials/webrtc/infrastructure/) has to be done. For sack of simplicity, this is written in javascript using WebSockets (see in `src/javascript` folder in both `tdd_game1_client` and `tdd_game1_server` projects). When a new client wants to join, it starts a signaling protocol with the server, if successful, a new WebRTC connection ([RTCPeerConnection](https://developer.mozilla.org/fr/docs/Web/API/RTCPeerConnection)) is created between the client and the server along with a pair of data channels (discussed above), the communication can begin. Each client maintains a single instance of `RTCPeerConnection` that represents his connection to the server. The server maintains a list of `RTCPeerConnection` that represents his connections to each client.

From here, the network communication is up and running and everything else is done in C. I wrote an API that hides the WebRTC concerns as much as possible so I could focus on implementing actual networking logic in C. My goal is that this game can be played over the internet with relatively "high ping", therefore I tried to make the network code as efficient as possible. Before I dive into network implementation details, here is a little bit of terminology:

- *Net message*: serializable data structure that represents something in the game (a state or an event). They are gameplay specific and used to implement the game protocol. Each net message has a type (a value between 0 and `MAX_MESSAGE_TYPES`) so that it can be recognized on the receiving end. A net message can be *reliable* or *unreliable*. A net message is not sent directly over the network but enqueued in the "send queue".
- *Send queue*: the send queue is a queue of net messages. At a given frequency, the send queue is flushed. Flushing the send queue means dequeuing the net messages inside the queue and pack them into packets. Every time the send queue is flushed, two packets are created, a reliable one containing only reliable net messages, and an unreliable one containing only unreliable net messages. The reliable packet is sent to the reliable data channel and the unreliable one on the unreliable data channel.
- *Packet*: a packet is simply a byte array that represents a collection of net messages packed together that will be sent over the network. The packet size is fixed (it's the same for every packet) and is calculated so that the largest net message can fit inside. For this game, the packet size is 128 bytes (see `MAX_PACKET_SIZE`, should probably be named `PACKET_SIZE` though...).

I believe the most important part of the network code is that we don't directly send net messages. Both client and server run at 60 Hz, a client roughly creates around 2-3 net messages every game loop iteration (depends on what is happening in the game, can be even more), and the server even more (because it has to forward net messages from every client). Sending net messages directly would mean sending data to data channels *at least* 120 times per second on client-side and at least `120 * 2 = 240` times per second (for 2 players, could be up to 4)  on server-side. This seemed like a little too much.
As an alternative, we enqueue net messages in a "send queue". At a quite "low" frequency (30 Hz for the client, 20 Hz for the server), we flush the send queue: pending net messages inside the queue get packed together inside one reliable packet and one unreliable packet within the limit of `MAX_PACKET_SIZE` (128 bytes). When packets are full or the queue is empty, they are sent on their respective data channels.

Regarding bandwidth, a client sends 128 bytes 30 times per second, so we are looking at a `(30 * 128) / 1000 = 3.8` kilobytes per second upload. The server sends 128 bytes of data 20 times per second for every client, the game supports a maximum of 4 clients so we are looking at a max upload of `((20 * 128) * 4) / 1000 = 10,2` kilobytes per second. Each client receives `(20 * 128) / 1000 = 2.5` kilobytes of data per second. The server receives at most `((30 * 128) * 4) / 1000 = 15.3`kilobytes of data per second.

One problem with sending data at "low frequency" is that it creates an unpleasant effect on client-side, clients will receive other player positions at 20 Hz while the game runs at 60 Hz, it will feel laggy. To address this issue, I implemented a well-known technique called "entity interpolation", see this [this great paper](https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking) for more information.

To reduce net messages size to the minimum, I implemented a bit-level serialization strategy based on this [awesome article](https://gafferongames.com/post/serialization_strategies/).

Here are the relevant sources regarding the above concepts:

- The whole [tdd_network](https://github.com/nathhB/raylib-webrtc_bomberman/tree/master/tdd_network) library (net message, packets, serialization etc.).
- Shared network code in [tdd_game1](https://github.com/nathhB/raylib-webrtc_bomberman/tree/master/tdd_game1/src) (`network.c`, `network.h` and everything inside the `network` folder).
- Client network code in [tdd_game1_client](https://github.com/nathhB/raylib-webrtc_bomberman/tree/master/tdd_game1_client/src) (`network.c` and `network.h`).
- Server network code in [tdd_game1_server](https://github.com/nathhB/raylib-webrtc_bomberman/tree/master/tdd_game1_server/src) (`network.c` and `network.h`).

## Navigate through the code

Here is a little summary of what you will find in the different folders of this repository.

- *tdd*: this is a kind of "utils" library that contains things that I use everywhere like linked lists, hash tables, timers, etc.
- *tdd_ecs*: this is a little ECS library i wrote
- *tdd_network*: network library, every network-related bits that I plan on reusing goes in here (or will end up in here)
- *tdd_game1*: library that contains any code shared between client and server sides
- *tdd_game1_client*: client code
- *tdd_game1_server*: server code
- *tdd_utests*: a very little library that wraps CuTest and adds a single macro `UNIT_TESTS` that I think is handy for integrating unit tests with `ctest`.

Almost every library has a `tests` folder that contains unit tests.

## Compiling and running

You will need to install CMake, emscripten and nodejs.
 
### Server

Compile it.

```
cd tdd_game1_server
mkdir build
cd build
emconfigure cmake ..
make # I used mingw32-make on Windows
```

Install node packages.

`npm install`

Run it.

`node tdd_game1_server.js 2 5`

The first argument is the number of players (2-4), the second argument is the required number of round victory to win the game.

### Client

Compile it.

```
cd tdd_game1_client
mkdir build
cd build
emconfigure cmake ..
make # I used mingw32-make on Windows
```

Run it.

```
cd tdd_game1_client/buid
python -m SimpleHTTPServer 8000
```

Navigate to http://localhost:8000. Open two or more tabs in your browser to test the game.

## Assets

I did not create the assets, they come from these two sites:

- [https://www.spriters-resource.com/](https://www.spriters-resource.com/)
- [https://spritedatabase.net/](https://spritedatabase.net/)

For now there is no music.

## Tools

- [rtexpacker](https://raylibtech.itch.io/rtexpacker) for texture packing
- VSCode as my coding environment
- [Aseprite](https://www.aseprite.org/) for pixel editing
- Gimp for global image editing

## Roadmap

There is still a lot to do but I don't have a clear roadmap yet, I will try to do it soon and release a public Trello in case someone would like to contribute. 

## Making it your own

I think all of this could be a good starting point if you'd like to create your own little multiplayer 2d game as a lot of what's in this repository could be reused to create another entirely different game. If you do anything with it I'd be happy to hear about it :) you can reach me on raylib discord (@toadd). Don't forget that the code is under the zlib license.
