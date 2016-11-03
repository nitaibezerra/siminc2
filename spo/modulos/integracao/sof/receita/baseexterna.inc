<?php
require_once APPRAIZ . "includes/cabecalho.inc";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/cargaExterna');

if ($_REQUEST['importar'])
{
	header("Keep-Alive: timeout=9999, max=9999");
	set_time_limit(0);

	try
	{
		$ws = new Spo_Ws_Sof_Receita('spo', Spo_Ws_Sof_Receita::PRODUCTION);

		$mensagem = 'Registros importados com sucesso.';

		$count = 0;
		$return = $ws->consultarDisponibilidadeCaptacaoBaseExterna(
            new consultarDisponibilidadeCaptacaoBaseExterna()
        )->return;

		if ($return->mensagensErro) {
			throw new Exception($return->mensagensErro);
        }
        // -- Limpando tabelas de armazenamento
        $sql = <<<DML
DELETE FROM wssof.ws_captacaobaseexternadto;
DELETE FROM wssof.ws_disponibilidadecaptacaobaseexternadto;
DELETE FROM wssof.ws_captacaodetalhebaseexternadto;
DML;
        $db->executar($sql);
        $db->commit();
        // -- Dados da captação
        $captacao = $return->captacoesBaseExterna->captacaoBaseExterna;
        $sql = <<<DML
INSERT INTO wssof.ws_captacaobaseexternadto(codigoCaptacaoBaseExterna, descricao, detalhesBaseExterna)
VALUES(%d, '%s', '%s')
DML;
        $stmt = sprintf(
            $sql,
            $captacao->codigoCaptacaoBaseExterna,
            str_replace("'", "''", $captacao->descricao),
            str_replace("'", "''", $captacao->detalhesBaseExterna)
        );

        $db->executar($stmt);
        $db->commit();

        // -- Detalhes da captação - disponibilização
        $sql = <<<DML
INSERT INTO wssof.ws_disponibilidadecaptacaobaseexternadto(gruponaturezareceita, exercicio, abertoparacaptacacao, iniciodisponibilidade, fimdisponibilidade)
  VALUES('%s', %d, '%s', %s, %s)
DML;
        $disponibilidades = $captacao->disponibilidades->disponibilidade;

        foreach ($disponibilidades?$disponibilidades:array() as $disp) {
            $campos = array_keys(get_object_vars($disp));

            // -- Tratamento de datas
            if (!empty($disp->inicioDisponibilidade)) {
                $disp->inicioDisponibilidade = "'{$disp->inicioDisponibilidade}'";
            } else {
                $disp->inicioDisponibilidade = 'null';
            }
            if (!empty($disp->fimDisponibilidade)) {
                $disp->fimDisponibilidade = "'{$disp->fimDisponibilidade}'";
            } else {
                $disp->fimDisponibilidade = 'null';
            }
            // -- Tratamento do booleano
            if ($disp->abertoParaCaptacacao) {
                $disp->abertoParaCaptacacao = 't';
            } else {
                $disp->abertoParaCaptacacao = 'f';
            }

            $stmt = vsprintf($sql, $disp);
            $db->executar($stmt);

            // -- Consultando os detalhes de cada grupo
            $detalhesGrupo = new ConsultarDetalhesPorGrupo();
            $detalhesGrupo->codigoCaptacaoBaseExterna = $captacao->codigoCaptacaoBaseExterna;
            $detalhesGrupo->grupoNaturezaReceita = $disp->grupoNaturezaReceita;

            $returnDet = $ws->consultarDetalhesPorGrupo($detalhesGrupo)
                ->return;

            if ($returnDet->mensagensErro) {
                throw new Exception($returnDet->mensagensErro);
            }

            if (!empty($returnDet->detalhesCaptacaoBaseExterna)
                && !empty($returnDet->detalhesCaptacaoBaseExterna->detalheCaptacaoBaseExterna)) {

                $detalhamentos = $returnDet->detalhesCaptacaoBaseExterna->detalheCaptacaoBaseExterna;

                foreach ($detalhamentos as $det) {
                    $valoresDetalhamento = get_object_vars($det);
                    unset($valoresDetalhamento['valoresBaseExterna']);

                    if (!isset($sqlDetGrupo)) {
                        $sqlDetGrupo = 'INSERT INTO wssof.ws_captacaodetalhebaseexternadto(' . implode(', ', array_keys($valoresDetalhamento)) . ')'
                             . " VALUES(" . implode(', ', array_fill(0, count($valoresDetalhamento), "'%s'")) . ')';
                    }

                    $valoresDetalhamento['versao'] = empty($valoresDetalhamento['versao'])?-1:$valoresDetalhamento['versao'];

                    $stmt = vsprintf($sqlDetGrupo, $valoresDetalhamento);
                    $db->executar($stmt);
                }
            }

            $db->commit();
        }

		$fm->addMensagem($mensagem, Simec_Helper_FlashMessage::SUCESSO);
	}
	catch (Exception $e)
	{
		$mensagem = "Ocorreu um problema ao importar os registros<br>{$e->getMessage()}";

		$fm->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);
	}
}
?>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li class="active">Integração</li>
            <li class="active">Sof Receitas do SIOP</li>
            <li class="active">Captar Base Externa</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div class="well">
        <fieldset>
            <form id="frmVinculacao" name="frmVinculacao" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            	<input type="hidden" name="importar" value="1">
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnExecutar" type="submit"><i class="glyphicon glyphicon-export"></i> Executar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>

    <?php
    $sql = <<<DML
