# vim: set noexpandtab shiftwidth=2 tabstop=2:
FROM fedora:latest

# Install RPM fusion
RUN dnf install -y \
	"https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm" \
	"https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm"

# Add repo for vscode
RUN rpm --import https://packages.microsoft.com/keys/microsoft.asc && \
	sh -c 'echo -e "[code]\nname=Visual Studio Code\nbaseurl=https://packages.microsoft.com/yumrepos/vscode\nenabled=1\ngpgcheck=1\ngpgkey=https://packages.microsoft.com/keys/microsoft.asc" > /etc/yum.repos.d/vscode.repo'

# Update packages
RUN dnf update -y --refresh

# Install some packages (Please keep this list sorted alphabetically)
RUN dnf install -y \
	bind-utils \
	binutils \
	bzip2 \
	cmake \
	cmus \
	code \
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
	liberation-fonts \
	make \
	mpv \
	nautilus \
	neovim \
	openfortivpn \
	openssh \
	openssh-clients \
	openssl \
	openssl-devel \
	openvpn \
	passwd \
	procps-ng \
	psmisc \
	readline-devel \
	ruby \
	ruby-doc \
	socat \
	sudo \
	telnet \
	the_silver_searcher \
	tmux \
	tree \
	vim \
	wget \
	x2goclient \
	xterm-resize \
	zlib-devel && \
dnf -y groups install 'C Development Tools and Libraries'

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

# Allow others in FUSE mounts
RUN sed -ri 's,^[[:space:]]*#*[[:space:]]*(user_allow_other[[:space:]]*)$,\1,g' /etc/fuse.conf
# Fix DNS resolution
RUN sed -ri 's,^hosts:.*,hosts: files resolve [NOTFOUND=continue] myhostname dns,g' /etc/nsswitch.conf

# Allow running openforticlient without passwd and other commands with (Last rule takes preference)
RUN echo "${CONT_USERNAME}"'	ALL=(ALL)	ALL' >> /etc/sudoers
RUN echo "${CONT_USERNAME}"'	ALL=(ALL)	NOPASSWD: /usr/bin/openfortivpn *' >> /etc/sudoers
RUN echo "${CONT_USERNAME}"'	ALL=(ALL)	NOPASSWD: /usr/bin/kill *' >> /etc/sudoers

# Make extra dirs
ARG EXTRA_DIRS
RUN if [[ -n "${EXTRA_DIRS:-}" ]]; then (IFS=: ; for p in ${EXTRA_DIRS}; do mkdir -p "$p" && chmod 1777 "$p"; done; ); fi

# Switch users
USER "${CONT_USERNAME}"
