<?php
include_once APPRAIZ . 'www/painel/_constantes.php'; 
include_once APPRAIZ . 'www/painel/_funcoes.php'; 

$nova  = $_GET['nova'];
$micid = $_GET['micid'];
$indid = $_GET['indid'];
$idvid = $_GET['idvid'];

if ($_GET['excluir'] && $_GET['idvid']){
	$sql = "delete from painel.indicadoresvinculados where idvid = '{$_GET['idvid']}'";
	$db->executar($sql);
	$db->commit();	
	echo '<script type="text/javascript">window.opener.location.reload();</script>';
	die;
}

if ( $micid && $indid ){
	$sql = "SELECT 
				indid
			FROM
				pde.monitoraitemchecklist
			WHERE
				micid = {$micid}";
	$indidvinculo = $db->pegaUm($sql);

	$where = $idvid ? " AND idvid = '$idvid'" : '';	

	$sql = "select * from painel.indicadoresvinculados iv 
				inner join pde.monitoraitemchecklist mic ON iv.indidvinculo = mic.indid
			where iv.indid = '{$indid}'
			and mic.micid = '{$micid}'
			and idvstatus = 'A'
			$where ";
	
	$dados = $db->pegaLinha( $sql );
//	ver($nova, $sql, $dados);
	
	if(!$nova){
		$idvid       = $dados['idvid'];
		$idvdsc      = $dados['idvdsc'];
		$idvmeta     = $dados['idvmeta'];
		$idvdatameta = $dados['idvdatameta'];
	}
	$arFiltro    = transformaFiltroIndicadorParaArray( $dados['idvfiltro'] );
	
}else{
	die('<script type="text/javascript"> 
			alert(\'Faltam parametros!\nTente novamente.\'); 
			window.close(); 
		 </script>');
}

if ( $_POST['salvaFiltro'] ){
	$idvdsc    = $_POST['idvdsc'];
	$idvfiltro = $_POST['idvfiltro'];
	
	$idvmeta   = $_POST['idvmeta'];
	
	if ( $idvid ){
		$sql = "UPDATE 
					painel.indicadoresvinculados 
				SET 
					idvdsc = '{$idvdsc}', 
					idvmeta = '{$idvmeta}', 
					idvfiltro = '{$idvfiltro}' 
				WHERE 
					idvid =  ". $idvid ."";			
	}else{
		$sql 	   = "INSERT INTO painel.indicadoresvinculados(
						indid, indidvinculo,idvstatus, idvfiltro, idvdsc, idvmeta
					  ) VALUES (
					  	". $indid .", ". $indidvinculo .",'A', '" . $idvfiltro . "', '" . $idvdsc . "', " . $idvmeta . "
					  )";			
	}
	$db->executar($sql);
	$db->commit();
	exit;
}

if($_POST['filtraEstado']){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$regcod = $_POST['regcod'];
	$filtro = $_POST['filtro'];
	
	if(!$regcod || $regcod == "todos"){
		$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by descricao";
	}else{
		$sql = "select estuf as codigo, estdescricao as descricao from territorios.estado where regcod = '$regcod' order by descricao";
	}
	
	if($filtro){
		$acao = "Indicador.filtraMunicipio";
	}else{
		$acao = "";
	}
	
	$db->monta_combo('estuf',$sql,'S','Selecione...',$acao,'','','100','N',"estuf","",$_POST['estuf']);	
	
	exit;
}

if($_REQUEST['filtraMunicipio'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$estuf = $_POST['estuf'];
	
	$sql = "select 
				muncod,mundescricao 
			from
				territorios.municipio
			where
				estuf = '{$estuf}'
			order by
				mundescricao";
	$municipios = $db->carregar($sql);
	$municipios = !$municipios ? array() : $municipios;
	
	?>
	<select name="muncod" id="muncod" style="width:200px;" onchange="//Indicador.filtraRegionalizador(this.value)" >
		<option value="todos">Todos os Municípios...</option>
	<? foreach($municipios as $mun){?>
		<option value="<? echo $mun['muncod'] ?>"><? echo $mun['mundescricao'] ?></option>
	<? } ?>
	</select>
<?
	exit;
}

if($_POST['filtraGrupoMunicipios']){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$gtmid  = $_POST['gtmid'];
	$filtro = $_POST['filtro'];
	
	if(!$gtmid || $gtmid == "todos"){
		$sql = "SELECT 
					tpmid AS codigo, 
					tpmdsc AS descricao 
				FROM 
					territorios.tipomunicipio 
				WHERE 
					tpmstatus = 'A' AND (1 = 2) 
				ORDER BY descricao";
	}else{
		$sql = "SELECT 
					tpmid AS codigo, 
					tpmdsc AS descricao 
				FROM 
					territorios.tipomunicipio 
				WHERE 
					gtmid = '$gtmid' AND 
					tpmstatus = 'A' 
				ORDER BY descricao";
	}
	
	if($filtro){
		$acao = "Indicador.filtraMunicipioPorTipoGrupo";
	}else{
		$acao = "";
	}
	
	$db->monta_combo('tpmid',$sql,'S','Selecione...',$acao,'','','200','N',"tpmid","",$_POST['tpmid']);	
	exit;
}

if($_REQUEST['filtraMunicipioPorTipoGrupo'] == 'true'){
	header('content-type: text/html; charset=ISO-8859-1');

	$tpmid = $_POST['tpmid'];
	$estuf = $_POST['estuf'];
	
	if(!$tpmid){
		$municipios = array();
	}elseif($tpmid && (!$estuf || $estuf == "todos")){
		$sql = "SELECT 
					muncod,mundescricao 
				FROM
					territorios.municipio
				WHERE
					muncod IN 	(
									SELECT muncod FROM territorios.muntipomunicipio WHERE tpmid = $tpmid 
								)
				ORDER BY
					mundescricao";
		$municipios = $db->carregar($sql);
		$municipios = !$municipios ? array() : $municipios;
	}elseif($tpmid && ($estuf || $estuf != "todos")){
		$sql = "SELECT 
					muncod,mundescricao 
				FROM
					territorios.municipio
				WHERE
					muncod IN	(
									SELECT muncod FROM territorios.muntipomunicipio WHERE tpmid = $tpmid AND estuf = '$estuf'
								)
				ORDER BY
					mundescricao";
		$municipios = $db->carregar($sql);
		$municipios = !$municipios ? array() : $municipios;
	}
	
	$municipios = !$municipios ? array() : $municipios;
	?>
	<select id="muncod" name="muncod" style="width:200px;" onchange="//Indicador.filtraRegionalizador(this.value)" >
		<option value="todos">Selecione...</option>
	<? foreach($municipios as $mun){?>
		<option value="<? echo $mun['muncod'] ?>"><? echo $mun['mundescricao'] ?></option>
	<? } ?>
	</select>
<?
	exit;
}


//if($_POST['filtraRegionalizador']){
//	header('content-type: text/html; charset=ISO-8859-1');
//	
//	$indid  = $_POST['indid'];
//	$muncod = $_POST['muncod'];
//	
//	$sql   = "SELECT regid FROM painel.indicador WHERE indid = $indid";
//	$regid = $db->pegaUm($sql);
//	
//	$sql = "SELECT
//				regsqlcombo
//			FROM
//				painel.regionalizacao
//			WHERE
//				regid = $regid";
//	$sqmCombo = $db->pegaUm($sql);
//	$sqmCombo = str_replace("{WHERE}"," AND mun.muncod = '$muncod' ",$sqmCombo);	
//	$db->monta_combo('regvalue',$sqmCombo,'S','Selecione...','','','','200','N',"regvalue","",$value);
//	exit;
//}

?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>Selecione o(s) Indicador(es) </title>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
		<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">
		var Indicador = {
		
			filtraEstadoDB : function (regcod,estuf){
				var div = document.getElementById('exibeEstado');
				var url = location.href;
				var req = $.ajax({
								  url  		 : url,
								  type 		 : 'post',
								  data 		 : {filtraEstado : true, 
								  		  	   regcod 		 : regcod, 
										  	   filtro        : true},
								  dataType   : "html",
								  async		 : false,
//								  beforeSend : function (){
//								  	divCarregando();
//								  },
								  success	 : function ( data ){
								  	div.innerHTML =  data;
								  	Indicador.extraiScript( data );
									if(estuf){ document.getElementById('estuf').value = estuf };
//								  	divCarregado();
								  }
						  });
			},
			
			filtraMunicipio : function (estuf){
				var div = document.getElementById('exibe_municipio');
				
				if(estuf != "todos"){
					var url = location.href;
					var req = $.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {filtraMunicipio : true, 
									  		  	    estuf 			: estuf, 
											  	    filtro        	: true},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
//									  	divCarregando();
										div.innerHTML = '<select id="muncod" style="width:200px;">' + 
														'<option disabled="disabled" selected="selected" value="todos">Carregando...</option>' + 
														'</select>';
									  },
									  success	 : function ( data ){
									  	div.innerHTML =  data;
//									  	divCarregado();
									  }
							  });				
				}else{
					div.innerHTML = '<select id="muncod" style="width:200px;">' +
									'<option disabled="disabled" selected="selected" value="todos">Todos os Municípios...</option>' + 
									'</select>';
				}
			
			},
			
			filtraGrupoMunicipios : function (gtmid, tpmid){
				var div = document.getElementById('exibe_tipo_municipio');
				var url = location.href;
				
				var req = $.ajax({
								  url  		 : url,
								  type 		 : 'post',
								  data 		 : {filtraGrupoMunicipios : true, 
								  		  	    gtmid 				  : gtmid,
								  		  	    filtro				  : true},
								  dataType   : "html",
								  async		 : false,
								  beforeSend : function (){
									div.innerHTML = '<select id="gtmid" style="width:200px;">' + 
													'<option disabled="disabled" selected="selected" value="todos">Carregando...</option>' + 
													'</select>';
								  },
								  success	 : function ( data ){
								  	div.innerHTML =  data;
								  	Indicador.extraiScript( data );
									if(tpmid){
										document.getElementById('tpmid').value = tpmid;
									}
								  }
						  });				
			},
			
			filtraMunicipioPorTipoGrupo : function (tpmid, muncod){
				var estuf = document.getElementById('estuf').value;
				var div   = document.getElementById('exibe_municipio');
				var url   = location.href;
				
				var req = $.ajax({
								  url  		 : url,
								  type 		 : 'post',
								  data 		 : {filtraMunicipioPorTipoGrupo : true, 
								  		  	    tpmid 				  		: tpmid,
								  		  	    estuf				  		: estuf},
								  dataType   : "html",
								  async		 : false,
								  beforeSend : function (){
									div.innerHTML = '<select id="gtmid" style="width:200px;">' + 
													'<option disabled="disabled" selected="selected" value="todos">Carregando...</option>' + 
													'</select>';
								  },
								  success	 : function ( data ){
								  	div.innerHTML =  data;
								  	Indicador.extraiScript( data );
									if(muncod){
										document.getElementById('muncod').value = muncod;
									}									
								  }
						  });				
			},			
			
			exibirRelatorioDashBoard : function () {
				var formulario = document.formulario;
				// verifica se tem algum agrupador selecionado
				agrupador = document.getElementById( 'agrupador' );
				if ( agrupador.value == 'todos' ) {
					alert( 'Escolha ao menos um item para agrupar o resultado.' );
					return;
				}
				if ( $('[name^=extracao]:checked').size() == 0 ) {
					alert( 'Escolha um tipo de extração para os dados.' );
					return;
				}
				document.getElementById('divdashboard').innerHTML="<center><br /><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...<br /></center>";
				var params='';
				
				selectAllOptions( $('[name^=tidid_1]')[0] );
				selectAllOptions( $('[name^=tidid_2]')[0] );
				
				if (document.getElementById('periodo_inicio')){
					if(document.getElementById('periodo_inicio').value != '') {
						params += '&periodo_inicio='+document.getElementById('periodo_inicio').value; 
					}
				}
				if (document.getElementById('periodo_fim')){	
					if(document.getElementById('periodo_fim').value != '') {
						params += '&periodo_fim='+document.getElementById('periodo_fim').value; 
					}
				}
				if (document.getElementById('regcod')){
					if(document.getElementById('regcod').value != '') {
						params += '&regcod='+document.getElementById('regcod').value; 
					}
				}
				if (document.getElementById('estuf')){
					if(document.getElementById('estuf').value != '') {
						params += '&estuf='+document.getElementById('estuf').value; 
					}
				}
				if (document.getElementById('gtmid')){	
					if(document.getElementById('gtmid').value != '') {
						params += '&gtmid='+document.getElementById('gtmid').value; 
					}
				}
				if (document.getElementById('tpmid')){
					if(document.getElementById('tpmid').value != '') {
						params += '&tpmid='+document.getElementById('tpmid').value; 
					}
				}
				if (document.getElementById('muncod')){
					if(document.getElementById('muncod').value != '') {
						params += '&muncod='+document.getElementById('muncod').value; 
					}
				}
				if (document.getElementById('tidid1')){
					if(document.getElementById('tidid1').value != '') {
						params += '&tidid1='+document.getElementById('tidid1').value; 
					}
				}
				if (document.getElementById('tidid2')){
					if(document.getElementById('tidid2').value != '') {
						params += '&tidid2='+document.getElementById('tidid2').value; 
					}
				}
				if (document.getElementById('sel_periodicidade')){
					if(document.getElementById('sel_periodicidade').value != '') {
						params += '&perid='+document.getElementById('sel_periodicidade').value; 
					}
				}
				if (document.getElementById('tidid1')){
					if(document.getElementById('tidid1').value != '') {
						params += '&tidid1='+document.getElementById('tidid1').value; 
					}
				}
				if (document.getElementById('tidid2')){
					if(document.getElementById('tidid2').value != '') {
						params += '&tidid2='+document.getElementById('tidid2').value; 
					}
				}
				if (document.getElementById('regvalue')){
					if(document.getElementById('regvalue').value != '') {
						params += '&regvalue='+document.getElementById('regvalue').value; 
					}
				}
				
				formulario.btnsubmit.value = 1;
//				alert($('#formdashboard').serialize());
				var req = $.ajax({
								  url  		 : '?modulo=principal/atividade_estrategico/relatorioDashBoard&acao=A',
								  type 		 : 'post',
								  data 		 : $('#formdashboard').serialize(),
//								  data 		 : $('#formdashboard').serialize() + params,
								  dataType   : "html",
								  async		 : false,
								  success	 : function ( data ){
								  	$('#fieldRelatorio').show();
								  	document.getElementById('divdashboard').innerHTML =  data;
								  	Indicador.extraiScript( data );
								  }
						  });				
			},
			
			salvar : function ( indid ){
//				var formulario = document.formulario;
				// verifica se tem algum agrupador selecionado
				var agrupador = document.getElementById( 'agrupador' );
				if ( agrupador.value == 'todos' ) {
					alert( 'Escolha ao menos um item para agrupar o resultado.' );
					return;
				}
				
				if ( $('[name^=extracao]:checked').size() == 0 ) {
					alert( 'Escolha um tipo de extração para os dados.' );
					return;
				}
				
				var agp			 = agrupador.value;	
				var extracao	 = $('[name^=extracao]:checked').val();
				
				selectAllOptions( $('[name^=tidid_1]')[0] );
				selectAllOptions( $('[name^=tidid_2]')[0] );
					
				var tidid_1	 = $.trim( $('[name^=tidid_1]').val() );	
				var tidid_2	 = $.trim( $('[name^=tidid_2]').val() );
				
//				tidid_1 = tidid_1 || '';
//				tidid_2 = tidid_2 || '';
				tidid_1 = tidid_1 || 'todos';	
				tidid_2 = tidid_2 || 'todos';	
				
				tidid_1 = (typeof tidid_1 == 'object' ? tidid_1.join(',') : tidid_1);
				tidid_2 = (typeof tidid_2 == 'object' ? tidid_2.join(',') : tidid_2);
					
				var dpeid_inicio = document.getElementById('periodo_inicio').value;
				var dpeid_fim 	 = document.getElementById('periodo_fim').value;
				var sehid 		 = document.getElementById('ultima_sehid').value;
				
				if(document.getElementById('unidade_inteiro')){
					var unidade_inteiro = document.getElementById('unidade_inteiro').value;
				}else{
					var unidade_inteiro = "null";
				}
				
				if(document.getElementById('unidade_moeda')){
					var unidade_moeda = document.getElementById('unidade_moeda').value;
				}else{
					var unidade_moeda = "null";
				}
				
				if(document.getElementById('indice_moeda')){
					var indice_moeda = document.getElementById('indice_moeda').value;
				}else{
					var indice_moeda = "null";
				}
				
				if(document.getElementById('regcod')){
					var regcod = document.getElementById('regcod').value;
				}else{
					var regcod = "todos";
				}
				
				if(document.getElementById('estuf')){
					var estuf = document.getElementById('estuf').value;
				}else{
					var estuf = "todos";
				}
				
				if(document.getElementById('muncod')){
					var muncod = document.getElementById('muncod').value;
				}else{
					var muncod = "todos";
				}
				
				if(document.getElementById('gtmid')){
					var gtmid = document.getElementById('gtmid').value;
				}else{
					var gtmid = "todos";
				}
				
				if(document.getElementById('tpmid')){
					var tpmid = document.getElementById('tpmid').value;
				}else{
					var tpmid = "todos";
				}
				
				if(document.getElementById('tidid1')){
					var tidid1 = document.getElementById('tidid1').value;
				}else{
					var tidid1 = "todos";
				}
				
				if(document.getElementById('tidid2')){
					var tidid2 = document.getElementById('tidid2').value;
				}else{
					var tidid2 = "todos";
				}
				
				var periodo_inicio = $('#periodo_inicio').val();
				var periodo_fim    = $('#periodo_fim').val();
						
				periodo_inicio = periodo_inicio || '';		
				periodo_fim    = periodo_fim || '';		
						
				var filtro = "agrupador=" 		 + agp 		   	   + 
							 ";extracao=" 		 + extracao 	   + 
							 ";tidid_1=" 		 + tidid_1 	   	   + 
							 ";tidid_2=" 		 + tidid_2 	   	   + 
						     ";periodo_inicio="  + periodo_inicio  + 
						     ";periodo_fim="  	 + periodo_fim     + 
						     ";indid=" 			 + indid 		   + 
							 ";dpeid=" 			 + dpeid_inicio    +
							 ";dpeid2=" 		 + dpeid_fim 	   +
							 ";unidade_inteiro=" + unidade_inteiro +
							 ";unidade_moeda="   + unidade_moeda   +
							 ";indice_moeda="    + indice_moeda    +
							 ";estuf="           + estuf    	   +
							 ";muncod="          + muncod    	   +
							 ";gtmid="           + gtmid    	   +
							 ";tpmid="           + tpmid    	   +
							 ";tidid1="          + tidid1    	   +
							 ";tidid2="          + tidid2    	   +
							 ";regcod="          + regcod; 	
					
				window.opener.$('#indid_filtro_meta').val( filtro );
				window.opener.$('#idvdsc_meta').val( $.trim($('#idvdsc').val()) );
				window.opener.$('#idvid_meta').val( $.trim($('#idvid').val()) );
				window.opener.$('#idvmeta_meta').val( $.trim($('#idvmeta').val()) );
				window.opener.$('#idvdatameta_meta').val( $.trim($('#idvdatameta').val()) );
				window.opener.$('#nova_meta').val( $.trim($('#nova').val()) );
				window.opener.$('#indid_vinculado_meta').val('<?php echo $indid; ?>');

				divCarregando();
				var urlLocal = location.href;
				$.ajax({
						  url  		 : urlLocal,
						  type 		 : 'post',
						  data 		 : {
						  				salvaFiltro : true,
						  				idvfiltro   : filtro,
						  				idvdsc	    : $.trim($('#idvdsc').val())
						  				},
						  dataType   : "html",
						  async		 : false,
						  success	 : function ( data ){
						  	//alert('Operação realizada com sucesso!');
						  	divCarregado();
						  	//window.opener.$('#form_item_check_list').submit()
						  	window.opener.salvarMonitoraItemChecklist();
							window.close();
						  }
				});				
			},
			
			exibeFiltros : function ( obj ){
				var tipoOperacao = $( obj ).find('img')
										   .attr('src')
										   .indexOf('menos.gif');
										   
				if ( tipoOperacao == -1 ){
					$( obj ).find('img')
							.attr('src', '../imagens/menos.gif');
					$('#filtros_painel').show();
				}else{										   
					$( obj ).find('img')
							.attr('src', '../imagens/mais.gif');
					$('#filtros_painel').hide();
				}	
			},
			
			extraiScript : function (texto){  
				//desenvolvido por Skywalker.to, Micox e Pita.  
				//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
				var ini, pos_src, fim, codigo;  
				var objScript = null;  
				ini = texto.indexOf('<script', 0)  
				while (ini!=-1){  
					var objScript = document.createElement("script");  
					//Busca se tem algum src a partir do inicio do script  
					pos_src = texto.indexOf(' src', ini)  
					ini = texto.indexOf('>', ini) + 1;
			
					//Verifica se este e um bloco de script ou include para um arquivo de scripts  
					if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
						//Marca como sendo o inicio do nome do arquivo para depois do src  
						ini = pos_src + 4;  
					//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
					fim = texto.indexOf('.', ini)+4;  
					//Pega o nome do arquivo  
					codigo = texto.substring(ini,fim);  
					//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
					codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
					// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
					objScript.src = codigo;  
				}else{
				//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
					// Procura o final do script
					fim = texto.indexOf('</script', ini);  
					// Extrai apenas o script
					codigo = texto.substring(ini,fim);  
					// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
					objScript.text = codigo;
				}
				
				//Adiciona o script ao documento  
				document.body.appendChild(objScript);
				
				// Procura a proxima tag de <script>  
				ini = texto.indexOf('<script', fim);
				
				//Limpa o objeto de script  
				objScript = null;  
			}  
		}
			
};

