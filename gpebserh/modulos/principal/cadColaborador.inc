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

if ( $_POST['req'] == 'salvar' ){
	salvar();
}


if( $_GET['acao'] == 'E' && ($_REQUEST['dpeid'] || $_SESSION['gpebserh']['dpeid']) ){
	$dpeidInicio = ($_REQUEST['dpeid'] ? $_REQUEST['dpeid'] : $_SESSION['gpebserh']['dpeid']);
	$_SESSION['gpebserh']['dpeid'] = $dpeidInicio;
	$dadosP = $db->pegaLinha("SELECT * FROM gpebserh.dadospessoais WHERE dpeid = '".$dpeidInicio."'");
	extract($dadosP);
	$dpecpf = formatar_cpf( $dpecpf );
	$_SESSION['gpebserh']['dpeid'] = $dpeid;
	$habilCpf = 'N';
}else{
	unset($_SESSION['gpebserh']['dpeid']);
	$habilCpf = 'S';
}

function getImagemServidor(){
	global $db;
	$dpeid = $_SESSION['gpebserh']['dpeid'];

	if ( $dpeid ){
		$sql = "SELECT arqid FROM gpebserh.fotosdadospessoais  WHERE dpeid = '".$dpeid."'";
		$arqid = $db->pegaUm( $sql );
	}
	return ($arqid ? $arqid : false);
	
}
//unset( $_SESSION['sessionFILES']);
//unset( $_FILES['Arquivo']);


