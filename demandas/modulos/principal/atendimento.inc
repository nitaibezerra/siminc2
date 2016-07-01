<?php
if(isset($_POST['registrar'])){
	$dataatend = formata_data_sql($_POST['dmddatahoraatendimento']).' '.'00:00';
	$hriniatend = formata_data_sql($_POST['dmddatahoraatendimento']).' '.$_POST['hiniciohoratendimento'];			
	$hrfimatend = formata_data_sql($_POST['dmddatahoraatendimento']).' '.$_POST['hfimhoratendimento'];
	$calchr = date('H:i',mktime(date('H', strtotime($hrfimatend)) - date('H', strtotime($hriniatend)), date('i', strtotime($hrfimatend)) - date('i', strtotime($hriniatend))));
	$horas = formata_data_sql($_POST['dmddatahoraatendimento']).' '.$calchr;
	$demanda = $_SESSION['dmdid'];
	$usuariocpf = $_POST['usucpfexecutor'];
	
	$sqlverifica = "SELECT COUNT(idregistrohoras) 
					FROM demandas.registroshorasdeatendimento 
					WHERE dmdid IN (SELECT dmdid FROM demandas.demanda WHERE usucpfexecutor = '$usuariocpf') 
					AND (data = '$dataatend')
					AND ( ('$hriniatend' BETWEEN horainicio AND horafim) OR ('$hrfimatend' BETWEEN horainicio AND horafim))";
	$registro = $db->Pegaum($sqlverifica);

	$msg = utf8_decode("O Responsável está alocado no horário informado ou em outra demanda.");
	
	if($registro){
		echo "<script language='javascript' type='text/javascript'>alert('$msg');</script>";		
	} else {
		$sql = "INSERT INTO demandas.registroshorasdeatendimento(dmdid, cpftecnicoresponsavel, data, horainicio, horafim, qtdhoras) VALUES ($demanda,'$usuariocpf','$dataatend','$hriniatend','$hrfimatend','$horas')"; 
	}
	
	$db->executar($sql);
	$db->commit();	
	exibirRegHoras();
	exit;
}	

if(isset($_POST['excluir'])){
	$sql = "DELETE FROM demandas.registroshorasdeatendimento WHERE idregistrohoras = ".$_POST['reghoras'];
	$db->executar($sql);
	$db->commit();	
	exibirRegHoras();
	exit;
}

$dmdid = $_SESSION['dmdid'];
$sidid = pegaSidid($dmdid);

$habil = 'N';
$mostraWorkflow = 'N';

//verifica se o usuário pode editar a demanda
$verificaEditaDemanda = verificaEditaDemanda();
if($verificaEditaDemanda == true){
	$habil = 'S';
	$mostraWorkflow = 'S';
}
else{
	$habil = 'N';
	$mostraWorkflow = 'N';
}

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();
//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
if( in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_GENERICO_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_ESTADO_INVALIDADA,DEMANDA_GENERICO_ESTADO_INVALIDADA,DEMANDA_ESTADO_VALIDADA_FORA_PRAZO)) ){
	$habil = 'N';
}

//desabilita campos para o perfil analista fnde
if (in_array(DEMANDA_PERFIL_ANALISTA_FNDE, arrayPerfil())){
	//$habil = 'N';
}

//habilita e desabilita campos para o perfil gerente fabrica de software
if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, arrayPerfil()) || in_array(DEMANDA_PERFIL_ANALISTA_FSW, arrayPerfil()) ){
	$habil = 'N';
	$mostraWorkflow = 'S';
}
if ( in_array(DEMANDA_PERFIL_GERENTE_FSW, arrayPerfil()) && in_array($esdid, array(DEMANDA_GENERICO_ESTADO_EM_ANALISE)) ){
	$habil = 'S';
	$mostraWorkflow = 'S';
	$gerenteFW = 'S';
}




function verificaClassificacao($dmdidx){
	global $db;
 	
	$sql = sprintf("SELECT 
					 u.usunome 
					FROM 
					 	demandas.demanda d
					 	inner join seguranca.usuario u ON u.usucpf = d.usucpfexecutor
					WHERE
					 dmdid = %d 
					", 
			$dmdidx);
	$usunome = $db->PegaUm($sql);
	
	echo $usunome;
	exit;
}  

