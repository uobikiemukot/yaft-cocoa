#!/bin/bash

set -eu

: ${GITHUB_TOKEN:?}

REPO="yaft-cocoa"
OWNER="uobikiemukot"
BASE="${1:-master}"
HEAD="${2:-develop}"

GITHUB_AUTHORIZATION_HEADER="Authorization: token ${GITHUB_TOKEN}"
GITHUB_PULLS_API="https://api.github.com/repos/${OWNER}/${REPO}/pulls"

PR_URL=$(curl -sS -H "${GITHUB_AUTHORIZATION_HEADER}" "${GITHUB_PULLS_API}" -G -d "state=open" --data-urlencode "base=${BASE}" --data-urlencode "head=${HEAD}" | jq -r '.[0].url')

echo "# description" > pr-body
echo >> pr-body
git fetch origin master:master
git fetch origin develop:develop
git log --pretty=format:"%s" --reverse --merges ${BASE}..${HEAD} \
  | cut -d' ' -f4 \
  | tr -d '#' \
  | xargs -I{} sh -c "curl -sS -H '${GITHUB_AUTHORIZATION_HEADER}' '${GITHUB_PULLS_API}/'{} | jq -r '.title' | sed 's/$/ #{}/g'" \
  | sed 's/^/- [x] &/g' >> pr-body

if [ -z "${PR_URL}" -o "${PR_URL}" = "null" ]; then
  # create pr
  jq -sR '{title: "'"Merge ${HEAD} into ${BASE}"'" , body: ., base: "'"${BASE}"'", head: "'"${HEAD}"'"}' pr-body \
    | curl -XPOST -sS -H "${GITHUB_AUTHORIZATION_HEADER}" "${GITHUB_PULLS_API}" -d @- > /dev/null
else
  # update pr
  jq -sR '{title: "'"Merge ${HEAD} into ${BASE}"'" , body: ., base: "'"${BASE}"'"}' pr-body \
    | curl -XPATCH -sS -H "${GITHUB_AUTHORIZATION_HEADER}" "${PR_URL}" -d @- > /dev/null
fi
