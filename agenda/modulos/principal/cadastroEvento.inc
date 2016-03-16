<?php
/** ATENÇÃO
 * 	&acao=A => CADASTRO
 *  &acao=E => EDIÇÃO
 */

switch ( $_REQUEST['op'] ){
	case 'salvar':
		$evento 		 = new Evento( $_POST['eveid'] );
		$dados 			 = $_POST;
		$dados['usucpf'] = $_SESSION['usucpf'];
		$dados['muncod'] = $dados['muncod'] ? $dados['muncod'] : null;
		$dados['estuf'] = $dados['estuf'] ? $dados['estuf'] : null;
		$dados['evedtinicio'] = $dados['evedtinicio'] ? $dados['evedtinicio'] : null;
		$dados['evedtfinal'] = $dados['evedtfinal'] ? $dados['evedtfinal'] : null;
		$dados['evehsinicio'] = $dados['evehsinicio'] ? $dados['evehsinicio'] : null;
		$dados['evehsfinal'] = $dados['evehsfinal'] ? $dados['evehsfinal'] : null;
		$dados['eveassunto'] = $dados['eveassunto'] ? $dados['eveassunto'] : null;
		
		$eveid = $evento->popularDadosObjeto( $dados )
			   			->salvar();
	
		// Controla PARTICIPANTES do evento	   			
		$participanteEvento = new EventoParticipante();
		$participanteEvento->deletaTodosPorEvento( $eveid );
		
		if ( !empty($_POST['prtid'][0]) ){
			foreach ( $_POST['prtid'] as $prtid ){
				$dados = array(
							   'eveid' => $eveid,
							   'prtid' => $prtid
							  );
				$participanteEvento->clearDados()
								   ->popularDadosObjeto( $dados )
								   ->salvar();	
			}		   
		}	   
		// Controla CONTATOS do evento
		$eventoContato = new EventoContato();
		$eventoContato->deletaTodosPorEvento( $eveid );
		
		if ( !empty($_POST['cevid'][0]) ){
			foreach ( $_POST['cevid'] as $cevid ){
				$dados = array(
							   'eveid' => $eveid,
							   'cevid' => $cevid
							  );
				$eventoContato->clearDados()
							  ->popularDadosObjeto( $dados )
							  ->salvar();	
			}		   
		}	   
		$db->commit();
		
		die('<script>
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/cadastroEvento&acao=E&eveid=' . $eveid . '\';
			 </script>');
//	case 'excluir':
//		if ( $_REQUEST['eveid'] ){
//			$msg 	= 'Operação realizada com sucesso!';
//			$evento = new Evento( $_REQUEST['eveid'] );
//			$evento->popularDadosObjeto( array('evestatus' => 'I') )
//				   ->salvar();
//			$db->commit();
//		}else{
//			$msg = 'Falha na execução da operação!';
//		}	
//			
//		die('<script>
//				alert(\'' . $msg . '\');
//				location.href=\'?modulo=principal/cadastroEvento&acao=A\';
//			 </script>');
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');
		
		$estuf = $_POST['estuf'];
		$sql = "SELECT 
					muncod AS codigo, 
					mundescricao AS descricao 
				FROM
					territorios.municipio
				WHERE
					estuf = '{$estuf}'
				ORDER BY
					mundescricao";
		echo $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 240, 'N', 'muncod');
		exit;		
		break;
		
	case 'entidade':
		header('content-type: text/html; charset=ISO-8859-1');
		$solicitante = new EntidadeAgenda();
		$sql 		 = $solicitante->listaComboPopupSQL( array('sltid' => $_POST['sltid']) );
		
		$filtro = array(
						array("codigo" 		=> "etadsc",
							  "descricao" 	=> "<b>Entidade/Pessoa:</b>",
							  "string" 		=> "1")
						);
		campo_popup('etaid',$sql,'Selecione...','','400x800','40', $filtro);
		?> 
		&nbsp;&nbsp;&nbsp;
		<a href="javascript:janela('?modulo=sistema/apoio/cadastroEntidade&acao=P&sltid=<?php echo $_POST['sltid'] ?>',600,530,'popupcadastro');">[Nova Entidade/Pessoa]</a>
		<?php		
		exit;
		break;
		
}

