#include "ros/ros.h"
#include <nodelet/loader.h>

#include <string>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ywen_flir_node");

    nodelet::Loader loader;

    // typedef std::map<std::string, std::string> M_string;
    // typedef std::vector<std::string> V_string;
    // See https://github.com/ros/nodelet_core/blob/noetic-devel/nodelet/include/nodelet/loader.h
    nodelet::M_string remap(ros::names::getRemappings());
    nodelet::V_string nargv;

    // `load`: Load a nodelet.
    loader.load(
        // name
        ros::this_node::getName(),
        // type
        "ywen_flir/FlirCameraNodelet",
        // remappings
        remap,
        // my_argv
        nargv);

    ros::spin();

    return 0;
}
