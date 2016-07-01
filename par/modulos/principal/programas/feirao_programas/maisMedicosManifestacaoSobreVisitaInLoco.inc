
<?php

include_once APPRAIZ . 'par/modulos/principal/programas/mais_medicos/manifestacao_visita_in_loco.php';

    /*
    // PRAZO - 23:59 de 01/09/2014
        $prazo = true;
        $agora = date('Y-m-d H:i:s');
        $maximo = date('Y-m-d H:i:s',strtotime('2014-09-01 23:59:00'));
        if( $agora > $maximo )
            $prazo = false;
        //$prazo = true;
    */
    /*
    CRATO/CE = 2304202
    BACABAL/MA = 2101202
    ANANINDEUA/PA = 1500800
    ITABORAÍ/RJ = 3301900
    ASSIS/SP = 3504008
    INDAIATUBA/SP = 3520509
    Pindamonhangaba/SP = 3538006

    ate 31/03/2015
    Ananindeua/PA = 1500800
     */
    $arrayMuncodPermitidos = array("2304202","2101202","1500800","3301900","3504008","3520509","3538006","1500800");
    $prazo = false;
    if( in_array($_SESSION["par"]["muncod"], $arrayMuncodPermitidos) ){
        $agora = date('Y-m-d H:i:s');

        if( $_SESSION["par"]["muncod"] == '1500800'){
            $maximo = date('Y-m-d H:i:s',strtotime('2015-04-02 18:00:00'));
        }else{
            $maximo = date('Y-m-d H:i:s',strtotime('2015-03-13 23:59:00'));           
        }
        if( $agora < $maximo )
            $prazo = true;
    }


