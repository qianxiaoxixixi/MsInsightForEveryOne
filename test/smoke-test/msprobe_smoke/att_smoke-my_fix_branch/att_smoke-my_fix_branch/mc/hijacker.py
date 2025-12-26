import sys
import importlib.abc
from abc import ABC, abstractmethod
import uuid


REPLACE = 0
PRE_HOOK = 1
POST_HOOK = 2

def hijack(*, stub, module, cls=None, function=None, action=REPLACE, priority=100):
    '''
    Hijack module import process or function execution process. 
    Supports attaching pre/post hooks to the process, or replacing function implementations.

    .. target::
        When only set 'module': module
        When set 'module' and 'function': function in module
        When set 'module', 'cls' and 'function': function in class

    .. warning:: 
        The pre-hook of the module import process will only take effect if it is set before the module is imported. 
        If the module is modified in its post-hook, the impact cannot be restored even if the hijacking is released.

    Parameters
    ----------
    stub: Callable object.
        Follow different formats under different target and action.
        ---------------------------------------------------------------------------------------------------------------
        |  target  |  action   |           format             |                   description                         |
        |-------------------------------------------------------------------------------------------------------------|
        | module   | pre-hook  | callable()                   | Called before module import.                          |
        |-------------------------------------------------------------------------------------------------------------|
        | module   | post-hook | callable(m)                  | Called after module import. 'm' is the module.        |
        |-------------------------------------------------------------------------------------------------------------|
        | function | replace   | ret = callable(*args, **kws) | Replace original object.                              |
        |-------------------------------------------------------------------------------------------------------------|
        | function | pre-hook  | args, kws =                  | Called before function execution, and the return will |
        |          |           |     callable(*args, **kws)   | replace original input of the target function.        |
        |-------------------------------------------------------------------------------------------------------------|
        | function | post-hook | ret = callable(ret, *args    | Called after function execution, and the return wil   |
        |          |           |                **kws)        | replace original return of function.                  |
        ---------------------------------------------------------------------------------------------------------------
    module: str
        Fullname of target module.
    cls: str, optional
        Fullname of target class.
    function: str, optional
        Name of target function.
    action: enum, optional
        Choose between REPLACE, PRE_HOOK, and POST_HOOK.
    priority: int, optional
        The smaller the value, the higher the priority. When multiple hooks are set on the same target, they will be 
        executed by priority.

    Returns
    -------
    handler: 
        Handler to a hijacking.
    '''
    HiJackerManager.initialize()
    return HiJackerManager.add_unit(HiJackerUnit(stub, module, cls, function, action, priority))

def release(handler):
    '''
    Cancel a hijacking. 'handler' is returned by function 'hijack'.
    '''
    HiJackerManager.remove_unit(handler)

class HiJackerUnit:
    def __init__(self, stub, module, cls, function, action, priority):
        self.stub = stub
        self.module = module
        self.cls = cls
        self.function = function
        self.action = action
        self.priority = priority

        self._check_para_valid()

        self.target = f"{module}-{cls}-{function}"

    def _check_para_valid(self):
        if not isinstance(self.module, str):
            raise TypeError("\'module\' should be a str")
        if not self.module:
            raise TypeError("\'module\' is required")
        if self.cls and not isinstance(self.cls, str):
            raise TypeError("\'cls\' should be a str")
        if self.function and not isinstance(self.function, str):
            raise TypeError("\'function\' should be a str")
        if not self.action in (REPLACE, PRE_HOOK, POST_HOOK):
            raise TypeError("\'action\' should be REPLACE, PRE_HOOK, or POST_HOOK")
        if not isinstance(self.priority, int):
            raise TypeError("\'priority\' should be an int")
        if self.cls and not self.function:
            raise TypeError("\'function\' should be used when \'cls\' used")

        if not self.cls and not self.function:
            self.target_type = HiJackerManager.TYPE_MODULE
        elif self.cls:
            self.target_type = HiJackerManager.TYPE_METHOD
        else:
            self.target_type = HiJackerManager.TYPE_FUNCTION

        if self.target_type == HiJackerManager.TYPE_MODULE and self.action == REPLACE:
            raise NotImplementedError("replacement modules are not supported")
        else:
            if not callable(self.stub):
                raise TypeError("\'stub\' should be callable")

