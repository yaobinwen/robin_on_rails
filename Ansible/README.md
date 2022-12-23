# Study Notes

## 1. Overview

This document has my study notes of Ansible and also serves as a quick guide for me to recall how to use Ansible for various purposes. Ansible's source code is [here](https://github.com/ansible/ansible).

## 2a. CLI Options

`--limit SUBSET`: Use `ansible-playbook ... -l host1,host2,host3 <playbook>` to run the playbook on `host1`, `host2`, and `host3`.

## 2. Develop and Debug Ansible

See the article [_Ansible: Develop and Debug_](https://yaobinwen.github.io/2022/12/23/Ansible-develop-and-debug.html).

## 3. Sample Outputs

`ansible_facts` has [facts about the remote system](https://docs.ansible.com/ansible/latest/user_guide/playbooks_vars_facts.html#ansible-facts):
- Run `ansible <hostname> -m ansible.builtin.setup` to print the _raw_ information gathered for the remote system.
  - **NOTE**: If nothing is printed, try in a different folder which doesn't have `ansible.cfg`. I haven't looked into why a local `ansible.cfg` may cause the module `setup` to print nothing but I ran into this today (2021-08-12).
  - The raw information can be accessed directly, e.g., `"{{ansible_system}}"`.
  - It can be accessed via the variable `ansible_facts`, too: `{{ansible_facts.system}}` which is equivalent to `"{{ansible_system}}"`.
- Run `ansible <hostname> -m ansible.builtin.setup -a "filter=ansible_local"` to print just the information of the specified part.
- Click [`ansible_facts_raw.json`](./examples/ansible_facts_raw.json) to see a sample (from running `ansible.builtin.setup`).

The [Ansible document "Special Variables"](https://docs.ansible.com/ansible/latest/reference_appendices/special_variables.html) doesn't list the details of some of the special variables. Here are some concrete examples for reference:
- `hostvars`:
  - A dictionary/map with all the hosts in inventory and variables assigned to them.
  - [`hostvars.json`](./examples/hostvars.json)
- `groups`:
  - A dictionary/map with all the groups in inventory and each group has the list of hosts that belong to it.
  - [`groups.json`](./examples/groups.json)
- `group_names`:
  - List of groups the current host is part of.
  - [`group_names.json`](./examples/group_names.json)
- `inventory_hostname`:
  - The inventory name for the ‘current’ host being iterated over in the play.
  - [`inventory_hostname.json`](./examples/inventory_hostname.json)

## 4. Modules and Files

This section is based on the version `2.9.12`. Check out the tag [`v2.9.12`](https://github.com/yaobinwen/ansible/tree/v2.9.12).

The `bin/ansible` is a symbolic link pointing at `lib/ansible/cli/scripts/ansible_cli_stub.py` which is the entry point of all the CLI execution. For example, running `ansible-playbook` will eventually run into the main module of `ansible_cli_stub.py`.

`lib/ansible/cli` has multiple modules:
- `adhoc.py`: For arbitrary Ansible module execution (e.g., `ansible -m`).
- `config.py`: For the CLI `ansible-config`.
- `console.py`: For the CLI `ansible-console`.
- `doc.py`: For the CLI `ansible-doc`.
- `galaxy.py`: For the CLI `ansible-galaxy`.
- `inventory.py`: For the CLI `ansible-inventory`.
- `playbook.py`: For the CLI `ansible-playbook`.
- `pull.py`: For the CLI `ansible-pull`.
- `vault.py`: For the CLI `ansible-vault`.

The class `PlaybookCLI` (`lib/ansible/cli/playbook.py`) is the CLI for running `ansible-playbook`. Look at its `run` method which uses the class `PlaybookExecutor` (`lib/ansible/executor/playbook_executor.py`) to run the playbook. Look at its `run` method.

`PlaybookExecutor` uses a `TaskQueueManager` (`lib/ansible/executor/task_queue_manager.py`) to run the tasks. Look at its `run` method.

Note that `TaskQueueManager` actually uses the _strategy_ to run the tasks:

```python
        # load the specified strategy (or the default linear one)
        strategy = strategy_loader.get(new_play.strategy, self)
        if strategy is None:
            raise AnsibleError("Invalid play strategy specified: %s" % new_play.strategy, obj=play._ds)

        # ...

        # and run the play using the strategy and cleanup on way out
        display.debug("[ywen] Run the play using the strategy {s}".format(s=strategy))
        play_return = strategy.run(iterator, play_context)
```

## 5. How to Display Role Path

See the article [_Ansible: How to display a role's path_](https://yaobinwen.github.io/2022/12/23/Ansible-display-role-path.html).

## 6. How `check_mode` is set

See the article [_Ansible: How `check_mode` is set_](https://yaobinwen.github.io/2022/12/23/Ansible-check-mode.html).

## 7. CLI

The CLI entry point is at `lib/ansible/cli/scripts/ansible_cli_stub.py`. Part of the code with comments is listed below (see `NOTE(ywen)`):

```python
        # NOTE(ywen): This block figures out which sub-cli to run.
        if len(target) > 1:
            sub = target[1]
            myclass = "%sCLI" % sub.capitalize()
        elif target[0] == 'ansible':
            sub = 'adhoc'
            myclass = 'AdHocCLI'
        else:
            raise AnsibleError("Unknown Ansible alias: %s" % me)

        # NOTE(ywen): Get the actual CLI module.
        try:
            mycli = getattr(__import__("ansible.cli.%s" % sub, fromlist=[myclass]), myclass)
            display.debug("[ywen] The used CLI module is: {m}".format(m=repr(mycli)))
        except ImportError as e:
            # ImportError members have changed in py3
            if 'msg' in dir(e):
                msg = e.msg
            else:
                msg = e.message
            if msg.endswith(' %s' % sub):
                raise AnsibleError("Ansible sub-program not implemented: %s" % me)
            else:
                raise

        # NOTE(ywen): Create the working directory that will put the temporary
        # bundled modules.
        b_ansible_dir = os.path.expanduser(os.path.expandvars(b"~/.ansible"))
        try:
            display.debug("[ywen] Creating the working directory at '{p}'".format(p=b_ansible_dir))
            os.mkdir(b_ansible_dir, 0o700)
        except OSError as exc:
            if exc.errno != errno.EEXIST:
                display.warning("Failed to create the directory '%s': %s"
                                % (to_text(b_ansible_dir, errors='surrogate_or_replace'),
                                   to_text(exc, errors='surrogate_or_replace')))
        else:
            display.debug("Created the '%s' directory" % to_text(b_ansible_dir, errors='surrogate_or_replace'))

        try:
            # NOTE(ywen): `args` is all the arguments that are given to the CLI.
            args = [to_text(a, errors='surrogate_or_strict') for a in sys.argv]
            display.debug("[ywen] args: {args}".format(args=args))
        except UnicodeError:
            display.error('Command line args are not in utf-8, unable to continue.  Ansible currently only understands utf-8')
            display.display(u"The full traceback was:\n\n%s" % to_text(traceback.format_exc()))
            exit_code = 6
        else:
            # NOTE(ywen): Instantiate a CLI instance using `args`.
            cli = mycli(args)
            # NOTE(ywen): Run the CLI and get the exit code.
            exit_code = cli.run()
```

## 8. Valid keywords in a play

This section is based on Ansible 2.9.27.

What keywords are valid in a play? For example, is the following playbook valid?

```yaml
- import_playbook: show-message.yml
  vars:
    my_message: Hello, world!
```

`ansible-playbook` 2.9.27 accepts this playbook and can run it successfully.

The valid keywords (or "attributes") are verified in the method `def _validate_attributes(self, ds)` of `class FieldAttributeBase` that's defined in `lib/ansible/playbook/base.py`:

```python
    def _validate_attributes(self, ds):
        '''
        Ensures that there are no keys in the datastructure which do
        not map to attributes for this object.
        '''

        valid_attrs = frozenset(self._valid_attrs.keys())
        for key in ds:
            if key not in valid_attrs:
                raise AnsibleParserError("'%s' is not a valid attribute for a %s" % (key, self.__class__.__name__), obj=ds)
```

`self._valid_attrs` is constructed in the method `def _create_attrs(src_dict, dst_dict)` of `class BaseMeta`.

By printing out the values of `self._valid_attrs.keys()`, I figured the valid attributes are defined as class members of `Base`:

```python
class Base(FieldAttributeBase):

    _name = FieldAttribute(isa='string', default='', always_post_validate=True, inherit=False)

    # connection/transport
    _connection = FieldAttribute(isa='string', default=context.cliargs_deferred_get('connection'))
    _port = FieldAttribute(isa='int')
    _remote_user = FieldAttribute(isa='string', default=context.cliargs_deferred_get('remote_user'))

    # variables
    _vars = FieldAttribute(isa='dict', priority=100, inherit=False, static=True)

    # module default params
    _module_defaults = FieldAttribute(isa='list', extend=True, prepend=True)

    # flags and misc. settings
    _environment = FieldAttribute(isa='list', extend=True, prepend=True)
    _no_log = FieldAttribute(isa='bool')
    _run_once = FieldAttribute(isa='bool')
    _ignore_errors = FieldAttribute(isa='bool')
    _ignore_unreachable = FieldAttribute(isa='bool')
    _check_mode = FieldAttribute(isa='bool', default=context.cliargs_deferred_get('check'))
    _diff = FieldAttribute(isa='bool', default=context.cliargs_deferred_get('diff'))
    _any_errors_fatal = FieldAttribute(isa='bool', default=C.ANY_ERRORS_FATAL)
    _throttle = FieldAttribute(isa='int', default=0)

    # explicitly invoke a debugger on tasks
    _debugger = FieldAttribute(isa='string')

    # Privilege escalation
    _become = FieldAttribute(isa='bool', default=context.cliargs_deferred_get('become'))
    _become_method = FieldAttribute(isa='string', default=context.cliargs_deferred_get('become_method'))
    _become_user = FieldAttribute(isa='string', default=context.cliargs_deferred_get('become_user'))
    _become_flags = FieldAttribute(isa='string', default=context.cliargs_deferred_get('become_flags'))
    _become_exe = FieldAttribute(isa='string', default=context.cliargs_deferred_get('become_exe'))

    # used to hold sudo/su stuff
    DEPRECATED_ATTRIBUTES = []
```

What I haven't figured out is how these attributes are passed to `self._valid_attrs`. But assuming I am correct above, then `vars` is a valid keyword to be used in a playbook (and, in fact, we can use more other keywords).

## 9. Check if a variable is defined/exists/empty/true

Reference: [Ansible: When Variable Is – Defined | Exists | Empty | True](https://www.shellhacks.com/ansible-when-variable-is-defined-exists-empty-true/)

Check if Ansible variable is defined (exists):

```yaml
- shell: echo "The variable 'foo' is defined: '{{ foo }}'"
  when: foo is defined

- fail: msg="The variable 'bar' is not defined"
  when: bar is undefined
```

Check if Ansible variable is empty:

```yaml
- fail: msg="The variable 'bar' is empty"
  when: bar|length == 0

- shell: echo "The variable 'foo' is not empty: '{{ foo }}'"
  when: foo|length > 0
```

Check if Ansible variable is defined and not empty:

```yaml
- shell: echo "The variable 'foo' is defined and not empty"
  when: (foo is defined) and (foo|length > 0)

- fail: msg="The variable 'bar' is not defined or empty"
  when: (bar is not defined) or (bar|length == 0)
```

Check if Ansible variable is True or False:

```yaml
- shell: echo "The variable 'foo' is 'True'"
  when: foo|bool == True

- shell: echo "The variable 'bar' is 'False'"
  when: bar|bool == False
```
