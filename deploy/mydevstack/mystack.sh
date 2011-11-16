DEST=/data/stack

NOVA_REPO=https://github.com/openstack/nova.git
NOVA_BRANCH=stable/diablo

NOVACLIENT_REPO=https://github.com/rackspace/python-novaclient.git
NOVACLIENT_BRANCH=master

GLANCE_REPO=https://github.com/openstack/glance.git
GLANCE_BRANCH=stable/diablo

#KEYSTONE_REPO=https://github.com/openstack/keystone.git
#KEYSTONE_BRANCH=stable/diablo

NOVACLIENT_DIR=$DEST/python-novaclient
NOVA_DIR=$DEST/nova
GLANCE_DIR=$DEST/glance
#KEYSTONE_DIR=$DEST/keystone

FILES=`pwd`/files

function apt_get() {
    local sudo="sudo"
    [ "$(id -u)" = "0" ] && sudo="env"
    $sudo DEBIAN_FRONTEND=noninteractive apt-get \
        --option "Dpkg::Options::=--force-confold" --assume-yes "$@"
}

apt_get update
apt_get install `cat $FILES/apts/* | cut -d\# -f1 | grep -Ev "mysql-server|rabbitmq-server|memcached"`
sudo PIP_DOWNLOAD_CACHE=/var/cache/pip pip install --use-mirrors `cat $FILES/pips/*`

function git_clone {

    GIT_REMOTE=$1
    GIT_DEST=$2
    GIT_BRANCH=$3

    # do a full clone only if the directory doesn't exist
    if [ ! -d $GIT_DEST ]; then
        git clone $GIT_REMOTE $GIT_DEST
        cd $2
        # This checkout syntax works for both branches and tags
        git checkout $GIT_BRANCH
    elif [[ "$RECLONE" == "yes" ]]; then
        # if it does exist then simulate what clone does if asked to RECLONE
        cd $GIT_DEST
        # set the url to pull from and fetch
        git remote set-url origin $GIT_REMOTE
        git fetch origin
        # remove the existing ignored files (like pyc) as they cause breakage
        # (due to the py files having older timestamps than our pyc, so python
        # thinks the pyc files are correct using them)
        find $GIT_DEST -name '*.pyc' -delete
        git checkout -f origin/$GIT_BRANCH
        # a local branch might not exist
        git branch -D $GIT_BRANCH || true
        git checkout -b $GIT_BRANCH
    fi
}

git_clone $NOVA_REPO $NOVA_DIR $NOVA_BRANCH
git_clone $NOVACLIENT_REPO $NOVACLIENT_DIR $NOVACLIENT_BRANCH
git_clone $GLANCE_REPO $GLANCE_DIR $GLANCE_BRANCH
#git_clone $KEYSTONE_REPO $KEYSTONE_DIR $KEYSTONE_BRANCH

#cd $KEYSTONE_DIR; sudo python setup.py develop
cd $NOVACLIENT_DIR; sudo python setup.py develop
cd $NOVA_DIR; sudo python setup.py develop
cd $GLANCE_DIR; sudo python setup.py develop
