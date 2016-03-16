<?php
/**
 * Arquivo de listagem de detalhes de um lote
 * $Id: listarDetalhes.inc 102341 2015-09-11 13:20:45Z maykelbraz $
 */

$llfid = $_REQUEST['lote'];

if (!isset($llfid) || empty($llfid)):?>
    <div class="alert alert-danger col-md-4 col-md-offset-4 text-center">Lote solicitado inválido.</div>
<?php
  die();
endif;

// -- Dados do lote
$dadosLote = carregarDadosDoLote($llfid);

$listagem = new Simec_Listagem();
$listagem
    ->setCabecalho(
        array(
            'UG',
            'Observações',
            'Vinculação de pagamento',
            'Fonte de recurso',
            'Categoria de gastos',
            'Situação contábil',
            'Valor Atendido (R$)',
            'PF'
        )
    )
    ->esconderColuna('lfntransferencia')
    ->setAcoes(array('plus' => 'mostraErrosDoItem'))
    ->setAcaoComoCondicional('plus', array(array('campo' => 'lfntransferencia', 'valor' => 'S', 'op' => 'diferente')))
    ->addCallbackDeCampo('lfnvaloratendido', 'formataDinheiro')
    ->addCallbackDeCampo('numdocsiafi', 'formataCodSiafi')
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, 'lfnvaloratendido')
    ->totalizarColuna('lfnvalor');
?>
<style type="text/css">
blockquote ul{list-style:disc;margin:0}
blockquote{margin-bottom:5px;padding:5px}
</style>
<div class="col-md-8 col-md-offset-2">
    <div class="panel panel-primary">
        <div class="panel-heading">Informações do lote nº <?php echo str_pad($llfid, 7, '0', STR_PAD_LEFT); ?></div>
        <table class="table">
            <tr>
                <td><strong>Criador:</strong></td>
                <td><?php echo $dadosLote['usunome']; ?></td>
                <td><strong>UG:</strong></td>
                <td><?php echo $dadosLote['siafi_ug']; ?></td>
                <td><strong>Situação:</strong></td>
                <td><?php echo formatarEstadoLote($dadosLote['esdid'], $dadosLote['esddsc']); ?></td>
            </tr>
            <tr>
                <td><strong>Criação:</strong></td>
                <td><?php echo $dadosLote['llfinclusao']; ?></td>
                <td><strong>Qtd. liberações:</strong></td>
                <td><?php echo $dadosLote['qtdliberacoes']; ?></td>
                <td><strong>Liberações efetivadas:</strong></td>
                <td><?php echo $dadosLote['qtdatendidos']; ?></td>
            </tr>
            <tr>
                <td><strong>&nbsp;</strong></td>
                <td>&nbsp;</td>
                <td><strong>Total do lote (R$)</strong></td>
                <td><?php echo formataDinheiro($dadosLote['totallote']); ?></td>
                <td><strong>Total efetivado (R$):</strong></td>
                <td><?php echo formataDinheiro($dadosLote['totalloteatendido']); ?></td>
            </tr>
        </table>
    </div>
</div>
<div class="row">
    <div class="col-lg-11">
    <?php
    // -- Listagem de detalhes do lote
    $sql = <<<DML
    SELECT lfnid,
           ungcodfavorecida,
           lfnobservacao,
           vincod,
           ftrcod,
           ctgcod,
           stccod,
           lfnvaloratendido,
           COALESCE(numdocsiafi::varchar, '-') AS numdocsiafi,
           lfn.lfntransferencia
      FROM progfin.liberacoesfinanceiras lfn
      WHERE lfn.llfid = %d
DML;
    $stmt = sprintf($sql, $llfid);

    switch ($dadosLote['esdid']) {
        case ESDID_LOTE_ENVIADO_PARCIALMENTE:
            // -- Identificando a aba ativa
            $abaAtiva = (isset($_REQUEST['aba2'])?$_REQUEST['aba2']:'sucesso');

            // -- URL base das abas
            $urlBaseDasAbas = "progfin.php?modulo=principal/liberacoesfinanceiras/carga&acao=A&aba=detalhes&lote={$_REQUEST['lote']}&aba2=";
            $listaAbas = array();
            $listaAbas[] = array(
                "id" => 1,
                "descricao" => '<span class="glyphicon glyphicon-thumbs-up" style="color:#5cb85c"></span> Sucessos',
                "link" => "{$urlBaseDasAbas}sucesso"
            );
            $listaAbas[] = array(
                "id" => 2,
                "descricao" => '<span class="glyphicon glyphicon-thumbs-down" style="color:#d9534f"></span> Falhas',
                "link" => "{$urlBaseDasAbas}falhas"
            );
            $listaAbas[] = array(
                "id" => 3,
                "descricao" => '<span class="glyphicon glyphicon-remove" style="color:gray"></span> Cancelados',
                "link" => "{$urlBaseDasAbas}cancelados"
            );

            switch ($abaAtiva) {
                case 'sucesso':
                    $stmt .= " AND lfn.lfntransferencia = 'S'";
                    break;
                case 'falhas':
                    $stmt .= " AND lfn.lfntransferencia = 'E'";
                    break;
                case 'cancelados':
                    $stmt .= " AND lfn.lfntransferencia = 'C'";
                    break;
            }

            echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");
            break;
        default:
    }
    $listagem->setQuery($stmt);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    ?>
    </div>
    <div class="col-lg-1 wf_simec">
        <?php wf_desenhaBarraNavegacao($dadosLote['docid'], array()); ?>
    </div>
</div>