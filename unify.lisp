(defun constantize (sym)
  "Gives the symbol the property of being a constant"
  (setf (get sym 'termtype) 'constant)
  sym
)

(defun isconstant (sym)
  "Determines if the symbol is a constant"
  (cond
    ((equal (get sym 'termtype) 'constant) t)
    (t nil)
  )
)

(defun variablize (sym)
  "Gives the symbol the property of being a variable"
  (setf (get sym 'termtype) 'variable)
  sym
)

(defun isvariable (sym)
  "Determines if the symbol is a variable"
  (cond
    ((equal (get sym 'termtype) 'variable) t)
    (t nil)
  )
)

(defun applysub (sub exp newexp)
  "Documentation for applysub."
  (cond
    ((null exp) newexp)
    ((lisp (first exp) (applysub sub)))
    (t (if (= (first exp) (first sub)) (cons (second sub) newexp) (cons (first exp) newexp)))
  )
)

(defun unify (exp1 exp2 sublist)
  "Unifies two predicate calculus expressions, returning a substitution list"
  (cond
    ((and (null exp1) (null exp2)) sublist) 
    ((if (and (isconstant exp1) (isconstant exp2) (equal exp1 exp2)) nil 'fail))
    ((isvariable exp1)
      (cond 
        ((member exp1 exp2) 'fail)
        (t (cons (list exp1 exp2) sublist))
      )
    )
    ((isvariable exp2)
      (cond 
        ((member exp2 exp1) 'fail)
        (t (list exp2 exp1))
      )
    )
    ; if either exp1 or exp2 are empty, return FAIL
    ((or (null exp1) (null exp2)) 'fail)
    ; recursive case
    (t
      (setq sub1 (unify (first exp1) (first exp2)))
      (cond
        ((equal sub1 'fail) 'fail)
        (t (setq sub2 ))
      )
    )
  )
)