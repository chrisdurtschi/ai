(defun reversi (lst)
  "Takes a list and returns the atoms in reverse order"
  (cond
    ((null lst) nil)
    (t (cons (first (last lst)) (reversi (butlast lst))))
  )
)