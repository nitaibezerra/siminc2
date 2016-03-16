<?PHP
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $perfil = pegarPerfil($_SESSION['usucpf']);

    if($_REQUEST['requisicao']=='excluir'){
        header('Content-Type: text/html; charset=iso-8859-1');
        excluirPublicacao($_REQUEST['arquivo']);
        exibirListaPublicacao($perfil);
        exit();
    }

    if($_REQUEST['requisicao']=='cadastrar'){
            salvarPublicacao($_FILES,$_POST);
            exit();	
    }

    if($_GET['download'] == 'S'){
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($_GET['arqid']);
        echo"<script>window.location.href = 'cap.php?modulo=principal/publicacoes&acao=A';</script>";
        exit;
    }

    if(empty($_SESSION['cap']['afpid']) && isset($_REQUEST['afpid'])){
        $_SESSION['cap']['afpid'] = $_REQUEST['afpid'];
    }

    if($_SESSION['cap']['afpid']){
        $docid = criarDocumento($_SESSION['cap']['afpid']);
    } 

    $esdid = pegarEstadoDocumento($docid);

    #CHAMADA DE PROGRAMA
    include  APPRAIZ."includes/cabecalho.inc";
    include_once APPRAIZ . "includes/workflow.php";

    if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil)){
        $modulo = "Módulo Administrativo";
    } else {
        $modulo = "Módulo Servidor";
    }

    $titulo_modulo = "Afastamento do País";
    monta_titulo( $titulo_modulo, $modulo);

    cabecalhoCAP();

    $habilitado = "S";
    if($esdid == WF_SUBSTITUIDO_NOVO_FORMULARIO || $esdid == WF_PREENC_RELATORIO_VIAGEM || $esdid == WF_VIAGEM_CANCELADA){
        $habilitado = "N";
    }
?>

<br>

<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	

<script type="text/javascript">
    function excluirDocAnexo(arqid){
        if(confirm("Deseja realmente excluir a Publicação?")){
            $.ajax({
                url: 'cap.php?modulo=principal/publicacoes&acao=A',
                data: { requisicao: 'excluir', arquivo: arqid},
                async: false,
                success: function(data) {
                    alert('Publicação excluída com sucesso!');
                    $("#div_publicacao").html(data);
                }
            });
        }
    }

    function validarPublicacao(){
        if($('[name=pbldtdiario]').val() == ''){
            alert('O campo "Data Diário" é obrigatório!');
            $('[name=pbldtdiario]').focus();
            return false;
        }	
        if($('[name=pblnumsecao]').val() == ''){
            alert('O campo "Nº da Seção" é obrigatório!');
            $('[name=pblnumsecao]').focus();
            return false;
        }
        if($('[name=pblpagina]').val() == ''){
            alert('O campo "Nº da Página" é obrigatório!');
            $('[name=pblpagina]').focus();
            return false;
        }	
        if($('[name=arquivo]').val() == ''){
            alert('O campo "Arquivo" é obrigatório!');
            $('[name=arquivo]').focus();
            return false;
        }
        return true;		
    }
</script>

<?php if(!empty($_SESSION['cap']['afpid'])){ ?>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr bgcolor="#CCCCCC">
            <td><b>Servidor:</b>&nbsp;<?php echo exibirNomeServidor($_SESSION['cap']['afpid']); ?></td>
        </tr>
    </table>
    <br>
<?php 
    }

    $arMnuid = array();
    $db->cria_aba( ABA_CAD_AFASTAMENTO, $url, '', $arMnuid );
?>

<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align="center" colspan="2">
            Formulário de Autorização de Afastamento do País - MISSÃO OFICIAL<br>
            Prazo limite para a entrega da solicitação: 15 dias antes do ínicio da missão) 
    	</td>
    </tr>
 </table>

<?php if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil)){ ?>
<form id="formulario" method="post" name="formulario" action="/cap/cap.php?modulo=principal/publicacoes&acao=A" enctype="multipart/form-data" onsubmit="return validarPublicacao();">
    <input type="hidden" id="afpid" name="afpid" value="<?php echo $_SESSION['cap']['afpid']; ?>"/>
    <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
        <tr>
            <td width="95%">
                <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
                    <tr>
                        <td class="subtituloDireita">Data Diário:</td>
                        <td><?php echo campo_data2('pbldtdiario', 'S', 'S', 'Data da Portaria', 'S', '', '', $pbldtportaria); ;?></td>
                    </tr>		
                    <tr>
                        <td class="subtituloDireita">Nº da Seção:</td>
                        <td><?php echo campo_texto('pblnumsecao', 'N', 'S', 'Nº da Seção', '50', '10', '', '', '', '', '','id="pblnumsecao"'); ?></td>
                    </tr>	
                    <tr>
                        <td class="subtituloDireita">Nº da Página:</td>
                        <td><?php echo campo_texto('pblpagina', 'N', 'S', 'Nº da Página', '50', '10', '', '', '', '', '','id="pblpagina"'); ?></td>
                    </tr>							
                    <tr>
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" />&nbsp;
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                    <tr>
                        <td align="center" bgcolor="#CCCCCC" colspan="2">
                            <?if($habilitado == 'S'){?>
                                <input type="submit" value="Salvar" id="btnSalvar"/>
                            <?}?>
                        </td>
                    </tr>
                </table>
            </td>
            <?PHP
                if($docid){
            ?>
                <td valign="top"><br>
                    <?PHP wf_desenhaBarraNavegacao($docid, array('docid' => $docid, 'afpid' => $_SESSION['cap']['afpid']), ''); ?>
                </td>
            <?PHP
                }
            ?>
        </tr>
    </table>    
</form>
<?php } ?>

<br>

<div id="div_publicacao"><?php exibirListaPublicacao($perfil); ?></div>

<?php 
    if($_SESSION['proinfantil']['mgs']){ 
        alert($_SESSION['cap']['mgs']);
        unset($_SESSION['cap']['mgs']);
    } 
    
    if(in_array(PERFIL_GABINETE,$perfil) || $esdid == WF_VIAGEM_FINALIZADA || in_array(CAP_PERFIL_SERVIDOR,$perfil)){ 
?>
        <script language="javascript" type="text/javascript">
            jQuery('#btnSalvar').attr('disabled', true);
            jQuery('input, select, textarea').attr('disabled', true);
        </script>
<?PHP 
    } 
?>	