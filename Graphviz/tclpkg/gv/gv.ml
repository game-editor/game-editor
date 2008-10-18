open Swig
type c_enum_type = [ 
  `unknown
]
type c_enum_value = [ 
  `Int of int
]

type c_obj = c_enum_value c_obj_t
let module_name = "gv"

exception BadArgs of string
exception BadMethodName of c_obj * string * string
exception NotObject of c_obj
exception NotEnumType of c_obj
exception LabelNotFromThisEnum of c_obj
exception InvalidDirectorCall of c_obj

external _graph_f : c_obj list -> c_obj list = "_wrap_graph__SWIG_0gv" ;;
let _graph arg = match _graph_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _digraph_f : c_obj list -> c_obj list = "_wrap_digraphgv" ;;
let _digraph arg = match _digraph_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _strictgraph_f : c_obj list -> c_obj list = "_wrap_strictgraphgv" ;;
let _strictgraph arg = match _strictgraph_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _strictdigraph_f : c_obj list -> c_obj list = "_wrap_strictdigraphgv" ;;
let _strictdigraph arg = match _strictdigraph_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _readstring_f : c_obj list -> c_obj list = "_wrap_readstringgv" ;;
let _readstring arg = match _readstring_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _read_f : c_obj list -> c_obj list = "_wrap_read__SWIG_0gv" ;;
let _read arg = match _read_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _read_f : c_obj list -> c_obj list = "_wrap_readgv" ;;
let _read arg = match _read_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _graph_f : c_obj list -> c_obj list = "_wrap_graphgv" ;;
let _graph arg = match _graph_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _node_f : c_obj list -> c_obj list = "_wrap_nodegv" ;;
let _node arg = match _node_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _edge_f : c_obj list -> c_obj list = "_wrap_edge__SWIG_0gv" ;;
let _edge arg = match _edge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _edge_f : c_obj list -> c_obj list = "_wrap_edge__SWIG_1gv" ;;
let _edge arg = match _edge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _edge_f : c_obj list -> c_obj list = "_wrap_edge__SWIG_2gv" ;;
let _edge arg = match _edge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _edge_f : c_obj list -> c_obj list = "_wrap_edgegv" ;;
let _edge arg = match _edge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _setv_f : c_obj list -> c_obj list = "_wrap_setv__SWIG_0gv" ;;
let _setv arg = match _setv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _setv_f : c_obj list -> c_obj list = "_wrap_setv__SWIG_1gv" ;;
let _setv arg = match _setv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _setv_f : c_obj list -> c_obj list = "_wrap_setv__SWIG_2gv" ;;
let _setv arg = match _setv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _setv_f : c_obj list -> c_obj list = "_wrap_setv__SWIG_3gv" ;;
let _setv arg = match _setv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _setv_f : c_obj list -> c_obj list = "_wrap_setv__SWIG_4gv" ;;
let _setv arg = match _setv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _setv_f : c_obj list -> c_obj list = "_wrap_setvgv" ;;
let _setv arg = match _setv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _getv_f : c_obj list -> c_obj list = "_wrap_getv__SWIG_0gv" ;;
let _getv arg = match _getv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _getv_f : c_obj list -> c_obj list = "_wrap_getv__SWIG_1gv" ;;
let _getv arg = match _getv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _getv_f : c_obj list -> c_obj list = "_wrap_getv__SWIG_2gv" ;;
let _getv arg = match _getv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _getv_f : c_obj list -> c_obj list = "_wrap_getv__SWIG_3gv" ;;
let _getv arg = match _getv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _getv_f : c_obj list -> c_obj list = "_wrap_getv__SWIG_4gv" ;;
let _getv arg = match _getv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _getv_f : c_obj list -> c_obj list = "_wrap_getvgv" ;;
let _getv arg = match _getv_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nameof_f : c_obj list -> c_obj list = "_wrap_nameof__SWIG_0gv" ;;
let _nameof arg = match _nameof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nameof_f : c_obj list -> c_obj list = "_wrap_nameof__SWIG_1gv" ;;
let _nameof arg = match _nameof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nameof_f : c_obj list -> c_obj list = "_wrap_nameofgv" ;;
let _nameof arg = match _nameof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _findsubg_f : c_obj list -> c_obj list = "_wrap_findsubggv" ;;
let _findsubg arg = match _findsubg_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _findnode_f : c_obj list -> c_obj list = "_wrap_findnodegv" ;;
let _findnode arg = match _findnode_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _findedge_f : c_obj list -> c_obj list = "_wrap_findedgegv" ;;
let _findedge arg = match _findedge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _findattr_f : c_obj list -> c_obj list = "_wrap_findattr__SWIG_0gv" ;;
let _findattr arg = match _findattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _findattr_f : c_obj list -> c_obj list = "_wrap_findattr__SWIG_1gv" ;;
let _findattr arg = match _findattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _findattr_f : c_obj list -> c_obj list = "_wrap_findattrgv" ;;
let _findattr arg = match _findattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _headof_f : c_obj list -> c_obj list = "_wrap_headofgv" ;;
let _headof arg = match _headof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _tailof_f : c_obj list -> c_obj list = "_wrap_tailofgv" ;;
let _tailof arg = match _tailof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _graphof_f : c_obj list -> c_obj list = "_wrap_graphof__SWIG_0gv" ;;
let _graphof arg = match _graphof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _graphof_f : c_obj list -> c_obj list = "_wrap_graphof__SWIG_1gv" ;;
let _graphof arg = match _graphof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _graphof_f : c_obj list -> c_obj list = "_wrap_graphofgv" ;;
let _graphof arg = match _graphof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _rootof_f : c_obj list -> c_obj list = "_wrap_rootofgv" ;;
let _rootof arg = match _rootof_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _protonode_f : c_obj list -> c_obj list = "_wrap_protonodegv" ;;
let _protonode arg = match _protonode_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _protoedge_f : c_obj list -> c_obj list = "_wrap_protoedgegv" ;;
let _protoedge arg = match _protoedge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _ok_f : c_obj list -> c_obj list = "_wrap_ok__SWIG_0gv" ;;
let _ok arg = match _ok_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _ok_f : c_obj list -> c_obj list = "_wrap_ok__SWIG_1gv" ;;
let _ok arg = match _ok_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _ok_f : c_obj list -> c_obj list = "_wrap_ok__SWIG_2gv" ;;
let _ok arg = match _ok_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _ok_f : c_obj list -> c_obj list = "_wrap_okgv" ;;
let _ok arg = match _ok_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstsubg_f : c_obj list -> c_obj list = "_wrap_firstsubggv" ;;
let _firstsubg arg = match _firstsubg_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextsubg_f : c_obj list -> c_obj list = "_wrap_nextsubggv" ;;
let _nextsubg arg = match _nextsubg_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstsupg_f : c_obj list -> c_obj list = "_wrap_firstsupggv" ;;
let _firstsupg arg = match _firstsupg_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextsupg_f : c_obj list -> c_obj list = "_wrap_nextsupggv" ;;
let _nextsupg arg = match _nextsupg_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstedge_f : c_obj list -> c_obj list = "_wrap_firstedge__SWIG_0gv" ;;
let _firstedge arg = match _firstedge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextedge_f : c_obj list -> c_obj list = "_wrap_nextedge__SWIG_0gv" ;;
let _nextedge arg = match _nextedge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstout_f : c_obj list -> c_obj list = "_wrap_firstout__SWIG_0gv" ;;
let _firstout arg = match _firstout_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextout_f : c_obj list -> c_obj list = "_wrap_nextout__SWIG_0gv" ;;
let _nextout arg = match _nextout_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstedge_f : c_obj list -> c_obj list = "_wrap_firstedgegv" ;;
let _firstedge arg = match _firstedge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextedge_f : c_obj list -> c_obj list = "_wrap_nextedgegv" ;;
let _nextedge arg = match _nextedge_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstout_f : c_obj list -> c_obj list = "_wrap_firstoutgv" ;;
let _firstout arg = match _firstout_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextout_f : c_obj list -> c_obj list = "_wrap_nextoutgv" ;;
let _nextout arg = match _nextout_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firsthead_f : c_obj list -> c_obj list = "_wrap_firstheadgv" ;;
let _firsthead arg = match _firsthead_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nexthead_f : c_obj list -> c_obj list = "_wrap_nextheadgv" ;;
let _nexthead arg = match _nexthead_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstin_f : c_obj list -> c_obj list = "_wrap_firstin__SWIG_0gv" ;;
let _firstin arg = match _firstin_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextin_f : c_obj list -> c_obj list = "_wrap_nextin__SWIG_0gv" ;;
let _nextin arg = match _nextin_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstin_f : c_obj list -> c_obj list = "_wrap_firstingv" ;;
let _firstin arg = match _firstin_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextin_f : c_obj list -> c_obj list = "_wrap_nextingv" ;;
let _nextin arg = match _nextin_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firsttail_f : c_obj list -> c_obj list = "_wrap_firsttailgv" ;;
let _firsttail arg = match _firsttail_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nexttail_f : c_obj list -> c_obj list = "_wrap_nexttailgv" ;;
let _nexttail arg = match _nexttail_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstnode_f : c_obj list -> c_obj list = "_wrap_firstnode__SWIG_0gv" ;;
let _firstnode arg = match _firstnode_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextnode_f : c_obj list -> c_obj list = "_wrap_nextnode__SWIG_0gv" ;;
let _nextnode arg = match _nextnode_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstnode_f : c_obj list -> c_obj list = "_wrap_firstnodegv" ;;
let _firstnode arg = match _firstnode_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextnode_f : c_obj list -> c_obj list = "_wrap_nextnodegv" ;;
let _nextnode arg = match _nextnode_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstattr_f : c_obj list -> c_obj list = "_wrap_firstattr__SWIG_0gv" ;;
let _firstattr arg = match _firstattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextattr_f : c_obj list -> c_obj list = "_wrap_nextattr__SWIG_0gv" ;;
let _nextattr arg = match _nextattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstattr_f : c_obj list -> c_obj list = "_wrap_firstattr__SWIG_1gv" ;;
let _firstattr arg = match _firstattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextattr_f : c_obj list -> c_obj list = "_wrap_nextattr__SWIG_1gv" ;;
let _nextattr arg = match _nextattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _firstattr_f : c_obj list -> c_obj list = "_wrap_firstattrgv" ;;
let _firstattr arg = match _firstattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _nextattr_f : c_obj list -> c_obj list = "_wrap_nextattrgv" ;;
let _nextattr arg = match _nextattr_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _rm_f : c_obj list -> c_obj list = "_wrap_rm__SWIG_0gv" ;;
let _rm arg = match _rm_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _rm_f : c_obj list -> c_obj list = "_wrap_rm__SWIG_1gv" ;;
let _rm arg = match _rm_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _rm_f : c_obj list -> c_obj list = "_wrap_rmgv" ;;
let _rm arg = match _rm_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _layout_f : c_obj list -> c_obj list = "_wrap_layoutgv" ;;
let _layout arg = match _layout_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _render_f : c_obj list -> c_obj list = "_wrap_render__SWIG_0gv" ;;
let _render arg = match _render_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _render_f : c_obj list -> c_obj list = "_wrap_render__SWIG_1gv" ;;
let _render arg = match _render_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _render_f : c_obj list -> c_obj list = "_wrap_render__SWIG_2gv" ;;
let _render arg = match _render_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _render_f : c_obj list -> c_obj list = "_wrap_render__SWIG_3gv" ;;
let _render arg = match _render_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external _render_f : c_obj list -> c_obj list = "_wrap_rendergv" ;;
let _render arg = match _render_f (fnhelper arg) with
  [] -> C_void
| [x] -> (if false then Gc.finalise 
  (fun x -> ignore ((invoke x) "~" C_void)) x) ; x
| lst -> C_list lst ;;
external f_init : unit -> unit = "f_gv_init" ;;
let _ = f_init ()
let enum_to_int x (v : c_obj) =
   match v with
     C_enum _y ->
     (let y = _y in match (x : c_enum_type) with
       `unknown ->          (match y with
           `Int x -> (Swig.C_int x)
           | _ -> raise (LabelNotFromThisEnum v))
) | _ -> (C_int (get_int v))
let _ = Callback.register "gv_enum_to_int" enum_to_int
let int_to_enum x y =
    match (x : c_enum_type) with
      `unknown -> C_enum (`Int y)
let _ = Callback.register "gv_int_to_enum" int_to_enum

  let rec swig_val t v = 
    match v with
        C_enum e -> enum_to_int t v
      | C_list l -> Swig.C_list (List.map (swig_val t) l)
      | C_array a -> Swig.C_array (Array.map (swig_val t) a)
      | _ -> Obj.magic v

