#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

#include <ywen_flir/FlirConfig.h>
#include <dynamic_reconfigure/server.h>

namespace ywen_flir
{
class FlirCameraNodelet : public nodelet::Nodelet
{
public:
    FlirCameraNodelet()
    {
        // Empty
    }

    ~FlirCameraNodelet()
    {
        // Empty
    }

    void onInit()
    {
        ros::NodeHandle& pnh = getMTPrivateNodeHandle();

        // Read the parameters.
        bool retrieved = false;

        // For `ros::NodeHandle::param`, see:
        // https://docs.ros.org/en/noetic/api/roscpp/html/classros_1_1NodeHandle.html#a4d5ed8b983652e587c9fdfaf6c522f3f
        std::string frame_id;
        retrieved = pnh.param<std::string>("frame_id", frame_id, "camera");
        NODELET_INFO("param frame_id: %s", frame_id.data());

        bool frame_rate_auto = false;
        retrieved = pnh.param<bool>(
            "acquisition_frame_rate_enable", frame_rate_auto, false);
        NODELET_INFO(
            "param frame_rate_auto: %s", (frame_rate_auto ? "true" : "false"));

        double frame_rate = 1;
        retrieved = pnh.param<double>("acquisition_frame_rate", frame_rate, 1);
        NODELET_INFO("param frame_rate: %lf", frame_rate);

        // Set up dynamic reconfiguration.
        // How the `set_parameters` service is published:
        // https://github.com/ros/dynamic_reconfigure/blob/noetic-devel/include/dynamic_reconfigure/server.h#L182-L183
        dyn_reconfig_srv_ = std::make_shared<
            dynamic_reconfigure::Server<ywen_flir::FlirConfig> >(pnh);
        dynamic_reconfigure::Server<ywen_flir::FlirConfig>::CallbackType f =
            boost::bind(
                &ywen_flir::FlirCameraNodelet::reconfigure,
                this, _1, _2);

        dyn_reconfig_srv_->setCallback(f);
    }

    void reconfigure(const ywen_flir::FlirConfig& config, uint32_t level)
    {
        NODELET_INFO("reconfiguring the nodelet");
        NODELET_INFO("reconfiguration is done");
    }

private:
    std::shared_ptr<
        dynamic_reconfigure::Server<ywen_flir::FlirConfig>
    > dyn_reconfig_srv_;
};

PLUGINLIB_EXPORT_CLASS(
    ywen_flir::FlirCameraNodelet,
    nodelet::Nodelet)  // Needed for Nodelet declaration
}   // namespace ywen_flir

