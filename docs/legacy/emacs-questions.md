---
title: EMacs Questions
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/EMacs_Questions
---
Q: In C/C++ modes, I want:
* Tabs as spaces
* Tabs is 4 spaces
* Persistent off auto formatting.
* Double-click selection across _ (underscore)

A: Try this in your .emacs:
<pre>
(defun my-c-mode-common-hook ()
  (setq tab-width 4)
  ;; don't treat _ as word delimiter
  (modify-syntax-entry ?_ "w")
  ;; this will make sure spaces are used instead of tabs
  (setq indent-tabs-mode t)
  ;; we don't like auto-newline and hungry-delete
  (c-toggle-auto-hungry-state -1)
  (c-toggle-auto-state -1)
  (setq c-basic-offset 4)
  )
(add-hook 'c-mode-common-hook 'my-c-mode-common-hook)
</pre>

Q: How do I bind recompile to a key command

A: `(define-key global-map [f7] 'recompile)`

Q: How do I get tab to always insert 4 spaces in C++-mode?

Q: What is the command in C++-mode to move a region left or right as with command-[ ] in mac editors?

A: indent-rigidly is an interactive compiled Lisp function in `indent.el'.
It is bound to C-x TAB.
(indent-rigidly start end arg)

Indent all lines starting in the region sideways by arg columns.
Called from a program, takes three arguments, start, end and arg.
You can remove all indentation from a region by giving a large negative arg.

Q: Get bjam to compile / force execute