function verificaTempoAdicional($priidt){
	global $db;
	$sql = sprintf("SELECT
					 (case when t.ordid in (18,19,20,21) then
					 	lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0') 
					 else
					 	lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
					 end) as  tsptempo,
					 ( (case when d.dmdqtde > 0 then 
					 		case when char_length(((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar) = 1 then
								'0' || ((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar
							else
								((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar
						    end
						    || ':' ||
						    lpad(mod((d.dmdqtde*tspminuto),60)::varchar,2,'0') 
					 	else
					 		(case when t.ordid in (18,19,20,21) then
							 	lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0') 
							 else
							 	lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
							 end) 
					 	end) 
					 ) as tempototal
					FROM 
					 	demandas.demanda d
					 	inner join demandas.tiposervicoprioridade p ON p.tipid = d.tipid
					 	LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
					WHERE
					 d.dmdid = %d
					 AND p.priid = %d 
					 and p.tspstatus = 'A'
					", 
					$_SESSION['dmdid'],
					$priidt);
	$dados = $db->PegaLinha($sql);
	
	//verifica se existe severidade
	if($dados['tsptempo']){
		echo $dados['tsptempo'] .'|'. $dados['tempototal'];
	}
	else{
		echo "00:00|00:00";
	}
	exit;
}

//verifica classificacao da demanda
if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	verificaClassificacao($_POST['dmdidAjax']);
	exit;
}


//verifica calculo do tempo
if ($_REQUEST['priidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	//_funcoes.php
	$retorno = verificaCalculoTempoPrioridade($_POST['priidAjax'],$_POST['dtini'],$_REQUEST['tempoadicional']);
	echo $retorno;
	exit;
}

//verifica calculo do tempo
if ($_REQUEST['priidTempoAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	verificaTempoAdicional($_POST['priidTempoAjax']);
	exit;
}

if ($_POST){
	salva();
}


include APPRAIZ ."includes/cabecalho.inc";
include_once APPRAIZ . 'includes/workflow.php';

/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );

print '<br>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Dados de Atendimento - Cód. # '.$dmdid, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
//print '<br>';

/*******************
 * CARREGA DADOS DA PÁGINA
 *******************/ 
$sql = "SELECT
		 to_char(d.dmddatainclusao, 'DD/MM/YYYY') AS dmddatainclusao,
		 to_char(d.dmddatainiprevatendimento, 'DD/MM/YYYY') as dmddatainiprevatendimento,
		 d.dmddatafimprevatendimento,
		 d.dmddatainiprevatendimento as dmddatainiprevatendimento2,		 
		 to_char(d.dmddatainiprevatendimento, 'HH24:MI') AS hiniatendimento,
		 to_char(d.dmddatafimprevatendimento, 'HH24:MI') AS hfimatendimento,		 
		 dmdclassificacao,
		 dmdclassificacaosistema,
		 d.priid,
		 d.dmdjustprioridade,
		 usucpfexecutor,
		 usunome AS usunomeexecutor,
		 to_char(d.dmdtempoadicional, 'HH24:MI') as dmdtempoadicional,
		 (case when t.ordid in (18,19,20,21) then
		 	lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0') 
		 else
		 	lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
		 end) as tempocatalogo,
		 ( (case when d.dmdqtde > 0 then 
		 		case when char_length(((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar) = 1 then
					'0' || ((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar
				else
					((d.dmdqtde * tsphora)+((d.dmdqtde*tspminuto)/60)::integer)::varchar
			    end
			    || ':' ||
			    lpad(mod((d.dmdqtde*tspminuto),60)::varchar,2,'0') 
		 	else 
		 		(case when t.ordid in (18,19,20,21) then
				 	lpad(tsphora::varchar,3,'0') || ':' || lpad(tspminuto::varchar,2,'0') 
				 else
				 	lpad(tsphora::varchar,2,'0') || ':' || lpad(tspminuto::varchar,2,'0')
				 end)
		 	end) 
		 ) as tempototal,
		 dmdobstempoadicional,
		 (CASE WHEN d.dmdqtde > 0 THEN 
		 			d.dmdqtde
		 	   ELSE
		 	   		'1'	 
		 END) AS dmdqtde,
		 d.celid,
		 t.ordid
		FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid 
		 LEFT JOIN seguranca.usuario u ON d.usucpfexecutor = u.usucpf 
		 left join demandas.tiposervicoprioridade p ON p.tipid = d.tipid and p.priid = d.priid and p.tspstatus = 'A'
		WHERE
		 dmdid =".$dmdid;

$dados = (array) $db->carregar($sql);
if (is_array($dados[0])){
	extract($dados[0]);
}
?>
<html>
 <head>
 
  <!-- AUTO COMPLEMENTO -->
  <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
  <script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
  <script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
  <script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest_atendimento_demandas.js"></script>
  <script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>  
  <script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
  <script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
  <script type="text/javascript">
    jQuery.noConflict();
    jQuery(document).ready(function(){
    <?php 
        $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
        if( in_array($estadoAtualDaDemanda['esdid'], array(
        DEMANDA_ESTADO_EM_ANALISE
        ,DEMANDA_ESTADO_EM_ATENDIMENTO
        ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
        ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
        ) ) ) { ?>
            jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
    <?php } ?>
    });
  </script>
  
  <style>
	.suggestMarked
	{
		background-color: #AAAAFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		position: relative;
		z-index: 100;
	}
	.suggestUnmarked
	{
		background-color: #EEEEFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		z-index: 100;
		position: relative;
	}
  </style>
	
  <script type="text/javascript">
  	
  	d = document;
	
	dias_somar  = 0;
	horas_somar = 0;
	
	/******************
	 * ABRE POPUP DE PESQUISA DE USUÁRIO
	 * PARA ATRIBUIR NO CAMPO RESPONSÁVEL
	******************/
	function popupPesqUser(){
		param = d.getElementById('usucpfexecutor').value ? 'usucpf='+d.getElementById('usucpfexecutor').value : ''; 

		w = window.open('?modulo=principal/popup/pesqUser2&acao=A&'+param,'pesqUser','scrollbars=1,location=0,toolbar=0,menubar=0,status=0,titlebar=0,directories=0,width=900,height=600,left=250,top=125'); 
		w.focus();	
	}
	
	function checaPadrao(obj){	
		var elemento = d.getElementsByTagName('input');
		var total = elemento.length;
		
		// Loop em todos os elementos "input"
		for (i=0; i<total; i++){

			// Valida e elimina o tipo não desejado
			if (elemento[i].type != 'radio'){
				continue;
			}
			
			// Valida se é o campo desejado e tira o checked	
			if(elemento[i].name.indexOf('tispadrao') > -1 && elemento[i] != obj)
				elemento[i].checked = false;
					
		}
	}
	function popupObservacao(){
		w = window.open('?modulo=principal/popup/addObservacao&acao=A','addObservacao','scrollbars=yes,location=no,toolbar=no,menubar=no,width=550,height=200,left=250,top=125'); 
		w.focus();	
	}
	
	/******************
	 * VALIDA FORMULÁRIO
	 *
	******************/
	function validaFormx(){

		var datainc = d.getElementById('dmddatainclusao');		
		var dataini = d.getElementsByName('dmddatainiprevatendimento')[0];
		var datafim = d.getElementsByName('dmddatafimprevatendimento')[0];
		
		var horaini = d.getElementsByName('hiniatendimento')[0];
		var horafim = d.getElementsByName('hfimatendimento')[0];
		
		var usunome = d.getElementsByName('usunomeexecutor')[0];
		var usucpf  = d.getElementsByName('usucpfexecutor')[0];
		var classif = d.getElementsByName('dmdclassificacao')[0];
		var classis = d.getElementsByName('dmdclassificacaosistema')[0];

		
		if (d.getElementsByName('priid')[0].value == ''){
			alert('O campo prioridade deve ser preenchido!');
			d.getElementsByName('priid')[0].focus();
			return false;
		}

		if(document.formulario.priid.value != document.formulario.setaprioridade.value){
			if (document.formulario.dmdjustprioridade.value == ''){
				alert('O campo Justificativa da Prioridade deve ser preenchido!');
				document.formulario.dmdjustprioridade.focus();
				return false;
			}
		}		
		
		if (d.getElementById('tempoadicional').value != ''){
			if (d.getElementById('tempoadicional').value.length != 5){
				alert('O campo Tempo Adicional é inválido!');
				d.getElementById('tempoadicional').focus();
				d.getElementById('tempoadicional').select();
				return false;
			}
			if (d.getElementsByName('dmdobstempoadicional')[0].value == ''){
				alert('O campo Justificativa do Tempo Adicional deve ser preenchido!');
				d.getElementsByName('dmdobstempoadicional')[0].focus();
				d.getElementsByName('dmdobstempoadicional')[0].select();
				return false;
			}
		}
				

		if (dataini.value == ''){
			alert('O campo início do atendimento deve ser preenchido!');
			dataini.focus();
			dataini.select();
			return false;
		}

		if (horaini.value == ''){
			alert('O campo Previsão de início do atendimento deve ser preenchido!');
			horaini.focus();
			horaini.select();
			return false;
		}
		if (horaini.value.length != 5){
			alert('O campo Previsão de início é inválido!');
			horaini.focus();
			horaini.select();
			return false;
		}
		
				
		
		
		<?if(!$sidid){?>
			
			var dinc = datainc.value.split('/');		
			var dini = dataini.value.split('/');
			
			dinc = dinc[2] + dinc[1] + dinc[0];
			dini = dini[2] + dini[1] + dini[0];

			if (parseFloat(dini) < parseFloat(dinc)){
				alert('O campo Previsão de início do atendimento deve ser maior que a data de inclusão da demanda!');
				dataini.focus();
				dataini.select();
				return false;			
			}
			
			/*
			if (dini[2] < dinc[2]){
				alert('O campo Previsão de início do atendimento deve ser maior que a data de inclusão da demanda!');
				dataini.focus();
				dataini.select();
				return false;			
			}else if (dini[1] < dinc[1] && dini[2] == dinc[2]){
				alert('O campo Previsão de início do atendimento deve ser maior que a data de inclusão da demanda!');
				dataini.focus();
				dataini.select();
				return false;		
			}else if (dini[0] < dinc[0] && dini[1] == dinc[1] && dini[2] == dinc[2]){
				alert('O campo Previsão de início do atendimento deve ser maior que a data de inclusão da demanda!');
				dataini.focus();
				dataini.select();
				return false;		
			}
			*/		
		<?}?>

		
		
		if (datafim.value == ''){
			alert('O campo Previsão de término do atendimento deve ser preenchido!');
			datafim.focus();
			datafim.select();
			return false;		
		}
		
		if (horafim.value == ''){
			alert('O campo Previsão de término do atendimento deve ser preenchido!');
			horafim.focus();
			horafim.select();
			return false;
		}
		if (horafim.value.length != 5){
			alert('O campo Previsão de término é inválido!');
			horafim.focus();
			horafim.select();
			return false;
		}
		

		var dini = dataini.value.split('/');
		var dfim = datafim.value.split('/');
		
		var hini = horaini.value.split(':');
		var hfim = horafim.value.split(':');
		
		if (dfim[2] < dini[2]){
			alert('O campo Previsão de término do atendimento deve ser maior que a data de previsão de início!');
			datafim.focus();
			datafim.select();
			return false;			
		}else if (dfim[1] < dini[1] && dfim[2] == dini[2]){
			alert('O campo Previsão de término do atendimento deve ser maior que a data de previsão de início!');
			datafim.focus();
			datafim.select();
			return false;		
		}else if (dfim[0] < dini[0] && dfim[1] == dini[1] && dfim[2] == dini[2]){
			alert('O campo Previsão de término do atendimento deve ser maior que a data de previsão de início!');
			datafim.focus();
			datafim.select();
			return false;		
		}else if (hini[0] > hfim[0] && dfim[0] == dini[0] && dfim[1] == dini[1] && dfim[2] == dini[2]){
			alert('O campo Previsão de término do atendimento deve ser maior que a data de previsão de início!');
			horafim.focus();
			horafim.select();
			return false;		
		}else if (hini[1] > hfim[1] && hini[0] == hfim[0] && dfim[0] == dini[0] && dfim[1] == dini[1] && dfim[2] == dini[2]){
			alert('O campo Previsão de término do atendimento deve ser maior que a data de previsão de início!');
			horafim.focus();
			horafim.select();
			return false;		
		}
		
		//nova data nao pode ultrapassar 2 dias da data previsão de início - celula b - Daniel Brito
		<?if($celid == 2){?>
			var theDate = new Date(dini[2], dini[1], dini[0]);
			var theDate2 = new Date(dini[2], (dini[1]-1), dini[0]);
			var myNewDate = new Date(theDate);
			var myNewDate2 = new Date(theDate2);
			
			if(myNewDate2.getDay() == 5){//sexta
				myNewDate.setDate(myNewDate.getDate() + 3);
			}else{
				myNewDate.setDate(myNewDate.getDate() + 2);
			}
			
			var ndtano = myNewDate.getFullYear()+'';
			var ndtmes = myNewDate.getMonth()+'';
			var ndtdia = myNewDate.getDate()+'';
			
			if(ndtmes.length == 1) ndtmes = "0"+ndtmes;
			if(ndtdia.length == 1) ndtdia = "0"+ndtdia;
			
			var novadata = ndtano + ndtmes + ndtdia;
			var fimdtata = dfim[2] + dfim[1] + dfim[0];
			
			//verifica se data fim é final de semana 
			myNewDate3 = new Date(dfim[2], (dfim[1]-1), dfim[0]);
			if(myNewDate3.getDay() == 6 || myNewDate3.getDay() == 0){
				alert('Célula B - Daniel Brito \n\nAviso: O campo Previsão de término do atendimento não pode ser no final de semana!');
				datafim.focus();
				datafim.select();
				return false;
			}
			
			
			if (fimdtata > novadata){
				alert('Célula B - Daniel Brito \n\nAviso: O campo Previsão de término do atendimento não pode ultrapassar 2 dias da data de previsão de início!');
				datafim.focus();
				datafim.select();
				return false;			
			}
		<?}?>
		
		if (usunome.value == '' || usucpf.value == ''){
			alert('O campo responsável deve ser preenchido!');
			usunome.focus();
			usunome.select();
			return false;			
		}
		if (classif.value == ''){
			alert('O campo classificação da demanda deve ser preenchido!');
			classif.focus();
			return false;					
		}
		<?
		if ($sidid):
		?>
		if (classis.value == ''){
			alert('O campo tipo da demanda para sistemas de informação deve ser preenchido!');
			classis.focus();
			return false;					
		}
		<?
		endif;
		?>
		
		<?
			if($gerenteFW == 'S'){
				echo '
				 	  document.formulario.dmdtempoadicional.disabled=false;
					  document.formulario.dmddatainiprevatendimento.disabled=false;
					  document.formulario.hiniatendimento.disabled=false;
					  document.formulario.dmddatafimprevatendimento.disabled=false;
					  document.formulario.hfimatendimento.disabled=false;
					 ';
			}
		?>
		
		
		var dmdid = "<?=$dmdid?>";
		//Faz uma requisição ajax, passando o parametro 'dmdid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/atendimento&acao=A', {
							        method:     'post',
							        parameters: '&dmdidAjax=' + dmdid,
							        onComplete: function (res)
							        {	
										//var result = res.responseText.replace(/\s+/g,"");
										var result = res.responseText;
										if(result != ""){
											if(confirm('Esta demanda já foi atribuída para o técnico: '+res.responseText+' \n\nDeseja atribuir para técnico de sua escolha?')){
												d.formulario.submit();
											}
										}
										else{
											d.formulario.submit();
										}
							        }
							  });			
		
		
		
		
	}
	
	function calculaTermino2(){
	
		if (document.formulario.priid.value == ''){
			alert('Selecione o campo Prioridade para calcular a data de previsão de término !');
			document.formulario.priid.focus();
			return;
		}

		if(document.getElementById('dmddatainiprevatendimento').value == "" || document.getElementById('hiniatendimento').value == ""){
			document.getElementById('dmddatainiprevatendimento').value = "";
			document.getElementById('hiniatendimento').value = "";
		}
		
		var tempoadd = document.formulario.dmdtempoadicional.value
		if (document.formulario.dmdtempoadicional.value == '') tempoadd = "00:00";
		
		//Faz uma requisição ajax, passando o parametro 'dmdid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/atendimento&acao=A', {
							        method:     'post',
							        parameters: '&priidAjax=' + document.formulario.priid.value + '&tempoadicional=' + tempoadd + '&dtini='+ document.getElementById('dmddatainiprevatendimento').value + ' ' + document.getElementById('hiniatendimento').value,
							        onComplete: function (res)
							        {	
	        							//var result = res.responseText.replace(/\s+/g,"");
	        							var result = res.responseText;
										if(result == ""){
											alert('Não existe tempo cadastrado para esta prioridade no catálago de serviço!');
											return;
										}
										else{
											var retorno = result;
											var nretorno = retorno.split('|');										
											document.getElementById('dmddatainiprevatendimento').value = nretorno[0];
											document.getElementById('hiniatendimento').value = nretorno[1];
											document.getElementById('dmddatafimprevatendimento').value = nretorno[2];
											document.getElementById('hfimatendimento').value = nretorno[3];										
										}
							        }
							  });			
		
	}
	
	/**
	 * Função que verifica se o número de horas é maior que 18 e 
	 * soma o número de dias
	 * @author Fernando Araújo Bagno da Silva
	 * @since 02/01/2009
	 */
	function calcularDias( horas ){
		if ( horas > 18 ){
			return 1 + calcularDias( horas - 18 );
		}else{
			return 0;
		}
	}
	
	/**
	 * Função que calcula a previsão de termino de uma demanda
	 * @author Fernando Araújo Bagno da Silva
	 * @since 02/01/2009
	 */
	function calculaTermino(){
		
		// Pega os campos da tela
		var dtinicio  = document.getElementById('dmddatainiprevatendimento');
		var dttermino = document.getElementById('dmddatafimprevatendimento');
		var hrinicio  = document.getElementById('hiniatendimento');
		var hrfim     = document.getElementById('hfimatendimento');
		
		// Pega o valor da severidade (caso haja um cadastrado)
		var previsao = '<?php echo $db->pegaUm(" SELECT 
													 tistempo 
												  FROM  
													 demandas.tiposervicoseveridade ts
												  INNER JOIN
													 demandas.demanda d ON ts.tipid = d.tipid
												  WHERE
													 dmdid = {$_SESSION["dmdid"]} AND
													 dmdstatus = 'A' AND
													 tisstatus = 'A' ") ?>';
		
		// Pega a data atual
		var dtatual = '<?php echo date('d/m/Y'); ?>';
		var hratual = '<?php echo date('H:i'); ?>';
		var semana  = '<?php echo date('D'); ?>';
		
		// Se a data e a hora de início não estiverem informados, pega a atual
		if (!dtinicio.value){
			dtinicio.value = dtatual;
		}
		if(!hrinicio.value){
			hrinicio.value = hratual;
		}
		
		if ( dtinicio.value > dtatual ){
			
			if (hrinicio.value == ''){
				hrinicio.value = '08:00';
			}
			
		}else{
			
			<?if(!$sidid){?>
			// Valida se a hora informada é menor que a atual
			if ( hrinicio.value < hratual ){
				alert('A Hora de Início não pode ser menor do que a hora atual!');
				hrinicio.value = "";
				hrinicio.focus();
				return false;
			}
			<?}?>
		
		}
		
		// Valida se tem ou não severidade cadastrada para o serviço da demanda
		if(!previsao){
			alert('Não existe severidade cadastrada para este serviço!');
		}else{
			
			// Pega hora e minuto do início da demanda
			var inicioh  = hrinicio.value.substr(0,2); 
			var iniciom  = hrinicio.value.substr(3,2);
			
			// Pega hora e minuto da severidade da demanda			
			var previsaoh = previsao.substr(0,2); 
			var previsaom = previsao.substr(3,2);
			
			// Realiza a soma
			var minutos = Number(previsaom) + Number(iniciom);
			var horas   = Number(inicioh) + Number(previsaoh);
			
			// Verifica o dia do mês 
			var dia_mes = 30;
			var mes     = dtinicio.value.substr(3,2);
	
			// Se o mês tiver dia 31, o sistema altera
			switch( mes ){
				case 1:  dia_mes = 31;
				case 2:	 dia_mes = 28;
				case 3:	 dia_mes = 31;
				case 5:  dia_mes = 31;
				case 7:	 dia_mes = 31;
				case 8:	 dia_mes = 31;
				case 10: dia_mes = 31;
				case 12: dia_mes = 31;
			}
			
			// Valida se os minutos passaram de 60
			if ( minutos > 60 ){
				minutos = minutos - 60
				horas   = horas + 1;
			}
			
			// Valida se as horas passaram de 24 (recursividade - ver função)
			var somarDias = calcularDias( horas );
			
			if ( horas > 18 ){
				horas = horas - 18;
			}
			
			// Se for 12 ou 13 horas, é almoço. Adiciona mais horas (1 ou 2)
			switch( horas ){
				case 12: horas = horas + 2;
				case 13: horas = horas + 1;
			}
			
			// Se mudar o dia, soma 8 horas (início do expediente)
			if (somarDias > 0){
				horas = horas + 8;
			}
			
			// Realiza a soma do dia, caso seja necessário		
			var datafim =  parseFloat(dtinicio.value.substr(0,2)) + somarDias;
			
			// Valida se o dia passou de 31
			if ( datafim > 31 ){
				datafim = datafim - 31;
				mes++;
			}
			
			// Valida se o mês passou de 12
			if ( mes > 12 ){
				mes = mes - 12;
			}
			
			// Adiciona uma casa decimal ao dia e mês
			if ( mes.toString().length < 2 ) {
				mes = '0' + mes;
			}
			
			if ( datafim.toString().length < 2 ) {
				datafim = '0' + datafim;
			}
			
			// Adiciona uma casa decimal à hora e minuto
			if (horas.toString().length < 2){
				horas = '0' + horas;
			}
			
			if (minutos.toString().length < 2){
				minutos = '0' + minutos;
			}
			
			// Atribui os valores aos campos
			dttermino.value = datafim + '/' + mes + dtinicio.value.substr(5);
			hrfim.value = ( horas + ':' +  minutos );
			  
			
			/*
			var um   = new Date();
			
			um.setHours( parseInt(hrinicio.value) ); 
			um.setMinutes( parseInt(hrinicio.value.substr(3,2)) );
			
			var dois = new Date( um.getTime() );
			dois.setHours( ( parseInt( previsao) ) ); 
			dois.setMinutes( parseInt(previsao.substr(3,2)) );
			
			var time  = ( um.getTime() - dois.getTime() );
			var horas = time / 3600000;  
			
			alert(parseInt(horas) + ":" + ((time % 3600000) / 60000));			
			*/
			
		}
		
	}
	
	function tempoAdd(priid)
	{
		
		if(priid){

			if(document.getElementById('dmddatainiprevatendimento').value == "" || document.getElementById('hiniatendimento').value == ""){
				document.getElementById('dmddatainiprevatendimento').value = "";
				document.getElementById('hiniatendimento').value = "";
			}
			
			//Faz uma requisição ajax, passando o parametro 'dmdid', via POST
			var req = new Ajax.Request('demandas.php?modulo=principal/atendimento&acao=A', {
								        method:     'post',
								        parameters: '&priidTempoAjax=' + document.formulario.priid.value,
								        onComplete: function (res)
								        {	
											//var result = res.responseText.replace(/\s+/g,"");
											var result = res.responseText;
											if(result != ""){
												var retorno = result;
												var nretorno = retorno.split('|');										
												document.getElementById('tempocatalogo').value = nretorno[0];
												document.getElementById('tempototal').value = nretorno[1];
											}
								        }
								  });	
		}
		else{
			
			document.getElementById('tempocatalogo').value = "";
			document.getElementById('tempototal').value = "";

			document.getElementById('dmddatainiprevatendimento').value = "";
			document.getElementById('hiniatendimento').value = "";
			document.getElementById('dmddatafimprevatendimento').value = "";
			document.getElementById('hfimatendimento').value = "";
			
			document.formulario.dmdtempoadicional.value = "";
			//document.getElementById('dmdtempoadicional').value = "";
		}

		mostraJustPri();
		mostraJust();
		calculaTermino2();			
						  
	}
	
	function mostraJust(){
		tr     = d.getElementById('tr_obs');
		if(document.getElementById('tempoadicional').value)
		{
			tr.style.display = '';
		}
		else{
			tr.style.display = 'none';
		}
	}

	function mostraJustPri(){
		tr = d.getElementById('tr_justprioridade');
		if((document.formulario.priid.value != document.formulario.setaprioridade.value) && document.formulario.priid.value != '')
		{
			tr.style.display = '';
		}
		else{
			tr.style.display = 'none';
		}
	}	
	
	function salvarRegHoras(){
		jQuery.noConflict();
		if(validarRegHora()){
			jQuery.post("demandas.php?modulo=principal/atendimento&acao=A",{registrar:jQuery("#registrar").val(),usucpfexecutor:jQuery("#usucpfexecutor").val(),dmddatahoraatendimento:jQuery("#dmddatahoraatendimento").val(),hiniciohoratendimento:jQuery("#hiniciohoratendimento").val(),hfimhoratendimento:jQuery("#hfimhoratendimento").val()},
				function(data){
				jQuery("#divRegistroHora").html(data);
				jQuery("#dmddatahoraatendimento").val('');	
				jQuery("#hiniciohoratendimento").val('');		
				jQuery("#hfimhoratendimento").val('');	
			}); 
		} 
	}	

	function excluirRegHoras(reghoras){
		jQuery.noConflict();
		if(confirm('Deseja excluir os dados?')) {
			jQuery.post("demandas.php?modulo=principal/atendimento&acao=A",{excluir:'excluir',reghoras:reghoras},
			function(data){
				jQuery("#divRegistroHora").html(data);
			});  
		}
	}
	
	function validarRegHora(){
		hrs_ini = document.getElementById('hiniciohoratendimento').value.substr(0,2);
		min_ini = document.getElementById('hiniciohoratendimento').value.substr(3,5);
		hrs_fim = document.getElementById('hfimhoratendimento').value.substr(0,2);
		min_fim = document.getElementById('hfimhoratendimento').value.substr(3,5);

		if(jQuery("#hiniciohoratendimento").val().length < 5){
			jQuery("#hiniciohoratendimento").focus();	
			alert("Formato Incorreto.");
			return false;
		}
		
		if(jQuery("#hfimhoratendimento").val().length < 5){
			jQuery("#hfimhoratendimento").focus();	
			alert("Formato Incorreto.");
			return false;	
		}		
		
		if(jQuery("#dmddatahoraatendimento").val() == ""){
			alert("Informe a data.");
			jQuery("#dmddatahoraatendimento").focus();
			return false;	
		} 		
		
		if(jQuery("#hiniciohoratendimento").val() == ""){
			alert("Informe a hora de ínicio.");
			jQuery("#hiniciohoratendimento").focus();
			return false;	
		} else {
			if ((hrs_ini < 00 ) || (hrs_ini > 23) || ( min_ini < 00) ||( min_ini > 59)){ 
				alert("Hora de ínicio inválida.");
				jQuery("#hiniciohoratendimento").focus();
				return false;	
			} 
		}
		
		if(jQuery("#hfimhoratendimento").val() == ""){
			alert("Informe a hora de fim.");	
			jQuery("#hfimhoratendimento").focus();		
			return false;	
		} else {
			if ((hrs_fim < 00 ) || (hrs_fim > 23) || ( min_fim < 00) ||( min_fim > 59)){ 
				alert("Hora de fim inválida.");
				jQuery("#hfimhoratendimento").focus();	
				return false;	
			} 
		}
		
		if((hrs_ini >= hrs_fim) && (min_ini > min_fim)){
			alert("Hora de Ínicio maior que Hora Fim.");
			jQuery("#hfimhoratendimento").focus();	
			return false;	
		}
		return true;
	}
		
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?=cabecalhoDemanda(); ?>

<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
<input type="hidden" name="dmddatainiprevatendimento_old" id="dmddatainiprevatendimento_old" value="<?=$dmddatainiprevatendimento2; ?>">
<input type="hidden" name="dmddatafimprevatendimento_old" id="dmddatafimprevatendimento_old" value="<?=$dmddatafimprevatendimento; ?>">
<input type="hidden" name="priid_old" id="priid_old" value="<?=$priid; ?>">
<input type="hidden" name="dmdjustprioridade_old" id="dmdjustprioridade_old" value="<?=$dmdjustprioridade; ?>">
<input type="hidden" name="dmdtempoadicional_old" id="dmdtempoadicional_old" value="<?=$dmdtempoadicional; ?>">
<input type="hidden" name="dmdobstempoadicional_old" id="dmdobstempoadicional_old" value="<?=$dmdobstempoadicional; ?>">
<input type="hidden" name="usunomeexecutor_old" id="usunomeexecutor_old" value="<?=$usunomeexecutor; ?>">
<input type="hidden" name="dmdclassificacao_old" id="dmdclassificacao_old" value="<?=$dmdclassificacao; ?>">
<input type="hidden" name="dmdclassificacaosistema_old" id="dmdclassificacaosistema_old" value="<?=$dmdclassificacaosistema; ?>">
<input type="hidden" name="setaprioridade" id="setaprioridade" value="">

<table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="subtitulodireita">Prioridade da demanda:</td>
		<td>
			<div style="float:left;">
				<?php 
					
					$sql = "select distinct p.priid AS codigo, p.pridsc AS descricao
							from demandas.demanda d
							inner join demandas.tiposervicoprioridade AS t on t.tipid = d.tipid
							inner join demandas.prioridade AS p ON t.priid = p.priid 
							where d.dmdid = $dmdid
							and t.tsppontuacao <> 0
							and p.pristatus = 'A'
							order by p.priid";
					$priidcarga = $db->carregar($sql);	
					
					if(!$priidcarga){

						$sql = "SELECT p.priid AS codigo,	 p.pridsc AS descricao
								FROM demandas.prioridade p
								WHERE p.pristatus = 'A'
								ORDER BY p.priid";
						$priidcarga = $db->carregar($sql);	
						
					}
				?>
				<select  name='priid'  class='CampoEstilo'  style='width: auto' onchange="tempoAdd(this.value)" onkeyup="tempoAdd(this.value)">
					<option value="">-- Informe a prioridade --</option>
					<?php foreach($priidcarga as $priidcarga2){?>
						<option value="<?=$priidcarga2['codigo']?>" <?if($priidcarga2['codigo'] == $priid) echo "selected"?>><?=$priidcarga2['descricao']?></option>
					<?php }?>
				</select>
			<?php
			/*
			$sql = "SELECT 
					 priid AS codigo,
					 pridsc AS descricao
					FROM 
					 demandas.prioridade
					WHERE
					 pristatus = 'A' 
					ORDER BY 
					 priid";
			$db->monta_combo( 'priid', $sql, $habil, '-- Informe a prioridade --', 'tempoAdd', '' );
			*/
			echo obrigatorio(); 
			?>
			</div>
			
			<?php // if ($sidid){?>
				<!-- 
				<div style="float:left;align:bottom">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="button" name="bt_calcula_previsao" value="Calcular Previsão de Termino" onclick="calculaTermino2();" style="cursor:pointer;" <?if($habil=='N') echo "disabled"; ?>>
				</div>
				 -->
			<?php // }?>
			
		</td>
		<td rowspan="<?=$sidid ? 11 : 10 ?>" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
			<?php 
				if($mostraWorkflow == 'S'){
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );	
					//Barra outras opções
					criarNovaDemanda(); //www/demandas/_funcoes.php
				}else{
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>estado atual</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<?php 
												if($esdid){
													$sqlesd = "SELECT esddsc
															FROM workflow.estadodocumento
															WHERE esdid = $esdid";
													
													echo $db->pegaUm($sqlesd);
												}
												else{
													echo "Em Processamento";
												}
												?>					
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>ações</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												você não possui permissão
											</span>
										</td>
									</tr>
						</table>					
					<? 
				}
			?>
		</td>		
	</tr>
	<?
		$mostrajustprioridade = 'none';
		if($dmdjustprioridade) $mostrajustprioridade = '';
	?>
	<tr id="tr_justprioridade" style="display:<?=$mostrajustprioridade?>"> 
		<td class="subtitulodireita" valign="top">Justificativa da Prioridade:</td>
		<td valign="top">
			<?= campo_textarea( 'dmdjustprioridade', 'S', $habil, '', 63, 6, 300 ); ?>
		</td>
	</tr>
	
	<tr>
		<td class="subtitulodireita">
			Tempo catálogo 
			<?if($ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21) 
				echo '(hhh:mm)';
			  else 
			  	echo '(hh:mm)';
			?>:
		</td>
		<td>
			<?php 
				if(!$tempocatalogo) $tempocatalogo = "00:00";
				if(!$tempototal) $tempototal = "00:00";
			?>
			<?= campo_texto('tempocatalogo','N', 'N','',5,5,'##:##','','left','',0, 'id="tempocatalogo"') ?>
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			Qtd. demanda:
			<?= campo_texto('dmdqtde','N', 'N','',5,5,'####','','left','',0, 'id="dmdqtde"') ?>
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			Tempo Total 
			<?if($ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21) 
				echo '(hhh:mm)';
			  else 
			  	echo '(hh:mm)';
			?>:
			<?= campo_texto('tempototal','N', 'N','',5,5,'##:##','','left','',0, 'id="tempototal"') ?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tempo Adicional (hh:mm):</td>
		<td>
			<?= campo_texto('dmdtempoadicional','N', $habil,'',5,5,'##:##','','left','',0, 'id="tempoadicional"','','','mostraJust();calculaTermino2();') ?>
		</td>
	</tr>
	<?
		$mostrajustadd = 'none';
		if($dmdtempoadicional) $mostrajustadd = '';
	?>
	<tr id="tr_obs" style="display:<?=$mostrajustadd?>">
		<td class="subtitulodireita" valign="top">Justificativa do Tempo Adicional:</td>
		<td valign="top">
			<?= campo_textarea( 'dmdobstempoadicional', 'S', $habil, '', 63, 6, 300 ); ?>
		</td>
	</tr>
	<tr >
		<td class="subtitulodireita">Previsão de início do atendimento:</td>
		<td>
			<?//= campo_data( 'dmddatainiprevatendimento', 'S', $habil, '', 'S' ); ?>
			<?= campo_texto('dmddatainiprevatendimento','N', $habil,'',11,10,'##/##/####','','right','',0, 'id="dmddatainiprevatendimento"','','','mostraJust();calculaTermino2();') ?>
			<a href="javascript:show_calendar('formulario.dmddatainiprevatendimento');"><img src="../imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt=""></a>
			<?php echo obrigatorio();?>
			&nbsp;
			Hora:<?= campo_texto('hiniatendimento','S', $habil,'',5,5,'##:##','','left','',0, 'id="hiniatendimento"','','','mostraJust();calculaTermino2();') ?>
			<input type="hidden" name="dmddatainclusao" id="dmddatainclusao" value="<?=$dmddatainclusao; ?>">
		</td>
		
	</tr>
	<?
		$ordid = $db->pegaUm("SELECT 
						 		tp.ordid
							FROM 
								demandas.demanda AS d
							LEFT JOIN 
								demandas.tiposervico AS tp ON tp.tipid = d.tipid
							WHERE 
								dmdid = ".$dmdid." limit 1");
		
		//if(!$dmdclassificacao && $ordid != 1) $dmdclassificacao = "N";
		
		$habil2 = "N";
		if ($sidid || $ordid == '15' || $ordid == '16') $habil2 = "S";
	?>
	<tr>
		<td class="subtitulodireita">Previsão de término do atendimento:</td>
		<td>
			<?= campo_data( 'dmddatafimprevatendimento', 'S', $habil2, '', 'S' ); ?>&nbsp;&nbsp;
			Hora:<?= campo_texto('hfimatendimento','S', $habil2,'',5,5,'##:##','','left','',0,'id="hfimatendimento"') ?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Responsável:</td>
		<td>
			<?= campo_texto( 'usunomeexecutor', 'S', 'S', '', 60, 250, '', '', 'left', '', 0, 'id="usunomeexecutor" autocomplete="off"'); ?>
			<a href="#"><img align="absmiddle" border=0 onclick="popupPesqUser();" src="/imagens/pop_p.gif" title="abrir lista"/></a>
			<input type="hidden" name="usucpfexecutor" id="usucpfexecutor" value="<?=$usucpfexecutor?>">
			<br>
			<div id="divSuggestPessoas" style="position: absolute; z-index: 1000;"></div>
			<script>
				objSuggestPessoas = new window.Suggest( 
					document.getElementById( "usunomeexecutor" ) ,
					document.getElementById( "divSuggestPessoas" ),
					"ajax_autocomplete.php?dmdid=<?=$dmdid?>",
					document.getElementById( "usucpfexecutor" )
				);
			</script>
		</td>
	</tr>	
	<tr>
		<td class="subtitulodireita">Classificação da demanda:</td>
		<td>
			<select name="dmdclassificacao" <?if($habil=='N') echo "disabled"; ?>>
				<option value=""> -- Selecione -- </option>
				<!-- <option <?//=$dmdclassificacao == 'N' ? "selected='selected'" : ''; ?> value="N">Nenhuma</option>  -->
				<option <?=$dmdclassificacao == 'I' ? "selected='selected'" : ''; ?> value="I">Incidente</option>
				<option <?=$dmdclassificacao == 'P' ? "selected='selected'" : ''; ?> value="P">Resolução de problema</option>
				<option <?=$dmdclassificacao == 'M' ? "selected='selected'" : ''; ?> value="M">Requisição de mudança</option>
				<option <?=$dmdclassificacao == 'S' ? "selected='selected'" : ''; ?> value="S">Solicitação de Serviço</option>
			</select>
			<?=obrigatorio(); ?>
		</td>
	</tr>
	<?php if ($sidid): ?>
	<tr>
		<td class="SubTituloDireita">Tipo da demanda para Sistemas de Informação:</td>
		<td>
			<select name="dmdclassificacaosistema" <?if($habil=='N') echo "disabled"; ?>>
				<option value=""> -- Selecione -- </option>
				<option <?=$dmdclassificacaosistema == '1' ? "selected='selected'" : ''; ?> value="1">Inicial</option>
				<option <?=$dmdclassificacaosistema == '2' ? "selected='selected'" : ''; ?> value="2">Consultiva</option>
				<option <?=$dmdclassificacaosistema == '3' ? "selected='selected'" : ''; ?> value="3">Investigativa</option>
				<option <?=$dmdclassificacaosistema == '4' ? "selected='selected'" : ''; ?> value="4">Manutenção corretiva</option>
				<option <?=$dmdclassificacaosistema == '5' ? "selected='selected'" : ''; ?> value="5">Manutenção evolutiva</option>				
			</select>
			<?=obrigatorio(); ?>			
		</td>
	</tr>		
	<?php endif; ?>
	<?php 
		session_start();
		$_SESSION['esdid'] = $esdid;
		if($ordid == 18 || $ordid == 19 || $ordid == 20 || $ordid == 21){ 
			
			if($_SESSION['esdid'] == 108){
				$habilitar = 'S';
			} else {
				$habilitar = 'N';
			}  
			
			//verifica pausa da demanda
			$sql = "select count(pdmid)
					from demandas.pausademanda  
					where pdmstatus = 'A' 
					and pdmdatafimpausa is null
					and dmdid = $dmdid";
			$vPausa = $db->pegaUm($sql);
			if($vPausa > 0){
				$habilitar = 'N';
			}
			
			if($habil == 'N') $habilitar = $habil;
	?>							
	<tr>
		<td class="SubTituloDireita">Registro de Horas de Atendimento</td>
		<td>
			<table border="0" cellpadding="0" cellspacing="2" width="100%">
				<?php if($_SESSION['esdid'] == 108){ ?>
				<tr>
					<td id="divMostrarRegistroHora">
						Data: <?= campo_data('dmddatahoraatendimento','S',$habilitar,'','S','','',$POST['dmddatahoraatendimento']); ?>&nbsp;&nbsp;
						Hora Ínicio:<?= campo_texto('hiniciohoratendimento','S',$habilitar,'',5,5,'##:##','','left','',0,'id="hiniciohoratendimento"','',$_POST['hiniciohoratendimento']); ?>
						Hora Fim:<?= campo_texto('hfimhoratendimento','S',$habilitar,'',5,5,'##:##','','left','',0,'id="hfimhoratendimento"','',$POST['hfimhoratendimento']); ?>&nbsp;&nbsp;
						<?if($vPausa > 0){?>					
							<input id="registrar" type="button" class="botao" value="Registrar" name="registrar" onclick="alert('Não é possível registrar, pois a demanda está em pausa.');" <?if($habilitar == 'N') echo "disabled"; ?>>
						<?}else{?>
							<input id="registrar" type="button" class="botao" value="Registrar" name="registrar" onclick="salvarRegHoras();" <?if($habilitar == 'N') echo "disabled"; ?>>	
						<?}?>
					</td>
				</tr>
				<?php } ?>
				<tr>
					<td height="15"></td>
				</tr>
				<tr>
					<td id="divRegistroHora"><?php exibirRegHoras(); ?></td>			
				</tr>
			</table>
		</td>
	</tr>
	<?php } ?>
					
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='button' class='botao' value='Salvar' name='cadastrar' onclick="validaFormx()" <?if($habil=='N') echo "disabled"; ?>>&nbsp;
	    	<input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();' />	    	
		</td>			
	</tr>				
	<tr>
		<td colspan="3" height="60">&nbsp;</td>
	</tr>	
</table>
</form>
</body>
</html>

<?php
/************************
 SETA PRIORIDADE
 ************************/
$setaPrioridade = "1"; //prioridade baixa

//verifica se o demandante possui cargo de DAS, se a sala fica no gabinete ou se a demanda é urgente
$sql = "select u.ccoid as cargo, u.usdgabinete as gabinete, d.dmdatendurgente as urgente  
		from demandas.usuariodetalhes u
		inner join demandas.demanda d on d.usucpfdemandante = u.usucpf  
		where d.dmdid = $dmdid";
$usuarioDetalhes = $db->pegaLinha($sql);

//urgente
if($usuarioDetalhes['urgente'] == 't') $setaPrioridade = "2"; //prioridade média

//gabinete
if($usuarioDetalhes['gabinete'] == 't') $setaPrioridade = "3"; //prioridade alta

//cargo
if($usuarioDetalhes['cargo'] == 3) $setaPrioridade = "2"; //prioridade média
if($usuarioDetalhes['cargo'] == 4
   || $usuarioDetalhes['cargo'] == 5
   || $usuarioDetalhes['cargo'] == 6) $setaPrioridade = "3"; //prioridade alta

   
//verifica se existe prioridade
$existePrioridade = false;   
if($priidcarga){
	foreach($priidcarga as $priidcarga2){
		if($priidcarga2['codigo'] == $setaPrioridade){
			$existePrioridade = true;
			break;
		}
	}
	if(!$existePrioridade){
		$setaPrioridade = $priidcarga[0]['codigo'];
	}
}



if(!$priid){   
	echo "<script>
					document.formulario.setaprioridade.value=".$setaPrioridade.";
				  	document.formulario.priid.value=".$setaPrioridade.";
				  	document.formulario.priid_old.value=".$setaPrioridade.";
					tempoAdd(".$setaPrioridade.");
		 </script>";
}else{
	echo "<script>
					document.formulario.setaprioridade.value=".$setaPrioridade.";
		 </script>";
}

/***********************************
Exibe Registro Horas
***********************************/

function exibirRegHoras(){
	global $db;
	if($_SESSION['esdid'] == 108){
		$cabecalho = array('Atualiza&ccedil;&otilde;es','Respons&aacutevel','Data', 'Hora &Iacute;nicio', 'Hora Fim', 'Qtd. Horas');
		$campo = "'<center><img src=\"/imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirRegHoras(' || r.idregistrohoras || ');\"></center>' as acao,";
	} else {
		$cabecalho = array('Respons&aacutevel','Data', 'Hora &Iacute;nicio', 'Hora Fim', 'Qtd. Horas');
		$campo = "";
	} 
	
	$sql_lista = "SELECT 
							$campo
							u.usunome,
							to_char(r.data, 'DD/MM/YYYY') as data,
							to_char(r.horainicio,'HH24:MI') as horainicio,
							to_char(r.horafim,'HH24:MI') as horafim,
							to_char(r.qtdhoras,'HH24:MI') as qtdhoras
					FROM 
							demandas.registroshorasdeatendimento r
								INNER JOIN 
							seguranca.usuario u
								ON r.cpftecnicoresponsavel = u.usucpf
					WHERE 
							r.dmdid = {$_SESSION['dmdid']}
					ORDER BY 
							r.data, r.horainicio"; 
	$db->monta_lista_simples( $sql_lista, $cabecalho, 50, 10, 'N');	

	$sql_horas = "SELECT 
							to_char(sum(justify_hours(cast(to_char(qtdhoras,'HH24:MI') as interval))),'HH24:MI') as qtdhoras 
				  FROM 
							demandas.registroshorasdeatendimento r
								INNER JOIN 
							seguranca.usuario u
								ON r.cpftecnicoresponsavel = u.usucpf
				  WHERE 
				  			r.dmdid = {$_SESSION['dmdid']}";
	$horas = $db->pegaum($sql_horas);	
	if(!empty($horas)){		
	echo "<div style='width:120px; align:right; text-color #000; font-size:12px; font-weight: bold; padding-top:5px; padding-left:850px;'>Total Horas: $horas</div>";
	}
}

/***********************************
Salva dados da demanda
***********************************/
function salva(){
	global $db;
	
	$dmdid = $_SESSION['dmdid'];
	$sidid = pegaSidid($dmdid);
	$celid = $db->pegaUm("select celid from demandas.demanda where dmdid=".$dmdid);
	
	if (!$dmdid){
		die('<script>
				alert(\'Falta parametros!\nTente novamente.\');
				location.href = \'?modulo=inicio&acao=C\';
			 </script>');
	}
	
	/*
	$dmddatainclusao = $db->pegaUm("SELECT to_char(dmddatainclusao, 'HH24:MI' ) 
								    FROM demandas.demanda WHERE dmdid = ".$dmdid);

	$hiniatendimento = str_replace(":", "", $_POST['hiniatendimento']);
	$dmddatainclusao = str_replace(":", "", $dmddatainclusao);
	*/

	// faz a validação menos para sistemas
	if(!$sidid){
		$dmddatainclusao2 = $db->pegaUm("SELECT dmddatainclusao 
									    FROM demandas.demanda WHERE dmdid = ".$dmdid);	
		
	
		//data_1
		$hora1 = substr($dmddatainclusao2, 11, 2);	
		$min1 = substr($dmddatainclusao2, 14, 2);
		$mes1 = substr($dmddatainclusao2, 5, 2);
		$dia1 = substr($dmddatainclusao2, 8, 2);
		$ano1 = substr($dmddatainclusao2, 0, 4);
		if(!is_numeric($hora1)) $hora1 = "00";
		if(!is_numeric($min1)) $min1 = "00";
		
		//data_2
		$hora = substr($_POST['hiniatendimento'], 0, 2);	
		$min = substr($_POST['hiniatendimento'], 3, 2);
		$mes = substr($_POST['dmddatainiprevatendimento'], 3, 2);
		$dia = substr($_POST['dmddatainiprevatendimento'], 0, 2);
		$ano = substr($_POST['dmddatainiprevatendimento'], 6, 4);
		
		//data_3
		$hora2 = substr($_POST['hfimatendimento'], 0, 2);	
		$min2 = substr($_POST['hfimatendimento'], 3, 2);
		$mes2 = substr($_POST['dmddatafimprevatendimento'], 3, 2);
		$dia2 = substr($_POST['dmddatafimprevatendimento'], 0, 2);
		$ano2 = substr($_POST['dmddatafimprevatendimento'], 6, 4);
		
		$data_1 = mktime($hora1, $min1, 0, $mes1, $dia1, $ano1);
		$data_2 = mktime($hora, $min, 0, $mes, $dia, $ano);
		$data_3 = mktime($hora2, $min2, 0, $mes2, $dia2, $ano2);
		
		
		
		if($data_1 && !@checkdate($mes1,$dia1,$ano1)){
			die('<script>
					alert(\'A data de início de atendimento é inválida!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($hora && $min && ($hora > 23 || $hora < 00) || ($min > 59 || $min < 00)){
			die('<script>
					alert(\'O horário de início de atendimento é inválido!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($hora1 && $min1 && ($hora1 > 23 || $hora1 < 00) || ($min1 > 59 || $min1 < 00)){
			die('<script>
					alert(\'O horário de início de atendimento é inválido!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($hora2 && $min2 && ($hora2 > 23 || $hora2 < 00) || ($min2 > 59 || $min2 < 00)){
			die('<script>
					alert(\'O horário de término de atendimento é inválido!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($_POST['dmddatainiprevatendimento'] && !@checkdate($mes,$dia,$ano)){
			die('<script>
					alert(\'A data de início de atendimento é inválida!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($data_2 && !@checkdate($mes2,$dia2,$ano2)){
			die('<script>
					alert(\'A data de término de atendimento é inválida!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		//if( (integer) $hiniatendimento < (integer) $dmddatainclusao ){
		
		if( (integer) $data_1 > (integer) $data_2 ){
			die('<script>
					alert(\'A Previsão de início de atendimento não pode ser menor que a data de abertura da demanda!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
	}
		
		//dataini
		$hora_ini = substr($_POST['hiniatendimento'], 0, 2);	
		$min_ini = substr($_POST['hiniatendimento'], 3, 2);
		$mes_ini = substr($_POST['dmddatainiprevatendimento'], 3, 2);
		$dia_ini = substr($_POST['dmddatainiprevatendimento'], 0, 2);
		$ano_ini = substr($_POST['dmddatainiprevatendimento'], 6, 4);
		
		//datafim
		$hora_fim = substr($_POST['hfimatendimento'], 0, 2);	
		$min_fim = substr($_POST['hfimatendimento'], 3, 2);
		$mes_fim = substr($_POST['dmddatafimprevatendimento'], 3, 2);
		$dia_fim = substr($_POST['dmddatafimprevatendimento'], 0, 2);
		$ano_fim = substr($_POST['dmddatafimprevatendimento'], 6, 4);
		
		$data_ini = mktime($hora_ini, $min_ini, 0, $mes_ini, $dia_ini, $ano_ini);
		$data_fim = mktime($hora_fim, $min_fim, 0, $mes_fim, $dia_fim, $ano_fim);
		
		if($data_ini && !@checkdate($mes_ini,$dia_ini,$ano_ini)){
			die('<script>
					alert(\'A data de início de atendimento é inválida!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($hora_ini && $min_ini && ($hora_ini > 23 || $hora_ini < 00) || ($min_ini > 59 || $min_ini < 00)){
			die('<script>
					alert(\'O horário de início de atendimento é inválido!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		if($data_fim && !@checkdate($mes_fim,$dia_fim,$ano_fim)){
			die('<script>
					alert(\'A data de término de atendimento é inválida!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}

		if($hora_fim && $min_fim && ($hora_fim > 23 || $hora_fim < 00) || ($min_fim > 59 || $min_fim < 00)){
			die('<script>
					alert(\'O horário de término de atendimento é inválido!\');
					location.href = \'?modulo=principal/atendimento&acao=A\';
				 </script>');
		}
		
		//hora adicional
		//if(!$_POST['dmdtempoadicional']) $_POST['dmdtempoadicional'] = "00:00";
		if($_POST['dmdtempoadicional']){

			if(strlen($_POST['dmdtempoadicional']) != 5){
				die('<script>
						alert(\'O Tempo Adicional (hh:mm) é inválido!\');
						location.href = \'?modulo=principal/atendimento&acao=A\';
					 </script>');
			}
		
			$hora_tadd = substr($_POST['dmdtempoadicional'], 0, 2);	
			$min_tadd = substr($_POST['dmdtempoadicional'], 3, 2);
			
			if($hora_tadd && $min_tadd && ($hora_tadd > 23 || $hora_tadd < 00) || ($min_tadd > 59 || $min_tadd < 00)){
				die('<script>
						alert(\'O Tempo Adicional (hh:mm) é inválido!\');
						location.href = \'?modulo=principal/atendimento&acao=A\';
					 </script>');
			}
			
		}
		else{
			$_POST['dmdobstempoadicional'] = "";
		}
		
		//justificativa prioridade
		if($_POST['priid'] == $_POST['setaprioridade']) $_POST['dmdjustprioridade'] = "";
		
		if(!$_POST['usucpfexecutor']){
				die('<script>
						alert(\'Preencha o campo Responsável!\');
						location.href = \'?modulo=principal/atendimento&acao=A\';
					 </script>');
		}		
	
	
	$sql = sprintf("UPDATE
			         demandas.demanda
			        SET
			         dmddatainiprevatendimento = '%s',
			         dmddatafimprevatendimento = '%s',
			         dmdclassificacao  	       = '%s',
			         dmdclassificacaosistema   = %s,
			         usucpfexecutor			   = '%s',
			         usucpfanalise			   = '%s',	
			         priid					   = %d,
			         dmdtempoadicional	 	   = %s,
			         dmdobstempoadicional      = %s,
			         dmdjustprioridade 	       = %s
			        WHERE
			         dmdid = %d",
				formata_data_sql($_POST['dmddatainiprevatendimento']).' '.$_POST['hiniatendimento'],
				formata_data_sql($_POST['dmddatafimprevatendimento']).' '.$_POST['hfimatendimento'],
				$_POST['dmdclassificacao'],
				($_POST['dmdclassificacaosistema'] ? "'".$_POST['dmdclassificacaosistema']."'" : 'null'),
				$_POST['usucpfexecutor'],
				$_SESSION['usucpf'],
				$_POST['priid'],
				($_POST['dmdtempoadicional'] ? "'".$_POST['dmdtempoadicional']."'" : 'null'),
				($_POST['dmdobstempoadicional'] ? "'".simec_addslashes($_POST['dmdobstempoadicional'])."'" : 'null'),
				($_POST['dmdjustprioridade'] ? "'".simec_addslashes($_POST['dmdjustprioridade'])."'" : 'null'),
				$dmdid);
				//dbg($sql,1);
	$db->executar($sql);
	
	
	//pega prioridade
	$sql = sprintf("select pridsc
			         from demandas.prioridade
			        WHERE
			         priid = %d",
					$_POST['priid_old']);
	$pridsc_old = $db->PegaUm($sql);
	$sql = sprintf("select pridsc
			         from demandas.prioridade
			        WHERE
			         priid = %d",
					$_POST['priid']);
	$pridsc = $db->PegaUm($sql);
	

	//pega classificação demanda
	if($_POST['dmdclassificacao_old'] == "P") $descclassificacao_old = "Resolução de problema";
	if($_POST['dmdclassificacao_old'] == "M") $descclassificacao_old = "Requisição de mudança";
	if($_POST['dmdclassificacao'] == "P") $descclassificacao = "Resolução de problema";
	if($_POST['dmdclassificacao'] == "M") $descclassificacao = "Requisição de mudança";
	
	//pega classificação demanda sistemas
	if($_POST['dmdclassificacaosistema_old'] == "1") $descclassificacaosistema_old = "Inicial";
	if($_POST['dmdclassificacaosistema_old'] == "2") $descclassificacaosistema_old = "Consultiva";
	if($_POST['dmdclassificacaosistema_old'] == "3") $descclassificacaosistema_old = "Investigativa";
	if($_POST['dmdclassificacaosistema_old'] == "4") $descclassificacaosistema_old = "Manutenção corretiva";
	if($_POST['dmdclassificacaosistema_old'] == "5") $descclassificacaosistema_old = "Manutenção evolutiva";
	if($_POST['dmdclassificacaosistema'] == "1") $descclassificacaosistema = "Inicial";
	if($_POST['dmdclassificacaosistema'] == "2") $descclassificacaosistema = "Consultiva";
	if($_POST['dmdclassificacaosistema'] == "3") $descclassificacaosistema = "Investigativa";
	if($_POST['dmdclassificacaosistema'] == "4") $descclassificacaosistema = "Manutenção corretiva";
	if($_POST['dmdclassificacaosistema'] == "5") $descclassificacaosistema = "Manutenção evolutiva";
	
	//formata datas
	if($_POST['dmddatainiprevatendimento_old'])
	{ 
		$hora1 = substr($_POST['dmddatainiprevatendimento_old'], 11, 2);	
		$min1 = substr($_POST['dmddatainiprevatendimento_old'], 14, 2);
		$mes1 = substr($_POST['dmddatainiprevatendimento_old'], 5, 2);
		$dia1 = substr($_POST['dmddatainiprevatendimento_old'], 8, 2);
		$ano1 = substr($_POST['dmddatainiprevatendimento_old'], 0, 4);	
		$dataini_old = $dia1 ."/". $mes1 ."/". $ano1 ." ". $hora1 .":". $min1;
	}
	if($_POST['dmddatafimprevatendimento_old'])
	{ 
		//data_1
		$hora1 = substr($_POST['dmddatafimprevatendimento_old'], 11, 2);	
		$min1 = substr($_POST['dmddatafimprevatendimento_old'], 14, 2);
		$mes1 = substr($_POST['dmddatafimprevatendimento_old'], 5, 2);
		$dia1 = substr($_POST['dmddatafimprevatendimento_old'], 8, 2);
		$ano1 = substr($_POST['dmddatafimprevatendimento_old'], 0, 4);	
		$datafim_old = $dia1 ."/". $mes1 ."/". $ano1 ." ". $hora1 .":". $min1;
	}
	
		
	$obsdsc = "";
	
	if($dataini_old != $_POST['dmddatainiprevatendimento']." ".$_POST['hiniatendimento'])
	   $obsdsc .= "<br>- Previsão de início do atendimento: [".$dataini_old."] para [".$_POST['dmddatainiprevatendimento']." ".$_POST['hiniatendimento']."]";
	
	if($datafim_old != $_POST['dmddatafimprevatendimento']." ".$_POST['hfimatendimento'])
	   $obsdsc .= "<br>- Previsão de término do atendimento: [".$datafim_old."] para [".$_POST['dmddatafimprevatendimento']." ".$_POST['hfimatendimento']."]";
	   
	if($pridsc_old != $pridsc)
	   $obsdsc .= "<br>- Prioridade da demanda: [{$pridsc_old}] para [{$pridsc}]";

	if($_POST['dmdjustprioridade_old'] != $_POST['dmdjustprioridade'])			   
		$obsdsc .= "<br>- Justificativa da Prioridade: [{$_POST['dmdjustprioridade_old']}] para [{$_POST['dmdjustprioridade']}]";

	if($_POST['dmdtempoadicional_old'] != $_POST['dmdtempoadicional'])			   
		$obsdsc .= "<br>- Tempo Adicional (hh:mm): [{$_POST['dmdtempoadicional_old']}] para [{$_POST['dmdtempoadicional']}]";
		
	if($_POST['dmdobstempoadicional_old'] != $_POST['dmdobstempoadicional'])			   
		$obsdsc .= "<br>- Justificativa do Tempo Adicional: [{$_POST['dmdobstempoadicional_old']}] para [{$_POST['dmdobstempoadicional']}]";
		
	if($_POST['usunomeexecutor_old'] != $_POST['usunomeexecutor'])
	   $obsdsc .= "<br>- Técnico Responsável: [{$_POST['usunomeexecutor_old']}] para [{$_POST['usunomeexecutor']}]";
	   
	if($descclassificacao_old != $descclassificacao)
	   $obsdsc .= "<br>- Classificação da demanda: [{$descclassificacao_old}] para [{$descclassificacao}]";
			    
	if($descclassificacaosistema_old != $descclassificacaosistema)			   
		$obsdsc .= "<br>- Tipo da demanda para Sistemas de Informação: [{$descclassificacaosistema_old}] para [{$descclassificacaosistema}]";

		
		
			 
	if($obsdsc){
		$obsdsc = "O usuário [{$_SESSION['usunome']}] alterou os seguintes campos da tela de atendimendo:" . $obsdsc;
		$sql = "INSERT INTO demandas.observacoes (
					usucpf,
					dmdid,
					obsdsc,
					obsstatus,
					obslog,
					obsdata
				) VALUES (
					'".$_SESSION['usucpf']."', 
					'".$dmdid."',
					'".simec_addslashes($obsdsc)."',
					'A',
					'A',
					now()
				);";
		$db->executar($sql);
	}
		
	// Pega o "estado do documento", vinculado à demanda
	$esdidx = recuperaEstadoDocumento();
	//echo "ok=".$esdidx;
	//envia email
	if( in_array($esdidx, array(DEMANDA_ESTADO_EM_ATENDIMENTO,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO)) ){
		//enviaEmailAltera();
		//if($_POST['dmdobstempoadicional']) enviaEmailTempoAdicional();
	}	

	//somente para celula desenvolvimento web (celid = 9)
	if( in_array($esdidx, array(DEMANDA_GENERICO_ESTADO_EM_ANALISE)) && $celid == 9 ){
		enviaEmailEmAnalise();
	}
	
	//verifica se a demanda está atrasada e envia email para a "Célula B - Daniel Brito"
	if( in_array($esdidx, array(DEMANDA_ESTADO_EM_ATENDIMENTO, DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO, DEMANDA_ESTADO_EM_ANALISE, DEMANDA_GENERICO_ESTADO_EM_ANALISE)) && $celid == 2 ){
		
		if($_POST['dmddatafimprevatendimento_old'])
		{ 
			$mes = substr($_POST['dmddatafimprevatendimento_old'], 5, 2);
			$dia = substr($_POST['dmddatafimprevatendimento_old'], 8, 2);
			$ano = substr($_POST['dmddatafimprevatendimento_old'], 0, 4);	
		}
		
		if( ($datafim_old) &&
			($datafim_old != $_POST['dmddatafimprevatendimento']." ".$_POST['hfimatendimento']) && 
			(int)date("Ymd") >= (int)($ano.$mes.$dia) ){

			//dbg($datafim_old .' != '. $_POST['dmddatafimprevatendimento']);
			//if($datafim_old != $_POST['dmddatafimprevatendimento']." ".$_POST['hfimatendimento'])
	   		//$obsdsc .= "<br>- Previsão de término do atendimento: [".$datafim_old."] para [".$_POST['dmddatafimprevatendimento']." ".$_POST['hfimatendimento']."]";
	  
			/*
			$sql = "SELECT
	                 u1.usunome as analista,  u2.usunome as tecnico
	            FROM
	            	demandas.demanda as d
	            LEFT JOIN
	            	seguranca.usuario u1 ON u1.usucpf = d.usucpfanalise
	            LEFT JOIN
	            	seguranca.usuario u2 ON u1.usucpf = d.usucpfexecutor
	            WHERE
	            	d.dmdstatus = 'A'
	            	AND d.dmdid = ".$dmdid;
	    	$atrasados = $db->PegaLinha( $sql ); 
	    	*/
			enviaEmailAlteraAtrasoCelula();
		}
	
		
	}	
	
	
	$db->commit();
	
	
	die('<script>	
			alert(\'Operação realizada com sucesso!\');
			//location.href = window.location;	
			location.href = "demandas.php?modulo=principal/atendimento&acao=A";
		 </script>');

}

	
if($gerenteFW == 'S'){
	echo '<script>
					document.formulario.dmdtempoadicional.disabled=true;
					document.formulario.dmddatainiprevatendimento.disabled=true;
					document.formulario.hiniatendimento.disabled=true;
					document.formulario.dmddatafimprevatendimento.disabled=true;
					document.formulario.hfimatendimento.disabled=true;
		 </script>';
}
?>