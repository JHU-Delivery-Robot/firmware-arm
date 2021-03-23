#ifndef _BIT_UTIL_H_
#define _BIT_UTIL_H_

// Read-Modify-Write operations. Prefer special set/clr/toggle regs where
// possible to reduce cycle count
#define _SET_BIT(var, pos) var |= 1 << pos
#define _CLR_BIT(var, pos) var &= ~(1 << pos)
#define _INV_BIT(var, pos) var ^= 1 << pos

/**
 * Clear a multi-bit field in a register
 */
#define _FLDCLR(reg, field) (reg &= ~(field ## _bm << field ## _bp))
/**
 * Set the value of a register field if it has been zeroed before
 */
#define _FLDSET(reg, field, val) (reg |= ((val & field ## _bm) << field ## _bp))

#endif /* BIT_UTIL */
