
#====================================================
# Author: J. V. Cardenzana
# Description:
#    This script is intended to generate the online
#    documentation from whatever code branch it is
#    run from and push those changes to the server.
#====================================================

UpdateDocs()
{
   # Get the current branch name
   declare curBranch=$(git rev-parse --abbrev-ref HEAD)

   # Checkout the gh-pages branch
   git checkout gh-pages

   # Merge changes from the previous branch
   git merge $curBranch

   # Delete all of the documentation information
   git rm -r documentation/html
   rm -r documentation/html

   # Re-make the documentation
   doxygen

   # Add the html documentation to the repository
   git add -f documentation/html

   # Commit the documentation
   git commit -a -m "Updated documentation from the ${curBranch} branch."

   # Push the changes
   git pull
   git push

   # Finally switch back to the original branch
   git checkout $curBranch
}

UpdateDocs