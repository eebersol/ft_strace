/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstreduce.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eebersol <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/07 10:23:19 by eebersol          #+#    #+#             */
/*   Updated: 2015/12/07 11:12:54 by eebersol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lstreduce(t_list *lst, int (*f)(t_list *elem))
{
	t_list	*res;

	if (!lst || !f)
		return (NULL);
	res = NULL;
	while (lst)
	{
		if (f(lst) && lst != NULL)
			ft_lstaddend(&res, lst);
		lst = lst->next;
	}
	return (res);
}
