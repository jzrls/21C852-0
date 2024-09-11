import numpy as np

ia = np.random.random(1)
ib = np.random.random(1)
ic = np.random.random(1)


def clark_transform_power(v_a, v_b, v_c):
    # Clark�任����
    clark_matrix = np.array(
        [[2.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0], [0, 1.0 / np.sqrt(3), -1.0 / np.sqrt(3)]]
    )

    # ����Clark�任��ĵ�ѹ/����
    v_alpha_beta = np.dot(clark_matrix, [v_a, v_b, v_c])

    return v_alpha_beta[0], v_alpha_beta[1]
