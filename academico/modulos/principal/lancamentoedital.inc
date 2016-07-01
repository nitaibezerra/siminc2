<?php
pegaSessoes(1, 1, 0);
if ( !isset($_SESSION["academico"]["edpid"]) ){
	echo "<script>
			alert('É necessário cadastrar um edital primeiro!');
			window.location = '?modulo=principal/cadedital&acao=C';
		  </script>";
}

// instancia o objeto do módulo
$academico = new academico();

// Define as variáveis da tela
$prtid 			= $_SESSION['academico']['prtid'];
$entidcampus 	= $_SESSION['academico']['entidcampus'];
$entidentidade 	= $_SESSION['academico']['entid'];
$edpid 	 		= $_SESSION["academico"]["edpid"];
$prtid 	 		= $_SESSION["academico"]["prtid"];
$unidade 		= $_SESSION["academico"]["unidade"];
$ano   	 		= $_SESSION["academico"]["ano"];
$orgid 			= $_SESSION['academico']['orgid'];
$tpetipo 		= $_SESSION['academico']['tpetipo'];

if($_REQUEST["submetido"] == 1) {
	// Deleta os registros
	$sql_delete  = "DELETE FROM academico.lancamentoeditalportaria WHERE edpid = ".$edpid;
	$db->executar($sql_delete);	
	// Insere os dados
	if($_REQUEST["crgid"] && $_REQUEST["crgid"][0] != "") {
		
		
		switch($tpetipo) {
			case ACA_TPEDITAL_PUBLICACAO:
				for($i=0; $i<count($_REQUEST["crgid"]); $i++) {	
					$sql_insert = "INSERT INTO academico.lancamentoeditalportaria
		                   			(edpid, crgid, lepvlrpublicacao, lepano, lepdtinclusao)
		                			VALUES
		                   			(".$edpid.",".$_REQUEST["crgid"][$i].",".$_REQUEST["publicado"][$i].", '".$_SESSION['academico']['ano']."', now())";							
				
					$db->executar($sql_insert);				
				}
			break;
			case ACA_TPEDITAL_HOMOLOGACAO:
				
				for($i=0; $i<count($_REQUEST["crgid"]); $i++) {	
							$sql_insert = "INSERT INTO academico.lancamentoeditalportaria
				                   			(edpid, crgid, lepvlrhomologado, lepano, lepdtinclusao)
				                			VALUES
				                   			(".$edpid.",".$_REQUEST["crgid"][$i].",".$_REQUEST["homologado"][$i].", '".$_SESSION['academico']['ano']."', now())";
							$db->executar($sql_insert);				
				}
			break;			
			case ACA_TPEDITAL_NOMEACAO:
				for($i=0; $i<count($_REQUEST["crgid"]); $i++) {	
							$sql_insert = "INSERT INTO academico.lancamentoeditalportaria
				                   			(edpid, crgid, lepvlrprovefetivados, lepano, lepdtinclusao)
				                			VALUES
				                   			(".$edpid.",".$_REQUEST["crgid"][$i].",".$_REQUEST["efetivado"][$i].", '".$_SESSION['academico']['ano']."', now())";							
						
							$db->executar($sql_insert);				
				}				
			break;
		}		
			
	}
	
	$db->commit();
	
	echo "<script>
			alert('Dados salvos com sucesso.');
			window.location = '?modulo=principal/lancamentoedital&acao=" . $_GET['acao'] . "';
		</script>";
	exit;
}


if ( $_REQUEST['acao'] == 'H' ){
	$parametros = array(
						'',
						'&edpid=' . $_SESSION['academico']['edpidhomo']
					  );
}elseif ( $_REQUEST['acao'] == 'N' ){
	$parametros = array(
						'',
						'&edpid=' . $_SESSION['academico']['edpideditalhomologacao']
					  );	
}

// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";
//Monta as abas da tela
$db->cria_aba($abacod_tela,$url,$parametros);

// cria o titulo da tela
if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_PUBLICACAO)	    		
	$titulo_modulo = "Lançamento de Editais de Concurso";
if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_HOMOLOGACAO)
	$titulo_modulo = "Lançamento de Editais de Homologação";
if($_SESSION["academico"]["tpetipo"]==ACA_TPEDITAL_NOMEACAO)	    		
	$titulo_modulo = "Lançamento de Portaria de Efetivação";

