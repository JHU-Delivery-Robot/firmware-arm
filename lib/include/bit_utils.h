#ifndef BIT_UTIL_H_
#define BIT_UTIL_H_

// Read-Modify-Write operations. Prefer special set/clr/toggle regs where
// possible to reduce cycle count
#define _SET_BIT(var, pos) var |= 1 << pos
#define _CLR_BIT(var, pos) var &= ~(1 << pos)
#define _INV_BIT(var, pos) var ^= 1 << pos

/**
 * Clear a multi-bit field in a register
 */
#define _FLDCLR(reg, field) (reg &= ~(field ## _Msk << field ## _Pos))
/**
 * Set the value of a register field if it has been zeroed before
 */
#define _FLDSET(reg, field, val) (reg |= ((val & field ## _Msk) << field ## _Pos))

#endif /* BIT_UTIL_H_ */
