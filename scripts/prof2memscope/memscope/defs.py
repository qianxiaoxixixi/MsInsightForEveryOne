class Event:
    ALLOC = "MALLOC"
    FREE = "FREE"
    OP_LAUNCH = "OP_LAUNCH"


class EventType:
    PTA = "PTA"


class DumpEventFieldDefs:
    ID = "ID"
    EVENT = "Event"
    EVENT_TYPE = "Event Type"
    NAME = "Name"
    TIMESTAMP_NS = "Timestamp(ns)"
    PID = "Process Id"
    TID = "Thread Id"
    DEVICE_ID = "Device Id"
    PTR = "Ptr"
    ATTR = "Attr"


class PythonTraceEventFieldDefs:
    FUNC_INFO = "FuncInfo"
    START_TIME_NS = "StartTime(ns)"
    END_TIME_NS = "EndTime(ns)"
    TID = "Thread Id"
    PID = "Process Id"
