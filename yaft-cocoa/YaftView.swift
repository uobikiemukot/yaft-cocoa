import Cocoa

class YaftView: NSView {

    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)
        // Drawing code here.
        if let context = NSGraphicsContext.current {
            let cgContext = context.cgContext
            cgContext.setFillColor(NSColor.black.cgColor)
            cgContext.fill(dirtyRect)
        }
    }

}
