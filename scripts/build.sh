#!/bin/bash
SCRIPT_DIR=$(dirname "$(readlink -e "${BASH_SOURCE[0]}")")

DOCKER_EXE="${DOCKER_EXE:-podman}"
USE_CACHE="${USE_CACHE:-0}"

CONT_UID=$(id -u)
CONT_GID=$(id -g)
CONT_USERNAME=$(id -un)
CONT_GROUP=$(id -gn)
CONT_PASSWORD=${CONT_PASSWORD:-password}
IMAGE_REGISTRY=${IMAGE_REPO:-docker.io}
IMAGE_REPO=${IMAGE_REPO:-sandiptiwari87}
IMAGE_NAME=${IMAGE_NAME:-fedora-active}
IMAGE_TAG=${IMAGE_TAG:-test}

_docker_args=( )
if [[ "${USE_CACHE}" -eq 0 ]]; then
  _docker_args+=( "--no-cache" )
fi
_docker_build_args=( "CONT_USERNAME=${CONT_USERNAME}"
  "CONT_GROUP=${CONT_GROUP}"
  "CONT_UID=${CONT_UID}"
  "CONT_GID=${CONT_GID}"
  "CONT_PASSWORD=${CONT_PASSWORD}" )

_docker_args+=( "-t" "${IMAGE_REPO:+${IMAGE_REPO}/}${IMAGE_NAME}:${IMAGE_TAG}" )
_docker_args+=( "-f" "${SCRIPT_DIR}/../docker/Dockerfile" )
for _cur_build_arg in "${_docker_build_args[@]}"; do
  _docker_args+=( "--build-arg" "${_cur_build_arg}" )
done

# The docker file requires this
if [[ -d "${SCRIPT_DIR}/../docker/extra_rpms" ]]; then
  mkdir -p "${SCRIPT_DIR}/../docker/extra_rpms"
fi

${DOCKER_EXE} build "${_docker_args[@]}" "${SCRIPT_DIR}/../docker/"
