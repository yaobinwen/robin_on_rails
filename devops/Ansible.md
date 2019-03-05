# Ansible

## Run with a different Python interpreter

See [Working with Inventory: List of Behavioral Inventory Parameters](https://docs.ansible.com/ansible/latest/user_guide/intro_inventory.html#list-of-behavioral-inventory-parameters).

Use `ansible_python_interpreter` in the inventory file:

```yml
all:
  hosts:
    target_vm:
      ansible_user: ywen
      ansible_host: 192.168.16.37
      ansible_python_interpreter: /usr/bin/python3.5
```

Use `ansible_python_version` to confirm the Python version:

```yml
- hosts: all
  name: Test.
  tasks:
    - name: Show Python version being used.
      debug:
        var: ansible_python_version
```
