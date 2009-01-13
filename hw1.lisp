(defun rev (lst)
  "Takes a list and returns the atoms in reverse order"
  (drev lst nil)
)

(defun drev (src dest)
  "Construct a list, moving elements of src to dest"
  (cond
    ((null src) dest)
    (t (drev (rest src) (cons (first src) dest)))
  )
)

(defun rev* (lst)
  "Takes a list and creates a mirror image of it"
  (drev* lst nil)
)

(defun drev* (src dest)
  "Construct a list, moving elements of src to dest, going into sub lists"
  (cond
    ((null src) dest)
    ((listp (first src))
      (drev* (rest src) (cons (drev* (first src) nil) dest))
    )
    (t (drev* (rest src) (cons (first src) dest)))
  )
)

(defun towers (discs start finish extra)
  "Towers of Hanoi"
  (cond
    ((= discs 1) (princ "Move from ") (princ ) (princ " to ") (princ ) (terpri))
    (t 
      (towers (- discs 1) start finish extra) 
      (towers 1 start finish extra) 
      (towers (- discs 1) start finish extra)
    )
  )
)