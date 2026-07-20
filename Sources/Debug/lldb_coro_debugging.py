# Copied from https://releases.llvm.org/21.1.2/tools/clang/docs/DebuggingCoroutines.html

import lldb
from lldb.plugins.parsed_cmd import ParsedCommand

def _get_first_var_path(v, paths):
    """
    Tries multiple variable paths via `GetValueForExpressionPath`
    and returns the first one that succeeds, or None if none succeed.
    """
    for path in paths:
        var = v.GetValueForExpressionPath(path)
        if var.error.Success():
            return var
    return None


def _print_async_bt(coro_hdl, result, *, curr_idx, start, limit, continuation_paths, prefix=""):
    """
    Prints a backtrace for an async coroutine stack starting from `coro_hdl`,
    using the given `continuation_paths` to get the next coroutine from the promise.
    """
    target = coro_hdl.GetTarget()
    while curr_idx < limit and coro_hdl is not None and coro_hdl.error.Success():
        # Print the stack frame, if in range
        if curr_idx >= start:
            # Figure out the function name
            destroy_func_var = coro_hdl.GetValueForExpressionPath(".destroy")
            destroy_addr = target.ResolveLoadAddress(destroy_func_var.GetValueAsAddress())
            func_name = destroy_addr.function.name
            # Figure out the line entry to show
            suspension_addr_var = coro_hdl.GetValueForExpressionPath(".promise._coro_suspension_point_addr")
            if suspension_addr_var.error.Success():
                line_entry = target.ResolveLoadAddress(suspension_addr_var.GetValueAsAddress()).line_entry
                print(f"{prefix} frame #{curr_idx}: {func_name} at {line_entry}", file=result)
            else:
                # We don't know the exact line, print the suspension point ID, so we at least show
                # the id of the current suspension point
                suspension_point_var = coro_hdl.GetValueForExpressionPath(".coro_frame.__coro_index")
                if suspension_point_var.error.Success():
                    suspension_point = suspension_point_var.GetValueAsUnsigned()
                else:
                    suspension_point = "unknown"
                line_entry = destroy_addr.line_entry
                print(f"{prefix} frame #{curr_idx}: {func_name} at {line_entry}, suspension point {suspension_point}", file=result)
        # Move to the next stack frame
        curr_idx += 1
        promise_var = coro_hdl.GetChildMemberWithName("promise")
        coro_hdl = _get_first_var_path(promise_var, continuation_paths)
    return curr_idx

def _print_combined_bt(frame, result, *, unfiltered, curr_idx, start, limit, continuation_paths):
    """
    Prints a backtrace starting from `frame`, interleaving async coroutine frames
    with regular frames.
    """
    while curr_idx < limit and frame.IsValid():
        if curr_idx >= start and (unfiltered or not frame.IsHidden()):
            print(f"frame #{curr_idx}: {frame.name} at {frame.line_entry}", file=result)
        curr_idx += 1
        coro_var = _get_first_var_path(frame.GetValueForVariablePath("__promise"), continuation_paths)
        if coro_var:
            curr_idx = _print_async_bt(coro_var, result,
                                       curr_idx=curr_idx, start=start, limit=limit,
                                       continuation_paths=continuation_paths, prefix="[async]")
        frame = frame.parent


class CoroBacktraceCommand(ParsedCommand):
    def get_short_help(self):
        return "Create a backtrace for C++-20 coroutines"

    def get_flags(self):
        return lldb.eCommandRequiresFrame | lldb.eCommandProcessMustBePaused

    def setup_command_definition(self):
        ov_parser = self.get_parser()
        ov_parser.add_option(
            "e",
            "continuation-expr",
            help = (
                "Semi-colon-separated list of expressions evaluated against the promise object"
                "to get the next coroutine (e.g. `.continuation;.coro_parent`)"
            ),
            value_type = lldb.eArgTypeNone,
            dest = "continuation_expr_arg",
            default = ".continuation",
        )
        ov_parser.add_option(
            "c",
            "count",
            help = "How many frames to display (0 for all)",
            value_type = lldb.eArgTypeCount,
            dest = "count_arg",
            default = 20,
        )
        ov_parser.add_option(
            "s",
            "start",
            help = "Frame in which to start the backtrace",
            value_type = lldb.eArgTypeIndex,
            dest = "frame_index_arg",
            default = 0,
        )
        ov_parser.add_option(
            "u",
            "unfiltered",
            help = "Do not filter out frames according to installed frame recognizers",
            value_type = lldb.eArgTypeBoolean,
            dest = "unfiltered_arg",
            default = False,
        )
        ov_parser.add_argument_set([
            ov_parser.make_argument_element(
                lldb.eArgTypeExpression,
                repeat="optional"
            )
        ])

    def __call__(self, debugger, args_array, exe_ctx, result):
        ov_parser = self.get_parser()
        continuation_paths = ov_parser.continuation_expr_arg.split(";")
        count = ov_parser.count_arg
        if count == 0:
            count = 99999
        frame_index = ov_parser.frame_index_arg
        unfiltered = ov_parser.unfiltered_arg

        frame = exe_ctx.GetFrame()
        if not frame.IsValid():
            result.SetError("invalid frame")
            return

        if len(args_array) > 1:
            result.SetError("At most one expression expected")
            return
        elif len(args_array) == 1:
            expr = args_array.GetItemAtIndex(0).GetStringValue(9999)
            coro_hdl = frame.EvaluateExpression(expr)
            if not coro_hdl.error.Success():
                result.AppendMessage(
                    f'error: expression failed {expr} => {coro_hdl.error}'
                )
                result.SetError(f"Expression `{expr}` failed to evaluate")
                return
            _print_async_bt(coro_hdl, result,
                            curr_idx = 0, start = frame_index, limit = frame_index + count,
                            continuation_paths = continuation_paths)
        else:
            _print_combined_bt(frame, result, unfiltered=unfiltered,
                               curr_idx = 0, start = frame_index, limit = frame_index + count,
                               continuation_paths = continuation_paths)


