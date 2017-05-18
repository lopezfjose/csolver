#include "includes\Main.h"


/** Determines whether number is negative, positive, or zero */
void testNumber(struct _number *n) {
	// Determine whether number is rational or decimal
	if (n->status == RATIONAL) {
		if (n->type.rat.n < 0) {
			n->absval = -1;
		} else if (n->type.rat.n == 0) {
			n->absval =  0;
		} else {
			n->absval =  1;
		}
	} else if (n->status == DECIMAL) {
		// Number is decimal
		if (n->type.dec < 0) {
			n->absval = -1;
		} else if (n->type.dec == 0) {
			n->absval =  0;
		} else {
			n->absval =  1;
		}
	} else {
		// Invalid status
		
		#if DEBUG
			printf("[Error] Invalid status for number!");
		#endif
	}
}

/** Generic function to print number structure data type. Function determines whether number is rational or decimal and takes action accordingly. */

void printNumber(struct _number *n) {
	if (n->status == DECIMAL) {
		printf("%.4f", n->type.dec);
	} else if (n->status == RATIONAL) {
		printRationalNumber(&(n->type.rat));
	} else {
		#if DEBUG
			printf("[Error] Number has no status!\n");
		#endif
	}
}


/** Creating number structure on the stack */

struct _number newNumber(int status, int type, int a, int b, double c) {
	struct _number newNumber;
	
	if (status == RATIONAL) {
		// Create rational
		newNumber.status = RATIONAL;
		
		newNumber.type.rat.n = a;
		newNumber.type.rat.d = b;
	} else {
		// Create decimal
		newNumber.status = DECIMAL;
		
		newNumber.type.dec = c;
	}
	
	return newNumber;
}


/** Dynamic memory allocation of number structure on the heap */
/*
struct _number* createNumber(int status, int type, int a, int b, double c) {
	struct _number *newNumber = malloc(sizeof(struct _number));
	
	if (status == RATIONAL) {
		// Create rational
		newNumber->status = RATIONAL;
		
		newNumber->type.rat.n = a;
		newNumber->type.rat.d = b;
	} else {
		// Create decimal
		newNumber->status = DECIMAL;
		
		newNumber->type.dec = c;
	}
	
	return newNumber;
}
*/


/** Function that returns zero struct used during matrix creation
 *  Notes: Still haven't decided if the structs should be stack or heap-allocated
 *  (implemented on stack for now)
 *
 *  Struct type is rational
 */

struct _number createZeroNumberStruct() {
	struct _number newNumber;
	
	newNumber.status = RATIONAL;
	
	newNumber.type.rat.n = 0;
	newNumber.type.rat.d = 1;
	
	return newNumber;
}

/** Converts passed in integer to number structure - assumes whole numbers with no fractional parts */

struct _number convertToNumber(int a) {
	struct _number newNumber;
	
	newNumber.status = RATIONAL;
	
	newNumber.type.rat.n = a;
	newNumber.type.rat.d = 1;
	
	return newNumber;
}

/** Function to convert rational numbers to decimal form */
void convertToDecimal(struct _number *n) {
	// Verify that the number is not already a decimal
	if (n->status == RATIONAL) {
		n->status = DECIMAL;
		
		double a, b;
		
		a = n->type.rat.n;
		b = n->type.rat.d;
		
		n->type.dec = a / b;
	} else {
		// Do nothing; number is already in correct format
		
		#if DEBUG
			printf("[Error] Number is already in the specified format.\n");
		#endif
	}
}

/** Function to convert decimal numbers to rational form */
void convertToRational(struct _number *n) {
	// Verify that the number is not already a fraction
	if (n->status == DECIMAL) {
		// Attempt to convert decimal to rational
		
		// Check for zero first
		if (n->type.dec == 0) {
			n->status = RATIONAL;
			
			n->type.rat.n = 0;
			n->type.rat.d = 1;
		} else {
			// Find out if value is negative
			testNumber(n);
			
			if (n->absval == -1) {
				// Negative
				
				for (int i = -1; i >= -RATIONAL_CONVERSION_PRECISION_LIMIT; --i) {
					for (int j = 1; j <= RATIONAL_CONVERSION_PRECISION_LIMIT; ++j) {
						printf("%i / %i = %f <--> %f\n", i, j, (double) i / (double) j, n->type.dec);
						
						if (((double) i / (double) j) == n->type.dec) {
							n->status = RATIONAL;
							
							n->type.rat.n = i;
							n->type.rat.d = j;
						} else {
							printf("%f == %f --> ", (double) i / (double) j, n->type.dec);
							if (((double) i / (double) j) == n->type.dec)
								printf("TRUE\n");
							else
								printf("FALSE\n");
						}
					}
				}
			} else if (n->absval == 0) {
				// Zero
				
				n->status = RATIONAL;
				
				n->type.rat.n = 0;
				n->type.rat.d = 1;
			} else if (n->absval == 1) {
				// Number is positive
				
				for (int i = 1; i <= RATIONAL_CONVERSION_PRECISION_LIMIT; ++i) {
					for (int j = 1; j <= RATIONAL_CONVERSION_PRECISION_LIMIT; ++j) {
						if (((double) i/ (double) j) == n->type.dec) {
							n->status = RATIONAL;
							
							n->type.rat.n = i;
							n->type.rat.d = j;
						}
					}
				}
			} else {
				// Error; Invalid absval for number
				#if DEBUG
					printf("[Error] Invalid absval for number...\n");
				#endif
			}
			
		}
	} else {
		// Do nothing; number is already in correct format
		
		#if DEBUG
			printf("[Error] Number is already in the specified format.\n");
		#endif
	}
}


