(defun reversi (lst)
  "Takes a list and returns the atoms in reverse order"
  (cond
    ((null lst) nil)
    (t (cons (first (last lst)) (reversi (butlast lst))))
  )
)

(defun reversi* (lst)
  "Takes a list and creates a mirror image of it"
  (cond
    ((null lst) nil)
    ((atom (first (last lst))) (cons (first (last lst)) (reversi* (butlast lst))))
    (t (cons (reversi* (first (last lst))) (reversi* (butlast lst))))
  )
)

(defun towers (numdiscs start finish extra)
  "This function will solve the Towers of Hanoi problem"
  (cond
    ((> discs 1))
  )
)