from torch_npu.profiler.analysis.prof_parse._event_tree_parser import (
    _ExtraFields_PyCall,
    _ExtraFields_Allocation,
    MemoryUseBean,
    PyTraceEvent
)


def init_patch():
    patch_extra_fields_allocation_init()
    patch_extra_fields_pycall_init()


def patch_extra_fields_allocation_init():
    _origin_extra_fields_allocation_init_func = _ExtraFields_Allocation.__init__

    # 无侵入式修改的为profiler解析事件时初始化的_ExtraFields_Allocation扩展字段对象添加stream_ptr, pid和tid字段属性
    def _custom_extra_fields_allocation_init_func(self, bean: MemoryUseBean):
        _origin_extra_fields_allocation_init_func(self, bean)
        self.stream_ptr = bean.stream_ptr
        self.pid = bean.pid
        self.tid = bean.tid

    _ExtraFields_Allocation.__init__ = _custom_extra_fields_allocation_init_func


def patch_extra_fields_pycall_init():
    _origin_extra_fields_pycall_init_func = _ExtraFields_PyCall.__init__

    # 无侵入式修改的为profiler解析事件时初始化的_ExtraFields_PyCall扩展字段对象添加pid和tid字段属性
    def _custom_extra_fields_pycall_init_func(self, bean: PyTraceEvent):
        _origin_extra_fields_pycall_init_func(self, bean)
        self.pid = bean.pid
        self.tid = bean.tid

    _ExtraFields_PyCall.__init__ = _custom_extra_fields_pycall_init_func
