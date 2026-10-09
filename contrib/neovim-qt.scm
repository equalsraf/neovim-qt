(use-modules
  ((guix licenses) #:prefix license:)
  (guix utils)
  (guix git-download)
  (guix packages)
  (guix gexp)
  (guix build-system cmake)
  (gnu packages qt)
  (gnu packages serialization)
  (gnu packages vim))


;; git source path is relative to current source file
(define %source-path (canonicalize-path (string-append (current-source-directory) "/..")))

;; see https://guix.gnu.org/cookbook/en/html_node/Building-with-Guix.html
(define vcs-file?
  ;; Return true if the given file is under version control.
  (or (git-predicate %source-path)
      (const #t)))   

(define %version "0.git")

(package
  (name "neovim-qt")
  (version %version)
  (source (local-file %source-path "guile-checkout"
                    #:recursive? #t
                    #:select? vcs-file?))
  (build-system cmake-build-system)
  (arguments
   `(#:configure-flags '("-DUSE_SYSTEM_MSGPACK=1")
     #:tests? #f)) ; tests require X
  (inputs (list qtbase qtsvg msgpack-c neovim qtx11extras))
  (home-page "https://github.com/equalsraf/neovim-qt/")
  (synopsis "Qt GUI for neovim")
  (description "GUI frontend for the Neovim editor")
  (license license:isc))
