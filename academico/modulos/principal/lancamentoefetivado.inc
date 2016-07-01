<?php
pegaSessoes(1, 1, 0);
verificaEditalPortaria($_SESSION["academico"]["edpid"], ACA_TPEDITAL_NOMEACAO);

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
	
		for($i=0; $i<count($_REQUEST["crgid"]); $i++) {	
			$sql_insert = "INSERT INTO academico.lancamentoeditalportaria
                   			(edpid, crgid, lepvlrprovefetivados, lepano, lepdtinclusao)
                			VALUES
                   			(".$edpid.",".$_REQUEST["crgid"][$i].",".$_REQUEST["efetivado"][$i].", '".$_SESSION['academico']['ano']."', now())";						
		
			$db->executar($sql_insert);		
		}
	}
	
	$db->commit();
	
	echo "<script>
			alert('Dados salvos com sucesso.');
			window.location = '?modulo=principal/lancamentoefetivado&acao=C';
		</script>";
	exit;	
}

$parametros = array(
					'',
					'&edpid=' . $_SESSION['academico']['edpideditalhomologacao']
				  );	


// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";
//Monta as abas da tela
//$db->cria_aba($abacod_tela,$url,$parametros);

// cria o titulo da tela	    		
$titulo_modulo = "Lançamento de Portaria de Efetivação";

monta_titulo( $titulo_modulo, "");
//echo $academico->cabecalhoedital( $edpid, $prtid, $entidcampus );

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);

