/*
 * rcalc.x
 */

/* structure definitions, no enumerations needed */
struct list {
		double data;
		struct list *next;
};

/* program definition, no union or typdef definitions needed */
program RCALCPROG { /* could manage multiple servers */
	version RCALCVERS {
		list SUMSQRT_RANGE(list) = 1;
		list UPDATE_LIST(list) = 2;
	} = 1;
} = 0x20009849;  /* program number ranges established by ONC */