$(document).ready(function (){
	<?php 
	if ($arFiltro['tpmid']):
	?>
	Indicador.filtraGrupoMunicipios( $('#gtmid').val(), <?php echo $arFiltro['tpmid']; ?> )
	<?php 
	endif;
	?>
	<?php 
	if ($arFiltro['muncod']):
	?>
	Indicador.filtraMunicipioPorTipoGrupo( $('#tpmid').val(), <?php echo $arFiltro['muncod']; ?> )
	<?php 
	endif;
	?>
	
});

/**
 * Alterar visibilidade de um bloco.	 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}

/**
 * Alterar visibilidade de um campo.	 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function excluirMetaIntermediaria(idvid)
{
	window.location.href = window.location.href + '&excluir=1&idvid=' + idvid
}

function editarMetaIntermediaria(idvid)
{
	window.location.href = window.location.href + '&editar=1&idvid=' + idvid
}


function novaMeta()
{
	window.location.href = window.location.href + '&nova=1'
	return false;
}

		</script>
	</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
<?php
	$indid = $_GET['indid'];	
	$sql = "select
				ind.indnome,
				exo.exodsc,
				sec.secdsc,
				aca.acadsc,
				ind.indobjetivo,
				ind.indcumulativo,
				ind.regid,
				unm.unmid,
				unm.unmdesc,
				ind.indformula,
				ind.indtermos,
				ind.indfontetermo,
				ind.indobservacao,
				ind.indvispadrao,
				per.perdsc,
				est.estdsc,
				col.coldsc,
				reg.regdescricao,
				ume.umeid,
				ume.umedesc
			from
				painel.indicador ind
			left join
				painel.eixo exo ON exo.exoid = ind.exoid
			left join
				painel.secretaria sec ON sec.secid = ind.secid
			left join
				painel.acao aca ON aca.acaid = ind.acaid
			left join
				painel.unidademedicao unm ON unm.unmid = ind.unmid
			left join
				painel.periodicidade per ON per.perid = ind.perid
			left join
				painel.unidademeta ume on ind.umeid = ume.umeid
			left join
				painel.estilo est ON est.estid = ind.estid
			left join
				painel.coleta col ON col.colid = ind.colid
			left join
				painel.regionalizacao reg ON reg.regid = ind.regid
			where
				ind.indid = $indid
			limit 1";
	$ind = $db->pegaLinha($sql);
	
$sql = "select 
				'<img src=\"../imagens/alterar.gif\" title=\"Editar Item\" class=\"link img_middle\" onclick=\"editarMetaIntermediaria(' || iv.idvid || ')\"  />' || '  ' ||
				'<img src=\"../imagens/excluir.gif\" title=\"Editar Item\" class=\"link img_middle\" onclick=\"excluirMetaIntermediaria(' || iv.idvid || ')\"  />'  
				as \"Ação\",
				iv.idvdsc as \"Descrição\", 
				iv.idvmeta as \"Meta\",
				to_char(iv.idvdatameta, 'DD/MM/YYYY') as \"Data\" 
		from painel.indicadoresvinculados iv 
			inner join pde.monitoraitemchecklist mic ON iv.indidvinculo = mic.indid
		where iv.indid = '{$indid}'
		and mic.micid = '{$micid}'
		and idvstatus = 'A'
		order by idvdatameta ";	

$db->monta_lista($sql,'das',100,10,"N","center","N");
	
?>
<div style="margin: 10px 40px;"><a onclick="novaMeta()">Nova Meta</a></div>
	
<form name="formulario" method="post" id="formdashboard">
	<input type="hidden" name="nova" id="nova" value="<?php echo $nova; ?>" />
	<input type="hidden" name="idvid" id="idvid" value="<?php echo $idvid; ?>" />
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#f9f9f9">
			      <td colspan="2" align="left">
			  	  	<span style="font-size: 16px;font-weight: bold;"><? echo $indid." - ".$ind['indnome'] ?></span><br />
			  	  	<b><? echo $ind['exodsc'] ?></b> >> <b><? echo $ind['secdsc'] ?></b> >> <b><? echo $ind['acadsc'] ?></b>
			  	  </td>
		 </tr>
		 <tr>
			      <td colspan="2" align="left">
			  	  	<div style="padding:5px;width:98%;border: solid 1px #000000;background-color: #FFFFFF">
				  	  	<fieldset>
				  	  		<legend><span style="cursor:pointer;" onclick="Indicador.exibeFiltros(this);" ><img align="absmiddle"  title="menos" src="../imagens/menos.gif"/> Filtros</span></legend>
				  	  		<div id="filtros_painel" style="display:" >
								<input type="hidden" name="ultima_sehid" id="ultima_sehid" value="0" />
								<?php if($ind['indcumulativo'] == "S"){ ?>
									<input type="hidden" name="indcumulativo" id="indcumulativo" value="1" />
								<?php } ?>
								<div>
								
								
								<div id="opc_idvdsc" style="margin:5px;float:left;width:220px;">
									<fieldset style="padding:5px;">
										<legend>Novo Indicador:</legend>
										<span id="exibeIdvdsc" >
											<input type="text" name="idvdsc" id="idvdsc" value="<?php echo $idvdsc ?>" style="width:200px;">	
										</span>
									</fieldset>
								</div>
								<?php 
								if ( in_array($ind['unmid'], array(UNIDADEMEDICAO_MOEDA, UNIDADEMEDICAO_NUM_INDICE, UNIDADEMEDICAO_NUM_INTEIRO, UNIDADEMEDICAO_PERCENTUAL)) ):
									switch ( true ){
										case $ind['unmid'] == UNIDADEMEDICAO_MOEDA:
											$masc 	 = '[###.]###,##';
											$max  	 = '20';
											$idvmeta = number_format($idvmeta,2,',','.'); 
											break;
										case $ind['unmid'] == UNIDADEMEDICAO_NUM_INTEIRO:
											$masc = '[###]';
											$max  = '20';
											$idvmeta = number_format($idvmeta,0,'','');
											break;
										default:
											$masc = '###,##';
											$max  = '6';
											$idvmeta = number_format($idvmeta,2,',','.');											
											break;
										
									}
								?>
								<div id="opc_idvmeta" style="margin:5px;float:left;width:120px;">
									<fieldset style="padding:5px;">
										<legend>Nova Meta do Indicador:</legend>
										<span id="exibeIdvmeta" >
											<input type="text" name="idvmeta" id="idvmeta" value="<?php echo $idvmeta ?>" style="width:100px;" maxlength="<?php echo $max ?>" onkeyup="this.value=mascaraglobal('<?php echo $masc; ?>', this.value);">
											<font style="font-size: 8px;">(<?php echo $ind['unmdesc'] ?>)</font>
										</span>
									</fieldset>
								</div>
								<div id="opc_idvmeta" style="margin:5px;float:left;width:150px;">
									<fieldset style="padding:5px;">
										<legend>Novo Prazo do Indicador:</legend>
										<span id="exibeIdvmeta" >
										<?php echo campo_data2("idvdatameta","N","S","","","","",formata_data($idvdatameta)); ?>
										</span>
									</fieldset>
								</div>
								<?php 
								endif;
								?>
								<div id="opc_agrupador" style="margin:5px;float:left;width:150px;">
									<fieldset style="padding:5px;">
										<legend>Agrupador:</legend>
										<span id="exibeAgrupador" >
										<select name="agrupador" id="agrupador" style="width:150px;" >
											<option <?php echo ($arFiltro['agrupador'] ? '' : 'selected="selected"') ?> value="todos">Selecione...</option>
											<? 
											//Pega os agrupadores de acordo com os Detalhamentos do Indicador
											$arrAgrupadores = getAgrupadoresPorDetalhe($indid);
											//Pega os agrupadores de acordo com a Regionalização do Indicador
											$arrAgrPorReg = getAgrupadorPorRegionalizador($indid);
											//Inclui os agrupadores de Regionalização no array de agrupadores de Detalhe
											if(is_array($arrAgrPorReg)){
												foreach($arrAgrPorReg as $arrAgr){
													array_push($arrAgrupadores,$arrAgr);
												}
											}
											foreach($arrAgrupadores as $agp){?>
												<option value="<? echo $agp['codigo'] ?>" <?php echo ($arFiltro['agrupador'] == $agp['codigo'] ? 'selected="selected"' : '') ?>><? echo $agp['descricao'] ?></option>
											<? } ?>
										</select>
										</span>
									</fieldset>
								</div>
								
								<div id="opc_extracao" style="margin:5px;margin-left:15px;float:left;width:200px;">
									<fieldset style="padding:5px;">
										<legend>Tipo de Extração:</legend>
										<span id="exibeExtracao" >
										<label for="extracao_agrupador"><input type="radio" <?php echo ($arFiltro['extracao'] == "agrupador" ? 'checked="checked"' : '') ?> name="extracao" id="extracao_agrupador" value="agrupador">Qtd. de Agrupadores</label>
										<br>
										<?php
										if ( $ind['umedesc'] ){
										?>
										<label for="extracao_produto"><input type="radio" <?php echo ($arFiltro['extracao'] == "produto" ? 'checked="checked"' : '') ?> name="extracao" id="extracao_produto" value="produto">Qtd. de <?php echo $ind['umedesc'] ?></label>
										<br>
										<?php
										}
										?>
										<label for="extracao_valor"><input type="radio" <?php echo ($arFiltro['extracao'] == "valor" ? 'checked="checked"' : '') ?> name="extracao" id="extracao_valor" value="valor">Valor</label>
										</span>
									</fieldset>
								</div>
								<? 
//											$sql = "select 
//														tdiid,
//														tdidsc
//													from
//														painel.detalhetipoindicador
//													where
//														indid = $indid
//													and
//														tdistatus = 'A'";
//											$det = $db->carregar($sql);
//											
//											if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){
//												$option_barra .= "<option ".($tipo == "barra_1_{$det[0]['tdiid']}" ? "selected=selected" : "")." value=\"barra_1_{$det[0]['tdiid']}\" >Gráfico em Barra - {$det[0]['tdidsc']}</option>";
//												$option_barra .= "<option ".($tipo == "barracomp_1_{$det[0]['tdiid']}" ? "selected=selected" : "")." value=\"barracomp_1_{$det[0]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[0]['tdidsc']}</option>";
//												$option_pizza .= "<option ".($tipo == "pizza_1_{$det[0]['tdiid']}" ? "selected=selected" : "")." value=\"pizza_1_{$det[0]['tdiid']}\" >Gráfico em Pizza - {$det[0]['tdidsc']}</option>";
//											}
//											if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){
//												$option_barra .= "<option ".($tipo == "barra_2_{$det[1]['tdiid']}" ? "selected=selected" : "")." value=\"barra_2_{$det[1]['tdiid']}\" >Gráfico em Barra - {$det[1]['tdidsc']}</option>";
//												$option_barra .= "<option ".($tipo == "barracomp_2_{$det[1]['tdiid']}" ? "selected=selected" : "")." value=\"barracomp_2_{$det[1]['tdiid']}\" >Gráfico em Barra Comparativa - {$det[1]['tdidsc']}</option>";
//												$option_pizza .= "<option ".($tipo == "pizza_2_{$det[1]['tdiid']}" ? "selected=selected" : "")." value=\"pizza_2_{$det[1]['tdiid']}\" >Gráfico em Pizza - {$det[1]['tdidsc']}</option>";
//											}
											
											?>
											<!--
											
											<div id="opc_tipo_grafico" style="margin:5px;width:250px;float:left;">
												<fieldset style="padding:5px;">
													<legend>Apresentação:</legend>
													<select id="tipo_grafico" onchange="escondePeriodo(this.value);" style="width:220px;" >
															<option <? echo ($tipo == "linha" ? "selected=selected" : "") ?> value="linha" >Gráfico em Linha</option>
															<option <? echo ($tipo == "barra" ? "selected=selected" : "") ?> value="barra" >Gráfico em Barra</option>
															<? echo $option_barra; ?>
															<? echo $option_pizza; ?>
														</select>
													
												</fieldset>
											</div>
								
											-->
											
											
											<?php
											$sql = "SELECT 
														tdiid,
														tdidsc,
														tdinumero 
													FROM 
														painel.detalhetipoindicador 
													WHERE 
														indid='$indid' AND 
														tdistatus='A' 
													ORDER BY tdiordem";
											$arDetalhe = $db->carregar( $sql );
											$arDetalhe = ( $arDetalhe ? $arDetalhe : array() );
											
											foreach ($arDetalhe as $k => $detalhe){
											?>
											<div id="opc_tidid_<?php echo $detalhe['tdinumero'] ?>" style="margin:5px;width:250px;float:left;">
											<fieldset style="padding:5px;">
												<legend><?php echo $detalhe['tdidsc'] ?>:</legend>
												<?php
												$Sql = "SELECT 
															tidid AS codigo, 
															tiddsc AS descricao 
														FROM 
															painel.detalhetipodadosindicador 
														WHERE 
															tdiid = '{$detalhe['tdiid']}' AND
															tidstatus = 'A' 
														ORDER BY tiddsc";
												unset($arValue);
												if ( $arFiltro['tidid_' . $detalhe['tdinumero']][0] && $arFiltro['tidid_' . $detalhe['tdinumero']][0] != 'todos' ){
													$SqlCarregado = "SELECT 
																		tidid AS codigo, 
																		tiddsc AS descricao 
																	FROM 
																		painel.detalhetipodadosindicador 
																	WHERE 
																		tdiid = '{$detalhe['tdiid']}' AND
																		tidid IN (" . implode(", ", $arFiltro['tidid_' . $detalhe['tdinumero']]) . ") AND
																		tidstatus = 'A' 
																	ORDER BY tiddsc";
													
													$arValue = $db->carregar( $SqlCarregado );	
												}
												combo_popup( 'tidid_' . $detalhe['tdinumero'] , $Sql, 'Selecione o(s) Detalhe(s) ', '360x460', 0, array(), "", 'S', true, false, 4, 230,"","","",array(
													  array(
													  		"codigo" 	=> "tiddsc",
													  		"descricao" => "Detalhe"
													  		)
													  )	,$arValue,true,true);
												
												?>
											</fieldset>
											</div>
										<?php 
										}
										?>
											
											<div id="opc_periodo" style="margin:5px;width:180px;float:left;">
											<fieldset style="padding:5px;">
												<legend>Período:</legend>
											
												<? 
												//Carrega o período do indicador
												$sql = "select  
															dpe.dpeid,
															dpe.dpedsc,
															seh.sehid
														from 
															painel.seriehistorica seh
														inner join
															painel.detalheperiodicidade dpe ON dpe.dpeid = seh.dpeid
														where
															indid = $indid
														and
															sehstatus <> 'I'
														order by
															dpe.dpedatainicio asc";
												
												$periodos = $db->carregar($sql);
												if($periodos){
//													dbg( $arFiltro );
													if(count($periodos) > 12){
														$k1 = count($periodos) - 12;
													}else{
														$k1 = 0;
													}
													
													?>
													<select name="periodo_inicio" id="periodo_inicio">
													<? 
													foreach($periodos as $k => $periodo){ //inicio do foreach do periodo inicio
														if ( $arFiltro['periodo_inicio'] ){
															$selected = ($arFiltro['periodo_inicio'] == $periodo['dpeid'] ? "selected=\"selected\"" : "");
														}else{
															$selected = ($k == $k1 ? "selected=\"selected\"" : "");
														}
													?>
														<option <? echo $selected ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
													<? 
													} //fim do foreach do periodo inicio
													?>
													</select>
													<span id="ate_periodo" >até</span> 
													<select name="periodo_fim" id="periodo_fim">
													<option value="">Selecione</option>
													<? 
													foreach($periodos as $k => $periodo){ //inicio do foreach do periodo inicio
														if ( $arFiltro['periodo_fim'] ){
															$selected = ($arFiltro['periodo_fim'] == $periodo['dpeid'] ? "selected=\"selected\"" : "");
														}else{
															$selected = ($k == (count($periodos) - 1) ? "selected=\"selected\"" : "");
														}
													?>
														<option <? echo $selected ?> value="<? echo $periodo['dpeid'] ?>" ><? echo $periodo['dpedsc'] ?></option>
													<? 
													} //fim do foreach do periodo inicio
													?>
													</select>
												<? } ?>
											</fieldset>
											</div>
										<? $sql = "	select 
														unmdesc,
														i.indescala,
														per.perdsc
													from 
														painel.indicador i
													left join 
														painel.unidademedicao u on i.unmid = u.unmid
													left join 
														painel.periodicidade per ON per.perid = i.perid
													where 
														indid = $indid";
								
											$escala = $db->pegaLinha($sql);;
											
											if(strstr($escala['unmdesc'], 'Número inteiro') && $escala['indescala'] == "t"){?>
											
											<div id="opc_escala" style="margin:5px;float:left;width:150px">
											<fieldset style="padding:5px;">
												<legend>Aplicar escala em:</legend>
												<select name="unidade_inteiro" id="unidade_inteiro">
													<option value="1" <?php echo (($arFiltro['unidade_inteiro'] == 1 || empty($arFiltro['unidade_inteiro'])) ? 'selected="selected"' : '') ?>>Unidade</option>
													<option value="1000" <?php echo (($arFiltro['unidade_inteiro'] == 1000 ) ? 'selected="selected"' : '') ?>>Milhares</option>
													<option value="1000000" <?php echo (($arFiltro['unidade_inteiro'] == 1000000 ) ? 'selected="selected"' : '') ?>>Milhões</option>
													<option value="1000000000" <?php echo (($arFiltro['unidade_inteiro'] == 1000000000 ) ? 'selected="selected"' : '') ?>>Bilhões</option>
												</select>
											</fieldset>
											</div>
											<? }
											if(strstr($escala['unmdesc'], 'Moeda')){?>
											<div id="opc_escala_moeda" style="margin:5px;float:left;width:150px;">
											<fieldset style="padding:5px;">
												<legend>Aplicar escala em:</legend>
												<select name="unidade_moeda" id="unidade_moeda">
													<option value="1" <?php echo (($arFiltro['unidade_moeda'] == 1 || empty($arFiltro['unidade_moeda'])) ? 'selected="selected"' : '') ?>>Reais</option>
													<option value="1000" <?php echo (($arFiltro['unidade_moeda'] == 1000) ? 'selected="selected"' : '') ?>>Milhares de Reais</option>
													<option value="1000000" <?php echo (($arFiltro['unidade_moeda'] == 1000000) ? 'selected="selected"' : '') ?>>Milhões de Reais</option>
													<option value="1000000000" <?php echo (($arFiltro['unidade_moeda'] == 1000000000) ? 'selected="selected"' : '') ?>>Bilhões de Reais</option>
												</select>
											</fieldset>
											</div>
											<? }
											if(strstr($escala['unmdesc'], 'Moeda') && strstr($escala['perdsc'], 'Anual')){?>
											<div id="opc_escala_moeda" style="margin:5px;float:left;width:100px;">
											<fieldset style="padding:5px;">
												<legend>Aplicar índice:</legend>
												<select name="indice_moeda" id="indice_moeda">
													<option value="null" <?php echo ((empty($arFiltro['indice_moeda'])) ? 'selected="selected"' : '') ?>>Selecione...</option>
													<option value="ipca" <?php echo (($arFiltro['indice_moeda'] == 'ipca') ? 'selected="selected"' : '') ?>>IPCA Médio</option>
												</select>
											</fieldset>
											</div>
										</div>
									
									
											<? }
											if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 6 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || $ind['regid'] == REGIONALIZACAO_POLO){ //Estado?>
										<div style="clear:both">	
											<div id="opc_regiao" style="margin:5px;float:left;width:100px">
											<fieldset style="padding:5px;">
												<legend>Região:</legend>
												<select style="width:90px;" name="regcod" id="regcod" onchange="Indicador.filtraEstadoDB(this.value)" >
													<option <?php echo (empty($arFiltro['regcod']) ? 'selected="selected"' : '') ?> value="todos">Selecione...</option>
													<? 
													$sql = "select 
																TRIM(regcod) AS regcod, regdescricao
															from
																territorios.regiao
															order by
																regdescricao";
													$regiao = $db->carregar($sql);
													foreach($regiao as $rg){?>
														<option value="<? echo $rg['regcod'] ?>" <?php echo (($arFiltro['regcod'] == $rg['regcod']) ? 'selected="selected"' : '') ?>><? echo $rg['regdescricao'] ?></option>
													<? } ?>
												</select>
											</fieldset>
											</div>
									
											<div id="opc_estado" style="margin:5px;float:left;width:110px;">
											<fieldset style="padding:5px;">
												<legend>Estado:</legend>
												<span id="exibeEstado" >
													<select name="estuf" id="estuf" style="width:100px;" onchange="Indicador.filtraMunicipio(this.value)" >
														<option value="todos" <?php echo (empty($arFiltro['estuf']) ? 'selected="selected"' : '') ?>>Selecione...</option>
														<? 
														$sql = "select 
																	estuf, estdescricao
																from
																	territorios.estado
																order by
																	estdescricao";
														$estados = $db->carregar($sql);
														foreach($estados as $uf){?>
															<option value="<? echo $uf['estuf'] ?>" <?php echo (($arFiltro['estuf'] == $uf['estuf']) ? 'selected="selected"' : '') ?>><? echo $uf['estdescricao'] ?></option>
														<? } ?>
													</select>
													</span>
												</fieldset>
											</div>
											<? } if($ind['regid'] == 7 || $ind['regid'] == 2 || $ind['regid'] == 4 || $ind['regid'] == 5 || $ind['regid'] == 8 || $ind['regid'] == 9 || $ind['regid'] == 10 || $ind['regid'] == 11 || $ind['regid'] == 12 || $ind['regid'] == REGIONALIZACAO_POLO){ //Município?>
											<div id="opc_grp_mun" style="margin:5px;float:left;width:160px;">
											<fieldset style="padding:5px;">
												<legend>Grupo de Municípios:</legend>
												<span id="exibe_grupo_municipio">
												<select name="gtmid" id="gtmid" style="width:150px;" onchange="Indicador.filtraGrupoMunicipios(this.value)" >
													<option value="todos" <?php echo (empty($arFiltro['gtmid']) ? 'selected="selected"' : '') ?>>Selecione...</option>
												<? 
													$sql = "select 
																gtmid, gtmdsc
															from
																territorios.grupotipomunicipio
															where
																gtmstatus = 'A'
															order by
																gtmdsc";
													$grupoMun = $db->carregar($sql);
													foreach($grupoMun as $gm){?>
														<option value="<? echo $gm['gtmid'] ?>" <?php echo (($arFiltro['gtmid'] == $gm['gtmid']) ? 'selected="selected"' : '') ?>><? echo $gm['gtmdsc'] ?></option>
													<? } ?>
													</select>
												</span>
											</fieldset>
											</div>
											
											<div id="opc_tpo_mun" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend>Tipos de Municípios:</legend>
												<span id="exibe_tipo_municipio">
												<select name="tpmid" id="tpmid" style="width:200px;">
													<option disabled="disabled" selected="selected" value="todos">Selecione...</option>
												</select>
												</span>
											</fieldset>
											</div>
											
											<div id="opc_mun" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend>Município:</legend>
												<span id="exibe_municipio">
												<select name="muncod" id="muncod" style="width:200px;">
													<option disabled="disabled" selected="selected" value="todos">Selecione...</option>
												</select>
												</span>
												</fieldset>
											</div>
											<? } ?>
									
									<? if($det[0]['tdiid'] && $det[0]['tdiid'] != ""){ ?>
									<div style="clear:both">
										<? 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[0]['tdiid']}"; 
											$detalhe1 = $db->pegaUm($sql);
										?>
										<div id="opc_det1" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend><? echo $detalhe1 ?>:</legend>
											<? 
											$sql = "select 
														tidid as codigo,
														tiddsc as descricao
													from 
														painel.detalhetipodadosindicador 
													where 
														tdiid = {$det[0]['tdiid']}
													and
														tidstatus = 'A'"; 
											$db->monta_combo('tidid1',$sql,'S','Selecione...','','','','200','N',"tidid1","","");
											?>
										</fieldset>
											</div>
									<? } ?>
									<? if($det[1]['tdiid'] && $det[1]['tdiid'] != ""){ ?>
										
										<? 	$sql = "select tdidsc from painel.detalhetipoindicador where tdiid = {$det[1]['tdiid']}"; 
											$detalhe2 = $db->pegaUm($sql);
										?>
										<div id="opc_det2" style="margin:5px;float:left;width:210px;">
											<fieldset style="padding:5px;">
												<legend><? echo $detalhe2 ?>:</legend>
											<? 
											$sql = "select 
														tidid as codigo,
														tiddsc as descricao
													from 
														painel.detalhetipodadosindicador 
													where 
														tdiid = {$det[1]['tdiid']}
													and
														tidstatus = 'A'"; 
											$db->monta_combo('tidid2',$sql,'S','Selecione...','','','','200','N',"tidid2","","");
											?>
											</fieldset>
											</div>
									</div>
									
									<? } ?>
								<div style="clear:both;margin:5px;">								
								<input style="margin-left:5px;" type="button" name="button_visualizar" id="button_visualizar" onclick="Indicador.exibirRelatorioDashBoard();" value="Vizualizar" />
								<input style="margin-left:5px;" type="button" name="button_salvar_grafico" id="button_salvar_grafico" onclick="Indicador.salvar(<?php echo $indid; ?>);" value="Salvar" />
								<input style="margin-left:5px;" type="button" name="button_cancelar" id="button_cancelar" onclick="window.close();" value="Cancelar" />
								</div>
							</div>
				  	  	</fieldset>
				  	  </div>
			  	  </td>
		 </tr>
		 <tr>
		 	<td colspan="2">
			<div style="margin:5px;height:480px;padding:5px">
	  	  		<div style="margin:5px;overflow:auto;height:465px" >
		  	  		<fieldset id="fieldRelatorio" style="display: none;">
		  	  			
		  	  			<input type="hidden" name="indid" value="<?php echo $indid ?>">
		  	  			<input type="hidden" name="btnsubmit" value="1">
		  	  			
		  	  			<legend >Relatório</legend>
		  	  				<div>
		  	  				<?php 
		  	  				include APPRAIZ . 'pde/modulos/principal/atividade_estrategico/relatorioDashBoard.inc'; 
		  	  				?>
		  	  				</div>
		  	  		</fieldset>
	  	  		</div>
	  	  	</div>
		 	
		 	<!--
		 		<div id="div_graficos" style="width:98%;padding:5px;height:400px;overflow:auto;border: solid 1px #000000;background-color: #FFFFFF">
			  	  	<fieldset style="height:90%" >
			  	  		<legend>Gráficos</legend>
			  	  			<div style="height:100%;overflow: auto;">
			  	  			<div id="grafico_indicador"></div>
							<div id="legenda_grafico">
					  	  	</div>
			  	  		
					</fieldset>
				</div>
		 	-->
		 	</td>
		 </tr>
	</table>
</form>
</body>
</html>
<?php

?>