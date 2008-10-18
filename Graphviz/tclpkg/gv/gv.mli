open Swig
type c_enum_type = [ 
  `unknown
]
type c_enum_value = [ 
  `Int of int
]

type c_obj = c_enum_value c_obj_t
val module_name : string

exception BadArgs of string
exception BadMethodName of c_obj * string * string
exception NotObject of c_obj
exception NotEnumType of c_obj
exception LabelNotFromThisEnum of c_obj
exception InvalidDirectorCall of c_obj

val _graph : c_obj -> c_obj
val _digraph : c_obj -> c_obj
val _strictgraph : c_obj -> c_obj
val _strictdigraph : c_obj -> c_obj
val _readstring : c_obj -> c_obj
val _read : c_obj -> c_obj
val _read : c_obj -> c_obj
val _graph : c_obj -> c_obj
val _node : c_obj -> c_obj
val _edge : c_obj -> c_obj
val _edge : c_obj -> c_obj
val _edge : c_obj -> c_obj
val _edge : c_obj -> c_obj
val _setv : c_obj -> c_obj
val _setv : c_obj -> c_obj
val _setv : c_obj -> c_obj
val _setv : c_obj -> c_obj
val _setv : c_obj -> c_obj
val _setv : c_obj -> c_obj
val _getv : c_obj -> c_obj
val _getv : c_obj -> c_obj
val _getv : c_obj -> c_obj
val _getv : c_obj -> c_obj
val _getv : c_obj -> c_obj
val _getv : c_obj -> c_obj
val _nameof : c_obj -> c_obj
val _nameof : c_obj -> c_obj
val _nameof : c_obj -> c_obj
val _findsubg : c_obj -> c_obj
val _findnode : c_obj -> c_obj
val _findedge : c_obj -> c_obj
val _findattr : c_obj -> c_obj
val _findattr : c_obj -> c_obj
val _findattr : c_obj -> c_obj
val _headof : c_obj -> c_obj
val _tailof : c_obj -> c_obj
val _graphof : c_obj -> c_obj
val _graphof : c_obj -> c_obj
val _graphof : c_obj -> c_obj
val _rootof : c_obj -> c_obj
val _protonode : c_obj -> c_obj
val _protoedge : c_obj -> c_obj
val _ok : c_obj -> c_obj
val _ok : c_obj -> c_obj
val _ok : c_obj -> c_obj
val _ok : c_obj -> c_obj
val _firstsubg : c_obj -> c_obj
val _nextsubg : c_obj -> c_obj
val _firstsupg : c_obj -> c_obj
val _nextsupg : c_obj -> c_obj
val _firstedge : c_obj -> c_obj
val _nextedge : c_obj -> c_obj
val _firstout : c_obj -> c_obj
val _nextout : c_obj -> c_obj
val _firstedge : c_obj -> c_obj
val _nextedge : c_obj -> c_obj
val _firstout : c_obj -> c_obj
val _nextout : c_obj -> c_obj
val _firsthead : c_obj -> c_obj
val _nexthead : c_obj -> c_obj
val _firstin : c_obj -> c_obj
val _nextin : c_obj -> c_obj
val _firstin : c_obj -> c_obj
val _nextin : c_obj -> c_obj
val _firsttail : c_obj -> c_obj
val _nexttail : c_obj -> c_obj
val _firstnode : c_obj -> c_obj
val _nextnode : c_obj -> c_obj
val _firstnode : c_obj -> c_obj
val _nextnode : c_obj -> c_obj
val _firstattr : c_obj -> c_obj
val _nextattr : c_obj -> c_obj
val _firstattr : c_obj -> c_obj
val _nextattr : c_obj -> c_obj
val _firstattr : c_obj -> c_obj
val _nextattr : c_obj -> c_obj
val _rm : c_obj -> c_obj
val _rm : c_obj -> c_obj
val _rm : c_obj -> c_obj
val _layout : c_obj -> c_obj
val _render : c_obj -> c_obj
val _render : c_obj -> c_obj
val _render : c_obj -> c_obj
val _render : c_obj -> c_obj
val _render : c_obj -> c_obj
val enum_to_int : c_enum_type -> c_obj -> Swig.c_obj
val int_to_enum : c_enum_type -> int -> c_obj

  val swig_val : c_enum_type -> c_obj -> Swig.c_obj