// ####################################################################################################
// RESPOSTAS DE MANIFESTAÇÕES (popup)
// ####################################################################################################
if( $_GET['respostas'] ){
    ob_clean();
    require_once "config.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/funcoes.inc";

    if( !$_GET['respostas'] ){
        echo "<script>
            alert('Nota Técnica não identificada.');
            window.close();
        </script>";
        exit;
    }

    $_SESSION['maismedicos'] = true;
    $perfis = pegaArrayPerfil($_SESSION['usucpf']);
    $mntid = $_GET['respostas'];

    if (
        (in_array(PAR_PERFIL_PREFEITO, $perfis)
        OR in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfis)
        OR in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfis)
        OR in_array(PAR_PERFIL_ADMINISTRADOR, $perfis)
        OR in_array(PAR_PERFIL_SUPER_USUARIO, $perfis) ) AND !empty($mntid)) :

        // busca do nome do arquivo
        if( !empty($dadosManifestacaoRespMun['arqid']) ){
            $sql = " SELECT arqnome FROM public.arquivo WHERE arqid = ".$dadosManifestacaoRespMun['arqid']."; ";
            $nomearquivo = $db->pegaUm( $sql );
        }else{
            $nomearquivo = 'download';
        }

        $sql = "
            SELECT a.arqnome
            FROM public.arquivo a
            INNER JOIN maismedicomec.manifestacaonotatecnicaarquivo mnta ON mnta.arqid = a.arqid
            WHERE mnta.mntid = {$_GET['respostas']}
        ";
        $nomearquivo = $db->pegaUm($sql);
        monta_titulo('Respostas de Nota Técnica - '.$nomearquivo,'');

        ?>

        <script language="JavaScript" src="../../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

        <br>
        <form name="form-manif-resposta-nota-tecnica" id="formulario" method="post" action="" enctype="multipart/form-data">
            <input type="hidden" name="mntid" id="mntid" value="<?= $mntid ?>">
            <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td>
                        <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                            <tr>
                                <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Descrição documento do município</b></td>
                            </tr>

                            <?php if($prazo){ ?>
                                <tr>
                                    <td colspan="2">
                                        <?php echo campo_textarea('mrtdsc', 'S', 'S', '', '60', '2', '', '', 0, '', false, NULL, $dadosManifestacaoRespMun['mrtdsc'], '70%'); ?>
                                    </td>
                                </tr>
                            <?php } ?>

                            <?php if($prazo){ ?>
                                <tr width="25%">
                                    <td class="subtituloDireita">Arquivo:</td>
                                    <td>
                                        <input type="file" name="arqid">
                                        <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >
                                    </td>
                                </tr>
                            <?php } ?>

                            <tr>
                                <td colspan="2" class="SubTituloCentro">
                                    <?php if(!in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfis) ||  !in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfis)){?>
                                    <?php if($prazo){ ?>
                                        <input type="submit" value="Salvar" name="salvar_manifestacao_resposta_nota_tecnica" />
                                    <?php } ?>
                                    <?php } ?>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2">

                                    <?php
                                    $cabecalho = array("Ação","Data Inclusão","Nome Arquivo","Descrição Arquivo","Responsável");
                                    $alinhamento = array('left','left','left','left','left');
                                    $larguras = array('5%','10%','10%','65%','10%');
                                    // <a onclick=\"excluiRespostaNotaTecnica('||a.arqid||','||mrnt.mrtid||')\"><img src=\"/imagens/excluir.gif\"/></a>&nbsp;&nbsp;
                                    $sql = "
                                        SELECT
                                            '<a title=\"Baixar Arquivo\" onclick=\"baixarArquivoRespostaNotaTecnica('||a.arqid||',".$_GET['respostas'].")\"><img src=\"/imagens/clipe1.gif\"/></a>' AS acao,
                                            to_char(a.arqdata,'DD/MM/YYYY') AS datainclusao,
                                            a.arqnome AS nomearquivo,
                                            mrnt.mrtdsc AS descricaoarquivo,
                                            u.usunome AS responsavel
                                        FROM maismedicomec.manifrespostanotatecnica mrnt
                                        INNER JOIN maismedicomec.manifrespostanotatecnicaarquivo mrnta ON mrnt.mrtid = mrnta.mrtid
                                        INNER JOIN public.arquivo a ON a.arqid = mrnta.arqid
                                        INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpf
                                        WHERE mrnt.mntid = {$_GET['respostas']} AND mrnt.mrtstatus = 'A'
                                    ";
                                    $db->monta_lista($sql,$cabecalho,30,5,'N','','N','listaArquivoManifestacao',$larguras,$alinhamento);
                                    ?>

                                </td>
                            </tr>
                        </table>
                    </td>
                </tr>
            </table>
        </form>

    <?php elseif (empty($dadosManifestacaoRespMun['mrtid'])): ?>

        <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td bgcolor="#CCCCCC" colspan="2"> <b>Recurso não cadastrado pelo Município. </b> </td>
            </tr>
        </table>

    <?php endif; ?>

    <script>

    function excluiRespostaNotaTecnica( arqid, mrtid ){
        if( confirm('Deseja remover esse documento?') ){
            window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A&recurso=2&arqid='+arqid+'&id='+mrtid;
        }
    }

    function baixarArquivoRespostaNotaTecnica( arqid, respostas ){
        window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A&download=1&respostas='+respostas+'&arqid='+arqid;
    }

    </script>

    <?php
    // evita exclusao chocar com post
    if( $_POST ){
        echo "<script>
            window.location.href = window.location.href;
        </script>";
    }

    exit();
}
// ####################################################################################################
// FIM DE RESPOSTAS DE MANIFESTAÇÕES (popup)
// ####################################################################################################





// ####################################################################################################
// PAGINA DEFAULT
// ####################################################################################################
include_once APPRAIZ . 'includes/cabecalho.inc';
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A');
monta_titulo($titulo_modulo, recuperaMunicipioEstado());

$_SESSION['maismedicos'] = true;
$perfis = pegaArrayPerfil($_SESSION['usucpf']);

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
    <tr class="subtituloEsquerda">
        <td height="10" width="45%">
            <input type="button" name="btnVoltar" id="btnVoltar" value="Voltar" onclick="history.back()"/>
        </td>
    </tr>
</table>

<br>

