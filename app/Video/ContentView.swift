import SwiftUI
import UIKit
import MobileVLCKit

// https://stackoverflow.com/questions/59806850/vlckit-with-swiftui

struct ContentView: View {
    @State var player = VLCMediaPlayer()
    @State private var serverIP = "192.168.0.18"
    @State private var playing = false
    
    init() {
        player.libraryInstance.debugLoggingLevel = 1
    }
    
    var body: some View {
        NavigationView{
            VStack{
                Form {
                    HStack{
                        TextField("Server IP", text: $serverIP)
                        Button("Play", systemImage: "play.desktopcomputer"){
                            
                            run()
                        }
                    }
                }
                PlayerView(player: player)
                Spacer()
            }
        }
    }
    
    func run(){
        if !playing{
            print("Playing")
            let server = stream_server_new(cString(serverIP), 8080)
            if (server == nil){
                print("Failed to create server")
                return
            }
            print("server launched")
            Task{
                stream_server_run(server)
                print("Server done !")
            }
            let uri = URL(string: "tcp://127.0.0.1:9000")
            let media = VLCMedia(url: uri!)
            player.media = media
            player.play()
        }else{
            print("Stopping")
            player.stop()
        }
        playing = !playing
    }
    
    func cString(_ string: String) -> UnsafeMutablePointer<Int8>{
        UnsafeMutablePointer<Int8>(mutating: (string as NSString).utf8String!)
    }
}

struct PlayerView: UIViewRepresentable {
    let mediaPlayer: VLCMediaPlayer

    init(player: VLCMediaPlayer) {
        self.mediaPlayer = player
    }

    func makeUIView(context: Context) -> UIView {
        let controller = UIView()
        mediaPlayer.drawable = controller
        return controller
    }

    func updateUIView(_ uiView: UIView, context: UIViewRepresentableContext<PlayerView>) {
    }
}
