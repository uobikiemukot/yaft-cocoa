import Cocoa

// helper class for event handling
class Utility: NSObject {

    let UnicodeToEscSeq: [Int:String] = [
        NSUpArrowFunctionKey:    "\u{1b}[A",
        NSDownArrowFunctionKey:  "\u{1b}[B",
        NSRightArrowFunctionKey: "\u{1b}[C",
        NSLeftArrowFunctionKey:  "\u{1b}[D",
        NSF1FunctionKey: "\u{1b}[[A",
        NSF2FunctionKey: "\u{1b}[[B",
        NSF3FunctionKey: "\u{1b}[[C",
        NSF4FunctionKey: "\u{1b}[[D",
        NSF5FunctionKey: "\u{1b}[[E",
        NSF6FunctionKey: "\u{1b}[17~",
        NSF7FunctionKey: "\u{1b}[18~",
        NSF8FunctionKey: "\u{1b}[19~",
        NSF9FunctionKey: "\u{1b}[20~",
        NSF10FunctionKey: "\u{1b}[21~",
        NSF11FunctionKey: "\u{1b}[23~",
        NSF12FunctionKey: "\u{1b}[24~",
        NSF13FunctionKey: "\u{1b}[25~",
        NSF14FunctionKey: "\u{1b}[26~",
        NSF15FunctionKey: "\u{1b}[28~",
        NSF16FunctionKey: "\u{1b}[29~",
        NSF17FunctionKey: "\u{1b}[31~",
        NSF18FunctionKey: "\u{1b}[32~",
        NSF19FunctionKey: "\u{1b}[33~",
        NSF20FunctionKey: "\u{1b}[34~", // not exist in Mac?
    ]

    func getPasteboardString() -> String {
        guard let items = NSPasteboard.general.pasteboardItems else {
            return ""
        }
        return items.map { $0.string(forType: NSPasteboard.PasteboardType.string) ?? "" }.joined()
    }

    func handleKeyEventWithCommandKey(_ chars: String) -> String {
        switch chars {
        case "v":
            return getPasteboardString()
        default:
            return ""
        }
    }

    func handleKeyEvent(_ chars: String, _ commandKeyflag: Bool) -> String {
        guard commandKeyflag == false else {
            return handleKeyEventWithCommandKey(chars)
        }
        // XXX: handle only first UTF16 code
        let code = Int(chars.utf16[chars.utf16.startIndex])
        if let str = UnicodeToEscSeq[code] {
            return str
        } else {
            return chars
        }
    }

}
