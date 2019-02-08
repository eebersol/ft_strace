/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eebersol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 15:35:37 by eebersol          #+#    #+#             */
/*   Updated: 2018/08/17 15:35:38 by eebersol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/strace.h"


int	is_print(char c) { return (isprint(c) || c == '\n' ? 1 : 0); }
int	is_printable(char *str) {
	int i = -1;

	while (str && str[++i]) {
		if  (!is_print(str[i]))
			return (0);
	}
	if (i == 0)
		return (0);
	return (1);
}

unsigned long long int select_register(int i) {
	switch(i) {
		case 0: 	return regs.rdi;
		case 1: 	return regs.rsi;
		case 2: 	return regs.rdx;
		case 3: 	return regs.r10;
		case 4: 	return regs.r8;
		case 5: 	return regs.r9;
		default: 	return 0;
	}
}

void get_data(long reg) {
	char 	message[10000];
	char 	*temp;
	long 	res;
	int 	i;

	temp 	= message;
	res 	= 100;
	i 		= -1;
	while (++i < 8) {
		res = ptrace(PTRACE_PEEKDATA, child, reg + (i * 8), NULL);
		memcpy(temp, &res, 8);
		temp += 8;
	}
	if (!errno)
		is_printable(message) ? printf("\"%s\"", (message)) : printf("%p", &message);
	else
		printf("%ld", reg);	
}

void 	get_array_data(unsigned long addr, t_syscall const syscall) {
	int		len;
	char	**tmp;

	len = 0;
	tmp = (char **)addr;
	if (tmp) {
		while (tmp[len])
			len++;
	}
	if (len > syscall.sys_argc)
		printf("[*/ %d vars */]", len);
	else {
		printf("[");
		get_data(regs.rdi);
		printf("]");
	}
}

int  print_syscall(t_syscall const syscall) {
	int 	i;

	printf("%s(", syscall.sys_name);
	for (i = 0; i < syscall.sys_argc; i++) {
		if (regs.rdi == 0 && i == 0)
			regs.orig_rax == SYS_exit_group ? printf("0") : syscall.sys_argv[0] == E_INT ? printf("0") : printf("NULL");
		else {
			switch (syscall.sys_argv[i]) {
				case E_INT: 	printf("%lld", select_register(i));
					break;
				case E_UINT: 	printf("%llu", select_register(i));
					break;
				case E_STR: 	get_data(select_register(i));
					break;
				case E_SPE: 	get_array_data(select_register(i), syscall);
					break;
				case E_PTR: 	get_data(select_register(i));
					break;
				case E_STRUCT: 	select_register(i) ? printf("{ 0x%llx }", select_register(i)) : printf("NULL");
					break;
				default: 		printf("ERROR");
					break;
			}
		}
		if (i + 1 < syscall.sys_argc)
			printf(", ");
	}
	printf(")");
	return 0;
}

void	print_syscall_return(struct  user_regs_struct regs) {
	if ((long)regs.rax == -1 || regs.orig_rax == SYS_exit_group)
		printf(" = ?\n");
	else if ( (long)regs.rax < -1) {
		printf(" = -1 ");
		get_errno_name((long)regs.rax);
	}
	else if ((long)regs.rax > 10000 || (long)regs.rax < -1000)
		printf(" = %p\n", &regs.rax);
	else
		printf(" = %lld\n", regs.rax);
	return ;
}