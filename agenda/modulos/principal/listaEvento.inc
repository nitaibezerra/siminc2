<?php
/** ATENÇÃO
 * 	&acao=A => CADASTRO
 *  &acao=E => EDIÇÃO
 */

switch ( $_REQUEST['op'] ){
//	case 'salvar':
//		$evento 		 = new Evento( $_POST['eveid'] );
//		$dados 			 = $_POST;
//		$dados['usucpf'] = $_SESSION['usucpf'];
//		$eveid = $evento->popularDadosObjeto( $dados )
//			   			->salvar();
//	
//		// Controla PARTICIPANTES do evento	   			
//		$participanteEvento = new EventoParticipante();
//		$participanteEvento->deletaTodosPorEvento( $eveid );
//		
//		if ( !empty($_POST['prtid'][0]) ){
//			foreach ( $_POST['prtid'] as $prtid ){
//				$dados = array(
//							   'eveid' => $eveid,
//							   'prtid' => $prtid
//							  );
//				$participanteEvento->clearDados()
//								   ->popularDadosObjeto( $dados )
//								   ->salvar();	
//			}		   
//		}	   
//		$db->commit();
//		
//		die('<script>
//				alert(\'Operação realizada com sucesso!\');
//				location.href=\'?modulo=principal/cadastroEvento&acao=E&eveid=' . $eveid . '\';
//			 </script>');
	case 'excluir':
		if ( $_REQUEST['eveid'] ){
			$msg 	= 'Operação realizada com sucesso!';
			$evento = new Evento( $_REQUEST['eveid'] );
			$evento->popularDadosObjeto( array('evestatus' => 'I') )
				   ->salvar();
			$db->commit();
		}else{
			$msg = 'Falha na execução da operação!';
		}	
			
		die('<script>
				alert(\'' . $msg . '\');
				location.href=\'?modulo=principal/listaEvento&acao=A\';
			 </script>');
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

if ( !$agenda->possuiPermissaoAgenda( $agdid ) ){
	die('<script>
			alert(\'O usuário não possui permissão de acesso a agenda!\');
			location.href=\'?modulo=principal/listaAgenda&acao=A\';
		 </script>');		
}

$dados  = $agenda->getDados();
extract( $dados );	

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arAba = abaEdicaoAgenda();
echo montarAbasArray($arAba, "?modulo=principal/listaEvento&acao=A");
	
monta_titulo( 'Pesquisa de Eventos',
			  (verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) )
			  	? 
			   		'<img align="absmiddle" src="/imagens/print.png" style="" title="Despacho Inicial"> Indica Imprimir Despacho Inicial.'
			   	:
			   		''
			  ));

extract($_POST);
$evedtinicio = ($evedtinicio ? formata_data_sql( $evedtinicio ) : $evedtinicio);
$evedtfinal  = ($evedtfinal ? formata_data_sql( $evedtfinal ) : $evedtfinal);
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--
function validarFormEvento(){
	$('#sltid option').attr('selected', true);
	$('#etaid option').attr('selected', true);
	$('#cevid option').attr('selected', true);
	$('[name=op]').val('salvar');
	
	$('[name=formEvento]').attr('target', '').attr('action', '').submit();
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
		location.href = '?modulo=principal/listaEvento&acao=A&op=excluir&eveid=' + eveid;
	}
}

function abrirDespacho(eveid){
	$('#eveid').val( eveid );
	$('[name=formEvento]').attr('target', 'printEvento')
						  .attr('action', '?modulo=relatorio/despachoInicial&acao=A&eveid='+eveid)
						  .submit();
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

//-->
</script>

<form name="formEvento" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="agdid" id="agdid" value="<?php echo $agdid; ?>" type="hidden">
<input name="eveid" id="eveid" value="" type="hidden">
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
		<td class="SubTituloCentro" colspan="2">Filtros</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Assunto:</td>
		<td>
		<?=campo_texto("eveassunto","N","S","Assunto",50,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo:</td>
		<td>
		<?php
		$evento = new TipoEvento();
		$dados = $evento->listaCombo();
		echo $db->monta_combo("tpeid", $dados, 'S','Selecione...','', '', '',240,'N','tpeid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prioridade:</td>
		<td>
		<?php
		$prioridade = new TipoPrioridade();
		$dados = $prioridade->listaCombo();
		echo $db->monta_combo("tppid", $dados, 'S','Selecione...','', '', '',240,'N','tppid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
		<?
		$estuf = $_POST['estuf'];
		$sql = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				order by 2";
		?>
		<?=$db->monta_combo("estuf", $sql, "S",'Selecione...','carregarMunicipio', '', '',240,'N','estuf'); ?>
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
		$muncod = $_POST['muncod'];
		$habMun = ($disable == 'N' ? $disable : $habMun);
		echo $db->monta_combo("muncod", $sql, $habMun,'Selecione...','', '', '',240,'N','muncod'); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Solicitante:</td>
		<td>
		<?php
		$solicitante = new Solicitante();
		$sql 		 = $solicitante->listaComboPopupSQL();
		$sltid 		 = $solicitante->carregaComboPopupBySltid( (array) $sltid );
		
		$filtro = array(
						array("codigo" => "sltdsc",
							  "descricao" => "<b>Solicitante:</b>",
							  "string" => "1")
						);
		combo_popup('sltid', $sql, 'Solicitante(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Cargo:</td>
		<td>
		<?=campo_texto("sltcargo","N","S","Assunto",50,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Entidade/Pessoa:</td>
		<td id="td_entidade">
		<?php
		$entidade = new EntidadeAgenda();
		$sql 	  = $entidade->listaComboPopupSQL();
		$etaid 	  = $entidade->carregaComboPopupByEtaid( (array) $etaid );
		
		$filtro = array(
						array("codigo" 		=> "etadsc",
							  "descricao" 	=> "<b>Entidade/Pessoa:</b>",
							  "string" 		=> "1")
						);
		combo_popup('etaid', $sql, 'Entidade(s)/Pessoa(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Contato:</td>
		<td id="td_entidade">
		<?php
		$contato = new ContatosEvento();
		$sql 		 = $contato->listaComboPopupSQL();
		$cevid		 = $contato->carregaComboPopupByCevid( (array) $cevid );
		
		$filtro = array(
						array("codigo" => "cevcargo || cevexpressao || cevdsc",
							  "descricao" => "<b>Contato:</b>",
							  "string" => "1")
						);
						
		combo_popup('cevid', $sql, 'Contato(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>
<!--  
	<tr>
		<td class="SubTituloDireita" valign="top">Participante(s):</td>
		<td valign="top">
		<?
//		$participante = new Participantes();
//		$sql 		 = $participante->listaComboPopupSQL();
//		$prtid		 = $participante->carregaComboPopup( $eveid );
//		
//		combo_popup('prtid', $sql, 'Participante(s)', "400x400", 0, array(), "", "S", false, false, 5, 330 );
		?>
		</td>
	</tr>
-->	
	<tr>
		<td class="SubTituloDireita">Data:</td>
		<td>
		<?=campo_data2( 'evedtinicio', 'N', 'S', '', 'S' ); ?>
		&nbsp;
		Até
		&nbsp;
		<?=campo_data2( 'evedtfinal', 'N', 'S', '', 'S' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Hora:</td>
		<td>
		<?=campo_texto("evehsinicio","N","S","Hora Início",4,5,"##:##","","right")?>
		&nbsp;
		Até
		&nbsp;
		<?=campo_texto("evehsfinal","N","S","Hora Fim",4,5,"##:##","","right")?>
		</td>
	</tr>
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btpesq" value="Pesquisar" onclick="validarFormEvento()" class="botao">
	   	<?php 
	   	if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR, PERFIL_GESTOR) ) ){
	   	?>
	   	<input type="button" name="btnovo" value="Novo Evento" onclick="location.href='?modulo=principal/cadastroEvento&acao=A'" class="botao">
	   	<?php 
		}
	   	?>
	   </td>
	</tr> 
</table>
</form>
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Lista de Eventos Cadastrados</td>
	</tr>
	<tr>
		<td colspan="2" height="310" style="background-color: #F0F0F0;">
			<fieldset style="height:310px; background-color: #FFFFFF;">
				<div style="height:300px; overflow:auto;">
				<?php
				$evento = new Evento();
				$sql 	= $evento->listaSQL( ($_POST ? $_POST : array('agdid' => $agdid)) );
				
				$cabecalho = array("Ação", "Código", "Assunto", "Data - Hora Início", "Data - Hora Fim", "Tipo", "Prioridade", "Entidade/Pessoa", "Solicitante", "Responsável pelo Cadastro");
				$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
				?>
				</div>
			</fieldset>
		</td>
	</tr>
</table>
