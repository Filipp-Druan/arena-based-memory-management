# Суть статьи Райана Флери
Свою статью Райан начинает с мнения многих людей, будто бы управлять памятью в языке Си вручную очень сложно. Он не согласен с этим, и считает, что в этом виноват способ управления.

Обычно, для него используют функции `malloc` для выделения памяти произвольного размера, и `free` для её освобождения. У такого метода есть ряд важных свойств:
1) При помощи malloc можно выделять память произвольного размера.
2) Объекты, для которых мы так выделили память, могут жить самое разное время. Они не обязаны зависеть друг от друга.
3) Каждому вызову `malloc` мы должны поставить в соответствие один и только один вызов `free`. `free` нужно вызывать для каждого объекта.

Таким образом `malloc` и `free` - это очень универсальный способ управления памятью. Мы ничего не знаем об объектах, которые аллоцируем. По этому, оптимизировать процесс труднее, а добиться такой универсальности очень не просто. `free` и `malloc` - это довольно сложные функции.

Следить за тем, когда какую память нужно очистить ещё труднее, и часто тут бывают ошибки.

Если посмотреть, как мы управляем памятью, получатся интересные выводы. Порой мы знаем о наших объектах довольно много. Часто их время жизни связано, и связано иерархически. Бывает, они имеют один и тот же тип, один и тот же размер. Иногда мы точно знаем заранее, сколько памяти нам понадобится.

Такие мелкие наблюдения могут очень помочь нам, если для каждого случая мы приеним свой, наиболее подходящий метод распределения памяти.

Основным строительным кирпичиком в этом деле Райан предлагает назначить линейный распределитель памяти - арену. На её основе можно строить более сложные структуры.