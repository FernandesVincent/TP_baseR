Ce second TP qui se concentrait sur le cours de base des réseaux m'a permis d'approfondir un nombre conséquent de notions clés de ce cours.

Mon problème le plus important pour ce TP a été que je n'ai pas réussi à ouvrir un serveur sur ma machine (Mac puce M1) malgré de nombreuses tentatives même en dehors des séances. En parallèle j'ai essayé d'installer une VM Linux (Ubuntu 22.04) mais l'installation de celle ci a été compliquée pour diverses raisons notamment le fait que je ne pouvais y dédier trop de temps à cause des révisions pour le CS d'électronique mineure.

J'ai donc dû faire avec, ce qui a été assez compliqué car je ne pouvais donc voir que la moitié des trames échangées, celles du client vers le serveur, car elles n'atteignaient jamais le serveur, ce qui était logique vu qu'il n'existait donc pas. Cependant, j'ai tout de même essayé de faire un code pour gettftp principalement, mais également le début de puttftp.

En relisant mon code à froid, je me suis rappelé que "struct addrinfo" et "socket()" sont les notions que j'ai eu le plus de mal à comprendre/traiter.

Pour les écritures de la RRQ et la WRQ je me suis servi de cette RFC : https://datatracker.ietf.org/doc/html/rfc1350 qui, contre toute attente de ma part, m'a semblée plutôt claire.
