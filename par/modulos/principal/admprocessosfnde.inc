<?php

if( $_REQUEST['req'] == 'carregaDados' && $_REQUEST['prpid'] ){
	$sql = "SELECT 
				pp.prpnumeroprocesso, 
				pp.muncod, 
				pp.prpbanco, 
				pp.prpagencia, 
				pp.nu_conta_corrente, 
				pp.prptipoexecucao,
				mun.estuf
			FROM 
				par.processopar pp
			INNER JOIN
				territorios.municipio mun on mun.muncod = pp.muncod
			WHERE
				pp.prpid = {$_REQUEST['prpid']}
				AND pp.prpstatus = 'A'
				";
	$dados = $db->pegaLinha( $sql );
	echo $dados['prpnumeroprocesso'].'|||'.$dados['muncod'].'|||'.$dados['prpbanco'].'|||'.$dados['prpagencia'].'|||'.$dados['nu_conta_corrente'].'|||'.$dados['prptipoexecucao'].'|||'.$dados['estuf'];
	die();
}

if( $_REQUEST['req'] == 'carregaMunicipio' && $_REQUEST['estuf'] ){
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundescricao";
	if( $_REQUEST['tipo'] == 2 ){
		$db->monta_combo( "muncod2", $sql, 'S', 'Selecione', '', '', '', '','N','muncod2' );
	} else {
		$db->monta_combo( "muncod", $sql, 'S', 'Selecione', '', '', '', '','S','muncod' );
	}
	die();
}

if ($_REQUEST['requisicao'] == 'excluirProcesso'){

	$prpnumeroprocesso = $_REQUEST['prpnumeroprocesso'];
	$prpid = $_REQUEST['prpid'];

	$sql = "Select empid From par.empenho Where empnumeroprocesso = '$prpnumeroprocesso' and empstatus = 'A'";
	$dados = $db->pegaLinha($sql);

	if($prpnumeroprocesso != '' && $prpid != '' && $dados['empid'] == ''){
		$sql = "Update par.processopar Set prpstatus = 'D' where prpid = $prpid returning prpid;";
		$result = $db->pegaLinha($sql);
		if($result['prpid'] > 0){
			$db->commit();
			die('ok');	
		}else{
			die;
		}
	}
}

