<?php


function bloqueiaParaConsultaEstMun()
{

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	if((in_array(  PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil )))
	{
            return false;
	}
	elseif (in_array(  PAR_PERFIL_CONSULTA_ESTADUAL, $perfil ) || in_array(  PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil ) || in_array( PAR_PERFIL_CONSULTA, $perfil ) )
	{
            return true;
	}
	else 
	{
            return false;
	}
}

function buscaComboTermos(){

	global $db;
	
	if( $_SESSION['par']['inuid'] ){
		
		$sqlMun = "SELECT 	
						dp.dopid as codigo,
						'('||dp.dopnumerodocumento||') '||d.mdonome as descricao
					FROM 
						par.documentopar  dp
					INNER JOIN par.modelosdocumentos   	d  ON d.mdoid  = dp.mdoid
					INNER JOIN par.processopar 			pp ON pp.prpid = dp.prpid 
					INNER JOIN par.instrumentounidade 	iu ON iu.inuid = pp.inuid
					WHERE dp.dopstatus = 'A'
						AND iu.inuid = {$_SESSION['par']['inuid']}";
		
		$db->monta_combo("dopid", $sqlMun, 'S', 'Selecione...', '', '', '', '', 'S', 'dopid' );
	}
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

if(!$_SESSION['par']['inuid']) {
		die("<script>
				alert('Problemas com variáveis. Clique a navegação novamente.');
				window.location='par.php?modulo=inicio&acao=C';
			 </script>");
}
else
{
	$inuid = $_SESSION['par']['inuid'];
}
	
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$caminhoAtual  = "par.php?modulo=principal/documentoUnidade&acao=A";

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = {$caminhoAtual}';</script>";
    exit;
}

if( $_GET['requisicao'] == 'excluir' )
{
	
	if( ! bloqueiaParaConsultaEstMun() )
	{
		// Validacao de perfil ( sem no momento )
		if( true )
		{
			
			$sql = "DELETE FROM par.documentounidade WHERE arqid = {$_GET['arqid']} and diuid = {$_GET['diuid']}";
			if($db->executar($sql))
			{
				include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				$file = new FilesSimec();
				$file->excluiArquivoFisico($_GET['arqid']);
				
				$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
				$db->executar($sql);
			}	
							
			$db->commit();
			
			echo '<script>
					alert("Documento excluído com sucesso!");
					document.location.href = \''.$caminhoAtual.'\';
				  </script>';
		}
		else
		{
			echo '<script>
					alert("Operação não permitida!");
					document.location.href = \''.$caminhoAtual.'\';
				  </script>';
		}
		exit;
	}
	else
	{
		echo '<script>
			alert("O Perfil do usuário não tem autorização para excluir o arquivo!");
			document.location.href = \''.$caminhoAtual.'\';
		  </script>';
	}
}

if($_REQUEST['salvar'] == '1')
{
	
	if( $inuid )
	{
		$tduid			= $_POST['tduid'];
		$dopid			= $_POST['dopid'] ? $_POST['dopid'] : 'NULL';
	    
		if($_FILES['arquivo']['error'] == 0) 
		{
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		
			$campos = array(
							"tduid"         	=>"$tduid",
							"diustatus" 			=> "'A'",
							"diudataatualizacao"	=> "NOW()",
							"usucpf" 				=> "'{$_SESSION['usucpf']}'",
							"inuid"			 		=> $inuid,
							"dopid"			 		=> $dopid
							);
	
			$file = new FilesSimec("documentounidade", $campos ,"par");
		 	$file->setUpload( 'Documento_Unidade' , null, true, '' );
			
		    echo"<script> alert('Operação realizada com sucesso!'); window.location.href = window.location.href;</script>";
		}
	}
	else 
	{
		die("<script>
				alert('Problemas com variáveis. Clique a navegação novamente.');
				window.location='par.php?modulo=inicio&acao=C';
			 </script>");
	}

}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){

	jQuery('#tduid').change(function(){

		if( jQuery(this).val() == '2' ){
			jQuery('#td_termos').load(window.location+'&req=buscaComboTermos');
			jQuery('#tr_termos').show();
		}else{
			jQuery('#tr_termos').hide();
		}
	});
});


function excluirDocumento( url, arqid, diuid)
{
	if( boAtivo = 'S' )
	{
		if(confirm("Deseja realmente excluir este Documento?")){
			window.location = url+'&arqid='+arqid+'&diuid='+diuid;
		} 
	}
}

function downloadArquivo( arqid )
{
	
	window.location='?modulo=principal/documentoUnidade&acao=A&download=S&arqid=' + arqid
	
}

