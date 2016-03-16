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

$dpeid = $_SESSION['gpebserh']['dpeid'];
if ( empty( $dpeid ) ){
	die("<script>
			alert('Faltam parametros para acessar esta tela!');
			history.go(-1);
		</script>");
}

$dadosP = $db->pegaLinha("SELECT * FROM gpebserh.dadospessoais WHERE dpeid=" . $dpeid );
extract($dadosP);

function salvar(){
	global $db;
//ver($_SESSION['cpf'],d);
	$dpeid = $_SESSION['gpebserh']['dpeid'];
		
	$valores = "dpeid = ".$dpeid;

	if($_POST['etcid']){
	 	$valores .= ", etcid = ".$_POST['etcid']; 
	}
	if($_POST['nesid']){
		$valores .= ", nesid = ".$_POST['nesid'];
	}
	if($_POST['nacid']){
	 	$valores .= ", nacid = ".$_POST['nacid']; 
	}
	if($_POST['paiidorigem']){
		$valores .= ", paiidorigem = ".$_POST['paiidorigem'];
	}
	if($_POST['gsgid']){
	 	$valores .= ",gsgid = ".$_POST['gsgid']; 
	}
	if($_POST['tpdid']){
		$valores .= ",tpdid = ".$_POST['tpdid'];
	}
	if($_POST['paiidlogradouro']){
	 	$valores .= ",paiidlogradouro = ".$_POST['paiidlogradouro']; 
	}
	
	if($_POST['dpenumagencia']){
		$valores .= ", dpenumagencia = '".$_POST['dpenumagencia']."'";
	}
	if($_POST['dpedigveragencia']){
		$valores .= ", dpedigveragencia = '".$_POST['dpedigveragencia']."'";
	}
	if($_POST['dpecontacorrente']){
		$valores .= ", dpecontacorrente = '".$_POST['dpecontacorrente']."'";
	}
	if($_POST['dpedigverconta']){
		$valores .= ", dpedigverconta = '".$_POST['dpedigverconta']."'";
	}
	if($_POST['dpenumctps']){
		$valores .= ", dpenumctps = '".$_POST['dpenumctps']."'";
	}
	if($_POST['dpenumseriectps']){
		$valores .= ", dpenumseriectps = '".$_POST['dpenumseriectps']."'";
	}
	if($_POST['dpenumrg']){
		$valores .= ", dpenumrg = '".$_POST['dpenumrg']."'";
	}
	if($_POST['deporgaoexprg']){
		$valores .= ", deporgaoexprg = '".$_POST['deporgaoexprg']."'";
	}
	if($_POST['dpelogradouro']){
		$valores .= ", dpelogradouro = '".$_POST['dpelogradouro']."'";
	}
	if($_POST['dpenumlogradouro']){
		$valores .= ", dpenumlogradouro = '".$_POST['dpenumlogradouro']."'";
	}
	if($_POST['dpecompllogradouro']){
		$valores .= ", dpecompllogradouro = '".$_POST['dpecompllogradouro']."'";
	}
	if($_POST['dpebairrologradouro']){
		$valores .= ",dpebairrologradouro =  '".$_POST['dpebairrologradouro']."'";
	}
	if($_POST['depceplogradouro']){
		$valores .= ", depceplogradouro = '".$_POST['depceplogradouro']."'";
	}
	if($_POST['dpenumddd']){
		$valores .= ",dpenumddd =  '".$_POST['dpenumddd']."'";
	}
	if($_POST['dpenumddd']){
		$valores .= ", dpenumtelefone = '".$_POST['dpenumddd']."'";
	}
	if($_POST['dpeemail']){
		$valores .= ", dpeemail = '".$_POST['dpeemail']."'";
	}
	if($_POST['dpenumbanco']){
		$valores .= ", dpenumbanco = '".$_POST['dpenumbanco']."'";
	}
	if($_POST['dpenomeagencia']){
		$valores .= ",dpenomeagencia =  '".$_POST['dpenomeagencia']."'";
	}
	if($_POST['dpenomemae']){
		$valores .= ", dpenomemae = '".$_POST['dpenomemae']."'";
	}
	if($_POST['dpefatorrh']){
		$valores .= ", dpefatorrh = '".$_POST['dpefatorrh']."'";
	}
	if($_POST['dpeqtddependente']){
		$valores .= ", dpeqtddependente = ".$_POST['dpeqtddependente'];
	}
	if($_POST['dpesexo']){
		$valores .= ", dpesexo = '".$_POST['dpesexo']."'";
	}
	if($_POST['dpeanochegada']){
		$valores .= ",dpeanochegada = '".$_POST['dpeanochegada']."'";
	}
	if($_POST['dpepispasep']){
		$valores .= ",dpepispasep = '".$_POST['dpepispasep']."'";
	}
	if($_POST['dpenumtitulo']){
		$valores .= ", dpenumtitulo = '".$_POST['dpenumtitulo']."'";
	}
	if($_POST['dpesiape']){
		$valores .= ", dpesiape = '".$_POST['dpesiape']."'";
	}
	if($_POST['estufctps']){
	 	$valores .= ",estufctps = '".$_POST['estufctps']."'"; 
	}
	if($_POST['estuflogradouro']){
		$valores .= ", estuflogradouro = '".$_POST['estuflogradouro']."'";
	}
	if($_POST['muncodlogradouro']){
	 	$valores .= ",muncodlogradouro = ".$_POST['muncodlogradouro']; 
	}
	if($_POST['estufrg']){
		$valores .= ",estufrg = '".$_POST['estufrg']."'";
	}
	if($_POST['depeditalreg']){
	 	$valores .= ", depeditalreg = '".$_POST['depeditalreg']."'"; 
	}
	if($_POST['dpedtnasc']){
		$dtnascimento = substr($_POST['dpedtnasc'], 6, 4).'-'.substr($_POST['dpedtnasc'], 3, 2).'-'.substr($_POST['dpedtnasc'], 0, 2);
		$valores .= ", dpedtnasc = '".$dtnascimento."'";
	}
	if($_POST['dpedtprimemprego']){
	 	$dtprimeiroemprego = substr($_POST['dpedtprimemprego'], 6, 4).'-'.substr($_POST['dpedtprimemprego'], 3, 2).'-'.substr($_POST['dpedtprimemprego'], 0, 2);
	 	$valores .= ",dpedtprimemprego = '".$dtprimeiroemprego."'"; 
	}

	$sql = "UPDATE gpebserh.dadospessoais SET ".$valores." where dpeid = ".$dpeid;
	//ver($sql,d);
	
	$db->executar($sql);
	$db->commit();
	
	//verifico a foto
	//$campos	= array("dpeid" => $cpf);
//	$file = new FilesSimec("dadospessoais", $campos, "gpebserh");
	//ver($_FILES, d);
//	if(is_file($_FILES["foto"]["tmp_name"])){
//		$arquivoSalvo = $file->setUpload($_FILES["foto"]["name"]);
//	}
//		ver($arquivoSalvo, d);
	
	echo "<script>
			alert('Operação realizada com sucesso!');
			window.location='gpebserh.php?modulo=principal/cadColaborador_documento&acao=A&cpf=' + '".$cpf."';
		  </script>";
	die;
	
}

if( $_REQUEST['req'] ){
$_REQUEST['req']();

}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
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
<form method="post" name="formulario" id="formulario" action="">
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
				<td>
					<h4>Características Físicas</h4>
				</td>
				<td>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Grupo Sanguíneo: </td>
				<td>
					<?php
					$sql = "SELECT gsgid as codigo, gsgdsc as descricao FROM gpebserh.gruposanguinio";
					$db->monta_combo("gsgid",$sql,"S","Selecione...",'','','','','',"tiposanguineo");
					?>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Fator RH: </td>
				<td>
					<?// ver($dpefatorrh,d);?>
					<select class="CampoEstilo" name="fatorrh" id="fatorrh" value="<?$dados[0]['fatorrh'];?>">
						<option value="">Selecione...</option>
						<option <?if( $dpefatorrh =='pos')  echo "selected=selected"; ?> value="pos">Positivo</option>
						<option <?if( $dpefatorrh =='neg' )  echo "selected=selected"; ?>  value="neg">Negativo</option> 
					</select>  
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Cor/Origem: </td>
				<td>
					<?php
					$sql = "SELECT etnid as codigo, etndsc as descricao FROM gpebserh.etnia";
					$db->monta_combo("etnid",$sql,"S","Selecione...",'','','','','S','etnid');
					?>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Tipo de Deficiência: </td>
				<td>
					<?
						$sql = "SELECT tpdid as codigo, tpddsc as descricao FROM gpebserh.tipodeficiencia";
						$db->monta_combo("tpdid",$sql,"S","Selecione...",'','');?>
					
				</td>
			</tr> 
			<tr>
				<td>
			
			
			<tr>
				<td class ="SubTituloDireita" align="right">  </td>
				<td><input type="button" class="salvar" value="Salvar"></td>
			</tr> 
		</tbody>
	</table>
</form>