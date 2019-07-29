### COMP1521 19t2 ... week 04 lab
### Compute factorials -- iterative function version

## Global data
	.data
msg1:	.asciiz "n  = "
msg2:	.asciiz "n! = "
eol:	.asciiz "\n"

### main() function
	.text
	.globl	main
main:
	# Set up stack frame
	sw	$fp, -4($sp)	# push $fp onto stack
	la	$fp, -4($sp)	# set up $fp for this function
	sw	$ra, -4($fp)	# save return address
	sw	$s0, -8($fp)	# save $s0 to use as ... int n
	addi	$sp, $sp, -12	# reset $sp to last pushed item

	# code for main()
	li	$s0, 0

	la	$a0, msg1
	li	$v0, 4
	syscall			# printf("n  = ")

	li	$v0, 5
	syscall			# scanf("%d", into $v0)

    move  $a0, $v0           #call fac function
    jal   fac
    la    $a0, msg2          #printf("n! = ")
    li    $v0, 4
    syscall
    move  $a0, $v1           #printf("n")
    li    $v0, 1
    syscall
    la    $a0, eol
    li    $v0, 4
    syscall                  # printf("\n");

    # clean up stack frame
    lw    $s0, -8($fp)       # restore $s0 value
    lw    $ra, -4($fp)       # restore $ra for return
    la    $sp, 4($fp)        # restore $sp (remove stack frame)
    lw    $fp, ($fp)          # restore $fp (remove stack frame)

    li    $v0, 0
    jr    $ra                # return 0



## fac() function
	.text
	.globl	fac
fac:
	# setup stack frame
	sw	$fp, -4($sp)	# push $fp onto stack
	la	$fp, -4($sp)	# set up $fp for this function
	sw	$ra, -4($fp)	# save return address
	sw	$s0, -8($fp)	# save $s0 to use as ... int i;
	sw	$s1, -12($fp)	# save $s1 to use as ... int prod;
	addi	$sp, $sp, -16	# reset $sp to last pushed item

	# code for fac()

	# TODO: place your code for the body of fac() here
	# .... use the value of n in $a0, place n! in $v0
    li    $t0, 1             # i = 1
    li    $t1, 1             # product = 1
for:
   bgt   $t0, $a0, end_for
   mul  $t1, $t0, $t1
   addi  $t0, $t0, 1
   j     for
end_for:
   move  $v1, $t1

	# clean up stack frame
	lw	$s1, -12($fp)	# restore $s1 value
	lw	$s0, -8($fp)	# restore $s0 value
	lw	$ra, -4($fp)	# restore $ra for return
	la	$sp, 4($fp)	# restore $sp (remove stack frame)
	lw	$fp, ($fp)	# restore $fp (remove stack frame)
	jr	$ra		# return