monta_titulo( $titulo_modulo, "");
//echo $academico->cabecalhoedital( $edpid, $prtid, $entidcampus );

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);
$habil = $bloqueado ? 'N' : $habil;
$habilitado = $bloqueado ? false : $habilitado;
$disabled = $bloqueado ? 'disabled=\"disabled\"' : '';

// Monta o Cabeçalho
if (isset($_SESSION["academico"]["edpid"])){ 
$edpid=$_SESSION["academico"]["edpid"];
	$autoriazacaoconcursos = new autoriazacaoconcursos();
	 echo $autoriazacaoconcursos->cabecalhoedital($edpid, $prtid, $entidcampus );
}

?>
<script type="text/javascript">
<!--
function calculaTotalInicial(id_coluna, id_total) {
	
	var i, soma = 0;
		
	form = document.getElementById("formulario");
			
	for(i=0; i<form.length; i++) {
		
		id_form = form.elements[i].id.substr(0,(form.elements[i].id.lastIndexOf("_")));
		
		if((id_coluna == id_form) && (form.elements[i].value != '')) {
			soma = soma + parseInt(form.elements[i].value);
		}	
	}
	if(document.getElementById(id_total))
		document.getElementById(id_total).value = soma;	

	return soma;
}
//-->
</script>

<style type="text/css" media="screen">
   
       .aba{
            text-align:center;
			padding-top:1px;
			padding-bottom:10px;
			border-bottom:1px solid #ccc;
			border-right:1px solid #ccc;
			background:#f5f5f5;
       }
       
</style>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./geral/js/academico.js"></script>
<body>
<form name="formulario" id="formulario" method="post" action="" >
<input type="hidden" name="submetido" id="submetido" value="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
<tr>
<td>
	<div id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100%;"><b>Docentes</b></li>
			</ul>
		</div>
		  
		<div id="aba_tece" class="aba">
			<?
				include('planodistribuicaocargosDocente.inc');	
			?>
		</div>
	</div>	

	<div id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100%;"><b>TA Nível E</b></li>
			</ul>
		</div>
		  
		<div id="aba_tece" class="aba" >
			<?
				include('planodistribuicaocargosTecE.inc');
				
			?>
		</div>
	</div>	
	
	<div  id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100%;"><b>TA Nível D</b></li>
			</ul>
		</div>
		  
		<div id="aba_tecd" class="aba">
			<?
				include('planodistribuicaocargosTecD.inc');
			?>
		</div>
	</div>
	
	<div id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100%;"><b>TA Nível C</b></li>
			</ul>
		</div>
		  
		<div  id="aba_tecc" class="aba">
			<?
				include('planodistribuicaocargosTecC.inc');
			?>
		</div>
	</div>
	
	<div id="abas" style="display: block;">
		<div class="abaMenu"> 
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100%;"><b>TA Nível B</b></li>
			</ul>
		</div>
		  
		<div id="aba_tecb" class="aba">
			<?
				include('planodistribuicaocargosTecB.inc');				
			?>
		</div>
	</div>
	
</td>
</tr>	
<tr bgcolor="#C0C0C0">
	<td  align="left">
		<input type="button" id="salvar" value="Salvar" onclick="salvarDistribuicao();" <?php echo $disabled; ?>>
	</td>
</tr>
</table>
</form>
</body>
<script type="text/javascript"><!--

function salvarDistribuicao() {
	document.getElementById('submetido').value = 0;
	var lancado = document.getElementsByName("lancado[]");
	
	document.getElementById("salvar").disabled = true;
	
	var publicado  = document.getElementsByName("publicado[]");
	var homologado  = document.getElementsByName("homologado[]");
	var efetivado  = document.getElementsByName("efetivado[]");
	
	for (var i = 0; i < publicado.length; i++) {		
		var elem = publicado[i];
		if((elem.value == '') && (elem.style.display != 'none')){
			alert('todos os campos devem ser peenchidos');
			elem.focus();
			document.getElementById("salvar").disabled = false;
			return;						
		}
	}
	
	for (var i = 0; i < homologado.length; i++) {
		var elem = homologado[i];
		if((elem.value == '') && (elem.style.display != 'none')){
			alert('todos os campos devem ser peenchidos');
			elem.focus();
			document.getElementById("salvar").disabled = false;
			return;						
		}
	}
	
	for (var i = 0; i < efetivado.length; i++) {
		var elem = efetivado[i];
		if((elem.value == '') && (elem.style.display != 'none')){		
			alert('todos os campos devem ser peenchidos');
			elem.focus();
			document.getElementById("salvar").disabled = false;
			return;						
		}
	}
	
	document.getElementById("submetido").value = 1;
	document.formulario.submit();
}