function listaDados($where=null){
	global $db;
	
	$substituir = array(".", "/", "-");
	$prpnumeroprocesso = str_replace($substituir, "", $_REQUEST['processo']);
	
	if ($_REQUEST['requisicao'] == 'carregaPesquisa'){ 
		//ver($_REQUEST);
		if($prpnumeroprocesso != ''){
			$where = "AND foo.prpnumeroprocesso = '$prpnumeroprocesso'";
		}
		if($_REQUEST['estuf2']){
			$where .= "AND foo.estado = '".$_REQUEST['estuf2']."'";
		}
		if($_REQUEST['muncod2']){
			$where .= "AND foo.muncod = '".$_REQUEST['muncod2']."'";
		}
		if($_REQUEST['esfera'] <> ''){
			$where .= "AND foo.itrid = ".$_REQUEST['esfera'];
		}
	}else{
		$where = "";
	}
	
	$iconDesabilitado = "'&nbsp &nbsp<img src=\"/imagens/excluir_01.gif\" border=0\"></a> </center>'";
	$iconAbilitado = "'&nbsp &nbsp<a style=\"margin: 0 0px 0 0px;\" href=\"#\" onclick=\"excluirProcesso('|| pp.prpnumeroprocesso ||','|| pp.prpid ||')\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Selecionar\"></a> </center>'";
	
	$sql = "
		SELECT
			Distinct foo.acao,
			foo.prpnumeroprocesso, 
			foo.mundescricao, 
			foo.prpbanco, 
			foo.prpagencia, 
			foo.prpdatainclusao, 
			foo.prpseqconta, 
			foo.seq_conta_corrente, 
			foo.nu_conta_corrente, 
			foo.tipo, 
			foo.prpdocumenta 
		FROM (
			Select	Distinct '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreDados('|| pp.prpid ||')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' || 
					Case When emp.empid is null Then $iconAbilitado Else $iconDesabilitado end  as acao,
					pp.prpnumeroprocesso, 
					m.mundescricao, 
					pp.prpbanco, 
					pp.prpagencia, 
					to_char(pp.prpdatainclusao, 'DD/MM/YYYY') as prpdatainclusao, 
					pp.prpseqconta, 
					pp.seq_conta_corrente, 
					pp.nu_conta_corrente, 
					Case When pp.prptipoexecucao = 'T' Then 'Termo de Compromisso' Else 'Convênio' End as tipo, 
					pp.prpdocumenta,
					CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as estado,
					prpstatus,
					iu.itrid,
					m.muncod
			From par.processopar pp
			inner join par.instrumentounidade iu on iu.inuid = pp.inuid
			Left Join par.empenho emp on emp.empnumeroprocesso = pp.prpnumeroprocesso and empstatus = 'A'
			Left Join territorios.municipio m ON m.muncod = pp.muncod
			Order by m.mundescricao, pp.prpnumeroprocesso ) as foo
			WHERE foo.prpstatus = 'A'  $where
				
	";
	//ver($sql);
	$cabecalho = Array('Ação','Numero do Processo','Município','Banco','Agência', 'Data de Inclusão', 'Sequencial da Conta', 'Sequencial da Conta Corrente', 'Número da Conta Corrente', 'Tipo de Execução', 'Documenta');
	$db->monta_lista( $sql, $cabecalho, 50, 20, 'N', 'center','N');
}

if ($_REQUEST['requisicao'] == 'salvarProcesso'){
	if( $_POST['prpnumeroprocesso']){
		if( $_POST['prpid'] ){//UPDATE
			
			$sql = "UPDATE
						par.processopar
					SET
						prpnumeroprocesso = '".$_POST['prpnumeroprocesso']."', 
						muncod = '".$_POST['muncod']."',
						prpbanco = '".$_POST['prpbanco']."',
						prpagencia = '".$_POST['prpagencia']."',
						nu_conta_corrente = '".$_POST['nu_conta_corrente']."',
						prptipoexecucao = '".$_POST['prptipoexecucao']."'
					WHERE
						prpid = ".$_POST['prpid'];
			$db->executar($sql);
			$db->commit();
	
			/*echo "<script>
					alert('Operação realizada com sucesso!');
					//window.location.reload();
						window.location.submit();
				</script>";*/
			die('ops');	
			
		} else { //INSERT
			
			if($_POST['muncod']){ //municipal
				$inuid = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$_POST['muncod']."'" );
			} else { //estadual
				$inuid = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$_POST['estuf']."'" );
			}
			
			$sql = "SELECT prpnumeroprocesso FROM par.processopar WHERE prpnumeroprocesso = '".$_POST['prpnumeroprocesso']."' and prpstatus = 'A' ";
			if( $db->pegaUm( $sql ) ){
				/*echo "<script>
					alert('O Processo já existe!');
					//window.location.reload();
						window.location.submit();
				</script>";*/
				die('ope');
			} else {
			
				$sql = "INSERT INTO par.processopar(
					            prpnumeroprocesso, muncod, prpbanco, prpagencia, prpdatainclusao, 
					            usucpf, prptipo, nu_conta_corrente, inuid, prptipoexecucao)
						VALUES
							(
							'".$_POST['prpnumeroprocesso']."','".$_POST['muncod']."','".$_POST['prpbanco']."','".$_POST['prpagencia']."',NOW(),
							'".$_SESSION['usucpf']."','O','".$_POST['nu_conta_corrente']."',{$inuid},'".$_POST['prptipoexecucao']."')";
				$db->executar($sql);
				$db->commit();
		
				/*echo "<script>
						alert('Operação realizada com sucesso!');
						//window.location.reload();
						window.location.submit();
					</script>";*/
				die('ops');	
			}
			
		}
	}
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../includes/calendario.js"></script>
<div id="loader-container">
	<div id="loader">
		<img src="../imagens/wait.gif" border="0" align="middle">
		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>
