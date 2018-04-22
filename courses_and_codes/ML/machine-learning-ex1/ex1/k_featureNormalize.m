function [X_norm, mu, sigma] = k_featureNormalize(X)

X_norm = X;
mu = zeros(1, size(X, 2));
sigma = zeros(1, size(X, 2));
    
mu = mean(X);
X_norm = X-mu;
sigma = std(X);
for i=1:size(X,1)
  for j=1:size(X,2)
    X_norm(i,j) = X_norm(i,j)/sigma(j);
  
%fprintf('X_norm:\n');
%fprintf('%f %f\n',X_norm);

end
