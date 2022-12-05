# Study Notes

## 1. Overview

This document has my study notes of Ansible and also serves as a quick guide for me to recall how to use Ansible for various purposes. Ansible's source code is [here](https://github.com/ansible/ansible).

## 2a. CLI Options

`--limit SUBSET`: Use `ansible-playbook ... -l host1,host2,host3 <playbook>` to run the playbook on `host1`, `host2`, and `host3`.

## 2. Develop and Debug Ansible

The latest version (i.e., the `devel` version) of the developer guide is here: [Developer Guide](https://docs.ansible.com/ansible/devel/dev_guide/index.html). Note that Ansible may re-organize their documentation site so the links may become inaccessible. Should this happen, search the key word "Ansible Developer Guide".

This section uses the following references:
- [2.1] [Developing Ansible modules](https://docs.ansible.com/ansible/devel/dev_guide/developing_modules_general.html)
  - [2.1.1] [Preparing an environment for developing Ansible modules](https://docs.ansible.com/ansible/devel/dev_guide/developing_modules_general.html#preparing-an-environment-for-developing-ansible-modules)
  - [2.1.2] [Creating an info or a facts module](https://docs.ansible.com/ansible/devel/dev_guide/developing_modules_general.html#creating-an-info-or-a-facts-module)
  - [2.1.3] [Creating a module](https://docs.ansible.com/ansible/devel/dev_guide/developing_modules_general.html#creating-a-module)

### 2.1 Prepare Development Environment to Develop a Module

The development environment preparation is part of a larger scenario: Developing Ansible modules [2.1].

To prepare the development environment, refer to [2.1.1]. The main steps are as follows (assuming Ubuntu):

- 1). Install prerequisites:
  - The Debian packages:
    - build-essential
    - libssl-dev
    - libffi-dev
    - python-dev
    - python3-dev
    - python3-venv

- 2). Clone the Ansible repository: `$ git clone https://github.com/ansible/ansible.git`
- 3). Change directory into the repository root dir: `$ cd ansible`
- 4). Create a virtual environment:
  - `$ python3 -m venv venv` (or for Python 2 `$ virtualenv venv`. Note, this requires you to install the `virtualenv` package: `$ pip install virtualenv`)
- 5). Activate the virtual environment: `$ . venv/bin/activate`
- 6). Install development requirements: `$ pip install -r requirements.txt`
  - Make sure to upgrade `pip`: `pip install --upgrade pip` because Ubuntu 18.04 provides `pip 9.0.1` which is too old.
  - May need to install `setuptools_rust` using the latest version of `pip`: `pip install setuptools_rust`.
- 7). Run the environment setup script for each new development shell process: `$ . hacking/env-setup`

After the initial setup above, every time you are ready to start developing Ansible you should be able to just run the following from the root of the Ansible repo: `$ . venv/bin/activate && . hacking/env-setup`.

### 2.2 Decide Module Type

The document [2.1] mentions three types of modules:

| Type | Filename | Description | Template |
|:----:|:--------:|:------------|:--------:|
| info | `*_info.py` | Gathers information on other objects or files. | [2.1.2] |
| facts | `*_facts.py` | Gather information about the target machines. | [2.1.2] |
| general-purpose | `*.py` | For other purposes other than information or facts gathering. | [2.1.3] |

### 2.3 Testing the Module

[2.1] also talks about how to test the newly created module, such as sanity test and unit test.

As a note says:

> Ansible uses `pytest` for unit testing.

Usually, one cannot test the `run_module()` function directly because it requires two things:
- `stdin` as `AnsibleModule` reads its input arguments from the standard input.
- `exit_json()` and `fail_json()` call `sys.exit()` which will cause the test program to exit.

