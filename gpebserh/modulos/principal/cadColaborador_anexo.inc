<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

include_once APPRAIZ . "includes/classes/file.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//ver($_SESSION['cpf'],d);
//	ver($_POST['muncodlogradouro']);
if( $_REQUEST['cpf'] || $_SESSION['cpf'] ){
	$cpfInicio = $_SESSION['cpf'] ? $_SESSION['cpf'] : $_REQUEST['cpf'];
	$_SESSION['cpf'] = $cpfInicio;
	$dadosP = $db->pegaLinha("SELECT * FROM gpebserh.dadospessoais WHERE dpecpf = '".$cpfInicio."'");
	extract($dadosP);
	$_SESSION['dpeid'] = $dpeid;
	$cadcpf = $dpecpf; 
}

function listaAnexo($dpeid){
	global $db;
	$cabecalho = array("&nbsp;", "Nome do Arquivo");
	$colunaAcoes = "'<center>'|| '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarDado(\'' || dpeid || '\');\" title=\"Editar os Dados\" border=\"0\">' || '&nbsp;<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirDependente(' || dpeid || ');\"> ' || 
					   	' </center>' as acoes,";
		
	$sql = " SELECT ".$colunaAcoes." arqnome from public.arquivo  where arqid in (select arqid from gpebserh.documentos  where dpeid = ".$dpeid.")";
		ver(simec_htmlentities($sql),d);
	//ver($sql,d);		
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','listacolaborador');
}

unset( $_SESSION['sessionFILES']);
//unset( $_FILES['Arquivo']);


function salvar(){
	
	global $db;
	$cpf = str_replace('.','',$_POST['cadcpf']);
	$cpf = str_replace('-','',$cpf);
	$_SESSION['cpf']= $cpf; 
	$sql = "SELECT dpeid FROM gpebserh.dadospessoais where dpecpf = '".$cpf."'";
	$dpeid = $db->pegaUm($sql);
	if($dpeid){
		//alteração
	
	if($_FILES["Arquivo"]){	
		$campos	= array("dpeid"	=>$dpeid);	
		$file = new FilesSimec("documentos", $campos ,"gpebserh");	
		$arquivoSalvo = $file->setUpload("Documentos Anexados");
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert(" Sucesso.");</script>';
		}	
	}
		
	}
	
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='gpebserh.php?modulo=principal/cadColaborador_anexo&acao=A&cpf=' + '".$cpf."';
		  </script>";
	die;
}



include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
echo '<div id="teste">';
$db->cria_aba( $abacod_tela, $url, '' );
echo '</div>';
monta_titulo( $titulo_modulo, '&nbsp;' );
//ver($abacod_tela, $url,$titulo_modulo,d);
?>
<script src="/includes/prototype.js"></script> 
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
jQuery.noConflict(); 
 
jQuery(document).ready(function(){

	jQuery('.salvar').click(function(){
		jQuery('#req').val('salvar');
		
		var erro = false;
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).val() == '' ){
				erro = true;
				jQuery(this).focus();
				return false;
			}
		});
		
		if( erro ){
			alert('Campo obrigarório.');
			return false;
		}
		
		jQuery('#formulario').submit();
	});
	
});

</script>
<form method="post" name="formulario" id="formulario" action="" enctype="multipart/form-data">
	<input type="hidden" name="arqid" id="arqid" value=""/>
	<input type="hidden" name="req" id="req" value=""/>
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<thead>
			<tr>
				<td class="textosTabela"></td>
				<td class="camposTabela"></td>
				<td class="textosTabela"></td>
				<td></td>
			</tr>
		</thead>
		<tbody>
            <tr>
            	<td class ="SubTituloDireita" align="right">Foto: </td>
				   <td> 	
				    	Arquivo: <input type="file" name="Arquivo" /> 
	     			   
				   </td>
					
            </tr> 

			<tr>
				<td class ="SubTituloDireita" align="right">  </td>
				<td><input type="button" class="salvar" value="Salvar"></td>
			</tr> 
 			<tr>
				<td colspan="3">
					<div id="listaDependente">
					<?=listaAnexo($dpeid);?>
					</div>
				</td>
			</tr>
		</tbody>
	</table>
</form>