(defun waterjugs (goal jug1 jug2)
  "Determines the actions required to fill a single jug with the desired 
  amount of water, specified by goal.  jug1 and jug2 are different sized jugs.
  Allowed operations are completely filling a jug, completely emptying a jug, 
  and pouring as much water as possible from one jug to another"
  (recursivejugs goal jug1 jug2 ((0 0)) nil nil)
)

(defun recursivejugs (goal jug1 jug2 open closed path)
  "Recursive function to implement the water jugs solution"
  (cond
    ((isgoal goal (first open)) path)
    (t
      ()
      (recursivejugs goal jug1 jug2 
        (rest open) 
        (cons (first open) closed) 
        (cons (first open) (reverse path))
      )
    )
  )
)

(defun isgoal (goal state)
  (or 
    (and (= goal (first state)) (= 0 (first (rest state)))) 
    (and (= 0 (first state)) (= goal (first (rest state))))
  )
)

(defun getchildren (jug1 jug2 state)
  (cond
    ((< 0 (first state)) (empty state))
    ((< 0 (first (rest state))) (empty (reverse state)))
    ((>= jug1 (first state)) (fill jug1 state))
    ((>= jug2 (first (rest state))) (fill jug2 (reverse state)))
    ((and (< 0 (first state)) (>= jug2 (first (rest state)))) (pouronetoanother jug1 jug2 state))
    ((and (< 0 (first (rest state))) (>x= jug1 (first state))) (pouronetoanother jug2 jug1 (reverse state)))
  )
)

(defun fill (jug state)
  (list (jug (first (rest state))))
)

(defun empty (state)
  (list 0 (first (rest state)))
)

(defun pouronetoanother (jug1 jug2 state)
  (cond
    (
      (< (first state) (- jug2 (first (rest state))))
      (list 0 (+ (first state) (first (rest state))))
    )
    (t (list ))
  )
)