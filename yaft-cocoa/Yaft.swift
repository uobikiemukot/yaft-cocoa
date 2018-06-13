import Cocoa

// wrap C API and provide swift function
class Yaft: NSObject {
    let buf: UnsafeMutablePointer<UInt8>!
    let bufSize: Int
    let bytesPerRow: Int
    let width: Int
    let height: Int
    
    override init() {
        if !c_init() {
            print("c_init() failed")
            // terminate app?
            //let delegate = NSApp.delegate  as! AppDelegate
            //delegate.window.performClose("yaft model")
        }
        buf         = fb.buf
        bufSize     = Int(fb.info.screen_size)
        bytesPerRow = Int(fb.info.line_length)
        width       = Int(fb.info.width)
        height      = Int(fb.info.height)
        super.init()
    }
    
    func checkPseudoTerminal() -> Bool {
        return c_select()
    }
    
    func writeToPseudoTerminal(str: String) {
        print("writing: " + str)
        c_write(str, str.utf8.count)
    }
    
    func convertColor(index: Int) -> NSColor {
        let RGBA = c_get_color(Int32(index))
        let red   = 0xFF & (RGBA >> fb.info.red.offset)
        let green = 0xFF & (RGBA >> fb.info.green.offset)
        let blue  = 0xFF & (RGBA >> fb.info.blue.offset)
        let alpha = 0xFF & (RGBA >> fb.info.alpha.offset)
        return NSColor(red: CGFloat(red), green: CGFloat(green), blue: CGFloat(blue), alpha: CGFloat(alpha))
    }
    
    func buildImage() -> NSImage {
        let data = Data(bytes: buf, count: bufSize)
        let ciimage = CIImage(bitmapData: data, bytesPerRow: bytesPerRow, size: CGSize(width: width, height: height), format: kCIFormatRGBA8, colorSpace: nil)
        let rep = NSCIImageRep(ciImage: ciimage)
        let nsimage = NSImage(size: NSSize(width: width, height: height))
        nsimage.addRepresentation(rep)
        return nsimage
    }
}
