%     plotData.m
%     gradientDescentMulti.m
%     computeCostMulti.m
%     featureNormalize.m
%     normalEqn.m

%% Initialization

%% ================ Part 1: Feature Normalization ================
%% Clear and Close Figures
clear ; close all; clc
fprintf('Loading data ...\n');
%% Load Data
%% Input Needed: x1,x2,x3,x4,x5,y as .csv
data = load('ex1data2.txt');
X = data(:, 1:5);
y = data(:, 6);
m = length(y);

% Print out some data points
fprintf('First 10 examples from the dataset: \n');
fprintf(' x = [%.0f %.0f], y = %.0f \n', [X(1:10,:) y(1:10,:)]');

fprintf('Program paused. Press enter to continue.\n');
pause;

% Scale features and set them to zero mean
fprintf('Normalizing Features ...\n');

[X mu sigma] = k_featureNormalize(X);
fprintf(' mu = [%.0f %.0f], sigma = [%.0f %.0f] \n', [mu sigma]');

fprintf('Program paused. Press enter to continue.\n');
pause;

% Add intercept term to X
X = [ones(m, 1) X];


%% ================ Part 2: Gradient Descent ================
fprintf('Running gradient descent ...\n');

% Choose some alpha value
alpha = 0.10;
num_iters = 500;

% Init Theta and Run Gradient Descent 
theta = zeros(6, 1);
[theta, J_history] = k_gradientDescentMulti(X, y, theta, alpha, num_iters);

% Plot the convergence graph
figure;
plot(1:numel(J_history), J_history, '-b', 'LineWidth', 2);
xlabel('Number of iterations');
ylabel('Cost J');

% Display gradient descent's result
fprintf('Theta computed from gradient descent: \n');
fprintf(' %f \n', theta);
fprintf('\n');

% Estimate the price of a 1650 sq-ft, 3 br house
% ====================== YOUR CODE HERE ======================
% Recall that the first column of X is all-ones. Thus, it does
% not need to be normalized.
visitor = theta(1);
for i=2:6
  visitor = visitor + theta(i)*(<?>-mu(i))/sigma(i);

% ============================================================

fprintf(['Predicted price of ~~~ ' ...
         '(using gradient descent):\n $%f\n'], visitor);

fprintf('Program paused. Press enter to continue.\n');
pause;

%% ================ Part 3: Normal Equations ================

fprintf('Solving with normal equations...\n');

%% Load Data
data = csvread('ex1data2.txt');
X = data(:, 1:5);
y = data(:, 6);
m = length(y);

% Add intercept term to X
X = [ones(m, 1) X];

% Calculate the parameters from the normal equation
theta = normalEqn(X, y);

% Display normal equation's result
fprintf('Theta computed from the normal equations: \n');
fprintf(' %f \n', theta);
fprintf('\n');


% Estimate the price of a 1650 sq-ft, 3 br house
% ====================== YOUR CODE HERE ======================
visitor = theta(1) + theta(2)*<?> + theta(3)* <?> + ...
price = theta(1)+theta(2)*1650+theta(3)*3;; % You should change this


% ============================================================

fprintf(['Predicted price of ~~~ ' ...
         '(using normal equations):\n $%f\n'], visitor);

