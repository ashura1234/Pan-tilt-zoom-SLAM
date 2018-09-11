import math



def overlap_pan_angle(fl_1, pan_1, fl_2, pan_2, im_width):
    """
    :param fl_1: focal length in pixel
    :param pan_1:  pan angle in degree
    :param fl_2:
    :param pan_2:
    :param im_width: image with in pixel
    :return: overlapped pan angle
    """
    # overlap angle (in degree) between two cameras

    w = im_width/2
    delta_angle = math.atan(w/fl_1) * 180.0/math.pi
    pan1_min = pan_1 - delta_angle
    pan1_max = pan_1 + delta_angle


    delta_angle = math.atan(w/fl_2) * 180.0/math.pi
    pan2_min = pan_2 - delta_angle
    pan2_max = pan_2 + delta_angle

    angle1 = max(pan1_min, pan2_min)
    angle2 = min(pan1_max, pan2_max)

    return max(0, angle2 - angle1)