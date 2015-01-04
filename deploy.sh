#!/bin/bash

#
# Deploy binaries built with travis-ci to GitHub Pages,
# where they can be accessed by OpenWrt opkg directly
# 
# Go to GitHub.com -> Settings -> Applications -> Personal Access Tokens 
# —> Create new token, and copy it to your clipboard
# travis encrypt TOKEN=123459... -r username/reponame
# travis-encrypt -r [repository slug] -k TOKEN -v [the token you created before]
# 
# In .travis.yml, set:
# env:
#  global:
#  - USER: <your GitHub username>
#  - REPO: <this GitHub repo name>
#  - PACKAGE: <the package name>
#  - secure: RIbIq8hI153J5trRa........
#

cd /tmp/

git clone https://github.com/${USER}/${REPO}.git --branch gh-pages --single-branch gh-pages 

cd gh-pages || exit 1
git config user.name "Travis CI"
git config user.email "travis@noreply"

cp $TRAVIS_BUILD_DIR/*ipk .
$TRAVIS_BUILD_DIR/sdk/OpenWrt-SDK-*/scripts/ipkg-make-index.sh . > Packages
gzip -c Packages > Packages.gz

cat > index.html <<EOF
<html><body><pre>
echo "src/gz announce http://${USER}.github.io/${PACKAGE}" >> /etc/opkg.conf
opkg update
opkg install ${PACKAGE}
</pre></body></html>
EOF
git add .
git commit -m "Deploy packages to gh-pages branch"
find .
# 2>&1 so that the key does not leak to the logs in case of errors
git push --force --quiet https://${TOKEN}@github.com/${USER}/${REPO}.git master:gh-pages > /dev/null 2>&1 || exit 1
cd -
