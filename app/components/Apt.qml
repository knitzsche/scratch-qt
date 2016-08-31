import QtQuick 2.2
import Ubuntu.Components 1.1
import Aptbrowser 1.0
import "../components"

Page {
    id: page1
    title: "Apt Browser"
    property int page_height: 120
    property int page_width: 160
    property int header_height: 12
    property int control_height: 10
    property int content_height: page_height - header_height - control_height
    property int status_height: 20
    property int deps_height: content_height
    property int content_height_with_status: content_height - status_height
    property int deps_width: page_width/2
    property int dep_height: deps_height
    property int dep_height_with_status: dep_height - (status_height/2)
    property int dep_width: deps_width
    property int text_height: 5
    property string current_pkg: "Enter package"
    property string previous_pkg: "no_pkg"
    property bool busy: false
    property bool deps_busy: false
    property bool recs_busy: false
    property bool pars_deps_busy: false
    property bool pars_recs_busy: false
    property bool policy_busy: false
    property int current_dep_idx: -1
    property int current_rec_idx: -1
    property int current_par_dep_idx: -1
    property int current_par_rec_idx: -1

    function getPolicy(pkg) {
        policy_busy = true;
        depends.get_policy(pkg);
    }
    function prepDeps(pkg) {
        current_pkg = pkg;
        if (previous_pkg != current_pkg) {
            depends_m.clear();
            recommends_m.clear();
            parents_depends_m.clear();
            parents_recommends_m.clear();
            pkg_entry_row.setPackage(current_pkg);
        }
    }
    function getDeps() {
        current_pkg = packageName.getText(0,packageName.length);
        deps_busy = true;
        current_dep_idx++;
        depends.get_dependencies(current_pkg, current_dep_idx);
        previous_pkg = current_pkg;
    }
    function getRecs() {
        current_pkg = packageName.getText(0,packageName.length);
        recs_busy = true;
        current_rec_idx++;
        depends.get_recommendations(current_pkg, current_rec_idx);
        previous_pkg = current_pkg;
    }
    function getParsDeps() {
        current_pkg = packageName.getText(0,packageName.length);
        pars_deps_busy = true;
        parents_depends_animation.start();
        parents_depends_busy.state = "ROTATING";
        current_par_dep_idx++;
        depends.get_parents_dependencies(current_pkg, current_par_dep_idx);
        previous_pkg = current_pkg;
    }
    function stopParsDeps(){
        depends.stop_parents_dependencies();
    }
    function getParsRecs() {
        current_pkg = packageName.getText(0,packageName.length);
        pars_recs_busy = true;
        parents_recommends_animation.start();
        parents_recommends_busy.state = "ROTATING";
        current_par_rec_idx++;
        depends.get_parents_recommendations(current_pkg, current_par_rec_idx);
        previous_pkg = current_pkg;
    }
    function stopParsRecs(){
        depends.stop_parents_recommends();
    }
    function addToParentsDepends(pdeps) {
        for (var i = 0; i < pdeps.length; i++) {
            parents_depends_m.append({"pkg_name":pdeps[i]});
        }
    }
    function addToParentsRecommends(deps) {
        for (var i = 0; i < deps.length; i++) {
            parents_recommends_m.append({"pkg_name":deps[i]});
        }
    }
    function addToRecommends(deps) {
        for (var i = 0; i < deps.length; i++) {
            recommends_m.append({"pkg_name":deps[i]});
        }
    }
    function resizeDeps(size){
        col_parent_depends.height = units.gu(size);
        col_parent_recommends.height = units.gu(size);
        col_child_depends.height = units.gu(size);
        col_child_recommends.height = units.gu(size);

        parents_depends_lv.height = units.gu(dep_height_with_status);
        parents_recommends_lv.height = units.gu(dep_height_with_status);
        child_depends_lv.height = units.gu(dep_height_with_status);
        child_recommends_lv.height = units.gu(dep_height_with_status);
    }
    function showStatus(msg) {
        resizeDeps(dep_height_with_status);
        status_label.text = msg;
        status_label.height = units.gu(status_height);
        status.height = units.gu(status_height);
        status.visible = true;
    }
    function hideStatus() {
        status_label.text = "";
        status.height = units.gu(0);
        status_label.height = units.gu(0);
        results.height = units.gu(content_height);
        resizeDeps(dep_height)
        status.visible = false;
    }

    Depends { // pulls in Depends c++ class
        id: depends
        onDependsChanged: {
            console.log("==== onDependsChanged heard");
            console.log("dep_idx: " + dep_idx + " current_dep_idx: " + current_dep_idx);
            if (dep_idx == current_dep_idx) {
                deps_busy = false;
                var deps = depends.depends;
                if (deps.length != 0){
                    var deps_ = deps.split('\n');
                    console.log("deps:\n" + deps_);
                    for (var i = 0; i < deps_.length; i++) {
                        depends_m.append({"pkg_name":deps_[i]});
                    }
                }
                child_depends_busy.state = "NORMAL";
            }
        }
        onRecommendsChanged: {
            console.log(" === onRecommendsChanged heard");
            if (rec_idx == current_rec_idx) {
                recs_busy = false;
                var recs = depends.recommends;
                if (recs.length != 0){
                    var recs_ = recs.split('\n');
                    console.log("recs:\n" + recs_);
                    for (var i = 0; i < recs_.length; i++) {
                        recommends_m.append({"pkg_name":recs_[i]});
                    }
                }
                child_recommends_busy.state = "NORMAL";
            }
        }
        onParentsDependsChanged: {
            console.log(" === onParentsDependsChanged heard");
            if (par_dep_idx == current_par_dep_idx) {
                pars_deps_busy = false;
                var pars_deps = depends.parentsDepends;
                if (pars_deps.length != 0){
                    var deps = pars_deps.split('\n');
                    console.log("pars deps:\n" + deps);
                    for (var i = 0; i < deps.length; i++) {
                        parents_depends_m.append({"pkg_name":deps[i]});
                    }
                }
                parents_depends_busy.state = "NORMAL";
                parents_depends_animation.stop();
            }
        }
        onParentsRecommendsChanged: {
            console.log(" === onParentsRecommendsChanged heard");
            if (par_rec_idx == current_par_rec_idx) {
                pars_recs_busy = false;
                var pars_recs = depends.parentsRecommends;
                if (pars_recs.length != 0){
                    var recs = pars_recs.split('\n');
                    console.log("pars recs:\n" + recs);
                    for (var i = 0; i < recs.length; i++) {
                        parents_recommends_m.append({"pkg_name":recs[i]});
                    }
                }
                parents_recommends_busy.state = "NORMAL";
                parents_recommends_animation.stop();
            }
        }
        onPolicyChanged: {
            console.log(" === onPolicyChanged heard");
            policy_busy = false;
            var policy = depends.policy;
            showStatus(policy);
            policy_busy_id.color = UbuntuColors.orange;
            policy_busy_id.state = "NORMAL";
        }
        onInvalidPackageFound: {
            console.log(" === onInvalidPackage heard");
            showStatus("Package not found, please try again");
            results.height = units.gu(content_height_with_status);
            deps_busy = false;
            child_depends_busy.state = "NORMAL";
            recs_busy = false;
            child_recommends_busy.state = "NORMAL";
            pars_deps_busy = false;
            parents_depends_busy.state = "NORMAL";
            pars_recs_busy = false;
            parents_recommends_busy.state = "NORMAL";
        }
    }
    Rectangle {
        id: main
        width: units.gu(page_width)
        height: units.gu(page_height)
        Rectangle {
            id: row1
            height: units.gu(control_height/2)
            width: units.gu(parent.width)
            Rectangle {
                id: pkg_entry_row
                width: units.gu(parent.width)
                height: units.gu(control_height/2)
                function setPackage(pkg) {
                    packageName.remove(0,packageName.length)
                    packageName.paste(pkg);
                }
                TextField {
                    id: packageName
                    placeholderText: current_pkg
                    width: units.gu(page_width - 2)
                    onAccepted: {
                        current_pkg = text;
                        prepDeps(current_pkg);
                    }
                }
            }
        }
        Rectangle {
            id: policybuttons
            height: units.gu(control_height/2)
            width: units.gu(parent.width)
            anchors.top: row1.bottom
            Rectangle {
                id: policy_busy_id
                width: units.gu(2)
                height: units.gu(2)
                color: UbuntuColors.orange
                state: "NORMAL"
                RotationAnimator {
                    id: rotate
                    running: policy_busy
                    target: policy_busy_id
                    loops: 100000
                    from: 0
                    to: 360
                    duration: 50
                    onRunningChanged: {
                        policy_busy_id.color = UbuntuColors.purple;
                        policy_busy_id.state = "ROTATING";
                    }
                }
                states: [
                    State {
                        name: "NORMAL"
                        PropertyChanges { target: policy_busy_id; color: UbuntuColors.orange }
                    },
                    State {
                        name: "ROTATING"
                        PropertyChanges { target: policy_busy_id; color: UbuntuColors.purple }
                    }
                ]
            }
            Button {
                id: getPolicy_b
                anchors.left: policy_busy_id.right
                text: "Policy"
                onClicked: {
                    if (status.visible == true) {
                        hideStatus();
                    } else {
                        if (packageName.length != 0) {
                            current_pkg = packageName.getText(0,packageName.length);
                            getPolicy(current_pkg);
                        } else {
                            packageName.font.bold = true;
                        }
                    }
                }
            }
        }
        Rectangle {
            id: status
            anchors.top: policybuttons.bottom
            width: units.gu(page_width)
            height: units.gu(0)
            visible: false
            //color: UbuntuColors.lightGrey
            Label {
                id: status_label
                height: units.gu(status_height)
                text: "Status displays here"
                color: UbuntuColors.purple
            }
        }
        Rectangle {
            id: results
            height: units.gu(content_height)
            anchors.top: status.bottom
            Rectangle {
                id: parents
                width: units.gu(deps_width)
                height: units.gu(deps_height)
                Rectangle { // parent depends
                    id: col_parent_depends
                    width: units.gu(dep_width/2)
                    height: units.gu(dep_height)
                    Rectangle {
                        id: parents_depends_busy
                        width: units.gu(2)
                        height: units.gu(2)
                        color: UbuntuColors.orange
                        state: "NORMAL"
                        RotationAnimator {
                            id: parents_depends_animation
                            running: pars_deps_busy
                            target: parents_depends_busy
                            loops: 100000
                            from: 0
                            to: 360
                            duration: 50
                        }
                        states: [
                            State {
                                name: "NORMAL"
                                PropertyChanges { target: parents_depends_busy; color: UbuntuColors.orange }
                            },
                            State {
                                name: "ROTATING"
                                PropertyChanges { target: parents_depends_busy; color: UbuntuColors.purple }
                            }
                        ]
                    }
                    Button {
                        id: get_parents_depends
                        anchors.left: parents_depends_busy.right
                        text: "Parent Depends"
                        onClicked: {
                            console.log("==== Pars Deps clicked");
                            hideStatus();
                            if (parents_depends_busy.state == "ROTATING"){
                                parents_depends_busy.state = "NORMAL"
                                parents_depends_animation.stop()
                            }
                            else if (packageName.length != 0) {
                                prepDeps(packageName.getText(0,packageName.length));
                                getParsDeps();
                            } else {
                                packageName.font.bold = true;
                            }
                        }
                    }
                    ListModel {
                        id: parents_depends_m
                    }
                    Component {
                        id: parents_depends_delegate
                        ListButton {
                            text: pkg_name
                            onClicked: {
                                prepDeps(pkg_name);
                                getParsDeps();
                            }
                        }
                    }
                    ListView {
                        id: parents_depends_lv
                        width: units.gu(dep_width)
                        height: units.gu(dep_height)
                        anchors.top: get_parents_depends.bottom
                        spacing: units.gu(1)
                        model: parents_depends_m
                        delegate: parents_depends_delegate
                    }
                }
                Rectangle { // parent recommends
                    id: col_parent_recommends
                    width: units.gu(deps_width)
                    height: units.gu(dep_height)
                    anchors.left: col_parent_depends.right
                    Rectangle {
                        id: parents_recommends_busy
                        width: units.gu(2)
                        height: units.gu(2)
                        color: UbuntuColors.orange
                        state: "NORMAL"
                        RotationAnimator {
                            id: parents_recommends_animation
                            running: pars_recs_busy
                            target: parents_recommends_busy
                            loops: 100000
                            from: 0
                            to: 360
                            duration: 50
                        }
                        states: [
                            State {
                                name: "NORMAL"
                                PropertyChanges { target: parents_recommends_busy; color: UbuntuColors.orange}
                            },
                            State {
                                name: "ROTATING"
                                PropertyChanges { target: parents_recommends_busy; color: UbuntuColors.purple}
                            }
                        ]
                    }
                    Button {
                        id: get_parents_recommends
                        anchors.left: parents_recommends_busy.right
                        text: "Parent Recs"
                        onClicked: {
                            console.log("==== Pars Recs clicked");
                            console.log("==== parents_recommends_busy.state:" + parents_recommends_busy.state);
                            hideStatus();
                            if (parents_recommends_busy.state == "ROTATING"){
                                parents_recommends_busy.state = "NORMAL"
                                parents_recommends_animation.stop()
                            }
                            else if (packageName.length != 0) {
                                prepDeps(packageName.getText(0,packageName.length));
                                getParsRecs();
                            } else {
                                packageName.font.bold = true;
                            }
                        }
                    }
                    ListModel {
                        id: parents_recommends_m
                    }
                    Component {
                        id: parents_recommends_delegate
                        ListButton {
                            text: pkg_name
                            onClicked: {
                                prepDeps(pkg_name);
                                getParsRecs();
                            }
                        }
                    }
                    ListView {
                        id: parents_recommends_lv
                        width: units.gu(dep_width)
                        height: units.gu(dep_height)
                        anchors.top: get_parents_recommends.bottom
                        spacing: units.gu(1)
                        model: parents_recommends_m
                        delegate: parents_recommends_delegate
                    }
                }
            }
            Rectangle { //children
                id: children
                width: units.gu(deps_width)
                height: units.gu(deps_height)
                anchors.left: parents.right
                Rectangle { //child depends
                    id: col_child_depends
                    width: units.gu(deps_width/2)
                    height: units.gu(dep_height)
                    Rectangle {
                        id: child_depends_busy
                        width: units.gu(2)
                        height: units.gu(2)
                        state: "NORMAL"
                        RotationAnimator {
                            running: deps_busy
                            target: child_depends_busy
                            loops: 100000
                            from: 0
                            to: 360
                            duration: 50
                            onRunningChanged: {
                                child_depends_busy.state = "ROTATING";
                            }
                        }
                        states: [
                            State {
                                name: "NORMAL"
                                PropertyChanges { target: child_depends_busy; color: UbuntuColors.orange }
                            },
                            State {
                                name: "ROTATING"
                                PropertyChanges { target: child_depends_busy; color: UbuntuColors.purple }
                            }
                        ]
                    }
                    Button {
                        id: getDepends
                        anchors.left: child_depends_busy.right
                        text: "Depends"
                        onClicked: {
                            console.log("==== Deps clicked");
                            hideStatus();
                            if (packageName.length != 0) {
                                prepDeps(packageName.getText(0,packageName.length));
                                getDeps();
                            } else {
                                packageName.font.bold = true;
                            }
                        }
                    }
                    ListModel {
                        id: depends_m
                    }
                    Component {
                        id: dependsDelegate
                        ListButton {
                            text: pkg_name
                            onClicked: {
                                prepDeps(pkg_name);
                                getDeps();
                            }
                        }
                    }
                    ListView {
                        id: child_depends_lv
                        width: units.gu(deps_width)
                        height: units.gu(dep_height)
                        anchors.top: getDepends.bottom
                        spacing: units.gu(1)
                        model: depends_m
                        delegate: dependsDelegate
                    }
                }
                Rectangle { // child recommends
                    id: col_child_recommends
                    width: units.gu(deps_width/2)
                    height: units.gu(dep_height)
                    anchors.left: col_child_depends.right
                    Rectangle {
                        id: child_recommends_busy
                        width: units.gu(2)
                        height: units.gu(2)
                        color: UbuntuColors.orange
                        state: "NORMAL"
                        RotationAnimator {
                            running: recs_busy
                            target: child_recommends_busy
                            loops: 100000
                            from: 0
                            to: 360
                            duration: 50
                            onRunningChanged: {
                                //child_recommends_busy.color = UbuntuColors.purple;
                                child_recommends_busy.state = "ROTATING";
                            }
                        }
                        states: [
                            State {
                                name: "NORMAL"
                                PropertyChanges { target: child_recommends_busy; color: UbuntuColors.orange}
                            },
                            State {
                                name: "ROTATING"
                                PropertyChanges { target: child_recommends_busy; color: UbuntuColors.purple}
                            }
                        ]
                    }
                    Button {
                        id: getRecommends
                        anchors.left: child_recommends_busy.right
                        text: "Recommends"
                        onClicked: {
                            console.log("==== Get Recommends clicked");
                            hideStatus();
                            if (packageName.length != 0) {
                                prepDeps(packageName.getText(0,packageName.length));
                                getRecs();
                            } else {
                                packageName.font.bold = true;
                            }
                        }
                    }
                    ListModel {
                        id: recommends_m
                    }
                    Component {
                        id: recommendsDelegate
                        ListButton {
                            text: pkg_name
                            onClicked: {
                                prepDeps(pkg_name);
                                getRecs();
                            }
                        }
                    }
                    ListView {
                        id: child_recommends_lv
                        width: units.gu(deps_width)
                        height: units.gu(dep_height)
                        anchors.top: getRecommends.bottom
                        spacing: units.gu(1)
                        model: recommends_m
                        delegate: recommendsDelegate
                    }
                }
            }
        }
    }
}
