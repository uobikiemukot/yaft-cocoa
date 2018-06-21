import Cocoa

class YaftController: NSViewController {

    let yaft: Yaft         = Yaft()
    let yaftView: YaftView = YaftView()
    let utility: Utility   = Utility()

    let updateViewTimer = DispatchSource.makeTimerSource()
    let checkChildTimer = DispatchSource.makeTimerSource()
    var prevEventTimestamp: TimeInterval = 0.0
    var commandKeyFlag = false

    @IBOutlet weak var imageView: NSImageView!

    func sendPasteboardString() {
        // send clipboard string to terminal
        let board = NSPasteboard.general
        for element in board.pasteboardItems! {
            if let str = element.string(forType: NSPasteboard.PasteboardType.string) {
                yaft.writeToPseudoTerminal(str)
            }
        }
    }

    override func flagsChanged(with event: NSEvent) {
        if event.modifierFlags.contains(NSEvent.ModifierFlags.command) {
            commandKeyFlag = true
        } else {
            commandKeyFlag = false
        }
    }

    override func keyDown(with event: NSEvent) {
        // TODO: handle special keys (command + v, function keys, cursor keys) correctly
        if let chars = event.characters {
            let str = utility.handleKeyEvent(chars, commandKeyFlag)
            yaft.writeToPseudoTerminal(str)
        }
    }

    override func rightMouseDown(with event: NSEvent) {
        // XXX: rightMouseDown event occurs twice at the same time...
        if prevEventTimestamp != event.timestamp {
            let str = utility.getPasteboardString()
            yaft.writeToPseudoTerminal(str)
            prevEventTimestamp = event.timestamp
        }
    }

    func addEventMonitors() {
        NSEvent.addLocalMonitorForEvents(matching: .flagsChanged) {
            self.flagsChanged(with: $0)
            return $0
        }
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
        if yaft.checkPseudoTerminal() {
            DispatchQueue.main.async {
                let image = self.yaft.buildImage()
                self.imageView.image = image
                self.imageView.needsDisplay = true
            }
        }
    }

    func checkChild() {
        if !yaft.checkChildProcessIsAlive() {
            yaft.dieTerminal()
            DispatchQueue.main.async {
                // bad manner?
                if let delegate = NSApplication.shared.delegate as? AppDelegate {
                    delegate.window.performClose(self)
                }
            }
        }
    }

    func activateTimer() {
        /* check data arrival of PseudoTerminal and update view */
        updateViewTimer.schedule(deadline: .now(), repeating: .microseconds(15000), leeway: .microseconds(1000))
        updateViewTimer.setEventHandler { self.updateView() }
        updateViewTimer.activate()

        /* check child process is alive or not */
        checkChildTimer.schedule(deadline: .now(), repeating: .seconds(1), leeway: .seconds(1))
        checkChildTimer.setEventHandler { self.checkChild() }
        checkChildTimer.activate()
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do view setup here.
        addEventMonitors()

        // start periodic jobs
        activateTimer()
    }

}
