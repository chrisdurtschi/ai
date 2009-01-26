; Chris Durtschi
; CS4500 Artificial Intelligence
; Ron Peterson
; LISP Homework 2 - Missionaries and Cannibals

(defun ismissionarylunch (state)
  "Determines if cannibals will eat the missionaries"
  (or
    (> (length (first state)) (length (second state)))
    (> (length (third state)) (length (fourth state)))
  )
)

(defun isgoal (state)
  "We've reached our goal if nobody is left on the starting bank,
  there are an equal number of missionaries and cannibals on the ending bank,
  and the boat is at the ending bank."
  (and
    (= 0 (length (first state)))
    (= 0 (length (second state)))
    (= (length (third state)) (length (fourth state)))
    (= -1 (fifth state))
  )
)

(defun gettrips (state boatsize missionary cannibal trips open closed)
  "Documentation for gettrips."
  (cond
    ((= 0 boatsize) trips)
    ((= (1+ boatsize) cannibal) (gettrips state (1- boatsize) (1- boatsize) 0 trips open closed))
    (t
      (gettrips state boatsize (1- missionary) (1+ cannibal) 
        (cons
           (if ((fifth state)))
           trips)
        open closed)
    )
  )
)

(defun goodchildren (children good open closed)
  "Only returns children that aren't NIL, and in which the missionaries
  are not eaten."
  (cond
    ((null children) good) 
    ((and (not (null (first children)))
          (not (member (first children) open :test #'equal))
          (not (member (first children) closed :test #'equal))
          (not (ismissionarylunch (first children))))
      (goodchildren (rest children) (cons (first children) good) open closed)
    )
    (t (goodchildren (rest children) good open closed))
  )
) 

(defun getchildren (boatsize state open closed)
  "Gets all possible states that don't result in missionaries getting eaten."
  (goodchildren (gettrips state boatsize boatsize 0 nil open closed))
)

(defun recursivehelper (boatsize open closed path)
  "Documentation for recursivemissionarycannibal."
  (
    ((null open) "No solution found")
    ((isgoal (first open)) (reverse (cons (first open) path)))
    (t
      (recursivehelper
        boatsize
        (getchildren boatsize (first open) (rest open) closed) 
        (cons (first open) closed) 
        (cons (first open) path)
      )
    )
  )
)

(defun fillriverbank (person size bank)
  "Create a list of people with specified size"
  (cond
    ((= 0 size) bank)
    (t (fillriverbank person (1- size) (cons person bank)))
  )
)
  
(defun missionarycannibal (groupsize boatsize)
  "Documentation for missionarycannibal."
  (recursivehelper
    boatsize 
    (list 
      (list
        (fillriverbank 0 groupsize nil) 
        (fillriverbank 1 groupsize nil) 
        nil 
        nil
        1
      )
    ) 
    nil 
    nil
  )
)

(trace missionarycannibal)
(trace fillriverbank)
(trace recursivehelper)
(trace isgoal)