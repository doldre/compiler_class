/* A program to perform finding the maximum number of the three. */
int max(int x, int y, int z)
{
	int result;
	if (x > y)
	{
		result = x;
		if (z > result)
		{
			result = z;
		}
	}
	else
	{
		result = y;
		if (z > result)
		{
			result = z;
		}
	}
	return result;
}

void main(void)
{
	int a;
	int b;
	int c;
	/* int d; */
	a = input();
	b = input();
	c = input();
	output(max(a, b, c));
	
	/*
	d = 2 * max(a, b, c);
	output(d);
	*/
}
