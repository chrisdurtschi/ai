; Chris Durtschi
; CS4500 Artificial Intelligence
; Ron Peterson
; LISP Homework 2 - Missionaries and Cannibals

(defun ismissionarylunch (state)
  "Determines if cannibals will eat the missionaries"
  (or
    (and
      (> (length (second state)) 0)
      (> (length (first state)) (length (second state)))
    )
    (and
      (> (length (fourth state)) 0)
      (> (length (third state)) (length (fourth state)))
    )
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

(defun addlist (lst num val)
  "Add the given number of elements to the list"
  (cond
    ((= 0 num) lst)
    (t (addlist (cons val lst) (1- num) val))
  )
)

(defun sublist (lst num)
  "Subtract the given number of elements from the list"
  (cond
    ((= 0 num) lst)
    (t (sublist (rest lst) (1- num)))
  )
)

(defun transfer (state cannibal missionary)
  "Transfer the appropriate number of cannibals and missionaries
  from one bank to the other."
  ; we're at the starting bank
  (if (= (fifth state) 1)
    ; transfer from start bank to end bank
    (list
      (sublist (first state) cannibal)
      (sublist (second state) missionary)
      (addlist (third state) cannibal 'c)
      (addlist (fourth state) missionary 'm)
      (* -1 (fifth state))
    )
    ; else, transfer from end bank to start bank
    (list
      (addlist (first state) cannibal 'c)
      (addlist (second state) missionary 'm)
      (sublist (third state) cannibal)
      (sublist (fourth state) missionary)
      (* -1 (fifth state))
    )
  )
)

(defun issuccessfultrip (state cannibal missionary)
  "The number of cannibals and missionaries we want to transport across the
  river squares with the actual number of cannibals and missionaries
  on the given river bank."
  (cond
    ; we're at the starting bank
    ((= (fifth state) 1)
      (and (>= (length (first state)) cannibal)
           (>= (length (second state)) missionary))
    )
    ; we're at the ending bank
    (t
      (and (>= (length (third state)) cannibal)
           (>= (length (fourth state)) missionary))
    )
  )
)

(defun gettrips (state boatsize cannibal missionary trips)
  "Gets all possible combination of trips for the given boat size."
  (cond
    ((= 0 boatsize) trips)
    ((= (1+ boatsize) missionary) 
      (gettrips state (1- boatsize) (1- boatsize) 0 trips)
    )
    (t
      (gettrips state boatsize (1- cannibal) (1+ missionary) 
        (cons
           (cond
             ((issuccessfultrip state cannibal missionary)
              (transfer state cannibal missionary)
             )
             (t nil)
           )
           trips
        )
      )
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
  (goodchildren (gettrips state boatsize boatsize 0 nil) nil (cons state open) closed)
)

(defun recursivehelper (boatsize open closed path)
  "This function does the heavy lifting for the problem."
  (cond
    ((null open) "No solution found")
    ((isgoal (first open)) (reverse (cons (first open) path)))
    (t
      (recursivehelper
        boatsize
        (append (getchildren boatsize (first open) (rest open) closed) (rest open))
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

(defun printresults (results)
  (cond
    ((not (listp results)) (princ results))
    ((null results) nil)
    (t (princ (first results)) (terpri) (printresults (rest results)))
  )
)
  
(defun missionarycannibal (groupsize boatsize)
  "Solves the problem of the missionaries and cannibals, for a given number
  of couples and capacity of boat using depth-first state search.
  C's represent cannibals, M's represent missionaries.
  A given state is made up of a list of cannibals on the starting bank,
  a list of missionaries on the starting bank,
  a list of cannibals on the ending bank,
  a list of missionaries on the ending bank,
  and a single digit of 1 or -1, with 1 representing the boat positioned
  at the starting bank, and -1 representing the boat at the ending bank.
  A given state will look like this:
  ((C C C) (M M) (C) (M M) -1)
  In this case, there are 3 cannibals and 2 missionaries on the starting bank,
  1 cannibal and 2 missionaries on the ending bank, with the boat at the
  ending bank (unfortunately, the cannibals have eaten the missionaries
  in this state)."
  (printresults (recursivehelper
    boatsize 
    (list 
      (list
        (fillriverbank 'c groupsize nil) 
        (fillriverbank 'm groupsize nil) 
        nil
        nil
        1
      )
    ) 
    nil 
    nil
  ))
  nil
)