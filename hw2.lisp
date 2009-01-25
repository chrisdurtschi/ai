; Chris Durtschi
; CS4500 Artificial Intelligence
; Ron Peterson
; LISP Homework 2 - Waterjugs

(defun filljug (jug state)
  (cond
    ((< (first state) jug) (list jug (second state)))
  )
)

(defun emptyjug (state)
  (cond
    ((> (first state) 0) (list 0 (second state)))
  )
)

(defun pouronetoanother (jug1 jug2 state)
  (cond
    (
      (< (first state) (- jug2 (second state)))
      (list 0 (+ (first state) (second state)))
    )
    (t (list ))
  )
)

(defun isgoal (goal state)
  (or 
    (and (= goal (first state)) (= 0 (second state))) 
    (and (= 0 (first state)) (= goal (second state)))
  )
)

(defun goodchildren (children good open closed)
  (cond
    ((null children) good) 
    ((and (not (null (first children)))
          (not (member (first children) open :test #'equal))
          (not (member (first children) closed :test #'equal))
      )
      (goodchildren (rest children) (cons (first children) good) open closed)
    )
    (t (goodchildren (rest children) good open closed))
  )
)

(defun getchildren (jug1 jug2 state open closed)
  (goodchildren 
    (list 
      (emptyjug state)
      (emptyjug (reverse state)) 
      (filljug jug1 state)
      (filljug jug2 (reverse state))
      (pouronetoanother jug1 jug2 state)
      (pouronetoanother jug2 jug1 (reverse state))
    )
    nil
    (cons state open)
    closed
  )
)

(defun recursivejugs (goal jug1 jug2 open closed path)
  "Recursive function to implement the water jugs solution"
  (cond
    ((isgoal goal (first open)) (reverse path))
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
(trace emptyjug)
(trace filljug)
(trace pouronetoanother)