function excluirLinha2(index,tabela) {
	var tabela = document.getElementById(tabela);	
	tabela.deleteRow(index);
}


function valida_lancamento(nome_tabela, cargo_id){
	
	var id_disponivel_projetado  = 'disponivel_projetado_'+nome_tabela;
	var id_td_disponivel_projetado 	= 'td_disponivel_projetado_'+nome_tabela;
	var id_td_provimento_efetivado  = 'td_provimento_efetivado_'+nome_tabela;
	var id_td_disponivel_efetivacao = 'td_disponivel_efetivacao_'+nome_tabela;		
	var id_publicado  = 'publicado_'+nome_tabela+'_'+cargo_id;
	var id_homologado = 'homologado_'+nome_tabela+'_'+cargo_id;
	var id_efetivado  = 'efetivado_'+nome_tabela+'_'+cargo_id;

	var id_publicado_old  = 'publicado_old_'+nome_tabela+'_'+cargo_id;
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;
	var id_efetivado_old  = 'efetivado_old_'+nome_tabela+'_'+cargo_id;

	var id_total_projetado  = 'total_pub_'+nome_tabela;
	var id_total_publicado  = 'total_pub_'+nome_tabela;
	var id_total_homologado = 'total_hom_'+nome_tabela;
	var id_total_efetivado  = 'total_efe_'+nome_tabela;
	
	var disponivel_projetado 	 = document.getElementById(id_disponivel_projetado);
	var td_disponivel_projetado  = document.getElementById(id_td_disponivel_projetado);	
	var td_provimento_efetivado  = document.getElementById(id_td_provimento_efetivado);
	var td_disponivel_efetivacao = document.getElementById(id_td_disponivel_efetivacao);
	var publicado   		 	 = document.getElementById(id_publicado);	

	if(document.getElementById(id_publicado_old)){
		var val_publicado_old   = parseInt(document.getElementById(id_publicado_old).value);
	}else{
		var val_publicado_old = 0;
	}	
	if(document.getElementById(id_homologado_old)){
		var val_homologado_old   = parseInt(document.getElementById(id_homologado_old).value);
	}else{
		var val_homologado_old = 0;
	}
	if(document.getElementById(id_efetivado_old)){
		var val_efetivado_old   = parseInt(document.getElementById(id_efetivado_old).value);
	}else{
		var val_efetivado_old = 0;
	}	

	var val_disponivel_projetado = parseInt(disponivel_projetado.value);
	var val_publicado   		 = parseInt(publicado.value);
	if((val_publicado_old + val_disponivel_projetado) >= 0){	
		if((val_publicado > (val_publicado_old + val_disponivel_projetado)) || (isNaN(val_publicado))){		
			alert('O Valor publicado dever ser menor ou igual ao valor disponível para publicação.');
			publicado.value = val_publicado_old;
			publicado.focus();
			publicado.select();
			calculaTotal(publicado, id_total_publicado);		
			return;		
		}else{			
			document.getElementById(id_publicado_old).value = val_publicado;
			disponivel_projetado.value = (val_disponivel_projetado - (val_publicado -  val_publicado_old));	
			td_disponivel_projetado.innerHTML = (val_disponivel_projetado - (val_publicado -  val_publicado_old));
			calculaTotal(publicado, id_total_publicado);
		}
	}
		
	/******* caso exista campo homologado (lançamento de homologação) *********/
	if(document.getElementById(id_homologado)){
		if(document.getElementById(id_homologado).style.display != 'none' ){
			
			var homologado  	= document.getElementById(id_homologado);
			var val_homologado  = parseInt(homologado.value);
			
			if( ((val_homologado > val_publicado_old) && val_homologado != 0) || (isNaN(val_homologado)) ){
				alert('O Valor homologado dever ser menor ou igual ao Concurso Publicador.');
				homologado.value = val_homologado_old;
				homologado.focus();
				homologado.select();
				calculaTotal(homologado, id_total_homologado);
				return;				
			}
						
			if((val_homologado > 0) && (val_publicado_old == 0)){
			
				alert('É necessário cadastrar ao menos um Concurso Publicado.');
				homologado.value = val_homologado_old;
				homologado.focus();
				homologado.select();
				calculaTotal(homologado, id_total_homologado);		
				return;
			
			}
			
			calculaTotal(homologado, id_total_homologado);		
		}
	}
	
	/******* caso exista campo efetivado (lançamento de efetivação) *********/	
	if(document.getElementById(id_efetivado)){			
		if(document.getElementById(id_efetivado).style.display != 'none' ){
				
			var efetivado  				  = document.getElementById(id_efetivado);
			var val_efetivado 			  = parseInt(efetivado.value);
			var val_provimento_autorizado = parseInt(document.getElementById('provimento_autorizado_'+nome_tabela).value);
			var val_concurso_autorizado   = parseInt(document.getElementById('concurso_autorizado_'+nome_tabela).value);			
			var val_disponivel_efetivacao = parseInt(td_disponivel_efetivacao.innerHTML);
			
			if((val_efetivado > 0) && (val_homologado_old == 0)){
			
				alert('É necessário cadastrar ao menos um Concurso Homologado.');
				efetivado.value = val_efetivado_old;
				efetivado.focus();
				efetivado.select();
				calculaTotal(efetivado, id_total_efetivado);		
				return;
						
			}
			if( ! ((val_efetivado <= val_homologado_old) && (val_efetivado <= val_provimento_autorizado) && (val_efetivado <= val_concurso_autorizado) && (val_efetivado <= val_disponivel_efetivacao) 
				   ) && (val_efetivado != 0) || (isNaN(val_efetivado)) ){
				alert('O Valor da nomeação dever ser menor ou igual ao homologado e concurso autorizado e provimento autorizado.');
				efetivado.value = val_efetivado_old;
				efetivado.focus();
				efetivado.select();
				calculaTotal(efetivado, id_total_efetivado);
				return;				
			}else{						
				document.getElementById(id_efetivado_old).value = val_efetivado;
				td_provimento_efetivado.innerHTML = parseInt(td_provimento_efetivado.innerHTML) + (val_efetivado -  val_efetivado_old);							
				td_disponivel_efetivacao.innerHTML = parseInt(td_disponivel_efetivacao.innerHTML) -   (val_efetivado -  val_efetivado_old);			
			}
			
			calculaTotal(efetivado, id_total_efetivado);		
		}
	}		
}


