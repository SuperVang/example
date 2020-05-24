

#!/usr/bash

REPO_DIR=$(cd $(dirname $0); pwd)

rm -rf ${REPO_DIR}/_build ${REPO_DIR}/build* ${REPO_DIR}/cmake-build* ${REPO_DIR}/.idea ${REPO_DIR}/.vscode ${REPO_DIR}/bin ${REPO_DIR}/lib