$agdid 						 = ( $_REQUEST['agdid'] ? $_REQUEST['agdid'] : $_SESSION['agenda']['agdid']);
$_SESSION['agenda']['agdid'] = $agdid;

if ( empty($agdid) ){
	die('<script>
			alert(\'Faltam parametros para acessar a tela.\');
			location.href = \'?modulo=inicio&acao=C\';
		 </script>');
}

$agenda = new Agenda( $agdid );
$dados  = $agenda->getDados();
extract( $dados );	

$disable = 'S';

if ( $_REQUEST['eveid'] || $_REQUEST['acao'] == 'E' ){
	
	$eveid = ( $_REQUEST['eveid'] ? $_REQUEST['eveid'] : $_SESSION['agenda']['eveid']);
	$evento = new Evento( $eveid );
	$dados = $evento->getDados();
	extract( $dados );
	
	$_SESSION['agenda']['eveid'] = $eveid;
	
	$evehsinicio = explode(":",$evehsinicio); 
	array_pop($evehsinicio);
	$evehsinicio = implode(":",$evehsinicio); 
	
	$evehsfinal = explode(":",$evehsfinal); 
	array_pop($evehsfinal);
	$evehsfinal = implode(":",$evehsfinal);

	/*
	 * Valida edição do evento
	 */
	$userResp = new UsuarioResponsabilidade();
	$arAgdid  = $userResp->pegaRespAgenda();
	if ( !verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) && !in_array($agdid, $arAgdid) ){
		$arAevid = $userResp->pegaRespArea();
		$arEveid = $evento->pegaEventoPorArea( $arAevid, $agdid);
		$arEveid = ( $arEveid ? $arEveid : array(0) );

		if ( !in_array($eveid, $arEveid) ){
			die('<script>
					alert(\'O usuário não possui permissão de edição deste evento!\');
					location.href=\'?modulo=principal/listaAgenda&acao=A\';
				 </script>');		
		}
		$disable = 'N';		
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
if ( $_REQUEST['acao'] == 'E' ){
	$arAba = abaEdicaoEvento();
	echo montarAbasArray($arAba, "?modulo=principal/cadastroEvento&acao=E");
}elseif ( $_REQUEST['acao'] == 'A' ){
	$arAba = abaEdicaoEvento('novo');
	echo montarAbasArray($arAba, "?modulo=principal/cadastroEvento&acao=A");
}
monta_titulo( 'Cadastro de Eventos', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

$parametroagenda = new ParametroAgenda();

$dadosconsulta = $parametroagenda->carregaDados($_SESSION['agenda']['agdid']);

$dadosparametroagenda = array();

foreach( $dadosconsulta as $arr ){

	array_push($dadosparametroagenda, $arr['pradsccampo'] );
}


?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarFormEvento(){
	var errorText = new Array();
	
	//tpeid
	//tppid
	//sltid
		//cargo
	//etaid
	//prtid
		//cevid
	//evedtinicio
	//evehsinicio
	//evedtfinal
	//evehsfinal
	//estuf
	//muncod
	//evebairro
	//evelogradouro
	//evenumero
	//evecomplemento
	//eveassunto
	
	<?php if( $parametroagenda->verificaParametroAgenda( 'tpeid', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=tpeid]').val() ) == '' ){
			errorText.push('Tipo');
		}
	<? } ?>
		<?php if( $parametroagenda->verificaParametroAgenda( 'tppid', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=tppid]').val() ) == '' ){
			errorText.push('Prioridade');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'sltid', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=sltid]').val() ) == '' ){
			errorText.push('Solicitante');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'etaid', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=etaid]').val() ) == '' ){
			errorText.push('Entidade/Pessoa');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'prtid', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('#prtid option:first').val() ) == '' ){
			errorText.push('Participante(s)');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'cevid', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('#cevid option:first').val() ) == '' ){
			errorText.push('Contato(s)');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'evedtinicio', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=evedtinicio]').val() ) == '' ){
			errorText.push('Data Início');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'evedtfinal', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=evedtfinal]').val() ) == '' ){
			errorText.push('Data Fim');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'evehsinicio', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=evehsinicio]').val() ) == '' ){
			errorText.push('Hora Início');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'evehsfinal', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=evehsfinal]').val() ) == '' ){
			errorText.push('Hora Fim');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'estuf', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=estuf]').val() ) == '' ){
			errorText.push('UF');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'muncod', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=muncod]').val() ) == '' ){
			errorText.push('Município');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'evebairro', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=evebairro]').val() ) == '' ){
			errorText.push('Bairro');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'evelogradouro', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=evelogradouro]').val() ) == '' ){
			errorText.push('Logradouro');
		}
	<? } ?>
	<?php if( $parametroagenda->verificaParametroAgenda( 'eveassunto', $dadosparametroagenda ) ){ ?>
		if ( $.trim( $('[name=eveassunto]').val() ) == '' ){
			errorText.push('Assunto');
		}
	<? } ?>
	if ( errorText.length > 0 ){
		errorText = errorText.join("\n");
		errorText = 'Os campos listados abaixo são de preenchimento obrigatório:\n' + errorText;
		alert( errorText );
		return false;
	}
	
	$('#prtid option').attr('selected', true);
	$('#cevid option').attr('selected', true);
	$('[name=op]').val('salvar');
	$('[name=formEvento]').submit();
}

