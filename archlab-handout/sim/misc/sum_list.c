typedef struct ELE {
    long val;
    struct ELE* next;
} *list_ptr;
long sum_list(list_ptr ls)
{
    long val = 0;
    while (ls) {
	val += ls->val;
	ls = ls->next;
    }
    return val;
}