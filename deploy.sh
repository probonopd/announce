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

cd $TRAVIS_BUILD_DIR

git clone https://github.com/probonopd/announce.git --branch gh-pages --single-branch gh-pages 

cd gh-pages
git config user.name "Travis CI"
git config user.email "travis@noreply"

cp ../*ipk .
../sdk/OpenWrt-SDK-*/scripts/ipkg-make-index.sh . > Packages
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
git push --force --quiet "https://${TOKEN}@$github.com/${USER}/${REPO}.git" master:gh-pages > /dev/null 2>&1
