# Reprepro

## "Missing file ..."

If `reprepro checkpool` reports "Missing file ...", chances are at least one of the distributions is still using the `.deb` file but the `.deb` file has been removed (by accident).

If the removal is indeed the purpose, then just find the distribution and use `reprepro remove <dist> <package>` to remove the package, then use `reprepro deleteunreferenced` to clean the files.
