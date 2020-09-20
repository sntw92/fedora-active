#!/bin/bash
TCONT_DOCKER_EXE="${TCONT_DOCKER_EXE:-sudo docker}"
TCONT_IMAGE_REPO=${TCONT_IMAGE_REPO:-}
TCONT_IMAGE_NAME=${TCONT_IMAGE_NAME:-fedora-active}
TCONT_IMAGE_TAG=${TCONT_IMAGE_TAG:-test}
TCONT_CONTAINER_NAME=fedora-tower
TCONT_USE_TZ="${TCONT_USE_TZ:-Asia/Kolkata}"
TCONT_USE_HOME="${TCONT_USE_HOME:-${HOME}/.fedora-active}"
TCONT_USE_DISPLAY=${TCONT_USE_DISPLAY:-:0}

# fedora-active is from "sandiptiwari87/fedora-active" on DockerHub
tcont() {
	local cmd
	cmd="${1?A command is required}"
	case "${cmd}" in
	start|up)
		${TCONT_DOCKER_EXE} run \
		--name "${TCONT_CONTAINER_NAME}" \
		--init \
		--rm \
		--hostname fedora-active-cont \
		-u "$(id -un):$(id -gn)" \
		-w "/home/$(id -un)" \
		-v "${TCONT_USE_HOME}:/home/$(id -un)" \
		-v /dev/net/tun:/dev/net/tun \
		-v /etc/localtime:/etc/localtime:ro \
		-v "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY:/tmp/$WAYLAND_DISPLAY" \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v /dev/snd:/dev/snd \
		-v /dev/shm:/dev/shm \
		-v "/run/user/$(id -u)/pulse:/run/user/$(id -u)/pulse" \
		-e "XDG_RUNTIME_DIR=/home/$(id -un)/.tmp" \
		-e "WAYLAND_DISPLAY=$WAYLAND_DISPLAY" \
		-e DISPLAY="${TCONT_USE_DISPLAY}" \
		-e TZ="${TCONT_USE_TZ}" \
		--privileged  \
		-d \
		"${TCONT_IMAGE_REPO:+${TCONT_IMAGE_REPO}/}${TCONT_IMAGE_NAME}:${TCONT_IMAGE_TAG}" \
		tail -f /dev/null
		;;
	stop|down|dn)
		${TCONT_DOCKER_EXE} stop \
		"${TCONT_CONTAINER_NAME}"
		;;
	*)
		echo "Invalid command '${cmd}'. Must be one of {start,stop,up,down,dn}"
		return 1
		;;
	esac
}

# Launch a firefox instance inside the container
tfire() {
	${TCONT_DOCKER_EXE} exec \
		-e TZ="${TCONT_USE_TZ}" \
		"${TCONT_CONTAINER_NAME}" \
		firefox --new-instance &>/dev/null &
}

# Launch a bash shell inside the container
tbash() {
	${TCONT_DOCKER_EXE} exec \
		-it \
		"${TCONT_CONTAINER_NAME}" \
		bash ${1:+"$@"}
}