function carregarMunicipio( estuf ){
					var td	= $('#td_municipio');
					if ( estuf != '' ){
						var url = location.href;
						$.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {ajax  : 'municipio', 
									  		  	    estuf : estuf},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
									  	divCarregando();
										td.find('select option:first').attr('selected', true);
									  },
									  error 	 : function (){
									  	divCarregado();
									  },
									  success	 : function ( data ){
									  	td.html( data );
									  	divCarregado();
									  }
								});	
					}else{
						td.find('select option:first').attr('selected', true);
						td.find('select').attr('selected', true)
										 .attr('disabled', true);
					}			
}

function carregaEntidadeBySolicitante( sltid ){
					var td	= $('#td_entidade');
					if ( sltid != '' ){
						var url = location.href;
						$.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {ajax  : 'entidade', 
									  		  	    sltid : sltid},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
									  	divCarregando();
										td.find('input:first').val('Selecione...')
															  .attr('disabled', true);
									  },
									  error 	 : function (){
									  	divCarregado();
									  },
									  success	 : function ( data ){
									  	td.html( data );
									  	divCarregado();
										
										//atribui valor ao campo cargo
									  	var cargo = document.getElementById("sltid_dsc").value.split(" ");
									  	if(cargo.length > 0){
									  		document.getElementById("sltid_dsc").value = "";
									  		for(i=0; i<cargo.length; i++){
									  			if(i==0){
									  				document.getElementById("cargo").value = cargo[i];
									  			}else{
									  				document.getElementById("sltid_dsc").value += cargo[i] + ' ';
									  			} 
									  		}
									  	}
									  	
									  }
								});	
					}else{
						td.find('input:first').val('Selecione...')
											  .attr('disabled', true);
					}			
}

function alterarEvento(eveid){
	location.href = '?modulo=principal/cadastroEvento&acao=E&eveid=' + eveid;
}

function excluirEvento(eveid){
	if ( confirm('Deseja apagar esse evento?') ){
		location.href = '?modulo=principal/cadastroEvento&acao=A&op=excluir&eveid=' + eveid;
	}
}


//-->
</script>

<form name="formEvento" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="agdid" id="agdid" value="<?php echo $agdid; ?>" type="hidden">
<input name="eveid" id="eveid" value="<?php echo $eveid; ?>" type="hidden">


<?php
if ( $_REQUEST['eveid'] != "" ){
	 
	$evento = new Evento();
	$dados = $evento->carregaTodosDados( $_REQUEST['eveid']);

	?>
	<table class="tabela" align="center">
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Código</td>
			<td> <?php echo $dados['eveid']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Evento</td>
			<td> <?php echo $dados['tpedsc']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Data de Início</td>
			<td> <?php echo $dados['evedtinicio']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Solicitante</td>
			<td> <?php echo $dados['sltdsc']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Entidade</td>
			<td> <?php echo $dados['etadsc']; ?></td>
		</tr>
		<tr>
			<td width="200" class="SubTituloDireita" style="font-size: 10pt; font-weight: bold;">Assunto</td>
			<td> <?php echo $dados['eveassunto']; ?></td>
		</tr>
	</table>
<?php } 


