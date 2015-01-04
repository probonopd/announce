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

git clone https://${TOKEN}@github.com/${USER}/${REPO}.git --branch gh-pages \
--single-branch gh-pages > /dev/null 2>&1 || exit 1 # so that the key does not leak to the logs in case of errors

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

DATE=$(date "+%Y-%m-%d")
cat > README.md <<EOF
OpenWrt repository for ${PACKAGE}
========

Binaries built from this repository on $DATE can be downloaded from http://${USER}.github.io/${REPO}/.

To install the ${PACKAGE} package, run
\`\`\`
echo "src/gz announce http://${USER}.github.io/${PACKAGE}" >> /etc/opkg.conf
opkg update
opkg install ${PACKAGE}
\`\`\`
EOF

git add -A
git pull
git commit -a -m "Deploy Travis build $TRAVIS_BUILD_NUMBER to gh-pages"
git push -fq origin gh-pages > /dev/null 2>&1 || exit 1 # so that the key does not leak to the logs in case of errors
echo -e "Uploaded files to gh-pages\n"
cd -
