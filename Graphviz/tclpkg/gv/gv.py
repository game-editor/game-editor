# This file was created automatically by SWIG 1.3.26.
# Don't modify this file, modify the SWIG interface instead.

import _gv

# This file is compatible with both classic and new-style classes.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name) or (name == "thisown"):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types



digraph = _gv.digraph

strictgraph = _gv.strictgraph

strictdigraph = _gv.strictdigraph

readstring = _gv.readstring

node = _gv.node

findsubg = _gv.findsubg

findnode = _gv.findnode

findedge = _gv.findedge

headof = _gv.headof

tailof = _gv.tailof

rootof = _gv.rootof

protonode = _gv.protonode

protoedge = _gv.protoedge

firstsubg = _gv.firstsubg

nextsubg = _gv.nextsubg

firstsupg = _gv.firstsupg

nextsupg = _gv.nextsupg

firsthead = _gv.firsthead

nexthead = _gv.nexthead

firsttail = _gv.firsttail

nexttail = _gv.nexttail

layout = _gv.layout


read = _gv.read

graph = _gv.graph

edge = _gv.edge

setv = _gv.setv

getv = _gv.getv

nameof = _gv.nameof

findattr = _gv.findattr

graphof = _gv.graphof

ok = _gv.ok

firstedge = _gv.firstedge

nextedge = _gv.nextedge

firstout = _gv.firstout

nextout = _gv.nextout

firstin = _gv.firstin

nextin = _gv.nextin

firstnode = _gv.firstnode

nextnode = _gv.nextnode

firstattr = _gv.firstattr

nextattr = _gv.nextattr

rm = _gv.rm

render = _gv.render

