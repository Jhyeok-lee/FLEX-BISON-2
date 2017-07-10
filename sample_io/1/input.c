int wow () {
}

int rec (int a) {
	int i, ret;
	if (a <= 1) {
		return 1;
	}
	for (i = 1; i < a; i = i + 1) {
		ret = ret + rec (i);
	}

	return ret;
}

int main ()
{
	int i;
	int x;
	wow ();
	x = 2;
	for (i = 0; i < 10; i = i + 1)
	{
		x = x + i;
	}

	rec (a);

	return 0;
}
