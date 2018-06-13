import Cocoa

class YaftView: NSView {
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)
        // Drawing code here.
        let context = NSGraphicsContext.current?.cgContext
        context?.setFillColor(NSColor.black.cgColor)
        context?.fill(dirtyRect)
    }
    
}
