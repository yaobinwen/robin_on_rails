# Print the default values in a construction environment.
env = Environment()
for item in sorted(env.Dictionary().items()):
    print "{key} = {value}".format(key=item[0], value=item[1])
