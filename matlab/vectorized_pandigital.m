% vectorized_pandigital.m
% Finds the largest 1–9 pandigital concatenated product k×(1,2)

% 1. Generate all k from 1 to 9999
k = (1:9999)';

% 2. Compute k×2, digit counts, and the concatenated number in one shot
p2    = 2 * k;                         % k × 2
d2    = floor(log10(p2)) + 1;          % number of digits of p2
shift = 10 .^ d2;                      % shift factors (vector)
concat = k .* shift + p2;              % concatenated k×1||k×2

% 3. Filter exactly 9 digits
valid9 = (concat >= 1e8) & (concat < 1e9);
cands  = concat(valid9);
kvals  = k(valid9);

% 4. Convert to zero-padded 9-char strings and sort each row
strs       = compose('%09d', cands);         % string array, N×1
charMat    = char(strs);                     % N×9 character array
sortedMat  = sort(charMat, 2);               % sort digits in each row

% 5. Test for pandigital "123456789"
isPandig   = all(sortedMat == repmat('1':'9', size(sortedMat,1), 1), 2);

% 6. Extract the maximum
panVals = cands(isPandig);
panKs   = kvals(isPandig);
[maxVal, idx] = max(panVals);
bestK = panKs(idx);
bestN = 2;  % by construction

fprintf('Largest pandigital: %d  (k = %d, n = %d)\n', maxVal, bestK, bestN);
