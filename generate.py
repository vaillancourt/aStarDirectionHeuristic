#py

sizeX = 9
sizeY = 7
distanceX = 0.2
distanceY = 0.2


# generate nodes
index = 0
print( sizeX * sizeY )
for y in range( sizeY ):
  for x in range( sizeX ):
    print( index, x * distanceX, y * distanceY )
    index += 1

print()


index = 0
# Generate arcs
print( 2 * ( sizeY * (sizeX - 1) + (sizeY - 1) * sizeX  ) )
for y in range(sizeY):
  ybase = y * sizeX
  for x in range(sizeX - 1):
    print( index, ybase + x, ybase + x + 1 )
    index += 1
    print( index, ybase + x + 1, ybase + x )
    index += 1
      
for y in range(sizeY - 1):
  ybase = y * sizeX
  for x in range(sizeX):
    print( index, ybase + x, ybase + x + sizeX )
    index += 1
    print( index, ybase + x + sizeX, ybase + x )
    index += 1


