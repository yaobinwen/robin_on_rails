ARG UBUNTU_VERSION=22.04
FROM ubuntu:${UBUNTU_VERSION}
ARG USER_NAME=ywen
ARG USER_ID=1000
ARG GROUP_ID=1000
ARG LOCALE="en_US.UTF-8"
ARG LANGUAGE="en_US"
ARG TZ=Etc/UTC

RUN groupadd -g ${GROUP_ID} ${USER_NAME} && \
    useradd -r --create-home -u ${USER_ID} -g ${USER_NAME} ${USER_NAME}

RUN apt update

RUN DEBIAN_FRONTEND=noninteractive apt -y install \
    software-properties-common

RUN DEBIAN_FRONTEND=noninteractive apt -y install \
    locales \
    openssh-server

# Install additional utilities.
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install \
    tree \
    vim

# Generate and set the locale to UTF-8.
RUN locale-gen ${LOCALE}
ENV LANG ${LOCALE}
ENV LC_ALL ${LOCALE}
ENV LANGUAGE ${LANGUAGE}

# Set the timezone.
RUN ln -snf /usr/share/zoneinfo/${TZ} /etc/localtime
RUN echo ${TZ} > /etc/timezone
