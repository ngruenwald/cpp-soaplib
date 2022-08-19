# SCM Info

Extract information from source control.

Supported source control systems:
  * git

Retrieved information:

| Information         | Variable                                            |
|---------------------|-----------------------------------------------------|
| source control type | SCM_TYPE                                            |
| branch / tag name   | SCM_BRANCH                                          |
| revision            | SCM_REVISION / PROJECT_VERSION_SCM                  |
| tag version         | SCM_TAG_VERSION (_MAJOR / _MINOR / _PATCH / _TWEAK) |

## Usage

In your main CMakeLists.txt

```
include(scm_info)
```

You can output the SCM information by calling the function _print_scm_info_.

```
print_scm_info()
```
