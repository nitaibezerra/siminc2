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

	$dpeid = $_SESSION['gpebserh']['dpeid'];
	
	$valores = "dpeid = " . $dpeid;

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
	//if($_POST['arqid']){
	//	$valores .= ",arqid = ".$_POST['arqid'];
	//}
	
	if($_POST['dpenumagencia']){
		$valores .= ", dpenumagencia = '".$_POST['dpenumagencia']."'";
	}
	if(!($_POST['dpedigveragencia']=="")){
		$valores .= ", dpedigveragencia = '".$_POST['dpedigveragencia']."'";
	}
	if($_POST['dpecontacorrente']){
		$valores .= ", dpecontacorrente = '".$_POST['dpecontacorrente']."'";
	}
	if($_POST['dpedigverconta'] != ""){
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

	$sql = "UPDATE gpebserh.dadospessoais SET ".$valores." where dpeid = " . $dpeid;
	//ver($_POST,d);
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
			window.location='gpebserh.php?modulo=principal/cadColaborador_dependente&acao=A&cpf=' + '".$cpf."';
		  </script>";
	die;
	
}

if( $_REQUEST['req'] ){
$_REQUEST['req']();

}

//Chamada de programa
if ($_POST['filtraMunicipio'] && $_POST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipioEBS($_POST['estuf']);
	exit;
}

