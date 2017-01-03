#!/bin/bash
snap remove scratch-qt 
snapcraft clean 
snapcraft prime
snap try prime $1
snap connect scratch-qt:platform ubuntu-app-platform:platform
#sudo /usr/lib/snapd/snap-discard-ns aptbrowser
scratch-qt.run
