<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
if($_REQUEST['requisicao'] && $_REQUEST['requisicao'] == 'aprova_solicitacao'){
    global $db;
    $slcid = $_POST['slcid'];
    
    try {
        $retorno = posAcaoObraSolicitacao(null,$slcid);
        $mensagem = 'Solicitação atualizada com sucesso!';
        $query = <<<DML
            UPDATE obras2.solicitacao SET aprovado = 'S' WHERE slcid = $slcid

DML;
        $db->executar($query);
        $db->commit();
    } catch(Exception $e) {
        $log = $e->getMessage();
        $retorno = false;
        $mensagem = 'Falha no disparo de email.';
    }
    echo simec_json_encode(array('retorno' => $retorno, 'mensagem' => $mensagem, 'log' => $log));
    die();
}

if($_REQUEST['requisicao'] && $_REQUEST['requisicao'] == 'reprova_solicitacao'){
    global $db;
    $slcid = $_POST['slcid'];

    try {

        $sql = "select * from obras2.solicitacao where slcid = $slcid";
        $dados = $db->pegaLinha($sql);


        $sql = "select * from workflow.documento where docid = $dados[docid]";
        $dados1 = $db->pegaLinha($sql);

        if($dados1['esdid'] == ESDID_SOLICITACOES_DEFERIDO){

            $acao = 3845;
        }
        if($dados1['esdid'] == ESDID_SOLICITACOES_INDEFERIDO){

            $acao = 3846;
        }

        wf_alterarEstado($dados['docid'],$acao,"Pedido reprovado",array());
        $mensagem = 'Solicitação atualizada com sucesso!';
        $query ="UPDATE obras2.solicitacao SET reprovado = 'S' WHERE slcid = $slcid";
        $db->executar($query);
        $db->commit();
    } catch(Exception $e) {
        $log = $e->getMessage();
        $retorno = false;
        $mensagem = 'Falha no disparo de email.';
    }
    echo simec_json_encode(array('retorno' => $retorno, 'mensagem' => $mensagem, 'log' => $log));
    die();
}

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

$habilitado = false;
$habilitado_super = false;
if (possui_perfil($pflcods)) {
    $habilitado = true;
    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))){
        $habilitado_super = true;
    }
}

if( $_GET['acao'] != 'V' ){
	// Inclusão de arquivos padrão do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do módulo
	echo '<br>';
	if($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,array());
	} else {
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,array());
	}
}
print cabecalhoObra($obrid);
monta_titulo('Solicitações', $linha2);
?>

<script type="text/javascript">
    function aprovaSolicitacao(slcid){
        if(confirm('Você deseja prosseguir com a aprovação?')){
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=aprova_solicitacao&slcid=" + slcid,
                async: false,
                dataType: 'json',
                success: function(res) {
                    alert(res.mensagem);
                    if(res.retorno){
                        window.location.href = 'obras2.php?modulo=principal/solicitacoes&acao=A';
                    }
                },
                error: function (jqXHR, textStatus, errorThrown){
                }
            });
        }
        
    }

    function abreSolicitacao(obrid, tipo, slcid){
        var url = "/obras2/obras2.php?modulo=principal/solicitacao&acao=A" +
            "&obrid=" + obrid +
            "&slcid="+ slcid +
            "&tslid[]=" + tipo;
        popup1 = window.open(
            url,
            "Solicitação",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }

    function printQuestionario(obrid,qrpid,slcid,queid) {
        var url = "/obras2/obras2.php?modulo=principal/questionarioImpressaoSolicitacoes&acao=A&qrpid="+qrpid+"&obrid="+obrid+"&slcid="+slcid+"&queid="+queid;
        popup2 = window.open(
            url,
            "_blank",
            "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
        );
    }
</script>

<?
foreach(capturaSolicitacoesObra($obrid) as $solicitacao):?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="3" align="center">
                <b>Solicitação N° <?=$solicitacao['slcid']?></b>
            </td>
        </tr>
        <tr>
            <td colspan='2'></td>
            <td align="right">
                <? if($habilitado_super && ($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO) && $solicitacao['aprovado'] == 'N'):?>
                <button type="button" class='' onclick='aprovaSolicitacao(<?=$solicitacao['slcid']?>);'>APROVAR</button>
                <? endif;?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Solicitação</td>
            <td><span onclick="abreSolicitacao(<?=$obrid?>,<?=$solicitacao['tslid']?>,<?=$solicitacao['slcid']?>);" style="cursor:pointer;" title="Acessar Solicitação"><img src="../imagens/alterar.gif" style="border:0;"> <?=$solicitacao['tsldescricao']?></span></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Situação</td>
            <? if($habilitado): ?>
            <td><?=$solicitacao['esddsc'] ?></td>
            <? else: ?>
            <td><?=$solicitacao['aprovado'] == 'N' ? ($solicitacao['esdid'] == ESDID_SOLICITACOES_CADASTRAMENTO ? '<b>Em Cadastramento</b>' : ($solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA ? '<b>Diligência</b>' : '<b>Aguardando Análise</b>')) : ('<b style="color:green;">'.retornaTextoEstadoSolicitacao($solicitacao['esdid']).'</b>')?></td>
            <? endif; ?>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Justificativa</td>
            <td><?=$solicitacao['slcjustificativa']?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Inserido por</td>
            <td><?=$solicitacao['criador']?></td>
        </tr>
        <? if(($solicitacao['esdid'] != ESDID_SOLICITACOES_CADASTRAMENTO && $solicitacao ['esdid'] != ESDID_SOLICITACOES_AGUARDANDO_ANALISE && $solicitacao['esdid'] != ESDID_SOLICITACOES_RETORNADO && $solicitacao['aprovado'] == 'S') || ($solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA)): ?>
            <?
            $analise = capturaAnaliseSolicitacao($solicitacao['docid']);
            ?>
            <tr>
                <td class="SubTituloDireita" width="20%">Data da Análise</td>
                <td><?=$analise['htddata']?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="20%">Analisado por</td>
                <td><?=$analise['analista']?></td>
            </tr>
            <? if($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA || $solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO): ?>
            <tr>
                <td class="SubTituloDireita" width="20%">Observações</td>
                <td>
                    <style>
                        .box-comentario{
                            padding: 0 0 0 2px;
                            border: 1px solid #ccc;
                            color: #7B7B7B;
                            margin: 1px;
                            width: 478px;
                            -webkit-border-radius: 3px;
                            -moz-border-radius: 3px;
                            border-radius: 3px;
                        }
                        .box-comentario-fnde{
                            border: 2px solid #97BAF9;
                            color:  #003DAB;
                            width: 476px;
                        }
                    </style>
                    <?=capturaAnaliseSolicitacaoObservacoes($solicitacao['docid'])?>
                </td>
            </tr>
            <? endif; ?>
        <? endif;?>
        <? if(($solicitacao['aprovado'] == 'S' || $solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA) and ($solicitacao['tslid'] == 1 || $solicitacao['tslid'] == 2 || $solicitacao['tslid'] == 3 || $solicitacao['tslid'] == 4  ||$solicitacao['tslid'] == 5 )): ?>
        <tr>
            <td class="SubTituloDireita" width="20%">Ver Análise</td>
            <td> <a onclick='printQuestionario(<?=$obrid?>,<?=simec_json_encode($solicitacao['qrpid'])?>,<?=$solicitacao['slcid']?>,<?=simec_json_encode($solicitacao['queid'])?>);'> <img src='/imagens/print.png' > </a></td>
        </tr>
        <? endif; ?>
    </table>
<?
endforeach;