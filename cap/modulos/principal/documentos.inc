<?PHP
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$perfil = pegarPerfil($_SESSION['usucpf']);

if($_REQUEST['requisicao']=='excluir'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirDocAnexo($_REQUEST['arquivo']);
	exit();
}

if($_REQUEST['requisicao']=='cadastrar'){
	salvarDocAnexo($_FILES,$_POST);
	exit();	
}

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = 'cap.php?modulo=principal/documentos&acao=A';</script>";
    exit;
}

if(empty($_SESSION['cap']['afpid']) && isset($_REQUEST['afpid'])){
	$_SESSION['cap']['afpid'] = $_REQUEST['afpid'];
}

if($_SESSION['cap']['afpid']){
    $docid = criarDocumento($_SESSION['cap']['afpid']);
} 

$esdid = pegarEstadoDocumento($docid);

//Chamada de programa
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

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	function excluirDocAnexo(arqid){
		if(confirm("Deseja realmente excluir o Arquivo?")){
			$.ajax({
			url: 'cap.php?modulo=principal/documentos&acao=A',
			data: { requisicao: 'excluir', arquivo: arqid},
			async: false,
			success:
                function() {
                    alert('Arquivo excluído com sucesso!');
                    window.location = window.location;
                }
			});
		}
	}

	function validarDocumento(){
		if($('[name=arquivo]').val() == ''){
			alert('O campo "Arquivo" é obrigatório!');
			$('[name=arquivo]').focus();
			return false;
		}
		if($('[name=tpdid]').val() == ''){
			alert('O campo "Tipo de Documento" é obrigatório!');
			$('[name=tpdid]').focus();
			return false;
		}
		if($('[name=arqdescricao]').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			$('[name=arqdescricao]').focus();
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
<?php } ?>

<?php 
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
    <tr>
        <td align="center" bgcolor="#CCCCCC" colspan="2"><b>Lista de Arquivos</b></td>
    </tr>    
 </table>

<form id="formulario" method="post" name="formulario" action="/cap/cap.php?modulo=principal/documentos&acao=A" enctype="multipart/form-data" onsubmit="return validarDocumento();">
    
    <input type="hidden" id="afpid" name="afpid" value="<?php echo $_SESSION['cap']['afpid']; ?>"/>
    <input type="hidden" id="requisicao" name="requisicao" value="cadastrar"/>
    
    <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
        <tr>
            <td width="95%">
                <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
                    <tr>
                        <td class="subtituloDireita">Arquivo:</td>
                        <td>
                            <input type="file" name="arquivo" <?php echo $habilitado == 'N' ? 'disabled' : '' ?> />&nbsp;
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Tipo de Documento:</td>
                        <td>
                            <?PHP
                                $sql = "
                                    SELECT
                                        tpdid as codigo,
                                        tpddsc as descricao
                                    FROM cap.tipodocumento
                                    WHERE tpdstatus = 'A'
                                    ORDER BY tpddsc
                                ";
                                $db->monta_combo('tpdid', $sql, $habilitado, 'Selecione...', '', '', '', '', 'S');
                            ?>			
                        </td>
                    </tr>
                    <tr>
                        <td class="subtituloDireita">Descrição:</td>
                        <td>
                            <?PHP 
                                echo campo_textarea('arqdescricao', 'S', $habilitado, '', '80', '5', '255'); 
                            ?>
                        </td>
                    </tr>		
                    <tr>
                        <td align="center" bgcolor="#CCCCCC" colspan="2">
                            <?PHP
                                if ($habilitado == 'S') { 
                            ?>
                                    <input type="submit" value="Salvar" id="btnSalvar"/>
                            <?PHP
                                } 
                            ?>
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

<br>

<div id="div_documento"><?php exibirListaDocAnexo('documentos',$perfil,$habilitado); ?></div>

<?PHP 
    if($_SESSION['proinfantil']['mgs']){ 
        alert($_SESSION['cap']['mgs']);
        unset($_SESSION['cap']['mgs']);
    } 

    if(in_array(PERFIL_GABINETE,$perfil) || ($esdid == WF_VIAGEM_FINALIZADA && in_array(CAP_PERFIL_SERVIDOR,$perfil))){ 
?>
	<script language="javascript" type="text/javascript">
            jQuery('#btnSalvar').attr('disabled', true);
            jQuery('input, select, textarea').attr('disabled', true);
	</script>
        
<?php } ?>	