function salvar(){
	
	global $db;
//	$cpf = str_replace('.','',$_POST['cadcpf']);
//	$cpf = str_replace('-','',$cpf);
//	$_SESSION['gpebserh']['cpf']= $cpf; 
//	$sql = "SELECT dpeid FROM gpebserh.dadospessoais where dpecpf = '".$cpf."'";
//	$dpeid = $db->pegaUm($sql);

	$dpeid 			 = $_POST['dpeid'];
	$_POST['dpecpf'] = pega_numero( $_POST['dpecpf'] );
	
	if($dpeid){
		//alteração
	//ver($_FILES["Arquivo"]["name"],d);
	if($_FILES["Arquivo"]["name"]){	
		if($db->pegaUm("SELECT arqid FROM gpebserh.fotosdadospessoais  WHERE dpeid = ".$dpeid)){
//			ver("teste",d);
			$sql = "DELETE FROM gpebserh.fotosdadospessoais WHERE dpeid = ".$dpeid;
		
			$db->executar($sql);
			$db->commit();	
		}
		$campos	= array("dpeid"	=>$dpeid);	
		$file = new FilesSimec("fotosdadospessoais", $campos ,"gpebserh");	
		$arquivoSalvo = $file->setUpload("Foto pessoal do colaborador");
		}
	
		$valores = "carid = ".$_POST['carid'].", dpestatus = 'A'";
		
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
		if(!($_POST['dpedigveragencia'] == "")){
			$valores .= ", dpedigveragencia = '".$_POST['dpedigveragencia']."'";
		}
		if($_POST['dpecontacorrente']){
			$valores .= ", dpecontacorrente = '".$_POST['dpecontacorrente']."'";
		}
		if(!($_POST['dpedigverconta'] == "")){
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
			$cep = str_replace('.','',$_POST['depceplogradouro']);
			$cep = str_replace('-','',$cep);
			$valores .= ", depceplogradouro = '".$cep."'";
		}
		if($_POST['dpenumddd']){
			$valores .= ",dpenumddd =  '".$_POST['dpenumddd']."'";
		}
		if($_POST['dpenumtelefone']){
			$valores .= ", dpenumtelefone = '".$_POST['dpenumtelefone']."'";
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
//		ver($_POST['muncodlogradouro'],d);
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
	
		$sql = "UPDATE gpebserh.dadospessoais SET ".$valores." where dpeid = " . $dpeid . "";
		//ver($sql,d);
	}else{
		
		$valores = "'".$_POST['dpecpf']."','".$_POST['estufnascimento']."', ".$_POST['carid'].",'".$_POST['dpenome']."'";
		$campo = "dpecpf, estufnascimento, carid, dpenome";
	
		if($_POST['etcid']){
		 	$valores .= ",".$_POST['etcid']; 
		 	$campo .= ", etcid";
		}
		if($_POST['nesid']){
			$valores .= ",".$_POST['nesid'];
			$campo .= ", nesid";  
		}
		if($_POST['nacid']){
		 	$valores .= ",".$_POST['nacid']; 
		 	$campo .= ", nacid";
		}
		if($_POST['paiidorigem']){
			$valores .= ",".$_POST['paiidorigem'];
			$campo .= ", paiidorigem";  
		}
		if($_POST['gsgid']){
		 	$valores .= ",".$_POST['gsgid']; 
		 	$campo .= ", gsgid";
		}
		if($_POST['tpdid']){
			$valores .= ",".$_POST['tpdid'];
			$campo .= ", tpdid";  
		}
		if($_POST['paiidlogradouro']){
		 	$valores .= ",".$_POST['paiidlogradouro']; 
		 	$campo .= ", paiidlogradouro";
		}
		//if($_POST['arqid']){
		//	$valores .= ",".$_POST['arqid'];
		//	$campo .= ", arqid";  
		//}
		
		if($_POST['dpenumagencia']){
			$valores .= ", '".$_POST['dpenumagencia']."'";
			$campo .= ", dpenumagencia";  
		}
		if(!($_POST['dpedigveragencia']=="")){
			$valores .= ", '".$_POST['dpedigveragencia']."'";
			$campo .= ", dpedigveragencia";  
		}
		if($_POST['dpecontacorrente']){
			$valores .= ", '".$_POST['dpecontacorrente']."'";
			$campo .= ", dpecontacorrente";  
		}
		if(!($_POST['dpedigverconta']=="")){
			$valores .= ", '".$_POST['dpedigverconta']."'";
			$campo .= ", dpedigverconta";  
		}
		if($_POST['dpenumctps']){
			$valores .= ", '".$_POST['dpenumctps']."'";
			$campo .= ", dpenumctps";  
		}
		if($_POST['dpenumseriectps']){
			$valores .= ", '".$_POST['dpenumseriectps']."'";
			$campo .= ", dpenumseriectps";  
		}
		if($_POST['dpenumrg']){
			$valores .= ", '".$_POST['dpenumrg']."'";
			$campo .= ", dpenumrg";  
		}
		if($_POST['deporgaoexprg']){
			$valores .= ", '".$_POST['deporgaoexprg']."'";
			$campo .= ", deporgaoexprg";  
		}
		if($_POST['dpelogradouro']){
			$valores .= ", '".$_POST['dpelogradouro']."'";
			$campo .= ", dpelogradouro";  
		}
		if($_POST['dpenumlogradouro']){
			$valores .= ", '".$_POST['dpenumlogradouro']."'";
			$campo .= ", dpenumlogradouro";  
		}
		if($_POST['dpecompllogradouro']){
			$valores .= ", '".$_POST['dpecompllogradouro']."'";
			$campo .= ", dpecompllogradouro";  
		}
		if($_POST['dpebairrologradouro']){
			$valores .= ", '".$_POST['dpebairrologradouro']."'";
			$campo .= ", dpebairrologradouro";  
		}
		if($_POST['depceplogradouro']){
			$cep = str_replace('.','',$_POST['depceplogradouro']);
			$cep = str_replace('-','',$cep);
			$valores .= ", '".$cep."'";
			$campo .= ", depceplogradouro";  
		}
		if($_POST['dpenumddd']){
			$valores .= ", '".$_POST['dpenumddd']."'";
			$campo .= ", dpenumddd";  
		}
		if($_POST['dpenumtelefone']){
			$valores .= ", '".$_POST['dpenumtelefone']."'";
			$campo .= ", dpenumtelefone";  
		}
		if($_POST['dpeemail']){
			$valores .= ", '".$_POST['dpeemail']."'";
			$campo .= ", dpeemail";  
		}
		if($_POST['dpenumbanco']){
			$valores .= ", '".$_POST['dpenumbanco']."'";
			$campo .= ", dpenumbanco";  
		}
		if($_POST['dpenomeagencia']){
			$valores .= ", '".$_POST['dpenomeagencia']."'";
			$campo .= ", dpenomeagencia";  
		}
		if($_POST['dpenomemae']){
			$valores .= ", '".$_POST['dpenomemae']."'";
			$campo .= ", dpenomemae";  
		}
		if($_POST['dpefatorrh']){
			$valores .= ", '".$_POST['dpefatorrh']."'";
			$campo .= ", dpefatorrh";  
		}
		if($_POST['dpeqtddependente']){
			$valores .= ", ".$_POST['dpeqtddependente'];
			$campo .= ", dpeqtddependente";  
		}
		if($_POST['dpesexo']){
			$valores .= ", '".$_POST['dpesexo']."'";
			$campo .= ", dpesexo";  
		}
		if($_POST['dpeanochegada']){
			$valores .= ", '".$_POST['dpeanochegada']."'";
			$campo .= ", dpeanochegada";  
		}
		if($_POST['dpepispasep']){
			$valores .= ", '".$_POST['dpepispasep']."'";
			$campo .= ", dpepispasep";  
		}
		if($_POST['dpenumtitulo']){
			$valores .= ", '".$_POST['dpenumtitulo']."'";
			$campo .= ", dpenumtitulo";  
		}
		if($_POST['dpesiape']){
			$valores .= ", '".$_POST['dpesiape']."'";
			$campo .= ", dpesiape";  
		}
		if($_POST['estufctps']){
		 	$valores .= ", '".$_POST['estufctps']."'"; 
		 	$campo .= ", estufctps";
		}
		if($_POST['estuflogradouro']){
			$valores .= ", '".$_POST['estuflogradouro']."'";
			$campo .= ", estuflogradouro";  
		}
		if($_POST['muncodlogradouro']){
		 	$valores .= ",".$_POST['muncodlogradouro']; 
		 	$campo .= ", muncodlogradouro";
		}
		if($_POST['estufrg']){
			$valores .= ",'".$_POST['estufrg']."'";
			$campo .= ", estufrg";  
		}
		if($_POST['depeditalreg']){
		 	$valores .= ", '".$_POST['depeditalreg']."'"; 
		 	$campo .= ", depeditalreg";
		}
		if($_POST['dpedtnasc']){
			$dtnascimento = substr($_POST['dpedtnasc'], 6, 4).'-'.substr($_POST['dpedtnasc'], 3, 2).'-'.substr($_POST['dpedtnasc'], 0, 2);
			$valores .= ", '".$dtnascimento."'";
			$campo .= ", dpedtnasc";  
		}
		if($_POST['dpedtprimemprego']){
		 	$dtprimeiroemprego = substr($_POST['dpedtprimemprego'], 6, 4).'-'.substr($_POST['dpedtprimemprego'], 3, 2).'-'.substr($_POST['dpedtprimemprego'], 0, 2);
		 	$valores .= ", '".$dtprimeiroemprego."'"; 
		 	$campo .= ", dpedtprimemprego";
		}
			
		$sql = "INSERT INTO gpebserh.dadospessoais(".$campo.") VALUES (".$valores.") RETURNING dpeid;";
		//ver($sql,d);
	}
	//ver($sql,d);
	if ($dpeid){
		$db->executar( $sql );
	}else{
		$dpeid = $db->pegaUm( $sql );
		if($_FILES["Arquivo"]){	
			$campos	= array("dpeid"	=>$dpeid);	
			$file = new FilesSimec("fotosdadospessoais", $campos ,"gpebserh");	
			$arquivoSalvo = $file->setUpload("Foto pessoal do colaborador");
		}
		
	}
	$db->commit();
	$_SESSION['gpebserh']['dpeid'] = $dpeid;
	
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
			window.location='gpebserh.php?modulo=principal/cadColaborador_fisica&acao=A';
		  </script>";
	die;
}

//Chamada de programa
if ($_POST['filtraMunicipio'] && $_POST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipioEBS($_POST['estuf']);
	exit;
}
if ($_POST['filtraNacionalidade'] && $_POST['nacionalidade']) {
	header('content-type: text/html; charset=ISO-8859-1');
	if (($_POST['nacionalidade']== 1)||($_POST['nacionalidade']== 2)){
		$pais = 1;
		echo 1;
		exit;
	}else{
		echo 99;
	exit;}
}

//Buscar CPF
if ($_POST['buscarCpf'] && $_POST['dpecpf']) {
	$cpf = pega_numero( $_POST['dpecpf'] );
	
	//$sql = "SELECT estufnascimento || '|' || etnid || '|' || carid || '|' || dpenome || '|' || dpedtinclusao || '|' || dpestatus FROM gpebserh.dadospessoais WHERE dpecpf = '".$cpf."'";
	$sql = "SELECT 	
				dp.dpeid
			FROM 
				gpebserh.dadospessoais dp
			WHERE 
				dpecpf = '".$cpf."'";
	
/*	$sql = "SELECT dpesiape , etcid , nesid , nacid , estufnascimento , paiidorigem , etnid
			 , gsgid , tpdid , estufctps , estuflogradouro , muncodlogradouro , carid 
			 , paiidlogradouro ,estufrg , depeditalreg , dpecpf , dpenome , dpesexo , dpeanochegada
			 , dpepispasep , dpenumtitulo , dpenomemae , dpefatorrh , dpeqtddependente , dpenumrg
			 , dpeorgaoexprg , dpelogradouro , dpenumlogradouro , dpecompllogradouro , dpebairrologradouro , depceplogradouro
			 , dpenumddd , dpenumtelefone , dpeemail , TO_CHAR(dpedtprimemprego,'dd/mm/YYYY')as dpedtprimemprego , dpenumbanco , dpenomeagencia , dpenumagencia
			 , dpedigveragencia , dpecontacorrente , dpedigverconta , dpenumctps , dpenumseriectps , TO_CHAR(dpedtnasc,'dd/mm/YYYY')as dpedtnasc FROM gpebserh.dadospessoais  WHERE dpecpf = '".$cpf."'";*/
	//$nome = $db->pegaUm($sql);
	$dpeid = $db->pegaUm( $sql );
	
	if( $dpeid ){
		$_SESSION['gpebserh']['dpeid'] = $dpeid;
		echo $dpeid;
	}else{
	echo 99;
	}
	exit;
}

//if( $dpecpf ){
//	$arMnuid = array();
//} else {
//	$arMnuid = array( //0 => 12630,
//					//1 => 12631,
//					2 => 12632,
//					3 => 12635,
//					4 => 12636,
//					 );
//}
include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
echo '<div id="teste">';
//$db->cria_aba( $abacod_tela, $url, '', $arMnuid );
$db->cria_aba($abacod_tela,$url,'');
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
	jQuery('.limpar').click(function(){
		jQuery('#dpecpf').val('');
		jQuery('#depeditalreg').val('');
	  	jQuery('#dpenome').val('');
		jQuery('#dpesiape').val('');
		jQuery('#dpepispasep').val('');
		jQuery('#etcid').val('');
		jQuery('#dpenomemae').val('');
		jQuery('#sexof').attr("checked",false);
		jQuery('#sexom').attr("checked",false);
		jQuery('#dpedtnasc').val('');
		jQuery('#nacid').val('');
		jQuery('#paiidorigem').val('');
		jQuery('#dpenome').val('');
		//jQuery('#dpeanochegada').val('');
		jQuery('#estufnascimento').val('');
		jQuery('#dpelogradouro').val('');
		jQuery('#dpenumlogradouro').val('');
		jQuery('#dpecompllogradouro').val('');
		jQuery('#estufnascimento').val('');
		jQuery('#dpebairrologradouro').val('');
		jQuery('#estuflogradouro').val('');
		jQuery('#muncodlogradouro').val('');
		jQuery('#depceplogradouro').val('');
		jQuery('#dpenumddd').val('');
		jQuery('#dpenumtelefone').val('');
		jQuery('#dpeemail').val('');
		jQuery('#nesid').val('');
		jQuery('#carid').val('');
//       	jQuery('#teste').html('<?//=$db->cria_aba( $abacod_tela, $url, '',$arMnuid ); ?>');

	});
	
	//jQuery('#dpecpf').blur(function(){
	//	var cpf = jQuery(this).val();
	//	jQuery.ajax({
	//		   type: "POST",
		//	   url: window.location,
		//	   data: "buscarCpf=true&dpecpf=" + cpf,
		//	   success: function(resp){
		//	  	   	if( resp != 99 ){
		//		   		dados = resp;
		//	   	    	dados = dados.split('|');
		//				jQuery('#nome').val(dados[3]);
		//				jQuery('#etnia').val(dados[1]);
		//				jQuery('#carid').val(dados[2]);
		//				jQuery('#estufnascimento').val(dados[0]);
						
		//		   	}
		//		}
	//	});
//	});
	jQuery('#dpecpf').blur(function(){
		var cpf = jQuery(this).val();
//		alert(cpf);
		//var teste;
     	 //Requisição Ajax
      	jQuery.ajax({
			   type: "POST",
			   url: window.location,
//			   dataType: "json", //Tipo de Retorno
			   data: "buscarCpf=true&dpecpf=" + cpf,
        	   success: function(resp){
        	   		if(resp != 99){
        	   		location.href = '?modulo=principal/cadColaborador&acao=E';
        	   		}
			  	   //	jQuery('#arqid').html(resp.arqid);
//			  	   	jQuery('#depeditalreg').val(resp.depeditalreg);
//			  	   	jQuery('#dpenome').val(resp.dpenome);
//			  	   	jQuery('#dpesiape').val(resp.dpesiape);
//			  	   	jQuery('#dpepispasep').val(resp.dpepispasep);
//			  	   	jQuery('#etcid').val(resp.etcid);
//			  	   	jQuery('#dpenomemae').val(resp.dpenomemae);
//			  	   	if(resp.dpesexo == 'm'){
//			  	   		jQuery('#sexof').attr("checked",false);
//			  	   		jQuery('#sexom').attr("checked",true);
//			  	   	}else if(resp.dpesexo == 'f'){
//			  	   		jQuery('#sexom').attr("checked",false);
//			  	   		jQuery('#sexof').attr("checked",true);
//			  	   	}
//			  	   	
//			  	   	jQuery('#dpedtnasc').val(resp.dpedtnasc);
//			  	   	jQuery('#nacid').val(resp.nacid);
//			  	   	jQuery('#paiidorigem').val(resp.paiidorigem);
//			  	   	jQuery('#dpenome').val(resp.dpenome);
//			  	  // 	jQuery('#dpeanochegada').val(resp.dpeanochegada);
//			  	   	jQuery('#estufnascimento').val(resp.estufnascimento);
//			  	   	jQuery('#dpelogradouro').val(resp.dpelogradouro);
//			  	   	jQuery('#dpenumlogradouro').val(resp.dpenumlogradouro);
//			  	   	jQuery('#dpecompllogradouro').val(resp.dpecompllogradouro);
//			  	   	jQuery('#estufnascimento').val(resp.estufnascimento);
//			  	   	jQuery('#dpebairrologradouro').val(resp.dpebairrologradouro);
//			  	   	jQuery('#estuflogradouro').val(resp.estuflogradouro);
//			  	   
//			  	   	
//			  	   	jQuery('#muncodlogradouro').val(resp.muncodlogradouro);
//			  	   	jQuery('#depceplogradouro').val(resp.depceplogradouro);
//			  	   	jQuery('#dpenumddd').val(resp.dpenumddd);
//			  	   	jQuery('#dpenumtelefone').val(resp.dpenumtelefone);
//			  	   	jQuery('#dpeemail').val(resp.dpeemail);
//			  	   	jQuery('#nesid').val(resp.nesid);
//			  	   	jQuery('#carid').val(resp.carid);
////			  	   	if(resp.carid){
////        	   			jQuery('#teste').html('<?//=$db->cria_aba( $abacod_tela, $url, '' ); ?>');
////        	   		}
//			  	   	
//			  	   	if ( resp.arqid ){
//			  	   		jQuery('#foto').attr('src', '../slideshow/slideshow/verimagem.php?arqid=' + resp.arqid + '&_sisarquivo=gpebserh');
//			  	   		jQuery('#foto').show();
//			  	   	}else{
//			  	   		jQuery('#foto').hide();
//			  	   	}
			}
		});
	});
	//Quando o Documento HTML estiver carregado
	
	jQuery('#nacionalidade').change(function(){
			
		var nacionalidade = jQuery(this).val();
		
		if(nacionalidade!=''){
			jQuery.ajax({
				   type: "POST",
				   url: window.location,
				   data: "filtraNacionalidade=true&nacionalidade=" + nacionalidade,
				   success: function(resp){
				   	  if(resp!=99){
						jQuery('#pais').val(resp);
					  }
					  else{
					  	
					  }
					}
			});
		}
	});
	jQuery('#estuflogradouro').change(function(){
			
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
<form method="post" name="formulario" id="formulario" action="" enctype="multipart/form-data">
	<input type="hidden" name="dpeid" id="dpeid" value="<?=$dpeid ?>"/>
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
				<td class="SubTituloDireita">CPF:</td>
				<td colspan="3">
				<?php
					echo campo_texto( 'dpecpf', 'N', $habilCpf, '', 20, 80, '###.###.###-##', '','left','',0,'id="dpecpf"','',$dpecpf,""); 
				?>
				</td>
				
			</tr>
		    <tr>
				<td class ="SubTituloDireita" align="right">Nome: </td>
					<td>
						<?= campo_texto('dpenome', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="dpenome" onblur="MouseBlur(this);"' ); ?>  
					</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Edital:</td>
				<td colspan="3">
				<?php
					echo campo_texto( 'depeditalreg', 'N', 'S', '', 20, 80, '', '','left','',0,'id="depeditalreg"','',$depeditalreg,""); 
				?>
				</td>
				
			</tr>
            <tr>
            	<td class ="SubTituloDireita" align="right">Foto: </td>
				      <td>
						<?if( !getImagemServidor()  ) { ?>
	     					Imagem não cadastrada.
	     				 
	     				<? }else{ ?>
				   
						        <div  style="width: 3cm; height: 4cm; border: 8px solid #CDC9C9; text-align: center;">
								<div style="width: 3cm; position: relative; height: 4cm; border: 4px solid white; width: 100%; left:-4px; top:-3px;">
		     						<img id="foto" width="3cm" height="4cm" src="../slideshow/slideshow/verimagem.php?arqid=<?=getImagemServidor();?>&_sisarquivo=gpebserh" id="imagem" style="width: 3cm; height: 4cm;">
		     					</div>
			     			</div>
							<? }?>
						Arquivo: <input type="file" name="Arquivo" />     
				</td>
					
            </tr> 

			<tr>
				<td class ="SubTituloDireita" align="right">Matrícula SIAPE: </td>
				<td>
					<?= campo_texto('dpesiape', 'N', $somenteLeitura, '', 12, 10, '', '', 'left', '',  0, 'id="dpesiape"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">PIS/PASEP: </td>
				<td>
					<?= campo_texto('dpepispasep', 'N', $somenteLeitura, '', 12, 10, '', '', 'left', '',  0, 'id="dpepispasep"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" >Estado Civil:</td>
				<td colspan="3">
				<?php
					$sql = "SELECT etcid as codigo, etcdsc as descricao FROM gpebserh.estado_civil";
					$db->monta_combo( 'etcid', $sql, 'S', 'Selecione', '', '','','','S','etcid' );
				?>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Nome da Mãe: </td>
				<td>
					<?= campo_texto('dpenomemae', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="dpenomemae" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Sexo: </td>
				<td>
					<input type="radio" name="dpesexo" id="sexom" <? if( $dpesexo == 'm') echo 'checked = checked'; ?> value="m"> Masculino &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<input type="radio" name="dpesexo" id="sexof" <? if( $dpesexo == 'f') echo 'checked = checked'; ?> value="f"> Feminino 
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Data de Nascimento: </td>
				<td>
					<?= campo_data2( 'dpedtnasc','S', 'S', '', 'S' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Nacionalidade: </td>
				<td>
				<?php
					
			        $sql = "SELECT nacid AS codigo, nacdsc AS descricao FROM gpebserh.nacionalidade ORDER BY nacdsc"; 
					$db->monta_combo( "nacid", $sql, "S", "Selecione...",'','','','','S','nacid'); 
					//$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">País: </td>
				<td>
				<?php
			        $sql = "SELECT paiid as codigo, paidescricao as descricao FROM territorios.pais"; 
					$db->monta_combo( "paiidorigem", $sql, "S", "Selecione...",'','','','','N','paiidorigem'); 
					//$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Naturalidade: </td>
				<td>
				<?php
			        $sql = "SELECT * FROM ( (SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao)
			        		UNION ALL
			        		(SELECT 'OU' As codigo, 'Outro' As descricao)) as foo"; 
					$db->monta_combo( "estufnascimento", $sql, "S", "Selecione...",'','','','',"S",'estufnascimento'); 
					//$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Endereço Residencial: </td>
				<td>
					<?= campo_texto('dpelogradouro', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="dpelogradouro" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Nr: </td>
				<td>
					
					<?= campo_texto('dpenumlogradouro', 'N', $somenteLeitura, '', 6, 200, '', '', 'left', '',  0, 'id="dpenumlogradouro" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Complemento: </td>
				<td>
					<? $enderecoresidencial = $dados[0]['enderecocomplemento']; ?>
					<?= campo_texto('dpecompllogradouro', 'N', $somenteLeitura, '', 30, 200, '', '', 'left', '',  0, 'id="dpecompllogradouro" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Bairro: </td>
				<td>
					<?= campo_texto('dpebairrologradouro', 'N', $somenteLeitura, '', 30, 200, '', '', 'left', '',  0, 'id="dpebairrologradouro" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" >Estado:</td>
				<td colspan="3">
				<?php
			        $sql = "SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao"; 
					$db->monta_combo( "estuflogradouro", $sql, "S", "Selecione...", '', "","","","N","estuflogradouro","",$estuflogradouro); 
					//$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
				?>
				</td>
			</tr>
				<tr>
				<td class="SubTituloDireita" >Cidade:</td>
				<td id = "td_municipio" colspan="3">
				
				<?php
				
					$sql = "SELECT ter.muncod AS codigo, ter.mundescricao AS descricao FROM territorios.municipio ter WHERE ter.estuf = '$estuflogradouro' ORDER BY ter.estuf, ter.mundescricao";
					$db->monta_combo('muncodlogradouro',$sql,'S','Selecione...','','','','','N','muncodlogradouro','',$muncodlogradouro);
					
					//$sql = "select 	muncod as codigo, mundescricao as descricao FROM territorios.municipio";
					//$db->monta_combo( "municipio", $sql, 'S', 'Selecione', '', '' );
				?>
				</td>
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">CEP: </td>
				<td>
					<?= campo_texto('depceplogradouro', 'N', $somenteLeitura, '', 10, 10, '##.###-###', '', 'left', '',  0, 'id="depceplogradouro" onkeyup="this.value=mascaraglobal(\'########\',this.value); onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">Telefone: </td>
				<td>
					(<?= campo_texto('dpenumddd', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '',  0, 'id="dpenumddd"  onkeyup="this.value=mascaraglobal(\'##\',this.value);" onblur="MouseBlur(this);"' ); ?>)  <?= campo_texto('dpenumtelefone', 'N', $somenteLeitura, '', 11, 9, '', '', 'left', '',  0, 'id="dpenumtelefone"  onkeyup="this.value=mascaraglobal(\'####-####\',this.value);" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">e-mail: </td>
				<td>
					<?= campo_texto('dpeemail', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '',  0, 'id="dpeemail" onblur="MouseBlur(this);"' ); ?>  
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" >Nível de Escolaridade:</td>
				<td colspan="3">
					<?
					$sql = "SELECT nesid as codigo, nesdsc as descricao FROM gpebserh.nivelescolaridade";
					
					$db->monta_combo( "nesid", $sql, 'S', 'Selecione', '', '', '', '', 'N', "nesid" );
					?>
				</td> 
			</tr>
			<tr>
				<td class ="SubTituloDireita" align="right">Cargo: </td>
				<td>
					<?
						$sqlCargo = "SELECT carid as codigo, cardsc as descricao FROM gpebserh.cargo";
						$db->monta_combo("carid", $sqlCargo, "S","Selecione","","","","","S","carid");
					?>
				</td>
			</tr> 
			<tr>
				<td class ="SubTituloDireita" align="right">  </td>
				<td>
					<input type="button" class="salvar" value="Salvar">
					<?php 
					if ( $_GET['acao'] == 'E' ):
					?>
					<!-- <input type="button" value="Novo Cadastro" onclick="location.href = '?modulo=principal/cadColaborador&acao=A';"> -->
					<?php 
					endif;
					?>
					<!--  
					<input type="button" class="limpar" value="Limpar">
					-->
				</td>
			</tr> 
 
		</tbody>
	</table>
</form>