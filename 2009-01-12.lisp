(defun replace (lst oldv newv)
  "Replace the old value in the list with the new value"
  (cond
    ((null lst) nil)
    ((eq (car lst) oldv)
      (cons newv (replace (cdr lst) oldv newv))
    )
    (t (cons (car lst) (replace (cdr lst) oldv newv)))
  )
)

(defun replace* (lst oldv newv)
  "Replace old value with new value in all levels of the list"
  (cond
    ((null lst) nil)
    ((eq (car lst) oldv)
      (cons newv (replace* (cdr lst) oldv newv))
    )
    ((listp (car lst)) 
      (cons (replace* (car lst) oldv newv) (replace* (cdr lst) oldv newv))
    )
    (t (cons (car lst) (replace* (cdr lst) oldv newv)))
  )
)

(defun reverse (lst)
  "Create separate list, move from one list to another."
  (dreverse lst nil)
)

(defun dreverse (src dest)
  "Move element from src to dest"
  (cond
    ((null src) dest)
    (t (dreverse (car src) dest))
  )
)
  
(defun mirror (lst)
  "Documentation for mirror."
  ())
  
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

; Move from a to b
; Move from a to c
; Move from b to c
; Move from a to b