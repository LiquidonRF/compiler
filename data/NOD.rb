a = 12
b = 486

max = 1
for i in 2..a
  if ((a % i == 0) && (b % i == 0)) then
    max = i
  end
end

puts max