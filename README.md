# Video simulation

## Run the server

```bash
cd server
make

# create the ffmpeg input
./server.sh     # in first terminal

# create the tcp server
./server        # in second terminal
```

## Run the client

```bash
cd client
make

# connect to the server and make the stream available
./client        # in first terminal

# display the stream
./client.sh     # in second terminal
```

## Run the app

### Build the dependencies
```
pod install
```

### Run
Set correctly the ip address of the server in [ContentView.swift](./app/Video/ContentView.swift),
and build the app in XCode.

## Architecture of the simulation

![](./architecture.drawio.svg)
