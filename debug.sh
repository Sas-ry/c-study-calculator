#!/bin/bash
./a.out << EOF
42
EOF

./a.out << EOF
(10*4+2)
EOF

./a.out << EOF
(42)
EOF

./a.out << EOF
((42))
EOF

./a.out << EOF
2*(10*2+1)
EOF

./a.out << EOF
4*10+2
EOF

./a.out << EOF
10*-4-2
EOF

./a.out << EOF
40--2
EOF

./a.out << EOF
+40--2
EOF