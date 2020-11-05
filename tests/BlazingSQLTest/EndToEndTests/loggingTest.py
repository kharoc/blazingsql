from Configuration import ExecutionMode
from Configuration import Settings as Settings
from pynvml import nvmlInit
from Runner import runTest
from Utils import Execution,  gpuMemory, init_context, skip_test

queryType = "Logging Test"


def main(dask_client, dir_data_file, bc, nRals):

    start_mem = gpuMemory.capture_gpu_memory_usage()

    def executionTest():
        # Run Query ------------------------------------------------------

        print("==============================")
        print(queryType)
        print("==============================")

        queryId = "TEST_01"
        query = """select * from bsql_logs"""
        runTest.run_query_log(
            bc,
            query,
            queryId,
            queryType,
        )

        queryId = "TEST_02"
        query = """SELECT log_time, query_id, duration FROM bsql_logs WHERE info = 'Query Execution Done' ORDER BY log_time DESC"""
        runTest.run_query_log(
            bc,
            query,
            queryId,
            queryType,
        )

        if Settings.execution_mode == ExecutionMode.GENERATOR:
            print("==============================")

    executionTest()

    end_mem = gpuMemory.capture_gpu_memory_usage()

    gpuMemory.log_memory_usage(queryType, start_mem, end_mem)


if __name__ == "__main__":

    Execution.getArgs()

    nvmlInit()

    bc, dask_client = init_context()

    nRals = Settings.data["RunSettings"]["nRals"]

    main(dask_client, Settings.data["TestSettings"]["dataDirectory"],
         bc, nRals)

    if Settings.execution_mode != ExecutionMode.GENERATOR:
        runTest.save_log()
        gpuMemory.print_log_gpu_memory()
