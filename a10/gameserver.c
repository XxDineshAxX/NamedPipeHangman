/*
 * read dictionary file to array of words & get ready to play the hangman!
 * wait for a request to come in (unique pipename)
 * respond with another unique pipename 
 * send a bunch of stars (indicating the word length)
 * fork() to enable games to proceed in parallel.
 * for each guess the client sends in, respond with a message 
 * and send updated display word.
 * /
