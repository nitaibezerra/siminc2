<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();
/*
 * Modifica as chamadas, caso seja interface Bootstrap
 */
if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao']) && $_SESSION['sislayoutbootstrap'] == 't') {
    $requisicao = $_REQUEST['requisicao'];
    include APPRAIZ . "includes/funcoesspo_componentes.php";
    include APPRAIZ . "includes/funcoesspo.php";
    include_once APPRAIZ . 'includes/library/simec/Listagem.php';
    switch ($requisicao) {
        case 'informacaoTramitacao':
            $aedid = $_REQUEST['aedid'];
            $sql = "SELECT
                        aeddescregracondicao
                    FROM
                        workflow.acaoestadodoc
                    WHERE
                        aedid = {$aedid}";
            $conteudo = $db->pegaUm($sql);
            echo montaItemAccordion("Observações sobre a Tramitação", 'observacoes', "<br/><spam style=\"text-align:left !important;\">{$conteudo}</spam><br/><br/>", array('aberto' => true));
            $sql = sprintf($_SESSION['sqlPessoasTramitar'], $aedid);
            #ver($sql,$aedid,$_SESSION['sqlPessoasTramitar'],d);
            $conteudo = pessoasParaTramitacao($aedid, $sql);
            echo montaItemAccordion("Pessoas que podem realziar essa transação", 'pessoas', $conteudo, array('aberto' => true));
            die;
        case 'historicoBootstrap':
            $docid = $_REQUEST['docid'];
            echo retornaHistoricoBootsrap($docid);
            die;
//        case 'historicoBootstrapComentario':
//            $docid = $_REQUEST['docid'];
//            echo retornaComentarioHistorico($docid);
//            die;
    }
}
/*
 * Apenas para inteface Bootstrap
 * Retorna as pessoas que podem tramitar nesse status
 * !IMPORTANE não se esqueça de setar a variavel de sessão da query, conforme necessidade do módulo.
 */

