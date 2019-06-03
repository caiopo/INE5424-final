Feedback Scheduling
===================

O programa de teste (feedback_scheduling.cc) demonstra o funcionamento
do algoritmo de escalonamento criando duas threads.

A primeira é IO bound, sempre deixando a CPU antes do time_slicer
executar reschedule, e a segunda é CPU bound, sempre utilizando seu
quantum por completo.

O programa demonstra que a thread IO bound nunca perde prioridade,
enquanto a thread CPU bound perde prioridade em todas as invocações
do time_slicer.