?> 

<table class="tabela" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dono da Agenda</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<b><?php echo $agddsc; ?></b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Dados do Evento</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo:</td>
		<td>
		<?php
		$evento = new TipoEvento();
		$dados = $evento->listaCombo();
		if( $parametroagenda->verificaParametroAgenda( 'tpeid', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		echo $db->monta_combo("tpeid", $dados, $disable,'Selecione...','', '', '',240,$obrigatorio,'tpeid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prioridade:</td>
		<td>
		<?php
		$prioridade = new TipoPrioridade();
		$dados = $prioridade->listaCombo();
		if( $parametroagenda->verificaParametroAgenda( 'tppid', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		echo $db->monta_combo("tppid", $dados, $disable,'Selecione...','', '', '',240,$obrigatorio,'tppid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Solicitante:</td>
		<td>
		<?php
		$solicitante = new Solicitante();
		$sql 		 = $solicitante->listaComboPopupSQL();
		$sltid 		 = $solicitante->carregaCampoPopup( $sltid );
		
		$filtro = array(
						array("codigo" => "sltdsc",
							  "descricao" => "<b>Solicitante:</b>",
							  "string" => "1")
						);
		if( $parametroagenda->verificaParametroAgenda( 'sltid', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		campo_popup('sltid',$sql,'Selecione o Solicitante','carregaEntidadeBySolicitante','400x800','40', $filtro, 1, true, ($disable == 'S' ? false : true), '', $obrigatorio);
		
		if ( $disable == 'S' ):
		?>
		&nbsp;&nbsp;&nbsp;
		<a href="javascript:janela('?modulo=sistema/apoio/cadastroSolicitante&acao=P&op=novo',600,500,'popupcadastro');">[Novo Solicitante]</a>
		<?php 
		endif;
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Cargo:</td>
		<td id="td_cargo">
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'cargo', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("cargo",$obrigatorio,"N","Cargo",50,255,"","","","","","id='cargo'")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Entidade/Pessoa:</td>
		<td id="td_entidade">
		<?php
		
		//unset($sltid);
		//if ( empty($sltid) ){
		//	campo_popup('etaid','','Selecione a Entidade/Pessoa','','400x800','40', $filtro, 1, true, ($disable == 'S' ? false : true));
		//}else{
			$entidade = new EntidadeAgenda();
			$sql 	  = $entidade->listaComboPopupSQL( array('sltid' => $sltid['codigo']) );
			$etaid 	  = $entidade->carregaCampoPopup( $etaid );
			
			$filtro = array(
							array("codigo" 		=> "etadsc",
								  "descricao" 	=> "<b>Entidade/Pessoa:</b>",
								  "string" 		=> "1")
							);
			if( $parametroagenda->verificaParametroAgenda( 'etaid', $dadosparametroagenda ) ){
				$obrigatorio = 'S';
			}else{
				$obrigatorio = 'N';
			}
			campo_popup('etaid',$sql,'Selecione a Entidade/Pessoa','','400x800','40', $filtro, 1, true, ($disable == 'S' ? false : true), '', $obrigatorio);
			
			if ( $disable == 'S' ):
		?>
			&nbsp;&nbsp;&nbsp;
			<a href="javascript:janela('?modulo=sistema/apoio/cadastroEntidade&acao=P&sltid=<?php echo $sltid['codigo'] ?>',600,530,'popupcadastro');">[Nova Entidade/Pessoa]</a>
		<?php	
			endif;
		//}
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Participante(s):</td>
		<td valign="top">
		<?
		$participante = new Participantes();
		$sql 		 = $participante->listaComboPopupSQL();
		$prtid		 = $participante->carregaComboPopup( $eveid );
		
		$filtro = array(
						array("codigo" => "prtdsc",
							  "descricao" => "<b>Participante:</b>",
							  "string" => "1")
						);
		if( $parametroagenda->verificaParametroAgenda( 'prtid', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		combo_popup('prtid', $sql, 'Selecione o(s) Participante(s)', "400x400", 0, array(), "", "S", false, false, 4, 320, null, null, false, $filtro);
		
		if ( $disable == 'S' ):
		?>
			&nbsp;&nbsp;&nbsp;
			<a href="javascript:janela('?modulo=sistema/apoio/cadastroParticipante&acao=P&op=novo',600,330,'popupcadastro');">[Novo Participante]</a>
		<?php 
		endif;
		?>	
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Contato(s):</td>
		<td valign="top">
		<?
		$contato = new ContatosEvento();
		$sql 		 = $contato->listaComboPopupSQL();
		$cevid		 = $contato->carregaComboPopup( $eveid );
		
		$filtro = array(
						array("codigo" => "cevcargo || cevexpressao || cevdsc",
							  "descricao" => "<b>Contato:</b>",
							  "string" => "1")
						);
						
		if( $parametroagenda->verificaParametroAgenda( 'cevid', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		combo_popup('cevid', $sql, 'Selecione o(s) Contato(s)', "400x400", 0, array(), "", "S", false, false, 4, 320, null, null, false, $filtro);
		
		
		if ( $disable == 'S' ):
		?>
			&nbsp;&nbsp;&nbsp;
			<a href="javascript:janela('?modulo=sistema/apoio/cadastroContato&acao=P&op=novo',600,530,'popupcadastro');">[Novo Contato]</a>
		</td>
		<?php 
		endif;
		?>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data Início:</td>
		<td>
		
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evedtinicio', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_data2( 'evedtinicio', $obrigatorio, $disable, '', $obrigatorio ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Hora Início:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evehsinicio', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("evehsinicio",$obrigatorio,$disable,"Hora Início",9,10,"##:##","","right")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Data Fim:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evedtfinal', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_data2( 'evedtfinal', $obrigatorio, $disable, '', 'S' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Hora Fim:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evehsfinal', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("evehsfinal",$obrigatorio,$disable,"Hora Fim",9,10,"##:##","","right")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
		<?
		$sql = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado";
 
		if( $parametroagenda->verificaParametroAgenda( 'estuf', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=$db->monta_combo("estuf", $sql, $disable,'Selecione...','carregarMunicipio', '', '',240,$obrigatorio,'estuf'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município:</td>
		<td id="td_municipio">
		<?php 
		if ($estuf){
			$sql = "SELECT 
						muncod AS codigo, 
						mundescricao AS descricao 
					FROM
						territorios.municipio
					WHERE
						estuf = '{$estuf}'
					ORDER BY
						mundescricao";
			$habMun = 'S';
		}else{
			$sql = array();
			$habMun = 'N';
		}		
		if( $parametroagenda->verificaParametroAgenda( 'muncod', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		$habMun = ($disable == 'N' ? $disable : $habMun);
		echo $db->monta_combo("muncod", $sql, $habMun,'Selecione...','', '', '',240,$obrigatorio,'muncod'); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Bairro:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evebairro', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("evebairro",$obrigatorio,$disable,"Bairro",50,255,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Logradouro:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evelogradouro', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("evelogradouro",$obrigatorio,$disable,"Logradouro",50,255,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evenumero', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("evenumero",$obrigatorio,$disable,"Número",10,10,"[#]","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Complemento:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'evecomplemento', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?=campo_texto("evecomplemento",$obrigatorio,$disable,"Complemento",50,255,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Assunto:</td>
		<td>
		<?php
		if( $parametroagenda->verificaParametroAgenda( 'eveassunto', $dadosparametroagenda ) ){
			$obrigatorio = 'S';
		}else{
			$obrigatorio = 'N';
		}
		?>
		<?= campo_textarea( 'eveassunto', $obrigatorio, $disable, '', 70, 5, 500 ); ?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   <?php 
	   if ( $disable != 'N' ):
	   ?>
		   	<input type="button" name="btalterar" value="Salvar" onclick="validarFormEvento()" class="botao">
		   	<?php 
		   	if ( $_REQUEST['acao'] == 'E' ):
		   	?>
		   	<input type="button" name="btnovo" value="Novo Evento" onclick="location.href='?modulo=principal/cadastroEvento&acao=A'" class="botao">
					   	
		   	<input type="button" name="btimprimir" value="Imprimir" onclick="window.open('?modulo=relatorio/despachoInicial&acao=A','printEvento');" class="botao">
		   	<?php 
		   	endif;
	   endif;
	   ?>
	   </td>
	</tr> 
</table>
</form>