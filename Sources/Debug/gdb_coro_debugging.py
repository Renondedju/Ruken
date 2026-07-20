# Copied from https://releases.llvm.org/21.1.2/tools/clang/docs/DebuggingCoroutines.html

import gdb
from gdb.FrameDecorator import FrameDecorator

class SymValueWrapper():
    def __init__(self, symbol, value):
        self.sym = symbol
        self.val = value

    def __str__(self):
        return str(self.sym) + " = " + str(self.val)

def get_long_pointer_size():
    return gdb.lookup_type('long').pointer().sizeof

def cast_addr2long_pointer(addr):
    return gdb.Value(addr).cast(gdb.lookup_type('long').pointer())

def dereference(addr):
    return int(cast_addr2long_pointer(addr).dereference())

class CoroutineFrame(object):
    def __init__(self, task_addr):
        self.frame_addr = task_addr
        self.resume_addr = task_addr
        self.destroy_addr = task_addr + get_long_pointer_size()
        self.promise_addr = task_addr + get_long_pointer_size() * 2
        # In the example, the continuation is the first field member of the promise_type.
        # So they have the same addresses.
        # If we want to generalize the scripts to other coroutine types, we need to be sure
        # the continuation field is the first member of promise_type.
        self.continuation_addr = self.promise_addr

    def next_task_addr(self):
        return dereference(self.continuation_addr)

class CoroutineFrameDecorator(FrameDecorator):
    def __init__(self, coro_frame):
        super(CoroutineFrameDecorator, self).__init__(None)
        self.coro_frame = coro_frame
        self.resume_func = dereference(self.coro_frame.resume_addr)
        self.resume_func_block = gdb.block_for_pc(self.resume_func)
        if self.resume_func_block is None:
            raise Exception('Not stackless coroutine.')
        self.line_info = gdb.find_pc_line(self.resume_func)

    def address(self):
        return self.resume_func

    def filename(self):
        return self.line_info.symtab.filename

    def frame_args(self):
        return [SymValueWrapper("frame_addr", cast_addr2long_pointer(self.coro_frame.frame_addr)),
                SymValueWrapper("promise_addr", cast_addr2long_pointer(self.coro_frame.promise_addr)),
                SymValueWrapper("continuation_addr", cast_addr2long_pointer(self.coro_frame.continuation_addr))
                ]

    def function(self):
        return self.resume_func_block.function.print_name

    def line(self):
        return self.line_info.line

class StripDecorator(FrameDecorator):
    def __init__(self, frame):
        super(StripDecorator, self).__init__(frame)
        self.frame = frame
        f = frame.function()
        self.function_name = f

    def __str__(self, shift = 2):
        addr = "" if self.address() is None else '%#x' % self.address() + " in "
        location = "" if self.filename() is None else " at " + self.filename() + ":" + str(self.line())
        return addr + self.function() + " " + str([str(args) for args in self.frame_args()]) + location

class CoroutineFilter:
    def create_coroutine_frames(self, task_addr):
        frames = []
        while task_addr != 0:
            coro_frame = CoroutineFrame(task_addr)
            frames.append(CoroutineFrameDecorator(coro_frame))
            task_addr = coro_frame.next_task_addr()
        return frames

class AsyncStack(gdb.Command):
    def __init__(self):
        super(AsyncStack, self).__init__("async-bt", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        coroutine_filter = CoroutineFilter()
        argv = gdb.string_to_argv(arg)
        if len(argv) == 0:
            try:
                task = gdb.parse_and_eval('__coro_frame')
                task = int(str(task.address), 16)
            except Exception:
                print ("Can't find __coro_frame in current context.\n" +
                       "Please use `async-bt` in stackless coroutine context.")
                return
        elif len(argv) != 1:
            print("usage: async-bt <pointer to task>")
            return
        else:
            task = int(argv[0], 16)

        frames = coroutine_filter.create_coroutine_frames(task)
        i = 0
        for f in frames:
            print('#'+ str(i), str(StripDecorator(f)))
            i += 1
        return

AsyncStack()

class ShowCoroFrame(gdb.Command):
    def __init__(self):
        super(ShowCoroFrame, self).__init__("show-coro-frame", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        argv = gdb.string_to_argv(arg)
        if len(argv) != 1:
            print("usage: show-coro-frame <address of coroutine frame>")
            return

        addr = int(argv[0], 16)
        block = gdb.block_for_pc(int(cast_addr2long_pointer(addr).dereference()))
        if block is None:
            print("block " + str(addr) + " is None.")
            return

        # Disable demangling since gdb will treat names starting with `_Z`(The marker for Itanium ABI) specially.
        gdb.execute("set demangle-style none")

        coro_frame_type = gdb.lookup_type(block.function.linkage_name + ".coro_frame_ty")
        coro_frame_ptr_type = coro_frame_type.pointer()
        coro_frame = gdb.Value(addr).cast(coro_frame_ptr_type).dereference()

        gdb.execute("set demangle-style auto")
        gdb.write(coro_frame.format_string(pretty_structs = True))

ShowCoroFrame()