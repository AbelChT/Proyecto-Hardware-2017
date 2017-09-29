//------------------------------------------------------------------------------
// Function Name: __c_copy
// This function copies 10 words from one address (src) to another (dst)
// src: pointer to the source block
// dst: pointer to the destination block
//------------------------------------------------------------------------------
void __c_copy_10(int *src, int *dst)
{
	int i;

	for (i = 0; i < 10; i++)
	{
		dst[i] = src[i];
	}
}
