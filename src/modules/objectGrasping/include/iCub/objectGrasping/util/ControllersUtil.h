#ifndef __ICUB_OBJECTGRASPING_CONTROLLERSUTIL_H__
#define __ICUB_OBJECTGRASPING_CONTROLLERSUTIL_H__

#include <iCub/objectGrasping/ObjectGraspingEnums.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/IEncoders.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/IInteractionMode.h>
#include <yarp/dev/IImpedanceControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/os/ResourceFinder.h>
#include <yarp/sig/Vector.h>

#include <vector>

namespace iCub {
    namespace objectGrasping {

        class ControllersUtil {

        private:

            yarp::dev::PolyDriver clientArm;
            yarp::dev::PolyDriver clientArmCartContr;

            yarp::dev::IEncoders *iEncs;
            yarp::dev::IControlMode2 *iCtrl;
            yarp::dev::IPositionControl *iPos;
            yarp::dev::IVelocityControl *iVel;
            yarp::dev::IInteractionMode *iInt;
            yarp::dev::IImpedanceControl *iImp;

            yarp::dev::ICartesianControl *iCart;

            yarp::sig::Vector armStoredPosition;
            int armJointsNum;
            std::vector<int> jointsStoredControlMode;
            std::vector<int> handJointsToMove;
            std::string whichHand;

            yarp::sig::Vector xInit, oInit;
            double incrOffset, goToXYTrajTime, goDownOffset, goDownTrajTime, goUpTrajTime, waitDown;
            std::vector<double> storedJointStiffness,storedJointDumping;
            double newJointStiffness,newJointDumping;

            /* ****** Debug attributes                              ****** */
            std::string dbgTag;

        public:

            ControllersUtil();

            bool init(yarp::os::ResourceFinder &rf);

            bool saveCurrentArmPosition();

            bool saveCurrentControlMode();

            bool saveCurrentPose();

            bool saveCurrentStiffness();
            bool setStiffness();
            bool restoreStiffness();

            bool testCartesianController();

            bool setArmInStartPosition(bool cartesianMode);

            bool setArmInGraspPosition(bool cartesianMode, bool back);

            bool raiseArm(bool cartesianMode);

            bool goToXY(int x, int y);

            bool goDown();

            bool restorePreviousArmPosition();

            bool restorePreviousControlMode();

            bool release();

            bool openHand();

            bool moveFingers();

            bool incrementEndEffectorPosition(double incrX, double incrY, double incrZ, double seconds);

            bool setPositionControlModeToArm(bool excludeHand, bool checkCurrent);

            bool disableTorsoJoints();

            bool enableTorsoJoints();

        private:

            bool waitMoveDone(const double &i_timeout, const double &i_delay);

            bool waitMoveDone(const double &i_timeout, const double &i_delay, bool excludeHand);

            bool setControlMode(int joint, int controlMode, bool checkCurrent);
        };
    } //namespace objectGrasping
} //namespace iCub

#endif

