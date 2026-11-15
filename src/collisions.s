;Two AABBs overlap when:
;   A.left < B.right
;   A.right > B.left
;   A.top < B.bottom
;   A.bottom > B.top
;All four conditions must hold.
;store one aabb for madeline
;and check it against every aabb with some general routine

check_aabb_collision:
    rts