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

## File generic_text.txt

Ho utilizzato inizialmente un file di testo contenente il famoso libro di Moby Dick, ma in seguito sono stati aggiunti altri libri presi dal sito di "Gutenberg project". In seguito ho scaricato un file
di testo generico di circa 100 MB.

Modifiche al repository del 05/01/2024:

- Effettuati esperimenti cambiando il numero di thread per l'esecuzione in parallelo: sono stati condotti esperimenti per valutare l'impatto del numero di thread sull'esecuzione in parallelo. Ho apportate modifiche al codice per consentire la regolazione del numero di thread utilizzati. Ho utilizzato la funzione omp_set_num_threads() per impostare il numero di thread a valori diversi, come ad esempio 4, 8 e 16. Sono stati misurati i tempi di esecuzione e valutati i risultati ottenuti per determinare l'ottimale utilizzo del numero di thread per il caso specifico.

- Cambiato il dataset di parole da 1 MB a 100 MB: Il dataset di parole utilizzato per i test è stato aumentato da 1 MB a 100 MB. Questo cambiamento è stato effettuato per valutare le prestazioni del sistema su un volume di dati più ampio e per verificare la scalabilità dell'algoritmo. Ho eseguito nuovi test e raccolti i tempi di esecuzione per confrontare le prestazioni con il dataset di dimensioni maggiori.

Modifiche al repository del 18/01/2024:
- Aggiunta delle funzioni 'computeWordBigramHistogramSequential' e 'computeWordTrigramHistogramSequential' per calcolare gli istogrammi dei bigrammi e trigrammi di parole in modo sequenziale.
- Aggiunta delle funzioni 'computeWordBigramHistogramParallel' e 'computeWordTrigramHistogramParallel' per calcolare gli istogrammi dei bigrammi e trigrammi di parole in modo parallelo.
- Aggiunta di commenti per dividere in blocchi le funzioni del codice, al fine di renderlo più leggibile e comprensibile.

  Modifiche al repository del 15/02/2024:
  - Testati diversi modi alternativi per valutare l'efficienza del codice, tra cui l'utilizzo di un unico for parallelo e una sezione critica senza un istogramma locale. I risultati risultano più scadenti del codice proposto.
  - Aggiunta un file C++ 'ForParallel.cpp' contenente una delle funzioni in parallelo rielaborata in un unico ciclo for, che dimostra che le prestazioni sono inferiori rispetto alle tecniche utilizzate nel file principale.
  - Aggiunta di 2 screenshot che testimoniamo l'inefficienza della soluzione adottata nel file 'ForParallel.cpp'
