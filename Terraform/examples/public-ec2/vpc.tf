module "vpc" {
  # https://registry.terraform.io/modules/terraform-aws-modules/vpc/aws/latest
  source  = "terraform-aws-modules/vpc/aws"
  version = "5.1.2"

  name = "example-vpc"

  azs = ["${var.aws_region}a", "${var.aws_region}b"]
}