class HiJackerWrapperObj(ABC):
    def __init__(self, name):
        self.name = name
        self.ori_obj = None
        self.replacement = []
        self.pre_hook = []
        self.post_hook = []
        self.mod_name, self.class_name, self.func_name = name.split('-')

    def add_unit(self, unit):
        if unit.action == REPLACE:
            self.replacement.append(unit)
            self.replacement.sort(key=lambda x : x.priority)
        elif unit.action == PRE_HOOK:
            self.pre_hook.append(unit)
            self.pre_hook.sort(key=lambda x : x.priority)
        else:
            self.post_hook.append(unit)
            self.post_hook.sort(key=lambda x : x.priority)

    def remove_unit(self, unit):
        if unit.action == REPLACE:
            self.replacement.remove(unit)
        elif unit.action == PRE_HOOK and unit in self.pre_hook:
            self.pre_hook.remove(unit)
        elif unit.action == POST_HOOK and unit in self.post_hook:
            self.post_hook.remove(unit)

    def is_empty(self):
        return not self.replacement and not self.pre_hook and not self.post_hook

    def set_ori_obj(self, obj):
        self.ori_obj = obj

    @abstractmethod
    def activate(self):
        pass

    @abstractmethod
    def deactivate(self):
        pass

class HiJackerWrapperModule(HiJackerWrapperObj):
    def __init__(self, name):
        HiJackerWrapperObj.__init__(self, name)

    def exec_pre_hook(self):
        for unit in self.pre_hook:
            unit.stub()

    def exec_post_hook(self, m):
        self.set_ori_obj(m)
        for unit in self.post_hook:
            unit.stub(m)
        return

    def activate(self):
        HijackerPathFinder.add_mod(self.mod_name)
        mod = sys.modules.get(self.mod_name)
        if mod:
            self.exec_post_hook(mod)

    def deactivate(self):
        HijackerPathFinder.remove_mod(self.mod_name)

class HiJackerWrapperFunction(HiJackerWrapperObj):
    def __init__(self, name):
        HiJackerWrapperObj.__init__(self, name)
        self.mod_hijacker = None
    
    def activate(self):
        def replace_closure(name, wrapper):
            def replacement(m):
                if not hasattr(m, name):
                    return None
                ori_obj = getattr(m, name)
                self.set_ori_obj(ori_obj)
                setattr(m, name, wrapper)
                return
            return replacement

        self.mod_hijacker = hijack(stub=replace_closure(self.func_name, self._get_wrapper()),
                                   module=self.mod_name,
                                   action=POST_HOOK,
                                   priority=0)
        return

    def deactivate(self):
        if self.mod_hijacker:
            release(self.mod_hijacker)
            self.mod_hijacker = None
        mod = sys.modules.get(self.mod_name)
        if mod and hasattr(mod, self.func_name) and self.ori_obj:
            setattr(mod, self.func_name, self.ori_obj)
        self.ori_obj = None

    def _get_wrapper(self):
        def wrapper(*args, **kws):
            if not self.ori_obj:
                raise RuntimeError
            for unit in self.pre_hook:
                args, kws = unit.stub(*args, **kws)

            f = self.ori_obj
            if self.replacement:
                f = self.replacement[0].stub
            ret = f(*args, **kws)

            for unit in self.post_hook:
                ret = unit.stub(ret, *args, **kws)

            return ret
        return wrapper

class HiJackerWrapperMethod(HiJackerWrapperObj):
    def __init__(self, name):
        HiJackerWrapperObj.__init__(self, name)
        self.mod_hijacker = None

    def activate(self):
        def replace_closure(class_name, func_name, wrapper):
            def replacement(m):
                class_obj = m
                class_chain = class_name.split('.')
                for c in class_chain:
                    if not hasattr(class_obj, c):
                        return None
                    class_obj = getattr(class_obj, c)
                if class_obj and hasattr(class_obj, func_name):
                    ori_obj = getattr(class_obj, func_name)
                    self.set_ori_obj(ori_obj)
                    setattr(class_obj, func_name, wrapper)
                return
            return replacement

        self.mod_hijacker = hijack(stub=replace_closure(self.class_name, self.func_name, self._get_wrapper()),
                                    module=self.mod_name,
                                    action=POST_HOOK,
                                    priority=0)
        return

    def deactivate(self):
        if self.mod_hijacker:
            release(self.mod_hijacker)
            self.mod_hijacker = None
        mod = sys.modules.get(self.mod_name)
        if mod and self.ori_obj:
            class_obj = mod
            class_chain = self.class_name.split('.')
            for c in class_chain:
                if not hasattr(class_obj, c):
                    self.ori_obj = None
                    return
                class_obj = getattr(class_obj, c)
            if class_obj and hasattr(class_obj, self.func_name):
                setattr(class_obj, self.func_name, self.ori_obj)
        self.ori_obj = None

    def _get_wrapper(self):
        def wrapper(this, *args, **kws):
            if not self.ori_obj:
                raise RuntimeError
            for unit in self.pre_hook:
                args, kws = unit.stub(this, *args, **kws)
            f = self.ori_obj
            if self.replacement:
                f = self.replacement[0].stub
            ret = f(this, *args, **kws)
            for unit in self.post_hook:
                ret = unit.stub(this, ret, *args, **kws)
            return ret
        return wrapper