function pessoasParaTramitacao($aedid, $sql) {

    if ($sql != '') {
        $listagem = new Simec_Listagem($tipoRelatorio = Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $cabecalho = array(
            'Nome',
            'E-mail'
        );
        $listagem->addCallbackDeCampo('usunome', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('usuemail', 'alinhaParaEsquerda');
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $tabela = $listagem->render();
    } else {
        $tabela = "Sem registros";
    }
    return $tabela;
}

/*
 * Apenas para inteface Bootstrap
 * Retorna as pessoas que podem tramitar nesse status
 * !IMPORTANE não se esqueça de setar a variavel de sessão da query, conforme necessidade do módulo.
 */

function retornaHistoricoBootsrap($docid) {

    $sql = "
		select  hd.hstid,
			ed.esddsc,
			ac.aeddscrealizada,
			us.usunome,
			TO_CHAR(hd.htddata, 'DD/MM/YYYY HH24:MI:SS') as datahora,
                        hd.docid
		from workflow.historicodocumento hd
			inner join workflow.acaoestadodoc ac on
				ac.aedid = hd.aedid
			inner join workflow.estadodocumento ed on
				ed.esdid = ac.esdidorigem
			inner join seguranca.usuario us on
				us.usucpf = hd.usucpf
			left join workflow.comentariodocumento cd on
				cd.hstid = hd.hstid
		where
			hd.docid = {$docid} 
		order by
			hd.htddata asc, hd.hstid asc
	";
    $listagem = new Simec_Listagem($tipoRelatorio = Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $cabecalho = array(
        'Onde Estava',
        'O que aconteceu',
        'Quem fez',
        'Quando fez'
    );
    $listagem->addCallbackDeCampo('esddsc', 'alinhaParaEsquerda');
    $listagem->addCallbackDeCampo('aeddscrealizada', 'alinhaParaEsquerda');
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->addAcao('plus',  array('func' => 'historicoBootstrapComentario', 'extra-params' => array('docid')));
    $listagem->esconderColunas('docid');
    $tabela = $listagem->render();
    return $tabela;
}

//function retornaComentarioHistorico($dados) {
//    if ($comentario != '-') {
//        $saida = montaItemAccordion("Detalhar", rand(0, 99999999), $comentario, array('aberto' => false));
//    } else {
//        $saida = '-';
//    }
//    return $saida;
//}

include APPRAIZ . 'includes/workflow.php';

$docid = (integer) $_REQUEST['docid'];
$documento = wf_pegarDocumento($docid);
$atual = wf_pegarEstadoAtual($docid);
$historico = wf_pegarHistorico($docid);
?>
<html>
    <head>
        <title><?php echo NOME_SISTEMA; ?></title>
        <script language="JavaScript" src="../../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../../includes/listagem.css">
        <!-- biblioteca javascript local -->
        <script type="text/javascript">

            IE = !!document.all;

            function exebirOcultarComentario(docid)
            {
                id = 'comentario' + docid;
                div = document.getElementById(id);
                if (!div)
                {
                    return;
                }
                var display = div.style.display != 'none' ? 'none' : 'table-row';
                if (display == 'table-row' && IE == true)
                {
                    display = 'block';
                }
                div.style.display = display;
            }

        </script>
        <script>
            $('.modal.in .modal-dialog').css({'width': '70%'});
        </script>
    </head>
    <body topmargin="0" leftmargin="0">
        <form action="" method="post" name="formulario">
            <table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 650px;">
                <thead>
                    <tr>
                        <td style="text-align: center; background-color: #e0e0e0;" colspan="6">
                            <b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
                            <div><?php echo $documento['docdsc']; ?></div>
                        </td>
                    </tr>
                    <?php if (count($historico)) : ?>
                        <tr>
                            <td style="width: 20px;"><b>Seq.</b></td>
                            <td style="width: 200px;"><b>Onde Estava</b></td>
                            <td style="width: 200px;"><b>O que aconteceu</b></td>
                            <td style="width: 90px;"><b>Quem fez</b></td>
                            <td style="width: 120px;"><b>Quando fez</b></td>
                            <td style="width: 17px;">&nbsp;</td>
                        </tr>
                    <?php endif; ?>
                </thead>
                <?php $i = 1; ?>
                <?php foreach ($historico as $item) : ?>
                    <?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7"; ?>
                    <tr bgcolor="<?= $marcado ?>" onmouseover="this.bgColor = '#ffffcc';" onmouseout="this.bgColor = '<?= $marcado ?>';">
                        <td align="right"><?= $i ?>.</td>
                        <td style="color:#008000;">
                            <?php echo $item['esddsc']; ?>
                        </td>
                        <td valign="middle" style="color:#133368">
                            <?php echo $item['aeddscrealizada']; ?>
                        </td>
                        <td style="font-size: 6pt;">
                            <?php echo $item['usunome']; ?>
                        </td>
                        <td style="color:#133368">
                            <?php echo $item['htddata']; ?>
                        </td>
                        <td style="color:#133368; text-align: center;">
                            <?php if ($item['cmddsc']) : ?>
                                <img
                                    align="middle"
                                    style="cursor: pointer;"
                                    src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/restricao.png"
                                    onclick="exebirOcultarComentario('<?php echo $i; ?>');"
                                    />
                                <?php endif; ?>
                        </td>
                    </tr>
                    <tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?= $marcado ?>" onmouseover="this.bgColor = '#ffffcc';" onmouseout="this.bgColor = '<?= $marcado ?>';">
                        <td colspan="6">
                            <div >
                                <?php echo simec_htmlentities($item['cmddsc']); ?>
                            </div>
                        </td>
                    </tr>
                    <?php $i++; ?>
                <?php endforeach; ?>
                <?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7"; ?>
                <tr bgcolor="<?= $marcado ?>" onmouseover="this.bgColor = '#ffffcc';" onmouseout="this.bgColor = '<?= $marcado ?>';">
                    <td style="text-align: right;" colspan="6">
                        Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
                    </td>
                </tr>
            </table>
            <br/>
            <div style="text-align: center;">
                <input class="botao" type="button" value="Fechar" onclick="window.close();">
            </div>
        </form>
    </body>
</html>