class CoroInflightCommand(ParsedCommand):
    def get_short_help(self):
        return "Identify all in-flight coroutines"

    def get_flags(self):
        return lldb.eCommandRequiresTarget | lldb.eCommandProcessMustBePaused

    def setup_command_definition(self):
        ov_parser = self.get_parser()
        ov_parser.add_option(
            "e",
            "continuation-expr",
            help = (
                "Semi-colon-separated list of expressions evaluated against the promise object"
                "to get the next coroutine (e.g. `.continuation;.coro_parent`)"
            ),
            value_type = lldb.eArgTypeNone,
            dest = "continuation_expr_arg",
            default = ".continuation",
        )
        ov_parser.add_option(
            "c",
            "count",
            help = "How many frames to display (0 for all)",
            value_type = lldb.eArgTypeCount,
            dest = "count_arg",
            default = 5,
        )
        ov_parser.add_argument_set([
            ov_parser.make_argument_element(
                lldb.eArgTypeExpression,
                repeat="plus"
            )
        ])

    def __call__(self, debugger, args_array, exe_ctx, result):
        ov_parser = self.get_parser()
        continuation_paths = ov_parser.continuation_expr_arg.split(";")
        count = ov_parser.count_arg

        # Collect all coroutine_handles from the provided containers
        all_coros = []
        for entry in args_array:
            expr = entry.GetStringValue(9999)
            if exe_ctx.frame.IsValid():
                coro_container = exe_ctx.frame.EvaluateExpression(expr)
            else:
                coro_container = exe_ctx.target.EvaluateExpression(expr)
            if not coro_container.error.Success():
                result.AppendMessage(
                    f'error: expression failed {expr} => {coro_container.error}'
                )
                result.SetError(f"Expression `{expr}` failed to evaluate")
                return
            for entry in coro_container.children:
                if "coroutine_handle" not in entry.GetType().name:
                    result.SetError(f"Found entry of type {entry.GetType().name} in {expr},"
                                    "  expected a coroutine handle")
                    return
                all_coros.append(entry)

        # Remove all coroutines that are currently waiting for other coroutines to finish
        coro_roots = {c.GetChildMemberWithName("coro_frame").GetValueAsAddress(): c for c in all_coros}
        for coro_hdl in all_coros:
            parent_coro = _get_first_var_path(coro_hdl.GetChildMemberWithName("promise"), continuation_paths)
            parent_addr = parent_coro.GetChildMemberWithName("coro_frame").GetValueAsAddress()
            if parent_addr in coro_roots:
                del coro_roots[parent_addr]

        # Print all remaining coroutines
        for addr, root_hdl in coro_roots.items():
            print(f"coroutine root 0x{addr:x}", file=result)
            _print_async_bt(root_hdl, result,
                            curr_idx=0, start=0, limit=count,
                            continuation_paths=continuation_paths, prefix="    ")


def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand("command container add -h 'Debugging utilities for C++20 coroutines' coro")
    debugger.HandleCommand(f"command script add -o -p -c {__name__}.CoroBacktraceCommand coro bt")
    debugger.HandleCommand(f"command script add -o -p -c {__name__}.CoroInflightCommand coro in-flight")
    print("Coro debugging utilities installed. Use `help coro` to see available commands.")

if __name__ == '__main__':
    print("This script should be loaded from LLDB using `command script import <filename>`")