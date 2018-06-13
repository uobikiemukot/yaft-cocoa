import Cocoa

class YaftController: NSViewController {
    
    var yaft: Yaft!
    var yaftView: YaftView!
    
    let timer = DispatchSource.makeTimerSource()
    var prevEventTimestamp: TimeInterval = 0.0
    
    @IBOutlet weak var imageView: NSImageView!
    
    override func keyUp(with event: NSEvent) {
        //print("keyup:" + event.characters!)
    }
    
    override func keyDown(with event: NSEvent) {
        //print("keydown:" + event.characters!)
        yaft.writeToPseudoTerminal(str: event.characters!)
    }
    
    override func flagsChanged(with event: NSEvent) {
        let modifier: String
        switch event.modifierFlags {
        case NSEvent.ModifierFlags.capsLock:
            modifier = "capsLock"
        case NSEvent.ModifierFlags.command:
            modifier = "command"
        case NSEvent.ModifierFlags.control:
            modifier = "controll"
        case NSEvent.ModifierFlags.function:
            modifier = "function"
        case NSEvent.ModifierFlags.help:
            modifier = "help"
        case NSEvent.ModifierFlags.option:
            modifier = "option"
        case NSEvent.ModifierFlags.shift:
            modifier = "shift"
        default:
            modifier = "unknown"
        }
        print("modifier:" + modifier)
    }
    
    override func rightMouseDown(with event: NSEvent) {
        print("rightMouseDown")
        if prevEventTimestamp == event.timestamp {
            return
        }

        let board = NSPasteboard.general
        for element in board.pasteboardItems! {
            if let str = element.string(forType: NSPasteboard.PasteboardType.string) {
                yaft.writeToPseudoTerminal(str: str)
            }
        }
        prevEventTimestamp = event.timestamp
    }
    
    override func rightMouseUp(with event: NSEvent) {
        print("rightMouseUp")
        print(event.buttonNumber, event.clickCount, event.timestamp)
    }
    
    func addEventMonitors() {
        NSEvent.addLocalMonitorForEvents(matching: .keyUp) {
            self.keyUp(with: $0)
            return $0
        }
        NSEvent.addLocalMonitorForEvents(matching: .keyDown) {
            self.keyDown(with: $0)
            return $0
        }
        NSEvent.addLocalMonitorForEvents(matching: .flagsChanged) {
            self.flagsChanged(with: $0)
            return $0
        }
        NSEvent.addLocalMonitorForEvents(matching: .rightMouseDown) {
            self.rightMouseDown(with: $0)
            return $0
        }
        NSEvent.addLocalMonitorForEvents(matching: .rightMouseUp) {
            self.rightMouseUp(with: $0)
            return $0
        }

    }
    
    func updateView() {
        if !yaft.checkPseudoTerminal() {
            return
        }
        print("update view")
        DispatchQueue.main.async {
            let image = self.yaft.buildImage()
            self.imageView.image = image
            self.imageView.needsDisplay = true
        }
    }
    
    func activateTimer() {
        timer.schedule(deadline: .now(), repeating: .microseconds(15000), leeway: .microseconds(1000))
        timer.setEventHandler { self.updateView() }
        timer.activate()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        addEventMonitors()
        
        // initialize view and model
        yaft = Yaft()
        yaftView = YaftView()
        
        // do something...
        activateTimer()
    }

}