//Buscar CPF
if ($_POST['buscarCpf'] && $_POST['cadcpf']) {
	$cpf = str_replace('.','',$_POST['cadcpf']);
	$cpf = str_replace('-','',$cpf);
//	$sql = "SELECT estufnascimento || '|' || etnid || '|' || carid || '|' || dpenome || '|' || dpedtinclusao || '|' || dpestatus FROM gpebserh.dadospessoais WHERE dpecpf = '".$cpf."'";
	$sql = "SELECT dpesiape , etcid , nesid , nacid , estufnascimento , paiidorigem , etnid
			 , gsgid , tpdid , estufctps , estuflogradouro , muncodlogradouro , carid , arqid
			 , paiidlogradouro ,estufrg , depeditalreg , dpecpf , dpenome , dpesexo , dpeanochegada
			 , dpepispasep , dpenumtitulo , dpenomemae , dpefatorrh , dpeqtddependente , dpenumrg
			 , deporgaoexprg , dpelogradouro , dpenumlogradouro , dpecompllogradouro , dpebairrologradouro , depceplogradouro
			 , dpenumddd , dpenumtelefone , dpeemail , dpedtprimemprego , dpenumbanco , dpenomeagencia , dpenumagencia
			 , dpedigveragencia , dpecontacorrente , dpedigverconta , dpenumctps , dpenumseriectps , dpedtnasc FROM gpebserh.dadospessoais  WHERE dpecpf = '".$cpf."'";
	/*$sql = "SELECT dpesiape || '|' || etcid || '|' || nesid || '|' || nacid || '|' || estufnascimento || '|' || paiidorigem || '|' || etnid
			 || '|' || gsgid || '|' || tpdid || '|' || estufctps || '|' || estuflogradouro || '|' || muncodlogradouro || '|' || carid || '|' || arqid
			 || '|' || paiidlogradouro || '|' ||estufrg || '|' || depeditalreg || '|' || dpecpf || '|' || dpenome || '|' || dpesexo || '|' || dpeanochegada
			 || '|' || dpepispasep || '|' || dpenumtitulo || '|' || dpenomemae || '|' || dpefatorrh || '|' || dpeqtddependente || '|' || dpenumrg
			 || '|' || deporgaoexprg || '|' || dpelogradouro || '|' || dpenumlogradouro || '|' || dpecompllogradouro || '|' || dpebairrologradouro || '|' || depceplogradouro
			 || '|' || dpenumddd || '|' || dpenumtelefone || '|' || dpeemail || '|' || dpedtprimemprego || '|' || dpenumbanco || '|' || dpenomeagencia || '|' || dpenumagencia
			 || '|' || dpedigveragencia || '|' || dpecontacorrente || '|' || dpedigverconta || '|' || dpenumctps || '|' || dpenumseriectps || '|' || dpedtnasc FROM gpebserh.dadospessoais 
			 WHERE dpecpf = '".$cpf."'";*/
	
	//$nome = $db->pegaUm($sql);
	$nome = $db->pegaLinha($sql);
	if( $nome ){
		echo $nome;
	} else {
		echo 99;
	}
	exit;
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
	
	jQuery('#cadcpf').blur(function(){
		var cpf = jQuery(this).val();
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "buscarCpf=true&cadcpf=" + cpf,
			   success: function(resp){
			  for(i=0;i<resp.length;i++){
					alert(resp[i]);
				}
				   	if( resp != 99 ){
				   		dados = resp;
			   	    	dados = dados.split('|');
						jQuery('#nome').val(dados[3]);
						jQuery('#etnia').val(dados[1]);
						jQuery('#carid').val(dados[2]);
						jQuery('#estufnascimento').val(dados[0]);
						
				   	}
				}
		});
	});
	
	jQuery('#estuf').change(function(){
			
		var estuf = jQuery(this).val();
		if(estuf!=''){
			
			jQuery.ajax({
				
				   type: "POST",
				   url: window.location,
				   data: "filtraMunicipio=true&estuf=" + estuf,
				   success: function(resp){
						jQuery('#td_municipio').html(resp);
					}
			});
		}
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
					<h4>Documentos</h4>
				</td>
				<td>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">CTPS Nº: </td>
				<td>
					<?= campo_texto('dpenumctps', 'N', $somenteLeitura, '', 20, 20, '', '', 'left', '',  0, 'id="dpenumctps"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">CTPS Nº de Série: </td>
				<td>
					<?= campo_texto('dpenumseriectps', 'N', $somenteLeitura, '', 20, 20, '', '', 'left', '',  0, 'id="dpenumseriectps"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">CTPS UF: </td>
				<td>
					<?
			        $sql = "SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao"; 
					$db->monta_combo( "estufctps", $sql, "S", "Selecione...", '',''); ?>
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Carteira de Identidade Nº: </td>
				<td>
					<?= campo_texto('dpenumrg', 'N', $somenteLeitura, '', 20, 20, '', '', 'left', '',  0, 'id="dpenumrg"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right"> Órgão Expedidor Identidade: </td>
				<td>
					<?= campo_texto('deporgaoexprg', 'N', $somenteLeitura, '', 10, 10, '', '', 'left', '',  0, 'id="deporgaoexprg" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right"> UF Identidade: </td>
				<td>
					<?$sql = "SELECT 
								estuf AS codigo, 
								estdescricao AS descricao
								FROM
								territorios.estado"; 
			                 
						$db->monta_combo('estufrg', $sql, 'S', "Selecione...", '', '', '', '100', 'N', 'estufrg');
					?>
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Data. 1º Emprego: </td>
				<td>
					<?= campo_data2( 'dpedtprimemprego','N', 'S', '', 'S' ); ?>

					
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right"> Título de Eleitor: </td>	
				<td>
					<?= campo_texto('dpenumtitulo', 'N', $somenteLeitura, '', 18, 20, '', '', 'left', '',  0, 'id="dpenumtitulo" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td>
					<h4>Dados Bancários</h4>
				</td>
				<td>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right"> Banco Número: </td>
				<td>
					<?= campo_texto('dpenumbanco', 'N', $somenteLeitura, '', 10, 50, '', '', 'left', '',  0, 'id="dpenumbanco" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right"> Nome da Agência: </td>
				<td>
					<?= campo_texto('dpenomeagencia', 'N', $somenteLeitura, '', 10, 50, '', '', 'left', '',  0, 'id="dpenomeagencia" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right"> Número e dígito da Agência: </td>
				<td>
					<?= campo_texto('dpenumagencia', 'N', $somenteLeitura, '', 10, 50, '', '', 'left', '',  0, 'id="dpenumagencia" onblur="MouseBlur(this);"' ); ?>  
					-
					<?= campo_texto('dpedigveragencia', 'N', $somenteLeitura, '', 1, 3, '', '', 'left', '',  0, 'id="dpedigveragencia" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right"> Número  e dígito da Conta: </td>
				<td>
					<?= campo_texto('dpecontacorrente', 'N', $somenteLeitura, '', 10, 50, '', '', 'left', '',  0, 'id="dpecontacorrente" onblur="MouseBlur(this);"' ); ?>  
					-
					<?= campo_texto('dpedigverconta', 'N', $somenteLeitura, '', 1, 3, '', '', 'left', '',  0, 'id="dpedigverconta" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			
			
			<tr>
				<td class ="SubTituloDireita" align="right">  </td>
				<td><input type="button" class="salvar" value="Salvar"></td>
			</tr> 
		</tbody>
	</table>
</form>