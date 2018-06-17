import Cocoa

class YaftController: NSViewController {

    var yaft: Yaft!
    var yaftView: YaftView!

    let updateViewTimer = DispatchSource.makeTimerSource()
    let checkChildTimer = DispatchSource.makeTimerSource()
    var prevEventTimestamp: TimeInterval = 0.0

    @IBOutlet weak var imageView: NSImageView!

    func sendPasteboardString() {
        // send clipboard string to terminal
        let board = NSPasteboard.general
        for element in board.pasteboardItems! {
            if let str = element.string(forType: NSPasteboard.PasteboardType.string) {
                yaft.writeToPseudoTerminal(str: str)
            }
        }
    }

    override func keyDown(with event: NSEvent) {
        // TODO: handle special keys (command + v, function keys, cursor keys) correctly
        if let str = event.characters {
            if event.modifierFlags.contains(NSEvent.ModifierFlags.command) {
                switch str {
                case "v": // command + v
                    sendPasteboardString()
                default:
                    print("not supported")
                }
            } else {
                switch event.keyCode {
                case 126: // cursor up
                    yaft.writeToPseudoTerminal(str: "\u{1b}[A")
                case 125: // cursor down
                    yaft.writeToPseudoTerminal(str: "\u{1b}[B")
                case 124: // cursor right
                    yaft.writeToPseudoTerminal(str: "\u{1b}[C")
                case 123: // cursor left
                    yaft.writeToPseudoTerminal(str: "\u{1b}[D")
                default:
                    yaft.writeToPseudoTerminal(str: str)
                }
            }
        }
    }

    override func rightMouseDown(with event: NSEvent) {
        // XXX: rightMouseDown event occurs twice at the same time...
        if prevEventTimestamp != event.timestamp {
            sendPasteboardString()
            prevEventTimestamp = event.timestamp
        }
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

        // initialize view and model
        yaft = Yaft()
        yaftView = YaftView()

        // start periodic jobs
        activateTimer()
    }

}
