import numpy as np
from filterpy.common import Q_discrete_white_noise

dt = .002

theta_0 = 45
thetaDot_0 = 0
thetaDotDot_0 = 0

pTheta = 2
pThetaDot = 10
pThetaDotDot = 10

stdQ = 10
stdWz = 8
stdTheta = 8

X = np.array([[theta_0,thetaDot_0,thetaDotDot_0]]).T
P = np.array([[pTheta,0,0],
              [0,pThetaDot,0],
              [0,0,pThetaDotDot]])

F = np.array([[1,dt,.5*dt**2],
                [0,1,dt],
                [0,0,1]])
Q = Q_discrete_white_noise(dim=3,dt=dt,var=stdQ**2)

H = np.array([[1,0,0],
              [0,1,0]])

Z = np.array([[-88.4978],
              [-.103166]])

R = np.array([[stdTheta**2, 0],
              [0, stdWz**2]])

I = np.eye(3)

X_Prior = np.dot(F,X)
P_Prior = np.dot(np.dot(F,P),F.T) + Q

X = X_Prior
P = P_Prior

HX = np.dot(H,X)
y = Z - HX
PH_t = np.dot(P,H.T)
HPH = np.dot(H,PH_t)
HPH_R = HPH + R
HPH_R_inv = np.linalg.inv(HPH + R)
K = np.dot(PH_t,HPH_R_inv)
X_Post = X + np.dot(K,y)

I_KH = I - np.dot(K,H)
P_1 = np.dot(np.dot(I_KH,P),I_KH.T)
KRK_t = np.dot(np.dot(K,R),K.T)
P_Post = P_1 + KRK_t

print('F:')
print(F)
print('Q:')
print(Q)
print('X_Prior:')
print(X_Prior)
print('P_Prior:')
print(P_Prior)
print('HX:')
print(HX)
print('y:')
print(y)
print('K:')
print(K)
print('X_Post:')
print(X_Post)
print('P_Post:')
print(P_Post)