# Fluxo comum de trabalho

Cada desenvolvedor criará uma branch a partir da master.

## Criando uma branch a partir da branch master
    
    $ git branch
    $ git fetch
    $ git checkout master
    $ git pull origin master
    $ git checkout -b tipo-nºissue-modulo-nomeDemanda
    Eg: $ git checkout -b hotfix-007-planejamento-documentos
    $ git push origin hotfix-007-planejamento-documentos

    master   o-----------------------------------------
                  | \
                  |  o---------------- teste ---------------
                  |
                  o---o----o----- hotfix-007-planejamento-documentos ---
                    

## Faça commits na sua branch e envie para o github (origin)
    
    $ git branch
    $ git status
    $ git add docs/Guia_de_operacao-desenvolvimento.md
    $ git status
    $ git commit -m '[ FIX ] Módulo X - funcionalidade y. Issue #007'
    $ git push origin hotfix-007-planejamento-documentos

 
    master   o-----------------------------------------
                  | \
                  |  o---------------- teste ---------------
                  |
                  o---o----o----- hotfix-007-planejamento-documentos ---

## Atualizando a branch de teste com as alterações mais recentes da demanda feita.

    $ git branch
    $ git fetch
    $ git checkout teste
    $ git pull origin teste
    $ git merge hotfix-007-planejamento-documentos
    $ git push origin teste

    master   o----------------------------------------------
                    \
                     o-----o----o----o---o teste -----------------
                      \                    /
                       o---o----o------o--- origin hotfix-007-planejamento-documentos


## Homologando e publicando uma versão para a master

Certifique-se de que a demanda está sem divergências e se foi criada do local certo como já foi citado acima, caso esteja tudo correto, siga os passoo a passos abaixo

(1) Caso de sucesso

    Solicite um pull request e adicione a revisão de um membro.
    
    Detalhe: Selecione apenas a branch que foi homologada e solicite um pull request para master.
(2) Caso de falha

    Utilize sua branch para realizar as alterações e depois prossiga o passo de mandar para teste.


## Termos e comandos usados neste documento

##### Mudar para a branch master
$ git checkout master
    
##### Atualizar a branch master de acordo com as mudanças no remoto (origin)
$ git pull origin master
    
##### Criar uma branch com o padrão usado no SIMINC2
$ git checkout -b tipo-nºissue-modulo-nomeDemanda

##### Adicionar as alterações feitas
$ git add docs/Guia_de_operacao-desenvolvimento.md

##### Remover branch local
$ git branch -D hotfix-007-planejamento-documentos

##### Fazer download dos últimos commits de todas as branch's remoto (origin)
$ git fetch

##### Mandar sua branch para remoto (origin)
$ git push origin hotfix-007-planejamento-documentos

##### Conferir branch atual 
$ git branch

##### Conferir status da branch
$ git status