#!/bin/bash
snap remove aptbrowser
snapcraft clean 
snapcraft prime
snap try prime
snap connect aptbrowser:platform ubuntu-app-platform:platform
sudo /usr/lib/snapd/snap-discard-ns aptbrowser
aptbrowser.run
