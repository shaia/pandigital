% MATLAB: Largest 1–9 Pandigital Concatenated Product

maxVal = 0;
bestK = 0;
bestN = 0;

for k = 1:9999
    s = '';
    n = 1;
    % build concatenated product string until length ≥ 9
    while length(s) < 9
        s = [s, num2str(k * n)];
        n = n + 1;
    end
    % if exactly 9 digits and pandigital, update max
    if length(s) == 9 && all(sort(s) == '123456789')
        val = str2double(s);
        if val > maxVal
            maxVal = val;
            bestK = k;
            bestN = n - 1;
        end
    end
end

fprintf('Largest pandigital: %d (k = %d, n = %d)\n', ...
        maxVal, bestK, bestN);
