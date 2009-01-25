; Chris Durtschi
; CS4500 Artificial Intelligence
; Ron Peterson
; LISP Homework 2 - Waterjugs

(defun fill1 (jug state)
  (cond
    ((< (first state) jug) (list jug (second state)))
  )
)

(defun fill2 (jug state)
  (cond
    ((< (second state) jug) (list (first state) jug))
  )
)

(defun empty1 (state)
  (cond
    ((> (first state) 0) (list 0 (second state)))
  )
)

(defun empty2 (state)
  (cond
    ((> (second state) 0) (list (first state) 0))
  )
)

(defun pour1to2 (jug2 state)
  (cond
    (
      (and (< (first state) (- jug2 (second state))) (> (first state) 0))
      (list 0 (+ (first state) (second state)))
    )
    (
      (and (> (- jug2 (second state)) 0) (> (first state) 0))
      (list 
        (- (first state) (- jug2 (second state)))
        (+ (second state) (- jug2 (second state)))
      )
    )
  )
)

(defun pour2to1 (jug1 state)
  (cond
    (
      (and (< (second state) (- jug1 (first state))) (> (second state) 0))
      (list (+ (second state) (first state)) 0)
    )
    (
      (and (> (- jug1 (first state)) 0) (> (second state) 0))
      (list 
        (+ (first state) (- jug1 (first state))) 
        (- (second state) (- jug1 (first state)))
      )
    )
  )
)

(defun isgoal (goal state)
  (or 
    (and (= goal (first state)) (= 0 (second state))) 
    (and (= 0 (first state)) (= goal (second state)))
  )
)

(defun goodchildren (children good open closed)
  "Only return children that aren't NIL, and that aren't already
  on open or closed stacks."
  (cond
    ((null children) good) 
    ((and (not (null (first children)))
          (not (member (first children) open :test #'equal))
          (not (member (first children) closed :test #'equal)))
      (goodchildren (rest children) (cons (first children) good) open closed)
    )
    (t (goodchildren (rest children) good open closed))
  )
)

(defun getchildren (jug1 jug2 state open closed)
  (goodchildren 
    (list 
      (empty1 state)
      (empty2 state) 
      (fill1 jug1 state)
      (fill2 jug2 state)
      (pour1to2 jug2 state)
      (pour2to1 jug1 state)
    )
    nil
    (cons state open)
    closed
  )
)

(defun recursivejugs (goal jug1 jug2 open closed path)
  "Recursive function to implement the water jugs solution.
  Base case 1: if the open stack is empty, then we've exhausted all
    the posibilites, and there's no solution.
  Base case 2: if the top item on open is our goal state, then we've found
    the solution; return our path.
  Recursive case: find all the children of the first item on open,
    place the first item of open on closed, and try again with the new
    open stack."
  (cond
    ((null open) (princ "No solution found"))
    ((isgoal goal (first open)) (reverse (cons (first open) path)))
    (t
      (recursivejugs goal jug1 jug2 
        (getchildren jug1 jug2 (first open) (rest open) closed) 
        (cons (first open) closed) 
        (cons (first open) path)
      )
    )
  )
)

(defun waterjugs (goal jug1 jug2)
  "Determines the actions required to fill a single jug with the desired 
  amount of water, specified by goal. jug1 and jug2 are different sized jugs.
  Allowed operations are completely filling a jug, completely emptying a jug,
  and pouring as much water as possible from one jug to another"
  (recursivejugs goal jug1 jug2 '((0 0)) nil nil)
)

(trace waterjugs)
(trace recursivejugs)
(trace getchildren)
(trace goodchildren)
(trace empty1)
(trace empty2)
(trace fill1)
(trace fill2)
(trace pour1to2)
(trace pour2to1)