# ParallelComputingMidTerm
Mid-term assignment
Questo repository contiene il codice sorgente per il progetto ParallelProject. Di seguito sono fornite alcune informazioni importanti riguardo allo sviluppo e all'esecuzione del progetto.

## Avviso sull'assenza di commit intermedi

A causa di un mio errore, non sono stati effettuati commit intermedi durante lo sviluppo di questo progetto. Pertanto, potrebbe sembrare che il codice sia stato caricato in una sola volta senza alcuna iterazione. 
Mi scuso per questa dimenticanza. Tuttavia, il codice caricato rappresenta la versione del progetto completa.

## Test e ottimizzazioni

Prima di arrivare a questa soluzione, ho effettuato diversi test per valutare le prestazioni e l'efficienza del codice. Ho eseguito test su diversi metodi di parallelizzazione e ho analizzato lo speedup
per identificare e risolvere eventuali problemi. La soluzione attuale è il risultato di questo processo di ottimizzazione e sembra essere la più performante tra le soluzioni valutate.

## Thread Sanitizer e falsi positivi

Ho utilizzato il framework di Thread Sanitizer per individuare eventuali problemi di concorrenza e race condition nel codice. 
Durante l'esecuzione dei test, sono stati segnalati due warning dal Thread Sanitizer. Tuttavia, dopo un'attenta analisi, 
ho determinato che questi warning potrebbero essere dei "falsi positivi". Il framework stesso raccomanda di silenziarli in quanto non rappresentano vere problematiche di concorrenza nel codice. 

## File MobyDick_chap1.txt

Ho utilizzato inizialmente un file di testo contenente il famoso libro di Moby Dick, ma in seguito sono stati aggiunti altri libri presi dal sito di "Gutenberg project".
