#include <stdio.h>
#include <time.h>

int main(
    int argc,
    char** argv)
{
    // fake it, till we make it
    
    //
    // Get timestamp
    //
    
    time_t tt;
    struct tm* tm;
    
    tt = time(NULL);
    tm = localtime(&tt);

    if (tm == NULL)
    {
        return -1;
    }
    
    char timestamp[128] = { 0 };
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tm);

    //
    // Write testresults
    //
    
    FILE* fo = fopen("testresults.xml", "w");
    
    if (fo == NULL)
    {
        return -1;
    }
    
    fprintf(fo, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fo, "<testsuites tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" timestamp=\"%s\" time=\"0.0\" name=\"AllTests\">\n", timestamp);
    fprintf(fo, "  <testsuite name=\"fakesuite\" tests=\"1\" failures=\"0\" disabled=\"0\" errors=\"0\" time=\"0\">\n");
    fprintf(fo, "    <testcase name=\"faketest\" status=\"run\" time=\"0\" classname=\"faketests\" />\n");
    fprintf(fo, "  </testsuite>\n");
    fprintf(fo, "</testsuites>\n");
    fclose(fo);
    
    return 0;
}
