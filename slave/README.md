# �����Ķ�

## ֹͣת�ٿ���

1. ָֹͣ��
2. Ӳ������
3. д EEPROM
4. �������
5. ���¹���
6. Ƿѹ����
7. ��ѹ����
8. ��������
9. ���ٹ���
10. �����л�

## ֹͣ����

1. ���ٹ���
2. ��������
3. ��ѹ����
4. Ƿѹ����
5. ���¹���
6. �������
7. Ӳ������

## ��������

1. ͨѶ��������ָ��
2. �������б�־������ת�٣����� Isd_Set = 0��TorqueAngleA = 0�� ����ת�����б�־Ϊ 1������λ�ÿ��Ʊ�־λ 0������ֹͣת�ٿ���Ϊ 0������ת�ؿ���Ϊ 1
3. ת�ٿ��ƣ�ת�ٻ���
4. ת�ؿ��ƣ���������


+-----------------+
|  speedControl() |
+-----------------+
         |
         v
+-----------------+
| �ٶȻ�����      |
|                 |
| if (Veloover_Set - Velo_Set > Velo_PerAdd) |
|   Velo_Set += Velo_PerAdd                  |
| else if (Velo_Set - Veloover_Set > Velo_PerAdd) |
|   Velo_Set -= Velo_PerAdd                  |
| else                                       |
|   Velo_Set = Veloover_Set                  |
+-----------------+
         |
         v
+-----------------+
| PI ���������ڴ���|
|                 |
| pi_velo.ctrl_index++                    |
| if (pi_velo.ctrl_index >= pi_velo.ctrl_period) |
|   pi_velo.ctrl_index = 0                |
|   pi_velo.pi_ref_reg = Velo_Set         |
|   pi_velo.pi_fdb_reg = Velo_Elec        |
|   pi_velo.calc(&pi_velo)                |
+-----------------+
         |
         v
+-----------------+
| ����б�ʵ���    |
|                 |
| if (IdSet >= 0) |
|   if (Isd_Set - IdSet > Is_PerAdd)       |
|     IdSet += Is_PerAdd                   |
|   else if (IdSet - Isd_Set > Is_DownPerAdd) |
|     IdSet -= Is_DownPerAdd               |
|   else                                   |
|     IdSet = Isd_Set                      |
| else                                     |
|   if (Isd_Set - IdSet > Is_DownPerAdd)   |
|     IdSet += Is_DownPerAdd               |
|   else if (IdSet - Isd_Set > Is_PerAdd)  |
|     IdSet -= Is_PerAdd                   |
|   else                                   |
|     IdSet = Isd_Set                      |
+-----------------+
         |
         v
+-----------------+
| ���� Id_Set �� Iq_Set |
|                     |
| Id_Set = -_IQmpy(_IQabs(pi_velo.pi_out_reg), _IQsinPU(TorqueAngleA)) + IdSet |
| Iq_Set = -_IQmpy(pi_velo.pi_out_reg, _IQcosPU(TorqueAngleA)) |
| Isdq_Set2 = Iq_Set |
+-----------------+
         |
         v
+-----------------+
| ����            |
+-----------------+
