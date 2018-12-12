#ifndef INTERNAL_H
#define INTERNAL_H

#include "rcheevos.h"

#define RC_TAG2(x,y) x ## y
#define RC_TAG(x,y) RC_TAG2(x,y)
#define RC_OFFSETOF(s, f) ((int)(long long)(&((s*)0)->f))
#define RC_ALIGNOF(t) RC_OFFSETOF(struct RC_TAG(_unnamed, __LINE__) {char c; t d;}, d)

#define RC_ALLOC(t, p) ((t*)rc_alloc((p)->buffer, &(p)->offset, sizeof(t), RC_ALIGNOF(t), &(p)->scratch))

typedef union {
  rc_operand_t operand;
  rc_condition_t condition;
  rc_condset_t condset;
  rc_trigger_t trigger;
  rc_term_t term;
  rc_expression_t expression;
  rc_lboard_t lboard;
  rc_richpresence_t richpresence;
  rc_richpresence_display_t richpresence_display;
  rc_richpresence_display_part_t richpresence_part;
  rc_richpresence_lookup_t richpresence_lookup;
  rc_richpresence_lookup_item_t richpresence_lookup_item;
}
rc_scratch_t;

typedef struct {
  int offset;

  lua_State* L;
  int funcs_ndx;

  void* buffer;
  rc_scratch_t scratch;
}
rc_parse_state_t;

void rc_init_parse_state(rc_parse_state_t* parse, void* buffer, lua_State* L, int funcs_ndx);
void rc_destroy_parse_state(rc_parse_state_t* parse);

void* rc_alloc(void* pointer, int* offset, int size, int alignment, rc_scratch_t* scratch);
char* rc_alloc_str(rc_parse_state_t* parse, const char* text, int length);

void rc_parse_trigger_internal(rc_trigger_t* self, const char** memaddr, rc_parse_state_t* parse);

rc_condset_t* rc_parse_condset(const char** memaddr, rc_parse_state_t* parse);
int rc_test_condset(rc_condset_t* self, int* reset, rc_peek_t peek, void* ud, lua_State* L);
void rc_reset_condset(rc_condset_t* self);

rc_condition_t* rc_parse_condition(const char** memaddr, rc_parse_state_t* parse);
int rc_test_condition(rc_condition_t* self, unsigned add_buffer, rc_peek_t peek, void* ud, lua_State* L);

int rc_parse_operand(rc_operand_t* self, const char** memaddr, int is_trigger, rc_parse_state_t* parse);
unsigned rc_evaluate_operand(rc_operand_t* self, rc_peek_t peek, void* ud, lua_State* L);

rc_term_t* rc_parse_term(const char** memaddr, rc_parse_state_t* parse);
unsigned rc_evaluate_term(rc_term_t* self, rc_peek_t peek, void* ud, lua_State* L);

rc_expression_t* rc_parse_expression(const char** memaddr, rc_parse_state_t* parse);
unsigned rc_evaluate_expression(rc_expression_t* self, rc_peek_t peek, void* ud, lua_State* L);

void rc_parse_value_internal(rc_value_t* self, const char** memaddr, rc_parse_state_t* parse);

void rc_parse_lboard_internal(rc_lboard_t* self, const char* memaddr, rc_parse_state_t* parse);

const char* rc_parse_line(const char* line, const char** end);
void rc_parse_richpresence_internal(rc_richpresence_t* self, const char* script, rc_parse_state_t* parse);

#endif /* INTERNAL_H */
