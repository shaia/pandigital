% pandigital_vectorized_full.m
% Finds the largest 1–9 pandigital concatenated product k×(1,2,…,n), n>1

ks = (1:9999)';   % all base integers

%% --- CASE n = 2 ---
p2    = 2*ks;                                 % k×2
d2    = floor(log10(p2)) + 1;                 % digit-length of each p2
shift2 = 10.^d2;                              % per-k shift
c2    = ks .* shift2 + p2;                    % concatenated k||2k
mask2 = (c2>=1e8)&(c2<1e9);                   % exactly 9 digits
c2    = c2(mask2); k2 = ks(mask2);

%% --- CASE n = 3 ---
% first build the n=2 partial concatenation
x12   = ks .* shift2 + p2;                    % same as c2 before filtering
p3    = 3*ks;
d3    = floor(log10(p3)) + 1;                 % digit-length of each 3k
shift3 = 10.^d3;
c3    = x12 .* shift3 + p3;                   % concatenated k||2k||3k
mask3 = (c3>=1e8)&(c3<1e9);                   % exactly 9 digits
c3    = c3(mask3); k3 = ks(mask3);

%% --- COMBINE AND FILTER PANDIGITAL ---
% Stack results with their respective n values
allVals = [c2, k2, repmat(2, numel(c2), 1);
           c3, k3, repmat(3, numel(c3), 1)];

% Convert to zero-padded strings, sort digits, and test
S = compose('%09d', allVals(:,1));            % Nx1 string array
C = char(S);                                   % Nx9 char matrix
isPan = all( sort(C,2) == repmat('1':'9', size(C,1),1), 2);

% Pick the maximum
cand = allVals(isPan,:);
[~, idx] = max(cand(:,1));
maxVal   = cand(idx,1);
bestK    = cand(idx,2);
bestN    = cand(idx,3);

fprintf('Largest pandigital: %d  (k = %d, n = %d)\n', maxVal, bestK, bestN);
