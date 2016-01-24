#include <string.h>
#include "lib/mlrutil.h"
#include "containers/mlr_dsl_ast.h"

// ----------------------------------------------------------------
mlr_dsl_ast_t* mlr_dsl_ast_alloc() {
	mlr_dsl_ast_t* past = mlr_malloc_or_die(sizeof(mlr_dsl_ast_t));
	past->pbegin_statements = sllv_alloc();
	past->pmain_statements  = sllv_alloc();
	past->pend_statements   = sllv_alloc();
	return past;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_alloc(char* text, int type) {
	mlr_dsl_ast_node_t* pnode = (mlr_dsl_ast_node_t*)mlr_malloc_or_die(
		sizeof(mlr_dsl_ast_node_t));
	pnode->text = mlr_strdup_or_die(text);
	pnode->type = type;
	pnode->pchildren = NULL;
	return pnode;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_copy(mlr_dsl_ast_node_t* pother) {
	mlr_dsl_ast_node_t* pnode = mlr_dsl_ast_node_alloc(pother->text, pother->type);
	return pnode;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_alloc_zary(char* text, int type)
{
	mlr_dsl_ast_node_t* pnode = mlr_dsl_ast_node_alloc(text, type);
	pnode->pchildren = sllv_alloc();
	return pnode;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_alloc_unary(char* text, int type,
	mlr_dsl_ast_node_t* pa)
{
	mlr_dsl_ast_node_t* pnode = mlr_dsl_ast_node_alloc(text, type);
	pnode->pchildren = sllv_alloc();
	sllv_add(pnode->pchildren, pa);
	return pnode;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_alloc_binary(char* text, int type,
	mlr_dsl_ast_node_t* pa, mlr_dsl_ast_node_t* pb)
{
	mlr_dsl_ast_node_t* pnode = mlr_dsl_ast_node_alloc(text, type);
	pnode->pchildren = sllv_alloc();
	sllv_add(pnode->pchildren, pa);
	sllv_add(pnode->pchildren, pb);
	return pnode;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_alloc_ternary(char* text, int type,
	mlr_dsl_ast_node_t* pa, mlr_dsl_ast_node_t* pb, mlr_dsl_ast_node_t* pc)
{
	mlr_dsl_ast_node_t* pnode = mlr_dsl_ast_node_alloc(text, type);
	pnode->pchildren = sllv_alloc();
	sllv_add(pnode->pchildren, pa);
	sllv_add(pnode->pchildren, pb);
	sllv_add(pnode->pchildren, pc);
	return pnode;
}

// ----------------------------------------------------------------
mlr_dsl_ast_node_t* mlr_dsl_ast_node_append_arg(
	mlr_dsl_ast_node_t* pa, mlr_dsl_ast_node_t* pb)
{
	if (pa->pchildren == NULL)
		pa->pchildren = sllv_alloc();
	sllv_add(pa->pchildren, pb);
	return pa;
}

mlr_dsl_ast_node_t* mlr_dsl_ast_node_set_function_name(
	mlr_dsl_ast_node_t* pa, char* name)
{
	free(pa->text);
	pa->text = mlr_strdup_or_die(name);
	return pa;
}

// ----------------------------------------------------------------
void mlr_dsl_ast_print(mlr_dsl_ast_t* past) {
	//printf("AST BEGIN STATEMENTS (%d):\n", past->pbegin_statements->length);
	for (sllve_t* pe = past->pbegin_statements->phead; pe != NULL; pe = pe->pnext)
		mlr_dsl_ast_node_print(pe->pvvalue);

	//printf("AST MAIN  STATEMENTS (%d):\n", past->pmain_statements->length);
	for (sllve_t* pe = past->pmain_statements->phead; pe != NULL; pe = pe->pnext)
		mlr_dsl_ast_node_print(pe->pvvalue);

	//xxx printf("AST END   STATEMENTS (%d):\n", past->pend_statements->length);
	for (sllve_t* pe = past->pend_statements->phead; pe != NULL; pe = pe->pnext)
		mlr_dsl_ast_node_print(pe->pvvalue);
}

// ----------------------------------------------------------------
static void mlr_dsl_ast_node_print_aux(mlr_dsl_ast_node_t* pnode, int level) {
	if (pnode == NULL)
		return;
	for (int i = 0; i < level; i++)
		printf("    ");
	printf("%s (%s)%s\n",
		pnode->text,
		mlr_dsl_ast_node_describe_type(pnode->type),
		(pnode->pchildren != NULL) ? ":" : ".");
	if (pnode->pchildren != NULL) {
		for (sllve_t* pe = pnode->pchildren->phead; pe != NULL; pe = pe->pnext) {
			mlr_dsl_ast_node_print_aux(pe->pvvalue, level + 1);
		}
	}
}

void mlr_dsl_ast_node_print(mlr_dsl_ast_node_t* pnode) {
	mlr_dsl_ast_node_print_aux(pnode, 0);
}

// ----------------------------------------------------------------
char* mlr_dsl_ast_node_describe_type(int type) {
	switch(type) {
	case MLR_DSL_AST_NODE_TYPE_STRNUM_LITERAL:    return "strnum_literal";    break;
	case MLR_DSL_AST_NODE_TYPE_BOOLEAN_LITERAL:   return "boolean_literal";   break;
	case MLR_DSL_AST_NODE_TYPE_REGEXI:            return "regexi";            break;
	case MLR_DSL_AST_NODE_TYPE_FIELD_NAME:        return "field_name";        break;
	case MLR_DSL_AST_NODE_TYPE_OOSVAR_NAME:       return "oosvar_name";       break;
	case MLR_DSL_AST_NODE_TYPE_FUNCTION_NAME:     return "function_name";     break;
	case MLR_DSL_AST_NODE_TYPE_OPERATOR:          return "operator";          break;
	case MLR_DSL_AST_NODE_TYPE_SREC_ASSIGNMENT:   return "srec_assignment";   break;
	case MLR_DSL_AST_NODE_TYPE_OOSVAR_ASSIGNMENT: return "oosvar_assignment"; break;
	case MLR_DSL_AST_NODE_TYPE_CONTEXT_VARIABLE:  return "context_variable";  break;
	case MLR_DSL_AST_NODE_TYPE_STRIPPED_AWAY:     return "stripped_away";     break;
	case MLR_DSL_AST_NODE_TYPE_FILTER:            return "filter";            break;
	case MLR_DSL_AST_NODE_TYPE_GATE:              return "gate";              break;
	case MLR_DSL_AST_NODE_TYPE_EMIT:              return "emit";              break;
	case MLR_DSL_AST_NODE_TYPE_BEGIN:             return "begin";             break;
	case MLR_DSL_AST_NODE_TYPE_END:               return "end";               break;
	default: return "???";
	}
}

// ----------------------------------------------------------------
static void mlr_dsl_ast_free_statement_list(sllv_t* plist) {
	for (sllve_t* pe = plist->phead; pe != NULL; pe = pe->pnext)
		mlr_dsl_ast_node_free(pe->pvvalue);
	sllv_free(plist);
}

void mlr_dsl_ast_free(mlr_dsl_ast_t* past) {
	mlr_dsl_ast_free_statement_list(past->pbegin_statements);
	mlr_dsl_ast_free_statement_list(past->pmain_statements);
	mlr_dsl_ast_free_statement_list(past->pend_statements);
	free(past);
}

// ----------------------------------------------------------------
void mlr_dsl_ast_node_free(mlr_dsl_ast_node_t* pnode) {
	if (pnode->pchildren) {
		for (sllve_t* pe = pnode->pchildren->phead; pe != NULL; pe = pe->pnext) {
			mlr_dsl_ast_node_t* pchild = pe->pvvalue;
			mlr_dsl_ast_node_free(pchild);
		}
		sllv_free(pnode->pchildren);
	}
	free(pnode->text);
	free(pnode);
}
