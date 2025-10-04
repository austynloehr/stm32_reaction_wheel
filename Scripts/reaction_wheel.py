import numpy as np
import matplotlib.pyplot as plt

class ReactionWheel:
    def __init__(self,
                 theta_0: float,
                 phi_dot_0: float,
                 mass: float,
                 l_cg: float,
                 j_body: float,
                 j_motor: float,
                 k_motor: float,
                 b_motor: float,
                 gear_ratio: float):
        
        # System parameters
        self.mass = mass  # kg
        self.l_cg = l_cg  # m
        self.j_body = j_body  # kg*m^2
        self.j_motor = j_motor  # kg*m^2
        self.k_motor = k_motor  # N*m/A
        self.b_motor = b_motor  # N*m/(rad/s)
        self.gear_ratio = gear_ratio  # unitless

        # State variables
        self.theta = theta_0  # rad, body angle
        self.theta_dot = 0.0  # rad/s, body angular velocity
        self.theta_ddot = 0.0  # rad/s^2, body angular acceleration
        self.phi_dot = phi_dot_0  # rad/s, wheel angular velocity.  
        self.phi_ddot = 0.0  # rad/s^2, initial

        # Control input 
        self.tau_m = 0.0  # N*m, net motor torque
        self.tau_fw = 0.0  # N*m, net torque applied to flywheel

    def update(self, current_cmd: float, dt: float):
        # Calculate motor torque
        self.tau_m = self.k_motor * current_cmd - self.b_motor * self.phi_dot / self.gear_ratio
        self.tau_fw = self.gear_ratio * self.tau_m
        print(f"Current Cmd: {current_cmd:.2f}, Tau_m: {self.tau_m:.4f}, Tau_fw: {self.tau_fw:.4f}")

        # Calculate body angular acceleration
        self.theta_ddot = self.mass * 9.81 * self.l_cg * np.sin(self.theta) / self.j_body + self.tau_m / self.j_body

        # Update angular acceleration of the wheel
        self.phi_ddot = self.tau_fw / self.j_motor - self.theta_ddot

        # Update angular velocity of the wheel
        self.phi_dot += self.phi_ddot * dt

        # Update body angular velocity and position
        self.theta_dot += self.theta_ddot * dt
        self.theta += self.theta_dot * dt

if __name__ == "__main__":
    dt = 0.001  # time step
    theta_0 = np.deg2rad(10)  # initial body angle
    phi_dot_0 = 0.0  # initial wheel angular velocity
    mass = 1.0  # kg
    l_cg = 0.05  # m
    j_body = 0.01  # kg*m^2
    j_motor = 0.001  # kg*m^2
    k_motor = 0.05  # N*m/A
    b_motor = 0.001  # N*m/(rad/s)
    gear_ratio = 3  # unitless

    rw = ReactionWheel(theta_0=theta_0,
                       phi_dot_0=phi_dot_0,
                       mass=mass,
                       l_cg=l_cg,
                       j_body=j_body,
                       j_motor=j_motor,
                       k_motor=k_motor,
                       b_motor=b_motor,
                       gear_ratio=gear_ratio)
    
    t_array = np.arange(0, 2, dt)
    n = t_array.size

    # Preallocate arrays (store degrees for easier plotting)
    theta = np.zeros(n)
    theta_dot = np.zeros(n)
    theta_ddot = np.zeros(n)
    phi_dot = np.zeros(n)

    for i, t in enumerate(t_array):
        rw.update(0, dt)
        theta[i] = np.rad2deg(rw.theta)
        theta_dot[i] = np.rad2deg(rw.theta_dot)
        theta_ddot[i] = np.rad2deg(rw.theta_ddot)
        phi_dot[i] = np.rad2deg(rw.phi_dot)

    # Create 4x1 stacked subplot figure (shared x-axis)
    fig, axs = plt.subplots(4, 1, figsize=(8, 10), sharex=True)

    axs[0].plot(t_array, theta, color='C0')
    axs[0].set_title('theta (deg)')
    axs[0].grid(True)

    axs[1].plot(t_array, theta_dot, color='C1')
    axs[1].set_title('theta_dot (deg/s)')
    axs[1].grid(True)

    axs[2].plot(t_array, theta_ddot, color='C2')
    axs[2].set_title('theta_ddot (deg/s^2)')
    axs[2].grid(True)

    axs[3].plot(t_array, phi_dot, color='C3')
    axs[3].set_title('phi_dot (deg/s)')
    axs[3].set_xlabel('time (s)')
    axs[3].grid(True)

    plt.tight_layout()
    plt.show()

