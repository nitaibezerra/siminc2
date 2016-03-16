<?php
/**
 * Listagem de erros de comunicação com o ws.
 * $Id: listarErros.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

$query = <<<DML
SELECT TO_CHAR(rqp.rqptimestamp, 'DD/MM/YYYY às HH24:MI:SS') AS rqptimestamp,
       rqp.rqpsucesso,
       rqpid
  FROM altorc.pedidoalteracaoorcamentaria pao
    INNER JOIN altorc.requisicoespedido rqp USING(rqpid)
  WHERE pao.paoid = %d
DML;
$stmt = sprintf($query, $dadospao['paoid']);
$dadosrequisicao = $db->pegaLinha($stmt);
?>
<div class="col-md-8 col-md-offset-2">
    <div class="panel panel-primary">
        <div class="panel-heading">Dados da última comunicação</div>
        <table class="table">
            <tr>
                <td><strong>Data e Hora:</strong></td>
                <td><?php echo $dadosrequisicao['rqptimestamp']; ?></td>
                <td><strong>Resultado:</strong></td>
                <td>
                <?php
                if ('t' == $dadosrequisicao['rqpsucesso']) {
                    echo '<span class="label label-sucess">Sucesso</span>';
                } else {
                    echo '<span class="label label-danger">Erro</span>';
                }
                ?>
                </td>
            </tr>
        </table>
    </div>
</div>
<br style="clear:both" />
<br />
<br />
<?php
$query = <<<DML
SELECT rqe.rqetipo AS tiperro,
       rqe.rqemensagem,
       1 AS ordenacao
  FROM altorc.requisicoeserros rqe
    INNER JOIN altorc.pedidoalteracaoorcamentaria pao USING(rqpid)
    WHERE pao.paoid = %d
      AND rqetipo = 'E'
UNION
SELECT rqe.rqetipo AS tiperro,
       rqe.rqemensagem,
       2
  FROM altorc.requisicoeserros rqe
    INNER JOIN altorc.pedidoalteracaoorcamentaria pao USING(rqpid)
    WHERE pao.paoid = %d
      AND rqetipo = 'A'
UNION
SELECT rqe.rqetipo AS tiperro,
       rqe.rqemensagem,
       3
  FROM altorc.requisicoeserros rqe
    INNER JOIN altorc.pedidoalteracaoorcamentaria pao USING(rqpid)
    WHERE pao.paoid = %d
      AND rqetipo = 'S'
ORDER BY 3
DML;
$stmt = sprintf($query, $dadospao['paoid'], $dadospao['paoid'], $dadospao['paoid']);
$listaErros = new Simec_Listagem();
$listaErros->setCabecalho(array('Tipo', 'Mensagens'));
$listaErros->setQuery($stmt);
$listaErros->addCallbackDeCampo('tiperro', 'tipoErroMensagem');
$listaErros->esconderColuna('ordenacao');
// -- Saída do relatório
if (false === $listaErros->render()): ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
<?php endif;