<?php if (in_array(PAR_PERFIL_ADMINISTRADOR, $perfis) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfis) || in_array(PAR_PERFIL_PREFEITO, $perfis) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfis) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfis)) : ?>
    <?php if (in_array(PAR_PERFIL_ADMINISTRADOR, $perfis) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfis) || in_array(PAR_PERFIL_PREFEITO, $perfis)) { ?>
    <form name="form-manifestacao-municipio" method="post" enctype="multipart/form-data">
        <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Nota Técnica Seres</b></td>
            </tr>

            <?php if($prazo){ ?>
                <tr>
                    <td colspan="2">
                        <?php echo campo_textarea('mntdsc', 'S', $habilita_rec, '', '60', '2', '', '', 0, '', false, NULL, '', '70%'); ?>
                    </td>
                </tr>
            <?php } ?>

            <?php if($prazo){ ?>
                <tr width="25%">
                    <td class="subtituloDireita">Arquivo:</td>
                    <td>
                        <input type="file" name="arqid">
                        <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório." >
                    </td>
                </tr>
            <?php } ?>

            <tr>
                <td colspan="2" class="SubTituloCentro">
                    <?php if($prazo){ ?>
                        <input type="submit" value="Salvar" name="salvar_manifestacao_visita_in_loco" />
                    <?php } ?>
                </td>
            </tr>
<?php } ?>
            <tr><td colspan="2">&nbsp;</td></tr>

            <tr>
                <td colspan="2">

                    <?php
                    $sql = "";
                    $cabecalho = array("Ação","Data Inclusão","Nome Arquivo","Descrição Arquivo","Responsável");
                    $alinhamento = array('left','left','left','left','left');
                    $larguras = array('5%','10%','10%','65%','10%');
                    // <a title=\"Excluir\" onclick=\"excluiNotaTecnica('||a.arqid||','||mnt.mntid||')\"><img src=\"/imagens/excluir.gif\"/></a>&nbsp;&nbsp;
                    $sql = "
                        SELECT
                            '<a title=\"Baixar Arquivo\" onclick=\"baixarArquivoNotaTecnica('||a.arqid||')\"><img src=\"/imagens/clipe1.gif\"/></a>&nbsp;&nbsp;<a title=\"Respostas de Nota Técnica\" onclick=\"abrePopupRespostas('||mnt.mntid||')\"><img src=\"/imagens/fluxodoc.gif\"/></a>' AS acao,
                            to_char(a.arqdata,'DD/MM/YYYY') AS datainclusao,
                            a.arqnome AS nomearquivo,
                            mnt.mntdsc AS descricaoarquivo,
                            u.usunome AS responsavel
                        FROM maismedicomec.manifestacaonotatecnica mnt
                        INNER JOIN maismedicomec.manifestacaonotatecnicaarquivo mnta ON mnt.mntid = mnta.mntid
                        INNER JOIN public.arquivo a ON a.arqid = mnta.arqid
                        INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpf
                        WHERE mnt.rqmid = {$manifestacaoVisitaInLoco->rqmid} AND mnt.mntstatus = 'A'
                    ";
                    $db->monta_lista($sql,$cabecalho,30,5,'N','','N','listaArquivoManifestacao',$larguras,$alinhamento);
                    ?>

                </td>
            </tr>
        </table>
    </form>


<?php elseif (empty($dadosManifestacaoMun['mntid'])): ?>

    <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td bgcolor="#CCCCCC" colspan="2"> <b>Nota Técnica não cadastrado pelo MEC. </b> </td>
        </tr>
    </table>

<?php endif; ?>

<script>

function excluiNotaTecnica( arqid, mntid ){
    if( confirm('Deseja remover esse documento?') ){
        window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A&recurso=1&arqid='+arqid+'&id='+mntid;
    }
}

function baixarArquivoNotaTecnica( arqid ){
    window.location.href = 'par.php?modulo=principal/programas/feirao_programas/maisMedicosManifestacaoSobreVisitaInLoco&acao=A&download=1&arqid='+arqid;
}

function abrePopupRespostas( mntid ){
    var popupRespostas = window.open(window.location.href+'&respostas='+mntid, 'popupRespostas', 'height=500,width=980,scrollbars=yes,top=50,left=0');
}

</script>

<script type="text/javascript" language="javascript">
	<?php if( (in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfis) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfis) ||  in_array(PAR_PERFIL_PREFEITO, $perfis) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $perfis)) ){  ?>
		jQuery("input,select,textarea").attr('disabled','disabled');
	<?php } ?>
</script>


<?php // evita exclusao chocar com post
if( $_POST['form-manifestacao-municipio'] ){
    echo "<script>
        window.location.href = window.location.href;
    </script>";
}
