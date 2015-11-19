#include <Windows.h>
#include <stdlib.h>

#define LESTATUS INT 
#define LE_NO_CHANGES_NEEDED (0)
#define LE_CHANGES_SUCCEEDED (1)
#define LE_CHANGES_FAILED   (-1)

/// <summary>
/// Converts the line-endings of an array of bytes representing a text-file into the standard DOS/Windows line ending format (\r\n).
/// This is necessary for parsing a file line-by-line with functions like strtok and wcstok, when the line ending format of the file is unknown.
/// </summary>
/// <param name="inData">An array representing the contents of a text file.</param>
/// <param name="inLen">The length, in bytes, of inData.</param>
/// <param name="outData">An array to receive the changes made to inData by this algorithm.</param>
/// <param name="outLen">The maximum capacity, in bytes, of outData.</param>
/// <param name="bytesWritten">A pointer to an integer that receives the number of bytes that are written to outData if changes were successful.</param>
/// <returns>
/// LE_NO_CHANGES_NEEDED if inData already has \r\n line endings.
/// LE_CHANGES_FAILED if outData is too small to hold the output of the algorithm.
/// LE_CHANGES_SUCCEEDED if inData does not already have \r\n line endings and outData is large enough to hold the changes.
/// </returns>
LESTATUS ConvertLineEndings(BYTE *inData, INT inLen, BYTE *outData, INT outLen, INT *bytesWritten)
{
	INT sourceIndex = 0, destIndex;

	// Fail immediately; no chance of success here.
	if (outLen < inLen)
		return LE_CHANGES_FAILED;

	// Try to determine if changes are needed
	while (sourceIndex < inLen)
	{
		// If an \r is immediately followed by an \n, no changes are needed to inData.
		if (inData[sourceIndex] == '\r')
		{
			if (sourceIndex < inLen - 1 && inData[sourceIndex + 1] == '\n')
			{
				memcpy(outData, inData, inLen);
				*bytesWritten = inLen;
				return LE_NO_CHANGES_NEEDED;
			}
			// If we encountered an \r without a following \n then changes are needed.
			break;
		}

		// If we encounter an \n without a preceding \r then changes are needed.
		if (inData[sourceIndex] == '\n')
			break;
		sourceIndex++;
	}

	// But, up to the point where we encountered the bad line ending, we can copy *that* much into outData.
	memcpy(outData, inData, sourceIndex);
	// If, however, that was the end of the array, then we still don't need to make changes.
	if (sourceIndex == inLen)
		return LE_NO_CHANGES_NEEDED;

	// Now, we begin the actual copying/rewriting of the array, so set destIndex to sourceIndex to begin counting.
	destIndex = sourceIndex;

	// Loop through the remainder of inData; if an \n or \r is encountered, rewrite it as appropriate into outData.
	// If outData's limits are reached before we're done, report that changes failed.
	while (sourceIndex < inLen)
	{
		switch (inData[sourceIndex])
		{
		case '\n':
		case '\r':
			sourceIndex++;
			if (destIndex + 2 >= outLen)
				return LE_CHANGES_FAILED;
			outData[destIndex++] = '\r';
			outData[destIndex++] = '\n';
			break;
		default:
			outData[destIndex++] = inData[sourceIndex++];
		}
	}
	// Otherwise, write bytesWritten and report that changes were successful.
	*bytesWritten = destIndex;
	return LE_CHANGES_SUCCEEDED;
}