Therefore, usually one can only test the functions that the Ansible module calls. But the [`patch_ansible_module()` function](https://github.com/yaobinwen/ansible/blob/devel/test/units/modules/conftest.py#L16-L31) makes it possible to test the Ansible module directly:

```python
@pytest.fixture
def patch_ansible_module(request, mocker):
    if isinstance(request.param, string_types):
        args = request.param
    elif isinstance(request.param, MutableMapping):
        if 'ANSIBLE_MODULE_ARGS' not in request.param:
            request.param = {'ANSIBLE_MODULE_ARGS': request.param}
        if '_ansible_remote_tmp' not in request.param['ANSIBLE_MODULE_ARGS']:
            request.param['ANSIBLE_MODULE_ARGS']['_ansible_remote_tmp'] = '/tmp'
        if '_ansible_keep_remote_files' not in request.param['ANSIBLE_MODULE_ARGS']:
            request.param['ANSIBLE_MODULE_ARGS']['_ansible_keep_remote_files'] = False
        args = json.dumps(request.param)
    else:
        raise Exception('Malformed data to the patch_ansible_module pytest fixture')

    mocker.patch('ansible.module_utils.basic._ANSIBLE_ARGS', to_bytes(args))
```

Currently (as of 2022-01-09), the only tests that use `patch_ansible_module()` is [`test_pip.py`](https://github.com/yaobinwen/ansible/blob/devel/test/units/modules/test_pip.py).

### 2.4 Display Messages

Use the module [`lib/ansible/utils/display.py`](https://github.com/yaobinwen/ansible/blob/devel/lib/ansible/utils/display.py). Search the code `from ansible.utils.display import Display` or something similar to find the examples in the codebase. Typically, it is used in the way below:

```python
from ansible.utils.display import Display

display = Display()

display.error("error message")
display.vvvvv("verbose message")
```

### 2.5 Debug Output vs `debug` Module Output

The "debug output" can refer to two things in Ansible, so be specific when talking about "debug output".

The first one is the log messages that are printed out by [`Display.debug()` method](https://github.com/yaobinwen/ansible/blob/devel/lib/ansible/utils/display.py):

```python
class Display(metaclass=Singleton):

    # ...

    def debug(self, msg, host=None):
        if C.DEFAULT_DEBUG:
            if host is None:
                self.display("%6d %0.5f: %s" % (os.getpid(), time.time(), msg), color=C.COLOR_DEBUG)
            else:
                self.display("%6d %0.5f [%s]: %s" % (os.getpid(), time.time(), host, msg), color=C.COLOR_DEBUG)
```

These debugging log messages can be toggled by the [environment variable `ANSIBLE_DEBUG`](https://docs.ansible.com/ansible/latest/reference_appendices/config.html#envvar-ANSIBLE_DEBUG) and the color can be configured by [`ANSIBLE_COLOR_DEBUG`](https://docs.ansible.com/ansible/latest/reference_appendices/config.html#envvar-ANSIBLE_COLOR_DEBUG). For example:

```
ywen@ywen-Precision-7510:~$ export ANSIBLE_COLOR_DEBUG="bright yellow"
ywen@ywen-Precision-7510:~$ export ANSIBLE_DEBUG=1
ywen@ywen-Precision-7510:~$ ansible -m ping localhost
```

The second one is the output of the [`ansible.builtin.debug` module](https://docs.ansible.com/ansible/latest/collections/ansible/builtin/debug_module.html).

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

When we debug a playbook, sometimes we want to figure out the actual path of the role in the playbook. As of `v2.9.12`, there doesn't seem to be a CLI option of `ansible` or `ansible-playbook` to show the role paths. But there are two other methods to do it.

The first method uses the debug output: Run [`export ANSIBLE_DEBUG=1`](https://docs.ansible.com/ansible/latest/reference_appendices/config.html#envvar-ANSIBLE_DEBUG) to enable debugging output. Then look for the debug log messages **"Loading data from"**:

```
 15267 1639582174.41790: Loading data from /home/ywen/ansible/roles/demo/defaults/main.yml
 15267 1639582174.41857: Loading data from /home/ywen/ansible/roles/demo/tasks/main.yml
```

The second method is to hack the code (assuming `v2.9.27`):
- `sudo vim /usr/lib/python2.7/dist-packages/ansible/playbook/role/definition.py`
- Find the following block (which should be line 90 ~ 94):

```python
        # first we pull the role name out of the data structure,
        # and then use that to determine the role path (which may
        # result in a new role name, if it was a file path)
        role_name = self._load_role_name(ds)
        (role_name, role_path) = self._load_role_path(role_name)
```

- Add a line of `display.v(...` right below the `_load_role_path` line:

```python
        (role_name, role_path) = self._load_role_path(role_name)
        display.v("Found role '{n}' at '{p}'".format(n=role_name, p=role_path))
```

- Running `ansible-playbook -v` (or any verbosity higher than `-v`) will print the used role path:

```
TASK [Task description] *********************************************************************************************************
Found role 'spinnaker' at '/home/ywen/ansible/roles/demo'
```

## 6. How `check_mode` is set

In general, there are (at least) two ways of setting `check_mode`:

- Use the option `--check` on the command line (e.g., `ansible-playbook --check` or `ansible --check`).
- Set `_ansible_check_mode` in `ANSIBLE_MODULE_ARGS`.

But the first method will (probably) eventually use the second method, because an Ansible module is not executed directly, but firstly packed into an `AnsibleZ` tarball (the input arguments included) and then sent to the target machine to run.

`lib/ansible/utils/vars.py` has the function `load_options_vars()`:

### 6.1 Loading the CLI option variables into `VariableManager`

`lib/ansible/vars/manager.py` has the class `VariableManager`. `VariableManager.__init__` calls `load_options_vars()` to load option variables in `self._options_vars = load_options_vars(version_info)`:

```python
def load_options_vars(version):

    if version is None:
        version = 'Unknown'
    options_vars = {'ansible_version': version}
    attrs = {'check': 'check_mode',
             'diff': 'diff_mode',
             'forks': 'forks',
             'inventory': 'inventory_sources',
             'skip_tags': 'skip_tags',
             'subset': 'limit',
             'tags': 'run_tags',
             'verbosity': 'verbosity'}

    for attr, alias in attrs.items():
        opt = context.CLIARGS.get(attr)
        if opt is not None:
            options_vars['ansible_%s' % alias] = opt

    return options_vars
```

### 6.2 `ansible_check_mode` is returned by `VariableManager.get_vars()`

`ansible_check_mode` is returned by `VariableManager._get_magic_variables()` in the "Set options vars" part (near the end of the function):

```python
        # Set options vars
        for option, option_value in iteritems(self._options_vars):
            variables[option] = option_value
```

In `VariableManager.get_vars()`, the magic variables are combined into `all_vars`: `all_vars = combine_vars(all_vars, magic_variables)`.

### 6.3 All the variables in `VariableManager` are loaded into `task_vars` in the strategy

As section "4. Modules and Files" says, `TaskQueueManager` is eventually used to run the Ansible tasks.

`TaskQueueManager.run()` loads the strategy and call the strategy's `run()` method: `play_return = strategy.run(iterator, play_context)`. By default, the strategy is the `linear` strategy.

In `linear.StrategyModule.run()`, the variables are loaded into `task_vars`:

```python
task_vars = self._variable_manager.get_vars(
    play=iterator._play, host=host, task=task,
    _hosts=self._hosts_cache, _hosts_all=self._hosts_cache_all
)
```

### 6.4 `task_vars` are passed into the worker process

Then `task_vars` are passed into `self._queue_task(host, task, task_vars, play_context)`.

In `StrategyBase._queue_task()`, `task_vars` are passed into `WorkerProcess`: ` worker_prc = WorkerProcess(self._final_q, task_vars, host, task, play_context, self._loader, self._variable_manager, plugin_loader)`.

In `WorkerProcess.__init__()`, `task_vars` are recorded in `self._task_vars`. In `WorkerProcess._run()`, `self._task_vars` are passed into `TaskExecutor` as its `job_vars`:

```python
            executor_result = TaskExecutor(
                self._host,
                self._task,
                self._task_vars,
                self._play_context,
                self._new_stdin,
                self._loader,
                self._shared_loader_obj,
                self._final_q
            ).run()
```

### 6.5 `TaskExecutor` passes the variables to the (normal) action plugin

`TaskExecutor.run()` calls `TaskExecutor._execute()` without setting `variables` (L147): `res = self._execute()`.

So `TaskExecutor._execute()` uses `self._job_vars` as `variables`:

```python
        if variables is None:
            variables = self._job_vars
```

`TaskExecutor._execute()` then runs `self._play_context = self._play_context.set_task_and_variable_override(task=self._task, variables=variables, templar=templar)` to override variables if needed.

Finally, `TaskExecutor._execute()` runs the handler (which is an action plugin) to run the module: `result = self._handler.run(task_vars=variables)`. `self._handler = self._get_action_handler(connection=self._connection, templar=templar)` which in the default case the `normal` handler that's defined in `lib/ansible/plugins/action/normal.py`.

### 6.6 `normal` action plugin updates the module arguments

The `normal` action plugin calls `ActionBase._execute_module()` to run the module. `ActionBase._execute_module()` runs `self._update_module_args(module_name, module_args, task_vars)` to update the module's arguments.

`ActionBase._update_module_args()`:

```python
        # set check mode in the module arguments, if required
        if self._play_context.check_mode:
            if not self._supports_check_mode:
                raise AnsibleError("check mode is not supported for this operation")
            module_args['_ansible_check_mode'] = True
        else:
            module_args['_ansible_check_mode'] = False
```

OK, so now `module_args` has `_ansible_check_mode` set.

### 6.7 `normal` action plugin executes the module

But note that the Ansible module is packed up into an `AnsibleZ` tarball together with the arguments and sent to the target machine to run. On the target machine, the module's `run_module()` function is called.

Typically, inside `run_module()`, `AnsibleModule` is instantiated. `AnsibleModule.__init__()` calls `AnsibleModule._check_arguments()` which does the following:

```python
        for k in PASS_VARS:
            # handle setting internal properties from internal ansible vars
            param_key = '_ansible_%s' % k
            if param_key in param:
                if k in PASS_BOOLS:
                    setattr(self, PASS_VARS[k][0], self.boolean(param[param_key]))
                else:
                    setattr(self, PASS_VARS[k][0], param[param_key])

                # clean up internal top level params:
                if param_key in self.params:
                    del self.params[param_key]
            else:
                # use defaults if not already set
                if not hasattr(self, PASS_VARS[k][0]):
                    setattr(self, PASS_VARS[k][0], PASS_VARS[k][1])
```

where `PASS_VARS` contains `check_mode`:

```python
PASS_VARS = {
    'check_mode': ('check_mode', False),
    # ...
}
```

For `AnsibleModule._check_arguments(self, check_invalid_arguments, spec=None, param=None, legal_inputs=None)`, when `param` is `None`:

```python
        if param is None:
            param = self.params
```

`self.params` is set in `def _load_params(self)`:

```python
    def _load_params(self):
        ''' read the input and set the params attribute.

        This method is for backwards compatibility.  The guts of the function
        were moved out in 2.1 so that custom modules could read the parameters.
        '''
        # debug overrides to read args from file or cmdline
        self.params = _load_params()
```

`_load_params()` eventually only returns `params['ANSIBLE_MODULE_ARGS']`. So if I want to override any `ansible_*` variable, I can include it in `params['ANSIBLE_MODULE_ARGS']` as `_ansible_*` (note there must be the leading underscore `_`). For example:

```python
{
    "ANSIBLE_MODULE_ARGS": {
        "_ansible_check_mode": True,
    }
}
```

See `demo/ansible/roles/unittest-module/library/test_my_test.py` for an example.

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