function valida_lancamento_docentes(nome_tabela, cargo_id){

	var id_disponivel_projetado    = 'disponivel_projetado_'+nome_tabela;
	var id_td_disponivel_projetado 	= 'td_disponivel_projetado_'+nome_tabela;
	var id_td_provimento_efetivado  = 'td_provimento_efetivado_'+nome_tabela;
	var id_td_disponivel_efetivacao = 'td_disponivel_efetivacao_'+nome_tabela;		
	
	var id_projetado  	  = 'projetado_'+nome_tabela;
	var id_publicado  	  = 'publicado_'+nome_tabela+'_'+cargo_id;
	var id_homologado 	  = 'homologado_'+nome_tabela+'_'+cargo_id;
	var id_efetivado  	  = 'efetivado_'+nome_tabela+'_'+cargo_id;	
	var id_publicado_old  = 'publicado_old_'+nome_tabela+'_'+cargo_id;
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;
	var id_efetivado_old  = 'efetivado_old_'+nome_tabela+'_'+cargo_id;
		
	var projetado   = document.getElementById(id_projetado);
	var publicado   = document.getElementById(id_publicado);
	
	if(document.getElementById(id_publicado_old)){
		var val_publicado_old   = parseInt(document.getElementById(id_publicado_old).value);
	}else{
		var val_publicado_old = 0;
	}

	if(document.getElementById(id_homologado_old)){
		var val_homologado_old   = parseInt(document.getElementById(id_homologado_old).value);
	}else{
		var val_homologado_old = 0;
	}

	if(document.getElementById(id_efetivado_old)){
		var val_efetivado_old   = parseInt(document.getElementById(id_efetivado_old).value);
	}else{
		var val_efetivado_old = 0;
	}
		
	var disponivel_projetado 	= document.getElementById(id_disponivel_projetado);	
	var td_disponivel_projetado = document.getElementById(id_td_disponivel_projetado);
	var td_provimento_efetivado  = document.getElementById(id_td_provimento_efetivado);
	var td_disponivel_efetivacao = document.getElementById(id_td_disponivel_efetivacao);
	
	var val_projetado   = parseInt(projetado.value);
	var val_publicado   = parseInt(publicado.value);
	var val_disponivel_projetado = parseInt(disponivel_projetado.value);
	if((val_publicado_old + val_disponivel_projetado) >= 0){
		if((val_publicado > (val_publicado_old + val_disponivel_projetado)) || (isNaN(val_publicado))){		
			alert('O Valor publicado dever ser menor ou igual ao valor disponível para publicação.');
			publicado.value = val_publicado_old;
			publicado.focus();
			publicado.select();			
			return;			
		}else{
			document.getElementById(id_publicado_old).value = val_publicado
			disponivel_projetado.value = (val_disponivel_projetado - (val_publicado -  val_publicado_old));			
			td_disponivel_projetado.innerHTML = (val_disponivel_projetado - (val_publicado -  val_publicado_old));		
		}
	}		
	
	
	if(document.getElementById(id_homologado)){
		if(document.getElementById(id_homologado).style.display != 'none' ){
		
			var homologado  	= document.getElementById(id_homologado);
			var val_homologado  = parseInt(homologado.value);		
			
			if( ((val_homologado > val_publicado_old)  && val_homologado != 0 ) 
					|| (isNaN(val_homologado)) ){
				alert('O Valor homologado dever ser menor ou igual ao Concurso Publicador.');
				homologado.value = val_homologado_old;
				homologado.focus();
				homologado.select();				
				return;				
			}
			
			if((val_homologado > 0) && (val_publicado_old == 0)){
			
				alert('É necessário cadastrar ao menos um Concurso Publicado.');
				homologado.value = val_homologado_old;
				homologado.focus();
				homologado.select();			
				return;			
			}			
		}
	}	

	if(document.getElementById(id_efetivado)){			
		if(document.getElementById(id_efetivado).style.display != 'none' ){
				
			var efetivado  		 		  = document.getElementById(id_efetivado);			
			var val_efetivado 			  = parseInt(efetivado.value);
			var val_provimento_autorizado = parseInt(document.getElementById('provimento_autorizado_'+nome_tabela).value);
			var val_concurso_autorizado   = parseInt(document.getElementById('concurso_autorizado_'+nome_tabela).value);			
			var val_disponivel_efetivacao = parseInt(td_disponivel_efetivacao.innerHTML);
			
			if((val_efetivado > 0) && (val_homologado_old == 0)){
			
				alert('É necessário cadastrar ao menos um Concurso Homologado.');
				efetivado.value = val_efetivado_old;
				efetivado.focus();
				efetivado.select();
				calculaTotal(efetivado, id_total_efetivado);		
				return;
						
			}
			
			if( ! ((val_efetivado <= val_homologado_old) && 
					(val_efetivado <= val_provimento_autorizado) && 
					(val_efetivado <= val_concurso_autorizado) && 
					(val_efetivado <= val_disponivel_efetivacao)					 
				   ) 
				   && (val_efetivado != 0) || (isNaN(val_efetivado)) ){				
				alert('O Valor da nomeação dever ser menor ou igual ao homologado e concurso autorizado e provimento autorizado.');
				efetivado.value = val_efetivado_old;
				efetivado.focus();
				efetivado.select();
				return;				
			}else{						
				document.getElementById(id_efetivado_old).value = val_efetivado;
				td_provimento_efetivado.innerHTML               = parseInt(td_provimento_efetivado.innerHTML) + (val_efetivado -  val_efetivado_old);							
				td_disponivel_efetivacao.innerHTML              = parseInt(td_disponivel_efetivacao.innerHTML) -   (val_efetivado -  val_efetivado_old);			
			}	
		}
	}	
}

function calculaTotal(coluna, id_total) {
	
	var id_coluna = coluna.id.substr(0,(coluna.id.lastIndexOf("_")));
		
	var i, soma = 0;
		
	form = document.getElementById("formulario");
			
	for(i=0; i<form.length; i++) {
		
		id_form = form.elements[i].id.substr(0,(form.elements[i].id.lastIndexOf("_")));
		
		if((id_coluna == id_form) && (form.elements[i].value != '')) {
			soma = soma + parseInt(form.elements[i].value);
		}	
	}
	if(document.getElementById(id_total))
		document.getElementById(id_total).value = soma;	

	return soma;
}
function atualizaTotal(lancamento, total_id) {
	var val_lancamento  = parseInt(lancamento.value);
	var val_cargo 		= parseInt(document.getElementById("cargo_id").value);
	
	document.getElementById("lancamento_id").value = val_lancamento + val_cargo;
	
}

--></script>

