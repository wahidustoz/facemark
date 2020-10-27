Figure* calFigure(Figure* fg, double* arr, int size)
{
    double sum = 0;
    double mean, mode, median, variance, stdDev, skewness = 0;
    int count = 1, freq = 1;
    int i, j, temp;
    //mean
    for (i = 0; i < size; i++) sum += arr[i];
    mean = sum / (double)size;
    //mode
    for (i = 0; i < size; i++)
    {
        freq = 1;
        for (j = i + 1; j < size; j++)
            if (arr[i] == arr[j]) freq++;
        if (freq >= count) 
        {
            mode = arr[i];
            count = freq;
        }
    }
    //median
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j + 1] < arr[j])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    if (size / 2 == 0)
        median = (arr[size / 2] + arr[size / 2 + 1]) / 2;
    else
        median = arr[(size + 1) / 2];
    //variance
    sum = 0;
    for (i = 0; i < size; i = i + 1)
        sum += (arr[i] - mean) * (arr[i] - mean);
    variance = sum / (size - 1);
    //stdDev
    stdDev = sqrt(variance);
    //skewness
    for (i = 0; i < size; i++)
        skewness += (arr[i] - mean) * (arr[i] - mean) * (arr[i] - mean);
    skewness = skewness / (size * stdDev * stdDev * stdDev);
    
    fg->mean = mean;
    fg->variance = variance;
    fg->stdDev = stdDev;
    fg->mode = mode;
    fg->median = median;
    fg->skewness = skewness;

    return fg;
}
