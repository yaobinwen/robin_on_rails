# Terraform: Quick Recap

## 1. Basic Syntax

Terraform configuration is written in the _Terraform language_.

### 1.1 Arguments

In the form of `argument_name = argument_value`, such as `image_id = "abc123"`.

### 1.2 Blocks

Example:

```terraform
resource "aws_instance" "example" {
  ami = "abc123"

  network_interface {
    # ...
  }
}
```

A **block** consists of:
- A _type_, e.g., `resource`.
- Various numbers of _labels_. The number of needed _labels_ depends on the _type_.
  - For example, the type `resource` requires two labels, while `network_interface` doesn't need any label.
- A _body_ that consists of:
  - Arguments
  - Nested blocks

### 1.3 Identifiers

**Identifiers** can contain:
- letters
- digits (which cannot be the first character)
- underscore (`_`)
- hyphens (`-`)

### 1.4 Comments

```
# Single line comment
// Single line comment
/* Mulitple
line
comments */
```

### 1.5 Style Conventions

See _[Style Conventions_](https://www.terraform.io/language/syntax/style) for more details. Or use the command `terraform fmt`.

## 2. Files

| File Feature | Value | Notes |
|:-------------|:-----:|:------|
| Extension | `.tf` | A JSON-based variant uses `.tf.json`. |
| Encoding | UTF-8 ||
| Line ending | Unix style (LF) | Windows style (CRLF) is also acceptable |

## 3. Modules

> A _module_ is a collection of `.tf` files kept together in a directory.
>
> A module only consists of the **top-level configuration files in a directory**; nested directories are treated as **completely separate** modules, and are **not** automatically included in the configuration.
>
> Terraform evaluates all of the configuration files in a module, effectively treating the **entire module as a single document**. Separating various blocks into different files is purely for the convenience of readers and maintainers, and has no effect on the module's behavior.

## 4. Root Module and Child Modules

A complete Terraform configuration consists of:
- A **single root module**, which is the working directory where Terraform is called.
- A **tree of child modules**.

## 5. Override Files

Most of the time, every `.tf` file should define a distinct set of configuration objects. It is an error when two files attempt to define the same object.

However, one can use files that are named as  `*_override.tf` (or just `override.tf`) to allow overriding configuration. But these cases should be rare in practice. Refer to [_Override Files_](https://www.terraform.io/language/files/override) for more details.

## 6. Dependency Lock File

A Terraform configuration may use two kinds of external dependencies from outside:
- **Providers** which are plugins for Terraform to interact with external systems such as AWS, Google Cloud, etc.
- **Modules**

The _dependency lock files_ are used to lock down the versions to make sure the same versions are always used in order to provide consistent behaviors. The concept is similar to [`yarn.lock`](https://classic.yarnpkg.com/lang/en/docs/yarn-lock/). See [_Version Constraints_](https://www.terraform.io/language/expressions/version-constraints) for more details.

The dependency lock files should be **check into version control system**.

## 7. Resources

A _resource_ describes an infrastructural object, such as a virtual network, a compute instance, or a higher-level component such as a DNS record.

### 7.1 Syntax

```
resource <TYPE> <LOCAL_NAME> {
  <ARGUMENT_NAME> = <ARGUMENT_VALUE>
  <ARGUMENT_NAME> = <ARGUMENT_VALUE>
  # ...
}
```

### 7.2 Scope

`LOCAL_NAME` is used to "refer to this resource from elsewhere **in the same Terraform module** but has no significance outside that module's scope."

The tuple `(<TYPE>, <LOCAL_NAME>)` must be unique within the same module. This makes sense, because within the same module, every resource instance must be unique.

### 7.3 Behaviors

A resource, when applied, can have four behaviors:

| Behavior | Description |
|---------:|:------------|
| Create | If the resource exists in the configuration but not in the Terraform state. |
| Destroy | If the resource no longer exists in the configuration but still exists in the Terraform state. |
| Update | If the resource arguments in the state are different from those in the configuration. |
| Re-create | If the resource needs update but cannot be updated in-place due to remote API limitation. |

The resources can have meta-arguments:
- `depends_on`, for specifying hidden dependencies
- `count`, for creating multiple resource instances according to a count
- `for_each`, to create multiple instances according to a map, or set of strings
- `provider`, for selecting a non-default provider configuration
- `lifecycle`, for lifecycle customizations
- `provisioner`, for taking extra actions after resource creation

### 7.4 Access

To access the arguments of a resource, use the expression `<TYPE>.<LOCAL_NAME>.<ARGUMENT_NAME>`

### 7.5 Dependencies

Most of the time, the dependencies among resources can be figured out automatically by the reference of one by the other. However, if needed, one can use the meta-argument `depends_on` to explicitly specify the dependency.

### 7.6 Local-only

Most of the resources are on the remote infrastructure provider such as AWS. However, there can be **local-only resources**, such as private keys, self-signed TLS certificates, random IDs.

## 8. Data Sources

> _Data sources_ allow Terraform to use information defined outside of Terraform, defined by another separate Terraform configuration, or modified by functions.

Example:

```terraform
data "aws_ami" "example" {
  most_recent = true

  owners = ["self"]
  tags = {
    Name   = "app-server"
    Tested = "true"
  }
}
```

## 9. Providers

_Providers_ provide the APIs to interact with the actual infrastructure providers, such as AWS, Google Cloud, etc.

Terraform interacts with the providers to manage the _resources_ and _data_ that the providers provide.

## Best Practices

- [Best Practices for Provider Versions](https://www.terraform.io/language/providers/requirements#best-practices-for-provider-versions)
  - "Do not use ~> (or other maximum-version constraints) for modules you intend to reuse across many configurations, even if you know the module isn't compatible with certain newer versions."
- [Standard Module Structure](https://www.terraform.io/language/modules/develop/structure)