SELECT codigocaptacaobaseexterna,
       descricao,
       detalhesbaseexterna
  FROM wssof.ws_captacaobaseexternadto
DML;
    $data = $db->pegaLinha($sql);
    ?>
    <div class="col-lg-8 col-lg-offset-2">
        <div class="panel panel-primary">
            <div class="panel-heading">Dados da captação</div>
            <table class="table">
                <tr>
                    <td style="text-align:right;font-weight:bold">Código:</td>
                    <td><?php echo $data['codigocaptacaobaseexterna']; ?></td>
                    <td style="text-align:right;font-weight:bold">Descrição:</td>
                    <td><?php echo $data['descricao']; ?></td>
                </tr>
                <tr>
                    <td style="text-align:right;font-weight:bold">Detalhamento:</td>
                    <td colspan="3" style="width:75%">
                        <?php echo $data['detalhesbaseexterna']?$data['detalhesbaseexterna']:'<p style="text-align:center">-</p>'; ?>
                    </td>
                </tr>
            </table>
            <div class="panel-footer" style="text-align:right">
                Última atualização: <?php echo $db->pegaUm("SELECT TO_CHAR(MAX(dataultimaatualizacao), 'dd/mm/yyyy') AS data FROM wssof.ws_captacaobaseexternadto LIMIT 1 "); ?>
            </div>
        </div>
    </div>
    <br style="clear:both" />
    <?php
    $sql = <<<DML
SELECT gruponaturezareceita,
       exercicio,
       abertoparacaptacacao,
       COALESCE(TO_CHAR(iniciodisponibilidade, 'DD/MM/YYYY'), '-') AS iniciodisponibilidade,
       COALESCE(TO_CHAR(fimdisponibilidade, 'DD/MM/YYYY'), '-') AS fimdisponibilidade
  FROM wssof.ws_disponibilidadecaptacaobaseexternadto
  ORDER BY abertoparacaptacacao DESC,
           exercicio,
           gruponaturezareceita
DML;

    $cabecalho = array(
        'Grupo Natureza Receita',
        'Exercício',
        'Aberto?',
        'Início do Período',
        'Fim do Período'
    );
    $listagem = new Simec_Listagem();
    $listagem->setCabecalho($cabecalho)
        ->addCallbackDeCampo('abertoparacaptacacao', function($abertoparacaptacacao){
            if ('t' == $abertoparacaptacacao) {
                return '<span class="label label-success">Sim</span>';
            } else {
                return '<span class="label label-default">Não</span>';
            }
        })
        ->setQuery($sql)
        ->render();
    ?>
</div>