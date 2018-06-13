import Cocoa

class YaftController: NSViewController {
    
    var yaft: Yaft!
    var yaftView: YaftView!
    
    let timer = DispatchSource.makeTimerSource()
    var prevEventTimestamp: TimeInterval = 0.0
    
    @IBOutlet weak var imageView: NSImageView!
    
    override func keyDown(with event: NSEvent) {
        yaft.writeToPseudoTerminal(str: event.characters!)
    }
    
    override func rightMouseDown(with event: NSEvent) {
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
    
    func addEventMonitors() {
        NSEvent.addLocalMonitorForEvents(matching: .keyDown) {
            self.keyDown(with: $0)
            return $0
        }
        NSEvent.addLocalMonitorForEvents(matching: .rightMouseDown) {
            self.rightMouseDown(with: $0)
            return $0
        }
    }
    
    func updateView() {
        if !yaft.checkPseudoTerminal() {
            return
        }
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
