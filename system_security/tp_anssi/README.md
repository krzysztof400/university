# Configuration recommendations of a linux system tests

## Introduction
ANSSI (Agence Nationale de la Sécurité des Systèmes d'Information) is the French national cybersecurity agency, which provides security guidelines and best practices for information systems. The ANSSI rules specify recommended configurations to enhance system security, covering aspects such as kernel settings, network configurations, access control policies, and privilege management.

Below we can find results of checking few ansii rules for machine 1 running wsl2 ubuntu 22.04.3 and machine 2 running ubuntu 24.04.1
Implementation of rules is explained in given rule file by code comments.

## How to run the tests?
Set up file system. \
Run:
```shell
sudo ./all_tests.sh
```
Rules 40, 42 require root privilage therefor we have to run it with sudo

## Rules analysis

### r9 - System should have prefered kernel configuration option given in anssi
Some OK, some KO
KO on machine 2:
KO: kernel.perf_cpu_time_max_percent is 25 (expected 1)
KO: kernel.perf_event_max_sample_rate is 100000 (expected 1)
KO: kernel.perf_event_paranoid is 4 (expected 2)
KO: kernel.kptr_restrict is 1 (expected 2)
KO: kernel.sysrq is 176 (expected 0)
KO: kernel.unprivileged_bpf_disabled is 2 (expected 1)
KO: kernel.panic_on_oops is 0 (expected 1)
### r10 - It is recommended to block the loading of kernel modules by activating the kernel.modules_disabled configuration option.
Returns KO, kernel.modules_disabled is 0 (expected 1).
### r12 - System should have prefered IPv4 configuration option given in anssi
Some OK, some KO. But most of KO results can be made persistent by adding them to /etc/sysctl.conf and running sysctl -p.
### r18 - System should have prefered compilation options for kernel modules given in anssi.
On machine 1: Some OK, some KO. Many values were missing so automatic KO because missing=not set/n
On machine 2: All KO. Also moslty due to missing files.
Can be fixed by changing .config file and recompiling kernel.
### r20 - System should have preferred compilation options for kernel security functions.
Most OK, one KO. Empty value means it is not set which corresponds with expected value.
### r28 - There is prefered system paritioning given in anssi.
machine 1: Everything returns KO.
machine 2: Everything returns KO except for nodev which returns OK.
### r32 - Local user sessions (console TTY, graphical session) must be locked after a certain period of inactivity.
The timeout is set with the variable TMOUT. We can see its value with:
```shell
echo $TMOUT
```
When running this command on my machine, no value is printed which means there is no timeout.
### r36 - The default value of UMASK for the shells must be set to 0077 in order to allow read and write access to its owner only.
Returns KO.
### r40 - The targeted users of a rule should be, as much as possible, non privileged users
Returns KO.
root    ALL=(ALL:ALL) ALL
### r42 - Policies applied by sudo through the sudoers file should not involve any negation
machine 1: Returns KO.
machine 2: Returns OK.
### r56 - Only trusted and specifically designed software for use with setuid or setgid special permissions can have these permissions set.
Everything is OK.
On machine two there is one file that has these privilages and is not owned by a trusted user.
### r59 - Only repositories internal to the organisation, or official (from the distribution or from an editor) should be used.
Returns OK.
We have defined that official repos are ones that do not start
with http or https. This is a basic search and more checks can be added to get more accurate results.

## conclusion
The tests reveal several security misconfigurations across both machines, with multiple rules failing compliance. Notably, Machine 2 exhibits a higher number of failed tests compared to Machine 1. Key areas requiring attention include:
- Kernel and IPv4 configurations (r9, r12, r18, r20)
- System partitioning (r28)
- User session security settings (r32, r36, r40)
- Sudo policy configurations (r42)