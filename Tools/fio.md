# fio

References:
- [Sample FIO Commands for Block Volume Performance Tests on Linux-based Instances](https://docs.oracle.com/en-us/iaas/Content/Block/References/samplefiocommandslinux.htm)

## IOPS Performance Tests

### Test random reads

```
sudo fio --filename=<device name> --direct=1 --rw=randread --bs=4k --ioengine=libaio --iodepth=256 --runtime=10 --numjobs=4 --time_based --group_reporting --name=iops-test-job --eta-newline=1 --readonly
```

### Test file random read/writes

```
sudo fio --filename=/media/ywen/writable/fio/hello --size=50MB --direct=1 --rw=randrw --bs=4k --ioengine=libaio --iodepth=256 --runtime=10 --numjobs=4 --time_based --group_reporting --name=iops-test-job --eta-newline=1
```

### Test random read/writes

The device must be in one of the two states:
- Unmounted
- Mounted with `allow_mounted_write`.

```
sudo fio --filename=<device name> --direct=1 --rw=randrw --bs=4k --ioengine=libaio --iodepth=256 --runtime=10 --numjobs=4 --time_based --group_reporting --name=iops-test-job --eta-newline=1
```

### Test sequential reads

```
sudo fio --filename=<device name> --direct=1 --rw=read --bs=4k --ioengine=libaio --iodepth=256 --runtime=10 --numjobs=4 --time_based --group_reporting --name=iops-test-job --eta-newline=1 --readonly
```

## Throughput Performance Tests

### Test random reads

Hm.. This seems to be the same as the IOPS test??

```
sudo fio --filename=<device name> --direct=1 --rw=randread --bs=64k --ioengine=libaio --iodepth=64 --runtime=10 --numjobs=4 --time_based --group_reporting --name=throu3ghput-te2st-job223 --eta-newline=1 --readonly
```

### Test file random read/writes

```
sudo fio --filename=</custom mount point/file> --size=50MB --direct=1 --rw=randrw --bs=64k --ioengine=libaio --iodepth=64 --runtime=10 --numjobs=4 --time_based --group_reporting --name=throughput-test-job --eta-newline=1 
```

### Test random read/writes

```
sudo fio --filename=<device name> --direct=1 --rw=randrw --bs=64k --ioengine=libaio --iodepth=64 --runtime=120 --numjobs=4 --time_based --group_reporting --name=throughput-test-job --eta-newline=1
```

### Test sequential reads

```
sudo fio --filename=<device name> --direct=1 --rw=read --bs=64k --ioengine=libaio --iodepth=64 --runtime=120 --numjobs=4 --time_based --group_reporting --name=throughput-test-job --eta-newline=1 --readonly
```

## Latency Performance Tests

### Test random reads for latency

```
sudo fio --filename=<device name> --direct=1 --rw=randread --bs=4k --ioengine=libaio --iodepth=1 --numjobs=1 --time_based --group_reporting --name=readlatency-test-job --runtime=120 --eta-newline=1 --readonly
```

### Test random read/writes for latency

```
sudo fio --filename=device name --direct=1 --rw=randrw --bs=4k --ioengine=libaio --iodepth=1 --numjobs=1 --time_based --group_reporting --name=rwlatency-test-job --runtime=120 --eta-newline=1
```