/** Add Number */

struct _number addNumber(struct _number *a, struct _number *b) {
	struct _number result;
	
	/** Check to see if both numbers are decimal or rational
	 *  Convert the rational number to decimal, perform operation, then convert back if possible.
	 *  If not, leave in decimal form.
	 */
	
	if (a->status == b->status) {
		if (a->status == DECIMAL) {
			// Perform float arithmetic
			result.status = DECIMAL;
			
			result.type.dec = a->type.dec + b->type.dec;
		} else {
			// Perform rational arithmetic
			if (a->type.rat.d != b->type.rat.d) {
				result.status = RATIONAL;
				
				result.type.rat.n = (a->type.rat.n * b->type.rat.d) + (b->type.rat.n * a->type.rat.d);
				result.type.rat.d = a->type.rat.d * b->type.rat.d;
			} else {
				result.status = RATIONAL;
				
				result.type.rat.n = a->type.rat.n + b->type.rat.n;
				result.type.rat.d = a->type.rat.d;
			}
		}
	} else {
		if (a->status == RATIONAL) {
			result.status = DECIMAL;
			
			convertToDecimal(a);
			result.type.dec = a->type.dec + b->type.dec;
			convertToRational(a);
		} else {
			result.status = DECIMAL;
			
			convertToDecimal(b);
			result.type.dec = a->type.dec + b->type.dec;
			convertToRational(a);
		}
	}
	
	return result;
}


/** Subtract Number */

struct _number subNumber(struct _number *a, struct _number *b) {
	struct _number result;
	
	// Verify they are the same type
	if (a->status == b->status) {
		if (a->status == DECIMAL) {
			// Do decimal arithmetic
			result.status = DECIMAL;
			
			result.type.dec = a->type.dec - b->type.dec;
		} else if (a->status == RATIONAL) {
			// Do rational arithmetic
			result.status = RATIONAL;
			
			if (a->type.rat.d != b->type.rat.d) {
				result.status = RATIONAL;
				
				result.type.rat.n = (a->type.rat.n * b->type.rat.d) - (b->type.rat.n * a->type.rat.d);
				result.type.rat.d = a->type.rat.d * b->type.rat.d;
			} else {
				result.status = RATIONAL;
				
				result.type.rat.n = a->type.rat.n - b->type.rat.n;
				result.type.rat.d = a->type.rat.d;
			}
		} else {
			#if DEBUG
				printf("[Error] No type set!\n");
			#endif
		}
	} else {
		if (a->status == RATIONAL) {
			result.status = DECIMAL;
			
			convertToDecimal(a);
			result.type.dec = a->type.dec - b->type.dec;
			convertToRational(a);
		} else if (b->status == RATIONAL) {
			result.status = DECIMAL;
			
			convertToDecimal(b);
			result.type.dec = a->type.dec - b->type.dec;
			convertToRational(b);
		} else {
			#if DEBUG
				printf("[Error] Incorrect types on variables!\nFunction: %s\n", __func__);
			#endif
		}
	}
	
	//simplify(&result);
	return result;
}



/** Multiply Number */
/*
struct _number mulNumber(struct _number *a, struct _number *b) {
	struct _number result;
	
	result.n = a->n * b->n;
	result.d = a->d * b->d;
	
	simplify(&result);
	return result;
}
*/


/** Divide Number */
/*
struct _number divNumber(struct _number *a, struct _number *b) {
	struct _number result;
	
	result.n = a->n * b->d;
	result.d = a->d * b->n;
	
	return result;
}
*/


/** Function to simplify fractions based on GCD */
/*
void simplify(struct _number *n) {
	int g = getGCD(n->n, n->d);
	
	if (g > 1) {
		n->n /= g;
		n->d /= g;
	}
}
*/


/** Function to print the number using printf function */

void printRationalNumber(struct _rational *n) {
	//simplify(n);
	
	if (n->d != 0) {
		if (n->n == 0) {
			printf("%5i ", n->n);
		} else if (n->d == 1) {
			printf("%5i ", n->n);
		} else {
			printf("%5i/%i ", n->n, n->d);
		}
	} else {
		printf("    NaN ");
	}
}
