/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9 at Tue Mar 20 14:40:17 2018. */

#include "SearchActionByName.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t SearchActionByNameReq_fields[2] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, SearchActionByNameReq, name, name, 0),
    PB_LAST_FIELD
};

const pb_field_t SearchActionByNameResp_fields[6] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, SearchActionByNameResp, action_id, action_id, 0),
    PB_FIELD(  2, STRING  , REQUIRED, STATIC  , OTHER, SearchActionByNameResp, action_name, action_id, 0),
    PB_FIELD(  3, INT64   , REQUIRED, STATIC  , OTHER, SearchActionByNameResp, execute_order, action_name, 0),
    PB_FIELD(  4, INT64   , REQUIRED, STATIC  , OTHER, SearchActionByNameResp, execute_time, execute_order, 0),
    PB_FIELD(  5, STRING  , REQUIRED, STATIC  , OTHER, SearchActionByNameResp, arguments, execute_time, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
