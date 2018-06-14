import Cocoa

// wrap C API and provide swift function
class Yaft: NSObject {
    let buf: UnsafeMutablePointer<UInt8>!
    let bufSize: Int
    let bytesPerRow: Int
    let width: Int  = 960
    let height: Int = 576

    override init() {
        if !c_init(Int32(width), Int32(height)) {
            print("c_init() failed")
            // terminate app?
            //let delegate = NSApp.delegate  as! AppDelegate
            //delegate.window.performClose("yaft model")
        }
        buf         = fb.buf
        bufSize     = Int(fb.info.screen_size)
        bytesPerRow = Int(fb.info.line_length)
        super.init()
    }

    func checkPseudoTerminal() -> Bool {
        return c_select()
    }

    func writeToPseudoTerminal(str: String) {
        c_write(str, str.utf8.count)
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
