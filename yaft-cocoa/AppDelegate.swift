import Cocoa

class AppDelegate: NSObject, NSApplicationDelegate {

    var window: NSWindow!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        window.makeKeyAndOrderFront(nil)
    }

    func applicationWillFinishLaunching(_ notification: Notification) {
        window = NSWindow(contentViewController: YaftController())
        window.title = "yaft"
        window.center()
        window.styleMask = [.titled, .borderless, .closable]
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }

}
