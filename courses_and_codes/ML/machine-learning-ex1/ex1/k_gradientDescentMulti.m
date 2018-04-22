function [theta, J_history] = k_gradientDescentMulti(X, y, theta, alpha, num_iters)


% Initialize some useful values
m = length(y); % number of training examples
J_history = zeros(num_iters, 1);

for iter = 1:num_iters
    delta = zeros(size(X,2),1);
    dd = 0;
    for j=1:size(X,2)
      dd = dd + theta(j)*X(i,j);
    for i = 1:m
      for j = 1:size(X,2)
        delta(j) = delta(j) + 1/m*(dd-y(i))*X(i,j);
    end;
    theta = theta - alpha*delta;
    % Save the cost J in every iteration (for debug)   
    J_history(iter) = k_computeCostMulti(X, y, theta);

end
end
