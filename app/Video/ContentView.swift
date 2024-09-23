import SwiftUI
import UIKit
import MobileVLCKit

// https://stackoverflow.com/questions/59806850/vlckit-with-swiftui

struct ContentView: View {
    // launch the local stream proxy
    let server = stream_server_new(cString("192.168.0.18"), 8080)
    
    var body: some View {
        NavigationView{
            VStack{
                Text("Stream")
                PlayerView(url: "tcp://127.0.0.1:9000")
                PlayerView(url: "udp://@127.0.0.1:9001")
            }.task {
                DispatchQueue.global(qos: .background).async {
                    // run the stream server in background
                    stream_server_run(server)
                }
            }
        }
    }
    
    static func cString(_ string: String) -> UnsafeMutablePointer<Int8>{
        UnsafeMutablePointer<Int8>(mutating: (string as NSString).utf8String!)
    }
}

struct PlayerView: UIViewRepresentable {
    let mediaPlayer = VLCMediaPlayer()
    let url: String

    init(url: String) {
        self.url = url
    }

    func makeUIView(context: Context) -> UIView {
        let controller = UIView()
        mediaPlayer.drawable = controller
        let uri = URL(string: url)
        let media = VLCMedia(url: uri!)
        mediaPlayer.media = media
        mediaPlayer.play()
        mediaPlayer.libraryInstance.debugLogging = true
        mediaPlayer.libraryInstance.debugLoggingLevel = 3
        return controller
    }

    func updateUIView(_ uiView: UIView, context: UIViewRepresentableContext<PlayerView>) {
    }
}