<div id="container">
	<form action="" method="post" name="anexaResolucao" id="anexaResolucao" enctype="multipart/form-data" >
		<input type="hidden" id="resid" name="resid" value="">
		<input type="hidden" id="prpid" name="prpid" value="">
		<input type="hidden" id="update" name="update" value="">
		<input type="hidden" id="anoAtual" name="anoAtual" value="<?=date("Y"); ?>">
		<input type="hidden" id="requisicao" name="requisicao" value="carregaPesquisa">
		<input type="hidden" id="arquivoexistente" name="arquivoexistente" value="">
		
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<!-- Campo para a pesquisa do processo -->
			<tr>
				<td class="SubTituloCentro" colspan="2" align="center">PESQUISA</td>
			</tr>
			<tr>
				<td class="subtituloDireita">Pesquisa efetuada pelo nº do <b>PROCESSO:</b></td>
				<td>
					<?php echo campo_texto("processo","N","S","processo","30","",'',"","","","","","",""); ?>
				</td> 			
			</tr>
			<tr>
				<td class="subtituloDireita">
					Pesquisa efetuada por Esfera:
				</td>
				<td>
					<?
					$arr = array( array('codigo' => 1, 'descricao' => 'Estadual'), array( 'codigo' => 2, 'descricao' => 'Municipal')); 
					$db->monta_combo( "esfera", $arr, 'S', 'Todas', '', '', '', '','N','esfera' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Pesquisa efetuada pelo Estado:
				</td>
				<td>
					<? 
					$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado";
					$db->monta_combo( "estuf2", $sql, 'S', 'Selecione', 'carregaMunicipio2', '', '', '','N','estuf2' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Pesquisa efetuada pelo Município:
				</td>
				<td id="tdmunicipio2">
					<?
					$arr = array(); 
					$db->monta_combo( "muncod2", $arr, 'S', 'Selecione', '', '', '', '','N','muncod2' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
				</td>
				<td>
					<input type="button" value="Pesquisar" onclick="pesq()">
					<input type="button" value="Mostrar Todos" onclick="mostrarTudo()">
				</td>
			</tr>
			<!-- dados do processo -->
			<tr>
				<td class="SubTituloCentro" colspan="2">DADOS DO PROCESSO</td>
			</tr>				
			<tr>
				<td class="subtituloDireita">
					Número do Processo:
				</td>
				<td>
					<? echo campo_texto('prpnumeroprocesso', 'S', 'S', '', 30, 25, '#################', '', 'right', '', 0, 'id="prpnumeroprocesso" style="text-align:right;" '); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Estado:
				</td>
				<td>
					<? 
					$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado";
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione', 'carregaMunicipio', '', '', '','S','estuf' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Município:
				</td>
				<td id="tdmunicipio">
					<?
					$arr = array(); 
					$db->monta_combo( "muncod", $arr, 'S', 'Selecione', '', '', '', '','S','muncod' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Banco:
				</td>
				<td>
					<? echo campo_texto('prpbanco', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="prpbanco" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Agência:
				</td>
				<td>
					<? echo campo_texto('prpagencia', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="prpagencia" style="text-align:right;" ' ); ?>
				</td>
			</tr><!--
			<tr>
				<td class="subtituloDireita">
					Sequencial da Conta:
				</td>
				<td>
					<? echo campo_texto('prpseqconta', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="prpseqconta" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Sequencial da Conta Corrente:
				</td>
				<td>
					<? echo campo_texto('seq_conta_corrente', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="seq_conta_corrente" style="text-align:right;" ' ); ?>
				</td>
			</tr>-->
			<tr>
				<td class="subtituloDireita">
					Número da Conta Corrente:
				</td>
				<td>
					<? echo campo_texto('nu_conta_corrente', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="nu_conta_corrente" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Tipo de Execução:
				</td>
				<td>
					<? 
					$arr = array(
								array('codigo' => 'T', 'descricao' => 'Termo de Compromisso'),
								array('codigo' => 'C', 'descricao' => 'Convênio')
							);
					$db->monta_combo( "prptipoexecucao", $arr, 'S', 'Selecione', '', '', '', '','S','prptipoexecucao' );
					?>
				</td>
			</tr>
			<!--<tr>
				<td class="subtituloDireita">
					Número do Convênio FNDE:
				</td>
				<td>
					<? echo campo_texto('prpnumeroconveniofnde', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="prpnumeroconveniofnde" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Ano do Convênio FNDE:
				</td>
				<td>
					<? echo campo_texto('prpanoconveniofnde', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="prpanoconveniofnde" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Número do Convênio SIAFI:
				</td>
				<td>
					<? echo campo_texto('prpnumeroconveniosiafi', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="prpnumeroconveniosiafi" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			-->
			<tr>
				<td class="subtituloDireita">
				</td>
				<td>
					<input type="button" name="salvar" value="Salvar" onclick="valida( this.form )">&nbsp;<input type="button" name="limpar" value="Limpar" onclick="limpa()">
				</td>
			</tr>
		</table>
	</form>
	<!-- tr>
		<td id="tdLista" colspan="2">
			<?php //listaDados(); ?>
		</td>
	</tr-->
	<div id="tdlista"><?php listaDados(); ?></div>
</div>


<script><!--
$('loader-container').hide();

var tdLista = $('tdLista');

function cadastroPI(){
	prpnumeroprocesso = document.getElementById('prpnumeroprocesso').value;
	window.open('par.php?modulo=principal/cadPlanoInterno&acao=A&prpnumeroprocesso='+prpnumeroprocesso,'gera','width=700,height=350,scrollbars=1');
}

function carregaMunicipio( estuf ){
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: "req=carregaMunicipio&estuf="+estuf,
		onComplete: function (msg){
	   		$('tdmunicipio').innerHTML = msg.responseText;
	   	}
	 });	
}

function carregaMunicipio2( estuf ){
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: "req=carregaMunicipio&tipo=2&estuf="+estuf,
		onComplete: function (msg){
	   		$('tdmunicipio2').innerHTML = msg.responseText;
	   	}
	 });	
}

function abreDados( prpid ){
	
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: "req=carregaDados&prpid="+prpid,
		onComplete: function (msg){
	   		mensagem = msg.responseText;
	   		dados = mensagem.split('|||');
	   		document.getElementById('prpid').value = prpid;
	   		document.getElementById('prpnumeroprocesso').value = dados[0];
	   		document.getElementById('estuf').value = dados[6];
	   		document.getElementById('prpbanco').value = dados[2];
	   		document.getElementById('prpagencia').value = dados[3];
	   		document.getElementById('nu_conta_corrente').value = dados[4];
	   		document.getElementById('prptipoexecucao').value = dados[5];
	   		carregaMunicipio( dados[6] );
	   		document.getElementById('muncod').value = dados[1];
	   	}
	 });	

}

function limpa(){
	document.getElementById('prpid').value = "";
	document.getElementById('prpnumeroprocesso').value = "";
   	document.getElementById('estuf').value = "";
   	document.getElementById('muncod').value = "";
   	document.getElementById('prpbanco').value = "";
   	document.getElementById('prpagencia').value = "";
   	document.getElementById('nu_conta_corrente').value = "";
   	document.getElementById('prptipoexecucao').value = "";
}


function valida( form ){

	var prpnumeroprocesso 	= jQuery("[name='prpnumeroprocesso']").val();
	var estuf 				= jQuery("[name='estuf']").val();
	var muncod 				= jQuery("[name='muncod']").val();
	var prpbanco 			= jQuery("[name='prpbanco']").val();
	var prpagencia 			= jQuery("[name='prpagencia']").val();
	var nu_conta_corrente	= jQuery("[name='nu_conta_corrente']").val();
	var prptipoexecucao 	= jQuery("[name='prptipoexecucao']").val();
	var prpid 				= jQuery("[name='prpid']").val();
	var ano 				= jQuery("[name='anoAtual']").val();
	
	if( prpnumeroprocesso.value == '' ){
		alert('Campo Número do Processo Obrigatório!');
		resnumero.focus();
		return false;
	}
	
	if( prpnumeroprocesso.length != 17 ){
		alert('O Número do Processo está Errado!');
		resnumero.focus();
		return false;
	}

	if( prpnumeroprocesso.substring(11, 15) > 2015 || prpnumeroprocesso.substring(11, 15) < 2000 ){
		alert('Verifique o Ano do Número do Processo!');
		resnumero.focus();
		return false;
	}
	
	if( muncod == '' ){
		alert('Campo Município Obrigatório!');
		resanoreferencia.focus();
		return false;
	}
	
	if( prptipoexecucao.value == '' ){
		alert('Campo Tipo de Execução Obrigatório!');
		arqid.focus();
		return false;
	}
	
	
	if( prpid ){
   		dados = "requisicao=salvarProcesso&prpnumeroprocesso=" + prpnumeroprocesso + "&muncod=" + muncod + "&prpbanco=" + prpbanco + "&prpagencia=" + prpagencia + "&nu_conta_corrente=" + nu_conta_corrente + "&prptipoexecucao=" + prptipoexecucao + "&prpid=" + prpid;
   	} else {
   		dados = "requisicao=salvarProcesso&prpnumeroprocesso=" + prpnumeroprocesso + "&muncod=" + muncod + "&prpbanco=" + prpbanco + "&prpagencia=" + prpagencia + "&nu_conta_corrente=" + nu_conta_corrente + "&prptipoexecucao=" + prptipoexecucao;
   	}
	
	//SALAVAR
	jQuery.ajax({
	   	type: "POST",
	   	url: window.location,
	   	data: dados,
	   	success: function(msg){		
	   	   	if(msg == "ope"){
	   		alert('O Processo já existe!');
	   			form.submit();
	   		}else if(msg == "ops"){
	   			alert('Dados atualizados com sucesso!');
	   			form.submit();
	   		}
		}
	});
}

function excluirProcesso(prpnumeroprocesso, prpid){
	if(confirm("Deseja realmente excluir este PROCESSO?")){
		jQuery.ajax({
		   type: "POST",
		   url : window.location,
		   data: "requisicao=excluirProcesso&prpnumeroprocesso=" + prpnumeroprocesso + "&prpid=" + prpid,
		   success: function(msg){
				if(msg == 'ok'){
					alert('PROCESSO excluído com sucesso!');
					anexaResolucao.submit();					
				}else{
					alert('Não foi possível excluir o PROCESSO!');
				}
		   }
	 	});		
	}
}

function pesq(){
//	if(jQuery("[name=processo]").val() == ''){
//		alert("Campo de Pesquisa Item obrigatório");
//		return false;
//	}
	
//	var processo  = jQuery("[name=processo]").val();
	
	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=carregaPesquisa"+ $('anexaResolucao').serialize(),
		success: function(msg){
			anexaResolucao.submit();
		}
	});
}

function mostrarTudo(){
	jQuery.ajax({
		type: "POST",
		url: window.location,
		data: "requisicao=carregaPesquisa&processo=",
		success: function(msg){
			anexaResolucao.submit();
		}
	});
}

--></script>
