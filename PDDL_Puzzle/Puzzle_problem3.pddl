(define (problem hard8)
    (:domain strips-sliding-tile)
    (:objects
        t1 t2 t3 t4 t5 
        t6 t7 t8 t9 t10 
        t11 t12 t13 t14 t15 
        t16 t17 t18 t19 t20 
        t21 t22 t23 t24 
        x1 x2 x3 x4 x5 y1 y2 y3 y4 y5
    )

    (:init
        (tile t1)(tile t2)(tile t3)(tile t4)(tile t5)
        (tile t6)(tile t7)(tile t8)(tile t9)(tile t10)
        (tile t11)(tile t12)(tile t13)(tile t14)(tile t15)
        (tile t16)(tile t17)(tile t18)(tile t19)(tile t20)
        (tile t21)(tile t22)(tile t23)(tile t24)
        (position x1)
        (position x2)
        (position x3)
        (position x4)
        (position x5)
        (position y1)
        (position y2)
        (position y3)
        (position y4)
        (position y5)
        (inc x1 x2)
        (inc x2 x3)
        (inc x3 x4)
        (inc x4 x5)
        (dec x5 x4)
        (dec x4 x3)
        (dec x3 x2)
        (dec x2 x1)
        (inc y1 y2)
        (inc y2 y3)
        (inc y3 y4)
        (inc y4 y5)
        (dec y5 y4)
        (dec y4 y3)
        (dec y3 y2)
        (dec y2 y1)
        (blank x4 y5)
        (at t1 x1 y1)(at t2 x2 y1)(at t3 x3 y1)(at t4 x4 y1)(at t5 x5 y1)
        (at t6 x1 y2)(at t7 x2 y2)(at t8 x3 y2)(at t9 x4 y2)(at t10 x5 y2)
        (at t11 x1 y3)(at t12 x2 y3)(at t13 x3 y3) (at t14 x4 y3) (at t15 x5 y3)
        (at t16 x1 y4)(at t17 x2 y4)(at t18 x3 y4)(at t19 x4 y4)(at t20 x5 y4)
        (at t21 x1 y5)(at t22 x2 y5)(at t23 x3 y5)(at t24 x5 y5)

    )

    (:goal
        (and
            (at t1 x1 y1) (at t2 x2 y1) (at t3 x3 y1) (at t4 x4 y1) (at t5 x5 y1) 
            (at t6 x1 y2) (at t7 x2 y2) (at t8 x3 y2) (at t9 x4 y2) (at t10 x5 y2) 
            (at t11 x1 y3) (at t12 x2 y3) (at t13 x3 y3) (at t14 x4 y3) (at t15 x5 y3)
            (at t16 x1 y4) (at t17 x2 y4) (at t18 x3 y4) (at t19 x4 y4) (at t20 x5 y4)
            (at t21 x1 y5) (at t22 x2 y5) (at t23 x3 y5) (at t24 x4 y5)
        )
    )

)