class HiJackerManager:
    TYPE_MODULE = 0
    TYPE_FUNCTION = 1
    TYPE_METHOD = 2

    _initialized = False
    _hijacker_units = {}
    _hijacker_wrappers = {}

    @classmethod
    def initialize(cls):
        if cls._initialized:
            return
        sys.meta_path.insert(0, HijackerPathFinder())

    @classmethod
    def add_unit(cls, unit):
        handler = uuid.uuid4().hex
        cls._hijacker_units[handler] = unit
        wrapper_obj = cls._hijacker_wrappers.get(unit.target)
        if not wrapper_obj:
            wrapper_obj = cls._build_wrapper_obj(unit.target)
            cls._hijacker_wrappers[unit.target] = wrapper_obj
            wrapper_obj.activate()
        wrapper_obj.add_unit(unit)
        return handler

    @classmethod
    def remove_unit(cls, handler):
        unit = cls._hijacker_units.get(handler)
        if not unit:
            return
        wrapper_obj = cls._hijacker_wrappers.get(unit.target)
        wrapper_obj.remove_unit(unit)
        if wrapper_obj.is_empty():
            wrapper_obj.deactivate()
            del cls._hijacker_wrappers[unit.target]
        del cls._hijacker_units[handler]

    @classmethod
    def get_module_wrapper(cls, name):
        return cls._hijacker_wrappers.get(f'{name}-None-None')

    @classmethod
    def _build_wrapper_obj(cls, name):
        m, c, f = name.split('-')
        wrapper_type = None
        if c != 'None':
            wrapper_type = HiJackerWrapperMethod
        elif f != 'None':
            wrapper_type = HiJackerWrapperFunction
        else:
            wrapper_type = HiJackerWrapperModule
        return wrapper_type(name)
            

class HijackerPathFinder(importlib.abc.MetaPathFinder):
    _modules_of_interest = set()

    @classmethod
    def add_mod(cls, name):
        cls._modules_of_interest.add(name)
    
    @classmethod
    def remove_mod(cls, name):
        cls._modules_of_interest.discard(name)

    def find_spec(self, fullname, path, target=None):
        if not fullname in self._modules_of_interest:
            return None
        for finder in sys.meta_path:
            if isinstance(finder, HijackerPathFinder):
                continue
            spec = finder.find_spec(fullname, path, target)
            if spec == None:
                continue
            return importlib.util.spec_from_loader(fullname, HijackerLoader(spec))
        return None

    def find_module(self, fullname, path=None):
        if not fullname in self._modules_of_interest:
            return None
        for finder in sys.meta_path:
            if isinstance(finder, HijackerPathFinder):
                continue
            loader = finder.find_module(fullname, path)
            if loader == None:
                continue
            return HijackerLoader(importlib.util.spec_from_loader(fullname, loader))
        return None   

class HijackerLoader(importlib.abc.Loader):
    def __init__(self, ori_spec):
        self.ori_spec = ori_spec

    def create_module(self, spec):
        module = importlib.util.module_from_spec(self.ori_spec)
        return module

    def load_module(self, fullname):
        module = self.ori_spec.loader.load_module(fullname)
        return module

    def exec_module(self, module):
        wrapper = HiJackerManager.get_module_wrapper(module.__name__)
        if wrapper:
            wrapper.exec_pre_hook()
        self.ori_spec.loader.exec_module(module)
        if wrapper:
            wrapper.exec_post_hook(module)

