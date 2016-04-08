
#include"loading.h"
#include"execution.h"
#include"result.h"
using namespace std;

int main(void)
{
    loading loads;
    loads.load();

    execution exe;
    result results;
    fstream outputs, errors;
    outputs.open("snapshot.rpt", ios::out | ios::binary );
    errors.open("error_dump.rpt", ios::out | ios::binary );

 
    while(1){

        results.output(outputs);

        exe.fetch(declaration::PC);
        exe.decode_execute();

        results.errorDetect(errors);


        if( declaration::halt == 1 )
            break;
    }

    outputs.close();
    return 0;


}
