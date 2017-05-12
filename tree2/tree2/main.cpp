#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_NAME_LEN	50
#define MAX_INT_LEN		21

struct tree
{
	double number;
	int count;
	tree *left;
	tree *right;
};

int isNumber(char Symbol)
{
	return (Symbol >= '0' && Symbol <= '9') ? (1) : (0);
}

tree *searchElement(double number, tree *root)
{
	if (number == root->number)
		return root;
	else if (number > root->number)
		searchElement(number, root->right);
	else
		searchElement(number, root->left);
}

tree *splay(tree *p, tree *root)
{
	if (root != p)
	{
		if (root->left == p)
		{
			tree *r = root;
			r->left = p->right;
			root = p;
			p->right = r;
		}
		else if (root->right == p)
		{
			tree *r = root;
			r->right = p->left;
			root = p;
			p->left = r;
		}
		else if (root->left != NULL)
		{
			if ((root->left)->left == p)
			{
				tree *r = root;
				tree *rl = root->left;
				rl->left = p->right;
				root = p;
				p->right = r->left;
				r->left = rl->right;
				rl->right = r;
			}
			else if ((root->left)->right == p)
			{
				tree *r = root;
				tree *rl = root->left;
				rl->right = p->left;
				r->left = p->right;
				root = p;
				p->left = rl;
				p->right = r;
			}
		}
		else if (root->right != NULL)
		{
			if ((root->right)->right == p)
			{
				tree *r = root;
				tree *rr = root->right;
				rr->right = p->left;
				root = p;
				p->left = rr;
				r->right = rr->left;
				rr->left = r;
			}

			else if ((root->right)->left == p)
			{
				tree *r = root;
				tree *rl = root->right;
				rl->left = p->right;
				r->right = p->left;
				root = p;
				p->right = rl;
				p->left = r;
			}
		}
		else if (root->number > p->number)
			root = splay(root->right, p);
		else
			root = splay(root->left, p);
	}
	return root;
}

int tryToMakeString(FILE *f, char c, char *str)
{
	char s[MAX_INT_LEN] = {"\0"};
	int i = 0;
	do
	{
		s[i] = c;
		c = fgetc(f);
		i++;
		if (!isNumber(c) && c != '.')
			break;
	} while (1);
	fseek(f, -(i + 2), SEEK_CUR);
	c = fgetc(f);
	if (c == '-')
	{
		for (int i = strlen(s) - 1; i >= 0; i--)
			s[i + 1] = s[i];
		s[0] = c;
	}
	if (isalpha(c) || c == '_')
	{
		fseek(f, i, SEEK_CUR);
		c = fgetc(f);
		return 0;
	}
	fseek(f, i, SEEK_CUR);
	c = fgetc(f);
	strcpy(str, s);
	return 1;
}

void printTree(tree *root)
{
	if (root != NULL) 
	{
		printTree(root->left);
		if ((int)(root->number) == root->number)
			printf("Number %d - %d times \n", (int)(root->number), root->count);
		else
			printf("Number %.4f - %d times \n", root->number, root->count);
		printTree(root->right);
		free(root);
	}
}

int checkChar(FILE *f, char c)
{
	if (c == '/')
	{
		c = fgetc(f);
		if (c == '/')
			do
			{
				c = fgetc(f);
			} while (c != '\n');
			return 1;
	}
	if (c == '\"')
	{
		do
		{
			c = fgetc(f);
		} while (c != '\n' && c != '\"');
		return 1;
	}
	if (c == '\'')
	{
		do
		{
			c = fgetc(f);
		} while (c != '\n' && c != '\'');
		return 1;
	}
	if (!isNumber(c))
		return 1;
	return 0;
}

int checkElement(double number, tree *root)
{
	while (root != NULL)
	{
		if (root->number == number)
		{
			root->count++;
			return 1;
		}
		if (number > root->number)
		{
			root = root->right;
		}
		else
		{
			root = root->left;
		}
	}
	return 0;
}

tree *addTree(double number, tree *root)
{
	if (root == NULL)
	{
		root = (tree *)malloc(sizeof(tree));
		root->number = number;
		root->count = 1;
		root->left = NULL;
		root->right = NULL;
	}
	else if (number > root->number)
	{
		root->right = addTree(number, root->right);
	}
	else
	{
		root->left = addTree(number, root->left);
	}
	return root;
}

int main()
{
	FILE *f;
	char path[MAX_NAME_LEN], c;
	while (1)
	{
		printf("File path: ");
		fflush(stdin);
		fgets(path, MAX_NAME_LEN, stdin);
		*strchr(path, '\n') = '\0';
		if (f = fopen(path, "r"))
			break;
		printf("Bad path! Try again\n");
	}
	tree *root = NULL, *p;

	while (1)
	{
		c = fgetc(f);
		if (feof(f))
			break;
		
		if (checkChar(f, c))
			continue;

		char s[MAX_INT_LEN] = { "\0" };
		if (!tryToMakeString(f, c, s))
			continue;

		double number = atof(s);

		if (!checkElement(number, root))
		{
			root = addTree(number, root);
		}

		tree *p = searchElement(number, root);
		if (p != root)
		{
			root = splay(p, root);
		}

	}

	printTree(root);

	fclose(f);
	system("pause");
	return 0;
}