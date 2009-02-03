(defun pred (&rest args)
  "Creates a predicate expression from the arguments passed in."
  (cons (first args) (rest args))
)

(defun constantize (sym)
  "Gives the symbol the property of being a constant"
  (setf (get sym 'termtype) 'constant)
  sym
)

(defun isconstant (sym)
  "Determines if the symbol is a constant"
  (cond
    ((and (atom sym) (equal (get sym 'termtype) 'constant)) t)
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
    ((and (atom sym) (equal (get sym 'termtype) 'variable)) t)
    (t nil)
  )
)

(defun applysubrec (sub exp newexp)
  "Apply a substitution to the remainder of an expression."
  (cond
    ((null exp) (reverse newexp))
    ((listp (first exp)) (applysubrec sub (rest exp) (cons (applysubrec sub (first exp) nil) newexp)))
    (t (if (equal (first exp) (first sub)) 
      (applysubrec sub (rest exp) (cons (second sub) newexp))
      (applysubrec sub (rest exp) (cons (first exp) newexp)))
    )
  )
)

(defun applysub (sub exp)
  "Apply a substitution to the remainder of an expression."
  (applysubrec sub exp nil)
)

(defun unify (exp1 exp2 sublist)
  "Unifies two predicate calculus expressions, returning a substitution list"
  (cond
    ; We've processed all the elements of the predicates,
    ; return substitution list
    ((and (null exp1) (null exp2)) sublist) 
    ((and (isconstant exp1) (isconstant exp2) (equal exp1 exp2)) nil)
    ((isvariable exp1)
      (cond
        ((or (and (atom exp2) (equal exp1 exp2)) (and (listp exp2) (member exp1 exp2))) 'fail)
        (t (cons (list exp1 exp2) sublist))
      )
    )
    ((isvariable exp2)
      (cond 
        ((or (and (atom exp1) (equal exp2 exp1)) (and (listp exp1) (member exp2 exp1))) 'fail)
        (t (cons (list exp2 exp1) sublist))
      )
    )
    ; if either exp1 or exp2 are empty, return FAIL
    ((or (null exp1) (null exp2)) 'fail)
    ; recursive case
    (t
      (setq sub1 (unify (first exp1) (first exp2) sublist))
      (cond
        ((equal sub1 'fail) 'fail)
        (t 
          (setq sub2 (unify (applysub sub1 (rest exp1)) (applysub sub1 (rest exp2)) sublist))
          (cond
            ((equal sub2 'fail) 'fail)
            (t (cons sub2 (cons sub1 sublist)))
          )
        )
      )
    )
  )
)

(trace unify)
(trace applysub)
(trace applysubrec)
(trace constantize)
(trace isconstant)
(trace variablize)
(trace isvariable)
(trace pred)


(unify (pred (constantize 'p) (constantize 'bob) (variablize 'x)) (pred (constantize 'p) (variablize 'y) (constantize 'jill)) nil)