FROM fedora:latest

# Install RPM fusion
RUN dnf install -y \
	"https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm" \
	"https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm"

# Update packages
RUN dnf update -y --refresh

# Install some packages (Please keep this list sorted alphabetically)
RUN dnf install -y \
  bind-utils \
	binutils \
	cmus \
	curl \
	evince \
	expect \
	firefox \
	fuse \
	fuse-libs \
	fuse-sshfs \
  gedit \
	golang \
	htop \
	iputils \
	mpv \
	nautilus \
	openfortivpn \
	openssh \
	openssh-clients \
	openvpn \
	passwd \
	procps-ng \
	psmisc \
	socat \
	sudo \
	telnet \
	the_silver_searcher \
  tmux \
	vim \
	wget \
	x2goclient \
	xterm-resize

# Pulse audio config to be able to use host pulseaudio
# Will need --volume /run/user/<uid>/pulse:/run/user/<uid>/pulse
COPY pulse-client.conf /etc/pulse/client.conf
RUN sed -ri 's,T_VARSTART_CONT_UID_VAREND_T,'"${CONT_UID}"',g' /etc/pulse/client.conf

# Username and Password for the user to be created inside container
# UID:GID in the container are not separately configurable from the values they have on the host.
# They MUST be the same as the user's UID:GID outside container.
ARG CONT_USERNAME
ARG CONT_GROUP
ARG CONT_PASSWORD
ARG CONT_UID
ARG CONT_GID

# Pushing this before extra rpm install since, some apps can create non-priv users and occupy id's like 1000
# Create user and groups
RUN groupadd -g "${CONT_GID}" "${CONT_GROUP}" && \
	useradd -g "${CONT_GID}" -M -u "${CONT_UID}" -d "/home/${CONT_USERNAME}" -p $(openssl passwd -1 "${CONT_PASSWORD}") -s /bin/bash "${CONT_USERNAME}"

# Install extra RPMS
COPY extra_rpms /tmp/extra_rpms
RUN if [[ "$(find /tmp/extra_rpms/ -type f -name '*.rpm' -print | wc -l)" -gt 0 ]]; then dnf install -y /tmp/extra_rpms/*.rpm; fi

# Allow running openforticlient without passwd and other commands with (Last rule takes preference)
RUN echo "${CONT_USERNAME}"'	ALL=(ALL)	ALL' >> /etc/sudoers
RUN echo "${CONT_USERNAME}"'	ALL=(ALL)	NOPASSWD: /usr/bin/openfortivpn *' >> /etc/sudoers
RUN echo "${CONT_USERNAME}"'	ALL=(ALL)	NOPASSWD: /usr/bin/kill *' >> /etc/sudoers

# Switch users
USER "${CONT_USERNAME}"