$perfilNotBloq = array(PERFIL_IFESCADBOLSAS, 
					   PERFIL_IFESCADCURSOS, 
					   PERFIL_IFESCADASTRO, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR,
                       PERFIL_INTERLOCUTOR_INSTITUTO,
                       PERFIL_REITOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança
$bloqueado = (!$bloqueado && $permissoes['gravar']) ? false : true;

$habil = $bloqueado ? 'N' : $habil;
$habilitado = $bloqueado ? false : $habilitado;
$disabled = $bloqueado ? 'disabled=\"disabled\"' : '';


// Monta o Cabeçalho
if (isset($_SESSION["academico"]["edpid"])){ 
	$edpid=$_SESSION["academico"]["edpid"];
	$autoriazacaoconcursos = new autoriazacaoconcursos();
	$cabecalhoEdital = $autoriazacaoconcursos->cabecalhoedital($edpid, $prtid, $entidcampus );
}
?>
<style type="text/css" media="screen">
      /* Abas */
	.tituloaba{	
		width: 95%;
		margin:10px 0 0 0;
		left:2.5%;
		border-left:1px solid red;
		
	}
	
	.tituloaba2{
		font-size:12px; 
		text-align: center; 
		background-color: #dcdcdc; 
		height: 20px;
	}
       
</style>

<body>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="./geral/js/academico.js"></script>

<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <? echo $cabecalhoEdital; ?>
<form name="formulario" id="formulario" method="post" action="" >
<input type="hidden" name="submetido" id="submetido" value="0">


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" >
<tr>
	<td><br/></td>
</tr>
<? 
if($_SESSION['academico']['orgid'] != ACA_ORGAO_TECNICO):
?>
<tr>
	<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center">
			<tr>
				<td class="tituloaba2">
					<b>Docentes do Magistério Superior</b>
				</td>
			</tr>
			<tr>	
				<td>
					<?php 
						criarTabelaDocentesEfetivado("tabela_docms", 1);
					?>			
				</td>
			</tr>
		</table>
	</td>
</tr>
<tr>
	<td><br/></td>
</tr>
<? 
endif;
?>
<tr>
	<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center">
			<tr>
				<td class="tituloaba2">
					<b>Professores da Educação Básica, Técnica e Tecnológica</b>
				</td>
			</tr>
			<tr>	
				<td>
					<?php 
						criarTabelaDocentesEfetivado("tabela_docente", 7);
					?>			
				</td>
			</tr>
		</table>
	</td>
</tr>
<tr>
	<td><br/></td>
</tr>
<tr>
	<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
			<tr>
				<td class="tituloaba2">
					<b>TA Nível E</b>
				</td>
			</tr>
			<tr>	
				<td>
					<?php 
						criarTabelaEfetivado("tabela_tece", 2);
					?>			
				</td>
			</tr>
		</table>
	</td>
</tr>
<tr>
	<td><br/></td>
</tr>
<tr>
	<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
			<tr>
				<td class="tituloaba2">
					<b>TA Nível D</b>
				</td>
			</tr>
			<tr>	
				<td>
					<?php 
						criarTabelaEfetivado("tabela_tecd", 3);
					?>			
				</td>
			</tr>
		</table>
	</td>
</tr>
<tr>
	<td><br/></td>
</tr>
<tr>
	<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
			<tr>
				<td class="tituloaba2">
					<b>TA Nível C</b>
				</td>
			</tr>
			<tr>	
				<td>
					<?php 
						criarTabelaEfetivado("tabela_tecc", 4);
					?>			
				</td>
			</tr>
		</table>
	</td>
</tr>
<tr>
	<td><br/></td>
</tr>
<tr>
	<td>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center" >
			<tr>
				<td class="tituloaba2">
					<b>TA Nível B</b>
				</td>
			</tr>
			<tr>	
				<td>
					<?php 
						criarTabelaEfetivado("tabela_tecb", 5);
					?>			
				</td>
			</tr>
		</table>
	</td>
</tr>
			
	

<?
if($habilitado){
echo("	<tr>
			<td>
				<br/>
			</td>
		</tr>
		<tr bgcolor='#C0C0C0'>
			<td  align='left'>
				<input type='button' id='salvar' value='Salvar' onclick='salvarDistribuicao();'>
			</td>
		</tr>");	
}
?>
</table>
</form>
</body>
<script type="text/javascript"><!--

function salvarDistribuicao() {
	document.getElementById('submetido').value = 0;
	var lancado = document.getElementsByName("lancado[]");
	
	document.getElementById("salvar").disabled = true;
	var efetivado  = document.getElementsByName("efetivado[]");
	
	
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

function valida_lancamento(nome_tabela, cargo_id){

	var txt = '';
	var id_td_provimento_efetivado  = 'td_provimento_efetivado_'+nome_tabela;
	var id_td_disponivel_efetivacao = 'td_disponivel_efetivacao_'+nome_tabela;		
		
	var id_homologado = 'homologado_'+nome_tabela+'_'+cargo_id;
	var id_efetivado  = 'efetivado_'+nome_tabela+'_'+cargo_id;
	
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;
	var id_efetivado_old  = 'efetivado_old_'+nome_tabela+'_'+cargo_id;
	
	var id_total_homologado = 'total_hom_'+nome_tabela;
	var id_total_efetivado  = 'total_efe_'+nome_tabela;
	
	
	var td_provimento_efetivado  = document.getElementById(id_td_provimento_efetivado);
	var td_disponivel_efetivacao = document.getElementById(id_td_disponivel_efetivacao);
	
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

	
	/******* caso exista campo efetivado (lançamento de efetivação) *********/	
	if(document.getElementById(id_efetivado)){			
		if(document.getElementById(id_efetivado).style.display != 'none' ){
				
			var efetivado  				  = document.getElementById(id_efetivado);
			var val_efetivado 			  = efetivado.value != '' ? parseInt(efetivado.value) : 0;
			var val_provimento_autorizado = parseInt(document.getElementById('provimento_autorizado_'+nome_tabela).value);
			var val_concurso_autorizado   = parseInt(document.getElementById('concurso_autorizado_'+nome_tabela).value);			
			var val_disponivel_efetivacao = parseInt(td_disponivel_efetivacao.innerHTML);
			var val_provimento_efetivado  = parseInt(td_provimento_efetivado.innerHTML);
			
			if (document.getElementById(id_total_homologado)){
				var total_homologado = parseInt(document.getElementById(id_total_homologado).value);
			}else{
				var total_homologado = val_homologado_old;
			}
			
			if (document.getElementById(id_total_efetivado)){
				var total_efetivado  = parseInt(document.getElementById(id_total_efetivado).value);
			}else{
				var total_efetivado  = val_efetivado;
			}
				
			if((val_efetivado > 0) && (val_homologado_old == 0)){
			
				alert('É necessário cadastrar ao menos um Concurso Homologado.');
				efetivado.value = val_efetivado_old;
				efetivado.focus();
				efetivado.select();
				calculaTotal(efetivado, id_total_efetivado);		
				return;
						
			}
//			if( ! ((val_efetivado <= val_homologado_old) && (val_efetivado <= val_provimento_autorizado) && (val_efetivado <= val_concurso_autorizado) && (val_efetivado <= val_disponivel_efetivacao) 
//				   ) && (val_efetivado != 0) || (isNaN(val_efetivado)) ){
//			alert(val_efetivado);
			
			if (isNaN(val_efetivado)){
				txt = 'O lançamento de Provimento Efetivado deve ser numérico!';
			}else if (val_efetivado > val_homologado_old){
				txt = 'O lançamento do Provimento Efetivado deve ser menor ou igual ao lançamento de concurso homologado!';
			}else if ( (total_efetivado) > val_provimento_autorizado){
				txt = 'A soma dos Provimentos Efetivados devem ser menor ou igual ao provimento autorizado!';
			}else if ( (total_efetivado) > val_concurso_autorizado){
				txt = 'A soma dos lançamentos de Provimentos Efetivados devem ser menor ou igual ao concurso autorizado!';
			}else if ( (val_disponivel_efetivacao - (val_efetivado - val_efetivado_old) ) < 0 ){
				txt = 'A soma dos lançamentos de Provimentos Efetivados devem ser menor ou igual ao disponível para efetivação!';
			}/*else if ( (val_provimento_efetivado + (val_efetivado - val_efetivado_old)) > total_homologado && val_efetivado > val_efetivado_old){
				txt = 'O provimento efetivado deve ser menor ou igual a soma dos lançamentos de concursos homologados!';
			}*/
			
			if (txt != ''){
				alert(txt);
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
/*			 
			if( ! ((val_efetivado < val_homologado_old) 
					&& (val_efetivado < val_provimento_autorizado) 
					&& (val_efetivado < val_concurso_autorizado) 
					&& (val_efetivado < val_disponivel_efetivacao) 
				   ) && (val_efetivado != 0) 
				   		 || (isNaN(val_efetivado)) ){
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
*/			
			calculaTotal(efetivado, id_total_efetivado);		
		}
	}		
}

function valida_lancamento_docentes(nome_tabela, cargo_id){

	valida_lancamento(nome_tabela, cargo_id);
	return;



	var id_td_provimento_efetivado  = 'td_provimento_efetivado_'+nome_tabela;
	var id_td_disponivel_efetivacao = 'td_disponivel_efetivacao_'+nome_tabela;		
	
	var id_publicado  	  = 'publicado_'+nome_tabela+'_'+cargo_id;
	var id_homologado 	  = 'homologado_'+nome_tabela+'_'+cargo_id;
	var id_efetivado  	  = 'efetivado_'+nome_tabela+'_'+cargo_id;	
	var id_publicado_old  = 'publicado_old_'+nome_tabela+'_'+cargo_id;
	var id_homologado_old = 'homologado_old_'+nome_tabela+'_'+cargo_id;
	var id_efetivado_old  = 'efetivado_old_'+nome_tabela+'_'+cargo_id;
	
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
		
	var td_provimento_efetivado  = document.getElementById(id_td_provimento_efetivado);
	var td_disponivel_efetivacao = document.getElementById(id_td_disponivel_efetivacao);
		
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

--></script>
