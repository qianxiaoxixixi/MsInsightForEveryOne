from typing import List

from utils import SqliteColumn, SqliteTable, SqliteDB
from .defs import DumpEventFieldDefs, PythonTraceEventFieldDefs

_MEMSCOPE_DUMP_TABLE_COLUMNS: List[SqliteColumn] = [
    SqliteColumn(name=DumpEventFieldDefs.ID, data_type=int, primary_key=True, autoincrement=True),
    SqliteColumn(DumpEventFieldDefs.EVENT),
    SqliteColumn(DumpEventFieldDefs.EVENT_TYPE),
    SqliteColumn(DumpEventFieldDefs.NAME),
    SqliteColumn(DumpEventFieldDefs.TIMESTAMP_NS, data_type=int),
    SqliteColumn(DumpEventFieldDefs.PID, data_type=int),
    SqliteColumn(DumpEventFieldDefs.TID, data_type=int),
    SqliteColumn(DumpEventFieldDefs.DEVICE_ID),
    SqliteColumn(DumpEventFieldDefs.PTR),
    SqliteColumn(DumpEventFieldDefs.ATTR)
]

_PYTHON_TRACE_TABLE_COLUMNS: List[SqliteColumn] = [
    SqliteColumn(PythonTraceEventFieldDefs.FUNC_INFO),
    SqliteColumn(PythonTraceEventFieldDefs.START_TIME_NS),
    SqliteColumn(PythonTraceEventFieldDefs.END_TIME_NS),
    SqliteColumn(PythonTraceEventFieldDefs.TID, data_type=int),
    SqliteColumn(PythonTraceEventFieldDefs.PID, data_type=int)
]


class MemScopeDb(SqliteDB):
    DUMP_TABLE_NAME = "leaks_dump"
    PYTHON_TRACE_TABLE_NAME_PREFIX = "python_trace"

    def __init__(self, path: str):
        super().__init__(path, auto_create=True)
        self.create_dump_table()

    def create_dump_table(self):
        self.create_table(SqliteTable(MemScopeDb.DUMP_TABLE_NAME, _MEMSCOPE_DUMP_TABLE_COLUMNS),
                          delete_if_exists=True)

    def create_python_trace_table(self, pid: int):
        self.create_table(SqliteTable(MemScopeDb.get_python_trace_table_name(pid), _PYTHON_TRACE_TABLE_COLUMNS),
                          delete_if_exists=True)

    def get_dump_table(self):
        return self.get_table_by_name(MemScopeDb.DUMP_TABLE_NAME)

    def get_python_trace_table(self, pid: int):
        python_trace_table_name = MemScopeDb.get_python_trace_table_name(pid)
        if not self.is_table_exists(python_trace_table_name):
            self.create_python_trace_table(pid)
        return self.get_table_by_name(python_trace_table_name)

    @classmethod
    def get_python_trace_table_name(cls, pid: int):
        return f"{cls.PYTHON_TRACE_TABLE_NAME_PREFIX}_{pid}"
