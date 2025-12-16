import json
from dataclasses import dataclass, asdict
from torch_npu.profiler.analysis.prof_parse._event_tree_parser import (
    _EventType,
    _ProfilerEvent,
    _ExtraFields_Allocation,
    _ExtraFields_PyCall,
)
from .defs import DumpEventFieldDefs, PythonTraceEventFieldDefs, Event, EventType


@dataclass
class MallocFreeEventAttr:
    addr: str
    size: str
    used: str
    total: str
    stream_ptr: str = "N/A"
    owner: str = "PTA"


class MemoryEvent:
    event: str
    event_type: str
    name: str
    timestamp: int
    pid: int
    tid: int
    did: int
    ptr: str
    attr: dict

    def __init__(self, profiler_event: _ProfilerEvent):
        self._init_from_alloc_events(profiler_event)

    def _init_from_alloc_events(self, profiler_event: _ProfilerEvent):
        extra: _ExtraFields_Allocation = profiler_event.extra_fields
        alloc_size = extra.alloc_size
        self.event = Event.ALLOC if alloc_size >= 0 else Event.FREE
        self.event_type = EventType.PTA
        self.name = "N/A"
        self.timestamp = profiler_event.start_time_ns
        self.tid = extra.tid
        self.pid = extra.pid
        self.did = extra.device_index
        self.ptr = hex(extra.ptr)
        self.attr = asdict(MallocFreeEventAttr(self.ptr, str(extra.alloc_size), str(extra.total_allocated),
                                               str(extra.total_reserved), str(extra.stream_ptr)))

    def _init_from_op_events(self, profiler_event: _ProfilerEvent):
        """
            当前在insight内存详情页签中未使用, 暂不做具体实现
        :param profiler_event:
        """
        ...

    def to_dict(self):
        return {
            DumpEventFieldDefs.EVENT: self.event,
            DumpEventFieldDefs.EVENT_TYPE: self.event_type,
            DumpEventFieldDefs.NAME: self.name,
            DumpEventFieldDefs.TIMESTAMP_NS: self.timestamp,
            DumpEventFieldDefs.PID: self.pid,
            DumpEventFieldDefs.TID: self.tid,
            DumpEventFieldDefs.DEVICE_ID: self.did,
            DumpEventFieldDefs.PTR: self.ptr,
            DumpEventFieldDefs.ATTR: json.dumps(self.attr)
        }


class PythonTraceEvent:
    func_info: str
    start_time_ns: int
    end_time_ns: int
    tid: int
    pid: int

    def __init__(self, profiler_event: _ProfilerEvent):
        extra: _ExtraFields_PyCall = profiler_event.extra_fields
        self.func_info = extra.name
        self.start_time_ns = profiler_event.start_time_ns
        self.end_time_ns = extra.end_time_ns
        self.pid = extra.pid
        self.tid = extra.tid

    def to_dict(self):
        return {
            PythonTraceEventFieldDefs.FUNC_INFO: self.func_info,
            PythonTraceEventFieldDefs.START_TIME_NS: self.start_time_ns,
            PythonTraceEventFieldDefs.END_TIME_NS: self.end_time_ns,
            PythonTraceEventFieldDefs.TID: self.tid,
            PythonTraceEventFieldDefs.PID: self.pid
        }