function enviar()
{

	var arq   = document.getElementById('arquivo');
	var tipoDocunid = document.getElementById('tduid');
	var dopid = document.getElementById('dopid');

	if( arq.value == '' )
	{
		alert('É necessário anexar um arquivo.');
		return false;
	}
	if( tipoDocunid.value == '' )
	{
		alert('É necessário informar o Tipo do Documento Unidade.');
		return false;
	}
	if( tipoDocunid.value == '2' && dopid.value == '' )
	{
		alert('É necessário escolher um termo.');
		return false;
	}
	
	//location.href= window.location+'&salvar=1';
	document.formulario.salvar.value=1;
	document.formulario.submit();
}
</script>
<?php

// Este parâmetro ($arrAbas) irá servir para retirar a aba "Acompanhamento" que só deverá estar visível quando ela mesma estiver ativa
$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO );
$perfil = pegaArrayPerfil($_SESSION['usucpf']);
if( in_array( array(PAR_PERFIL_SUPER_USUARIO, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO), $perfil )){
	$arrAbas = array( PAR_ESCONDE_ABA_ACOMPANHAMENTO, 15356 );
}

$db->cria_aba( $abacod_tela, $url, '', $arrAbas );
//PAR
monta_titulo( $titulo_modulo, 'Documentos Anexos da Unidade' );
//
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

//if(!) {?>

<table width="100%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">

		<tr>
			
			<td class="SubTitulocentro" >
				
			</td>
			
		</tr>
	
<?php 	if( ! bloqueiaParaConsultaEstMun())
		{

		?><tr>
			<td > 
			<form name=formulario id=formulario method=post enctype="multipart/form-data">
				<input type="hidden" name="salvar" value="0">
				<table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
				    <tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
				        <td width='50%'>
				            <input type="file" name="arquivo" id="arquivo"/>
				            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				        </td>      
				    </tr>  
				    <tr>
					    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo Documento Unidade:</td>
				        <td width='50%'>
					    <?php
							$sqlMun = "SELECT tduid as codigo, tdudescricao as descricao from par.tipodocumentounidade where tdustatus = 'A' ORDER BY tduid";
							$db->monta_combo("tduid", $sqlMun, 'S', 'Selecione...', '', '', '', '', 'S', 'tduid' );
					    ?>
				        </td>      
					</tr>
				    <tr id="tr_termos" style="display:none">
					    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Termos de compromisso:</td>
				        <td width='50%' id="td_termos">
				        </td>      
					</tr>
				    <tr style="background-color: #cccccc">
				        <td>&nbsp;</td>
				        <td><input type="button" name="botao" value="Salvar"  onclick="enviar()" <?=$disableSalvar ?>></td>
				    </tr> 
				</table>
			</form>
			
			</td>
			
		</tr>
		<?php
		}
		?>
			<tr>
				<td class="SubTitulocentro" colspan="2">Documentos Anexados</td>	
			</tr>
	<tr>
				<td colspan="2" class="SubTituloCentro">
					<?php 
					
					if( bloqueiaParaConsultaEstMun() )
					{
						$btnExcluir = "''";
					}
					else 
					{
						$btnExcluir = "'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\" onclick=\"javascript:excluirDocumento(\'" . $caminhoAtual . "&requisicao=excluir" . "\',' || diu.arqid || ','  || diu.diuid || ');\" >'";
					}
					
					
					$sql = "
						SELECT 
							'<img src=../imagens/icone_lupa.png style=cursor:pointer; onclick=\"downloadArquivo('|| diu.arqid ||');\">'
							||
							{$btnExcluir}
							as acao, 			
							tdu.tdudescricao||coalesce(' - ('||dop.dopnumerodocumento||') '||mod.mdonome,'') as descricao,
							usu.usunome
						FROM
							par.documentounidade diu
						INNER JOIN par.tipodocumentounidade tdu on tdu.tduid = diu.tduid
						INNER JOIN seguranca.usuario usu on diu.usucpf = usu.usucpf
						LEFT  JOIN par.documentopar dop 
							INNER JOIN par.modelosdocumentos mod  ON mod.mdoid  = dop.mdoid 
						ON dop.dopid = diu.dopid
						WHERE 
							inuid = $inuid";
						
				        $cabecalho = array('Ação', 'Tipo de Documento Unidade', 'Inserido por');
				        $db->monta_lista( $sql, $cabecalho, 20, 10, 'N', '', '' );
					?>
					
				</td>
			</tr>
		
	</table>


