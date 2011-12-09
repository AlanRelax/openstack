/*
 * This program resolve domain processing
*/

#include "mylib.h"

void list_id_domain(virConnectPtr conn) {

    int ids[10];
    int maxids=10;
    int num, i;

    num = virConnectListDomains(conn, ids, maxids);

    for(i = 0;i < num;i++) {
        printf("%d\n",ids[i]);
    }
}

int get_num_domain(virConnectPtr conn) {

    int num;
    num = virConnectNumOfDomains(conn);

    return num;
}

void list_info_domain(virDomainPtr domain) {

    virDomainInfo info;
//    virVcpuInfo vinfo;
    int interval = 2;
    struct timeval startTime;
    struct timeval endTime;
    int realTime;
    int cpuTime;
    double cpuUsage;

    virDomainGetInfo(domain, &info);
//    virDomainGetVcpus(domain, &vinfo,0, NULL,0);


    unsigned long long startCpuTime = info.cpuTime;
    if (gettimeofday(&startTime, NULL) == -1) {
        printf("Failed to get start time\n");
    }
    sleep(interval);
    virDomainGetInfo(domain, &info);
    unsigned long long endCpuTime =  info.cpuTime;
    if (gettimeofday(&endTime, NULL) == -1) {
        printf("Failed to get end time\n");
    }
//    printf("%lld-----%lld\n", startCpuTime, endCpuTime);
    cpuTime = (endCpuTime - startCpuTime)/1000;
    realTime = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
    cpuUsage = cpuTime / (double)(realTime);
//    printf("%s:\n", virDomainGetName(domain));
    printf("\t\tstate is %d\n", info.state);
    printf("\t\tvCPU is %d\n", info.nrVirtCpu);
    printf("\t\tMAXmemory is %ld\n", info.maxMem/1024);
    printf("\t\tmemory is %ld\n", info.memory/1024);
    printf("\t\tcpuUsage is %.2f%\n", cpuUsage*100);


//    printf("\t\trCPU is %d\n", vinfo.cpu);
}

void list_cpu_domain() {

/*    virCPUStatsPtr params;
    int nparams = 0;
    int cpuNum = VIR_CPU_STATS_ALL_CPUS;

    struct cpu_stats {
        unsigned long long user;
        unsigned long long sys;
        unsigned long long idle;
        unsigned long long iowait;
        unsigned long long util;
    } cpu_stats[2];
    double user_time, sys_time, idle_time, iowait_time, total_time;
    double usage;

    if (virNodeGetCPUStats(conn, cpuNum, params, &nparams, 0) != 0) {
        printf("Error\n");
        return FALSE;
    }

*/
}

void list_disk_domain(virDomainPtr domain) {

    virDomainBlockStatsStruct stats;

    size_t size;
    const char *disk = "vda";
    size = sizeof(stats);
    int interval = 2;

    virDomainBlockStats(domain, disk, &stats, size);
    long long start_rd_bytes = stats.rd_bytes;
    long long start_wr_bytes = stats.wr_bytes;
    sleep(interval);
    virDomainBlockStats(domain, disk, &stats, size);
    long long end_rd_bytes = stats.rd_bytes;
    long long end_wr_bytes = stats.wr_bytes;
    
    long rd_bytes = end_rd_bytes - start_rd_bytes;
    long wr_bytes = end_wr_bytes - start_wr_bytes;
    int rd_usage = rd_bytes/interval;
    int wr_usage= wr_bytes/interval;
//    printf("%s:\n", virDomainGetName(domain));
    printf("\t\tread: %dbytes/s\n", rd_usage);
    printf("\t\twrite: %dbytes/s\n", wr_usage);
    printf("\t\trd_req: %lld\n", stats.rd_req);
    printf("\t\trd_bytes: %lld\n", stats.rd_bytes);
    printf("\t\twr_req: %lld\n", stats.wr_req);
    printf("\t\twr_bytes: %lld\n", stats.wr_bytes);
}

int list_network_domain(virDomainPtr domain) {

    const char *path;
    virDomainInterfaceStatsStruct stats;
    size_t size;

    size = sizeof(stats);
    path = "vnet1";
    int interval = 2;

    if( virDomainInterfaceStats(domain, path, &stats, size) )
        return FALSE; 
    long long start_rx_bytes = stats.rx_bytes;
    long long start_tx_bytes = stats.tx_bytes;

    sleep(interval);

    if( virDomainInterfaceStats(domain, path, &stats, size) )
        return FALSE; 
    long long end_rx_bytes = stats.rx_bytes;
    long long end_tx_bytes = stats.tx_bytes;

    int rx_usage = (end_rx_bytes - start_rx_bytes)/interval;
    int tx_usage = (end_tx_bytes - start_tx_bytes)/interval;

    printf("\t\trx usage: %d bytes/s", rx_usage);
    printf("\trx bytes: %lld bytes", stats.rx_bytes);
    printf("\t\trx packets: %lld", stats.rx_packets);
    printf("\trx errs: %lld\n", stats.rx_errs);
    printf("\t\ttx usage: %d bytes/s", tx_usage);
    printf("\ttx bytes: %lld bytes", stats.tx_bytes);
    printf("\t\ttx packets: %lld", stats.tx_packets);
    printf("\ttx errs: %lld\n", stats.tx_errs);
}
