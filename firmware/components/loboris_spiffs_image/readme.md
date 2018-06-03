Test procedure
==============

1. make flashfs image=[imgfile]
2. make copyfs
3. make readbackfs image=[imgfile]
4. make unpackfs image=[imgfile]

Or

1. make flashfs image=[imgfile]
2. make readbackfs image=[imgfile]
3. make unpackfs image=[imgfile]

If the image argument is not explicitely supplied, make will look for default image name "image".
The md5 hashes of both written and readback files may not be the same due to compression during image flashing.
