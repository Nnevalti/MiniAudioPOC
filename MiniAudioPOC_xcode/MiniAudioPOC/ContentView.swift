//
//  ContentView.swift
//  MiniAudioPOC
//
//  Created by SoundX on 04/05/2023.
//

import SwiftUI

class Backend {
    
    init(){
        print("Init\n")
        BridgeEntry();
    }
}

let backend = Backend()

struct ContentView: View {
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundColor(.accentColor)
            Text("Hello, world!")
        }
        .padding()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
