clear all
Jacobian_sym = calculateSymbolicJacobian;
joint_angles = pi/180*[0, 110, -90, 0, 0, 0]; % Replace these with your joint angle values
link_lengths = [0.10, 0.150, 0.05, 0.01, 0.10];
% J_num1 = (subs(Jacobian_sym, [q1, q2, q3, q4, q5, q6], joint_angles));
Jnum2 = evaluateJacobian(Jacobian_sym, joint_angles)


function J_num = evaluateJacobian(J_sym,joint_angles)
    syms q1 q2 q3 q4 q5 q6 real
    J_num = double(subs(J_sym, symvar(J_sym), joint_angles));
end

function J = calculateSymbolicJacobian
syms q1 q2 q3 q4 q5 q6 b l1 l2 l3 l4 l5 l6 real
Sym_T = TranslationMatrix_Symbolic;
link_lengths = [0.04, 0.05, 0.10, 0.150, 0.05, 0.01, 0.10];
Sym_T = subs(Sym_T, [b, l1, l2, l3, l4, l5, l6], link_lengths);

X_pos = Sym_T(1,4);
Y_pos = Sym_T(2,4);
Z_pos = Sym_T(3,4);
% Extract the rotation matrix R from the transformation matrix Sym_T
R = Sym_T(1:3, 1:3);
% Compute symbolic Euler angles (XYZ)
% EulerAngles = [atan2(R(3,2), R(3,3));
%                -asin(R(3,1)); 
%                atan2(R(2,1), R(1,1))
%               ] ;

EulerAngles = [atan2(R(3,2), R(3,3));
               atan2(-1*(R(3,1)), sqrt(R(3,2)*R(3,2) + R(3,3)*R(3,3))); 
               atan2(R(2,1), R(1,1))
              ];

%Define the Task Space variables 
TaskSpace = [   EulerAngles(1);
                EulerAngles(2);
                EulerAngles(3);
                X_pos;
                Y_pos;
                Z_pos
            ];

%define the joint space variables
jointSpace = [  q1;
                q2;
                q3;
                q4;
                q5;
                q6;
             ];

%Compute the Symbolic Jacobian
J = sym(zeros(numel(TaskSpace), numel(jointSpace)));
for i = 1:numel(TaskSpace)
    for j = 1:numel(jointSpace)
        J(i,j) = diff(TaskSpace(i), jointSpace(j)); 
    end
end
% J = jacobian(TaskSpace, jointSpace)

joint_angles = pi/180*[0, 0, 0, 0, 0, 0]; % Replace these with your joint angle values
 
X_num = (subs(X_pos , [q1, q2, q3, q4, q5, q6], joint_angles));
Y_num = (subs(Y_pos , [q1, q2, q3, q4, q5, q6], joint_angles));
Z_num = (subs(Z_pos , [q1, q2, q3, q4, q5, q6], joint_angles));
EulerAngles_num = (subs(EulerAngles , [q1, q2, q3, q4, q5, q6], joint_angles));
Sym_T_num = double(subs(Sym_T, symvar(Sym_T), [joint_angles]))

X_pos = Sym_T_num(1,4)
Y_pos = Sym_T_num(2,4)
Z_pos = Sym_T_num(3,4)
eulXYZ = 180/pi*rotm2eul(Sym_T_num(1:3,1:3),'XYZ')'
end

function Sym_T = TranslationMatrix_Symbolic
syms q1 q2 q3 q4 q5 q6 b l1 l2 l3 l4 l5 l6   %Robot joint angle and link lengths

%Declare the variables
Sym_Cosines = [ cos(q1), cos(q2), cos(q3), cos(q4), cos(q5), cos(q6)];
Sym_Sines = [ sin(q1), sin(q2), sin(q3), sin(q4), sin(q5), sin(q6)];
Sym_link_lengths = [ b, l1, l2, l3, l4, l5, l6];

%form the Transformation matrices
Sym_T01 = [ Sym_Cosines(1)  -Sym_Sines(1)       0       0;
            Sym_Sines(1)     Sym_Cosines(1)     0       0;
                    0            0              1       Sym_link_lengths(1);
                    0            0              0       1
          ];

Sym_T12 = [ 1            0              0       0;
            0     Sym_Cosines(2)     -Sym_Sines(2)      0;
            0      Sym_Sines(2)     Sym_Cosines(2)      Sym_link_lengths(2);
            0            0              0       1
          ];

Sym_T23 = [ 1            0              0       0;
            0    Sym_Cosines(3)      -Sym_Sines(3)      Sym_link_lengths(3);
            0      Sym_Sines(3)     Sym_Cosines(3)      0;
            0            0              0       1
          ];
      
Sym_T34 = [ 1            0              0       0;
            0    Sym_Cosines(4)      -Sym_Sines(4)      Sym_link_lengths(4);
            0      Sym_Sines(4)     Sym_Cosines(4)      0;
            0            0              0       1
          ];


Sym_T45 = [ Sym_Cosines(5)   0       Sym_Sines(5)      0;
                0            1              0       Sym_link_lengths(5);
            -Sym_Sines(5)    0      Sym_Cosines(5)      0;
                0            0              0       1
           ];

Sym_T56 = [ Sym_Cosines(6)  -Sym_Sines(6)       0       0;
            Sym_Sines(6)     Sym_Cosines(6)     0       Sym_link_lengths(6);
                    0            0              1       0;
                    0            0              0       1
          ];
  
Sym_T67 = [ 1            0              0       0;
            0            1              0       0;
            0            0              1       -1*Sym_link_lengths(7);
            0            0              0       1
          ];

Sym_T = Sym_T01 * Sym_T12 * Sym_T23 * Sym_T34 * Sym_T45 * Sym_T56 * Sym_T67;
% disp('Tranfomration Matrix Symbolic:');
% disp(Sym_T);
end