//
// Created by dev on 2023/6/20.
//

#ifndef STUDYOPENGL3_0_COMMONDEF_H
#define STUDYOPENGL3_0_COMMONDEF_H

#include <unistd.h>
#include <cmath>
#include <stdio.h>
#include <stdint.h>

typedef struct _tag_PointF {
    float x;
    float y;

    _tag_PointF() {
        x = y = 0.0f;
    }

    _tag_PointF(float xBar, float yBar) {
        x = xBar;
        y = yBar;
    }

} PointF;

class PointFUtil {
public:
    /**
     * 2 点之间的距离
     * @param p0
     * @param p1
     * @return
     */
    static float Distance(PointF p0, PointF p1) {
        // sqrt()函数用于计算一个数的平方根。
        // double base = 2.0;
        //  double exponent = 3.0;
        // double result = pow(base, exponent);
        // 2 raised to the power of 3 is 8
        return static_cast<float>(sqrt(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2)));

    }


    /**
     * 距离的平方
     * @param p0
     * @param p1
     * @return
     */
    static float DistanceSquare(PointF p0, PointF p1) {
        return static_cast<float>(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2));
    }

    /**
     * 两个点 相减法
     * @param p0
     * @param p1
     * @return
     */
    static PointF PointMinus(PointF p0, PointF p1) {
        PointF pointF;
        pointF.x = p0.x - p1.x;
        pointF.y = p0.y - p1.y;
        return pointF;
    }

    /**
  * 两个点 相加
  * @param p0
  * @param p1
  * @return
  */
    static PointF PointAdd(PointF p0, PointF p1) {
        PointF pointF;
        pointF.x = p0.x + p1.x;
        pointF.y = p0.y + p1.y;
        return pointF;
    }

    /**
* 两个点 相乘
* @param p0
* @param p1
* @return
*/
    static PointF PointTimes(PointF p0, PointF p1) {
        PointF pointF;
        pointF.x = p0.x * p1.x;
        pointF.y = p0.y * p1.y;
        return pointF;
    }

    /**
*点 乘以一个系数
* @param p0
* @param p1   系数
* @return
*/
    static PointF PointTimes(PointF p0, float p1) {
        PointF pointF;
        pointF.x = p0.x * p1;
        pointF.y = p0.y * p1;
        return pointF;
    }

    /**
*点 除以一个点
* @param p0
* @param p1
* @return
*/
    static PointF PointDivide(PointF p0, PointF p1) {
        PointF pointF;
        pointF.x = p0.x / p1.x;
        pointF.y = p0.y / p1.y;
        return pointF;
    }

    /**
*点 除以一个系数
* @param p0
* @param p1   系数
* @return
*/
    static PointF PointDivide(PointF p0, float p1) {
        PointF pointF;
        pointF.x = p0.x / p1;
        pointF.y = p0.y / p1;
        return pointF;
    }

};

#endif //STUDYOPENGL3_0_COMMONDEF_H
