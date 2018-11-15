import Cocoa

// wrapper class of yaft core library (C API)
class Yaft: NSObject {

    private let buf: UnsafeMutablePointer<UInt8>!
    private let bufSize: Int
    private let bytesPerRow: Int
    static let width: Int  = 1280
    static let height: Int = 768

    override init() {
        if !c_init(Int32(Yaft.width), Int32(Yaft.height)) {
            print("c_init() failed")
            // bad manner?
            if let delegate = NSApplication.shared.delegate as? AppDelegate {
                delegate.window.performClose(nil)
            }
        }
        buf         = fb.buf
        bufSize     = Int(fb.info.screen_size)
        bytesPerRow = Int(fb.info.line_length)
        super.init()
    }

    func initTerminal(width: Int, height: Int) -> Bool {
        return c_init(Int32(width), Int32(height))
    }

    func dieTerminal() {
        c_die()
    }

    func checkPseudoTerminal() -> Bool {
        return c_select()
    }

    func checkChildProcessIsAlive() -> Bool {
        return c_child_alive()
    }

    func writeToPseudoTerminal(_ str: String) {
        c_write(str, str.utf8.count)
    }

    func buildImage() -> NSImage {
        let data = Data(bytes: buf, count: bufSize)
        let ciimage = CIImage(bitmapData: data, bytesPerRow: bytesPerRow, size: CGSize(width: Yaft.width, height: Yaft.height), format: kCIFormatRGBA8, colorSpace: nil)
        let rep = NSCIImageRep(ciImage: ciimage)
        let nsimage = NSImage(size: NSSize(width: Yaft.width, height: Yaft.height))
        nsimage.addRepresentation(rep)
        return nsimage
    }

}
