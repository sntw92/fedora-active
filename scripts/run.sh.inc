#!/bin/bash
DOCKER_EXE="${DOCKER_EXE:-sudo docker}"
IMAGE_REPO=${IMAGE_REPO:-}
IMAGE_NAME=${IMAGE_NAME:-fedora-active}
IMAGE_TAG=${IMAGE_TAG:-test}
USE_TZ="${USE_TZ:-Asia/Kolkata}"
USE_HOME="${USE_HOME:-${HOME}/.fedora-active}"
USE_DISPLAY=${USE_DISPLAY:-:0}

# fedora-active is from "sandiptiwari87/fedora-active" on DockerHub
tcont() {
	local cmd
	cmd="${1?A command is required}"
	case "${cmd}" in
	start|up)
		${DOCKER_EXE} run \
		--name fedora-active \
		--init \
		--rm \
		--hostname fedora-active-cont \
		-u "$(id -un):$(id -gn)" \
		-w "/home/$(id -un)" \
		-v "${USE_HOME}:/home/$(id -un)" \
		-v /dev/net/tun:/dev/net/tun \
		-v /etc/localtime:/etc/localtime:ro \
		-v "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY:/tmp/$WAYLAND_DISPLAY" \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v /dev/snd:/dev/snd \
		-v /dev/shm:/dev/shm \
		-v "/run/user/$(id -u)/pulse:/run/user/$(id -u)/pulse" \
		-e "XDG_RUNTIME_DIR=/home/$(id -un)/.tmp" \
		-e "WAYLAND_DISPLAY=$WAYLAND_DISPLAY" \
		-e DISPLAY="${USE_DISPLAY}" \
		-e TZ="${USE_TZ}" \
		--privileged  \
		-d \
		"${IMAGE_REPO:+${IMAGE_REPO}/}${IMAGE_NAME}:${IMAGE_TAG}" \
		tail -f /dev/null
		;;
	stop|down|dn)
		${DOCKER_EXE} stop \
		fedora-active
		;;
	*)
		echo "Invalid command '${cmd}'. Must be one of {start,stop,up,down,dn}"
		return 1
		;;
	esac
}

# Launch a firefox instance inside the container
tfire() {
	${DOCKER_EXE} exec \
	  -e TZ="${USE_TZ}" \
	  fedora-active \
	  firefox --new-instance &>/dev/null &
}

# Launch a bash shell inside the container
tbash() {
	${DOCKER_EXE} exec \
	-it \
	fedora-active \
	bash ${1:+"$@"}
}
