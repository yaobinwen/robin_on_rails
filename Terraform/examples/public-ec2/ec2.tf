resource "aws_instance" "example" {
  # AIMI name: ubuntu/images/hvm-ssd/ubuntu-bionic-18.04-amd64-server-20230531
  ami           = "ami-0bb220fc4bffd88dd"
  instance_type = "t2.micro"

  vpc_security_group_ids = [example-vpc.public_subnets[0].id]
}
