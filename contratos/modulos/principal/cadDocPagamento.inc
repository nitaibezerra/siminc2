<?php 
require_once APPRAIZ . "evento/classes/DocumentoPagamento.class.inc";

if( $_POST['formulario'] ){
	extract($_POST);
	$obDocumentoPagamento = new DocumentoPagamento();
	$obDocumentoPagamento->dpaid    	  = $dpaid;
	$obDocumentoPagamento->eveid 		  = $_SESSION['eveid'];
	$obDocumentoPagamento->tdpid 		  = $tdpid;
	$obData = new Data();
	$obDocumentoPagamento->dpadataemissao = $obData->formataData($dpadataemissao,"YYYY-mm-dd");
	if($dpavalor){
		$dpavalor = str_replace(',','.',str_replace('.','',$dpavalor));		
	}
	$obDocumentoPagamento->dpavalor 	  = $dpavalor;
	$obDocumentoPagamento->dpanumero 	  = $dpanumero;
	$obDocumentoPagamento->dpaobs 		  = $dpaobs;
	$obDocumentoPagamento->salvar();
	$obDocumentoPagamento->commit();
	$_REQUEST['acao'] = 'A';
	
	#ATUALIZAR VALORES - PAGAMENTOS DE DADOS.		
	$valorOs = str_replace(',','.',str_replace('.','',$_POST['osecustofinal']));
	$valorDp = str_replace(',','.',str_replace('.','',$_POST['dpavalor']));
	
	if( $_POST['ureid'] ){
		
		#se o valor do pagamento for MAIOR que o valor da ordem de serviço, 
		#subtrair o valor da ordem de serviço do valor do pagamento. 
		#O resultado da operação deverá ser SUBTRAÍDO do saldo da unidade (tabela unidaderecurso).
		if($valorDp > $valorOs){
			$diffValor = $valorDp - $valorOs;
			$sql = " 
				insert into evento.unidadecontacorrente 
					(ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf) 
						values
					({$_POST['ureid']}, {$_SESSION['eveid']}, 'Ajuste do pagamento do evento Nº. {$_SESSION['eveid']} ', {$diffValor}, '".date('Y-m-d')."', '{$_SESSION['usucpf']}'); 
			";

			$sql .= " update evento.unidaderecurso set urevalorsaldo = urevalorsaldo-{$diffValor} where ureid = {$_POST['ureid']}; ";
			
		#se o valor do pagamento for MENOR que o valor da ordem de serviço, 
		#subtrair o valor do pagamento do valor da ordem de serviço. 
		#O resultado da operação deverá ser SOMADO ao saldo da unidade (tabela unidaderecurso).		
		}elseif( $valorDp < $valorOs ){
			$diffValor = $valorOs - $valorDp;
			$sql = " 
				insert into evento.unidadecontacorrente
					(ureidpai, eveid, uccdesclancamento, uccvalorlancamento, uccdatalancamento, ucccpf)
						values
					({$_POST['ureid']}, {$_SESSION['eveid']}, 'Ajuste da O.S. do evento Nº. {$_SESSION['eveid']} ', '{$diffValor}', '".date('Y-m-d')."', '{$_SESSION['usucpf']}'); 
			";
	
			$sql .= " update evento.unidaderecurso set urevalorsaldo = urevalorsaldo+{$diffValor} where ureid = {$_POST['ureid']}; ";
		}
		if ( $db->executar( $sql ) ){
			$db->commit();
		}
	}	
	#FIM - ATUALIZAR VALORES - PAGAMENTOS DE DADOS.
	
	$db->sucesso("principal/cadDocPagamento","&dpaid=".$obDocumentoPagamento->dpaid);
	unset($obDocumentoPagamento);
	die;
}

if($_GET['dpaidExcluir']){
	$obDocumentoPagamento = new DocumentoPagamento();
	$obDocumentoPagamento->excluir($_GET['dpaidExcluir']);
	$obDocumentoPagamento->commit();
	unset($_GET['dpaidExcluir']);
	unset($obDocumentoPagamento);
	$_REQUEST['acao'] = 'A';
	$db->sucesso("principal/cadDocPagamento");
	die;
}

if( $_SESSION['eveid'] != ''){ 
	$sql = "
		SELECT 	ev.evetitulo,  
				ev.ungcod,
				ev.tpeid,			 
				ev.evedatainicio,		 
				ev.evedatafim,			 
				ev.eveemail, 		 
				ev.evenumeropi, 		 
				ev.evenumeroprocesso,   
				ev.evecustoprevisto, 
				ev.evequantidadedias,	 
				ev.evepublicoestimado,  
				ev.muncod, 				 
				ev.estuf, 			
				ev.eveurgente,	 
				ev.sevid,
				ev.ureid,
				ev.docid,
				u.ungdsc,
				us.usunome,
				to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao, 
				trim( to_char(os.osecustofinal, '999G999G999G999G999G999G999D99') ) as osecustofinal, 
				os.oseobsos ,
				dp.dpadataemissao
		FROM evento.evento AS ev
		LEFT JOIN evento.ordemservico AS os on os.eveid = ev.eveid
		LEFT JOIN evento.documentopagamento AS dp on dp.eveid = ev.eveid
		LEFT JOIN evento.tipoevento AS te ON te.tpeid = ev.tpeid
		LEFT JOIN public.unidadegestora AS u ON ev.ungcod = u.ungcod
		LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf 
		WHERE ev.eveid = '".$_SESSION['eveid']."'
	";
	$rsDadosEvento = $db->carregar( $sql );
	//ver($sql, $rsDadosEvento, d);
	 
}

$titulo_modulo = "Eventos";
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
echo'<br>'; 

$res = array(0 => array ( "descricao" => "Lista",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
				  		  ),
 				1 => array ( "descricao" => "Informações Básicas",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=principal/cadEvento&acao=A"
				  		  )
			);	  
				
if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res,
					array ("descricao" => "Documentos Anexos",
							    "id"        => "3",
							    "link" 		=> "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
							   ),
		   			array ("descricao" => "Infraestrutura",
							    "id"		=> "2",
							    "link"		=> "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
					  		   )
			  );
	/*
	$pflcod = pegaPerfil( $_SESSION['usucpf'] ); 
	
	 if( ( $pflcod == PERFIL_SUPER_USUARIO) || ( $pflcod == PERFIL_SAA ) )
	 { 
			array_push($res,	
					array ("descricao" => "Hotéis",
						    "id"		=> "1",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A"
				  		   ) 
				    );
	 }
	array_push($res,	
					 
				  	array ("descricao" => "Avaliação",
						    "id"		=> "0",
						    "link"		=> "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
				  		   )
					  );
	*/
}


/* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
*
if($_SESSION['eveid']){
	array_push($res,	
				  	array ("descricao" => "Estrutura Orçamentária",
						    "id"		=> "6",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
				  		   )
					  );	
}
*/
if(mostraAbaDocOS($_SESSION['eveid'])){
	array_push($res,	
				  	array ("descricao" => "Ordem de Serviço",
						    "id"		=> "7",
						    "link"		=> "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
				  		   )
					  );	
}

if(mostraAbaDocPagamento($_SESSION['eveid'])){
		array_push($res,	
				  	array ("descricao" => "Documento de Pagamento",
						    "id"		=> "5",
						    "link"		=> "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
				  		   )
					  );
}

if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res, 
			  	array ("descricao" => "Avaliação",
					    "id"		=> "0",
					    "link"		=> "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
			  		   )
				  );
	
}
					  
echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"); 
headEvento($rsDadosEvento[0]['evetitulo'],$rsDadosEvento[0]['usunome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao']);

$obDocumentoPagamento = new DocumentoPagamento($_GET['dpaid']);


#verifica permissão edição
$permitEdicao = dPagamentoPermissaoEdicao();

$docid = evtCriarDoc($_SESSION['eveid']);
$esdid = verificaEstadoDocumento($docid);

$perfil = array();
$perfil = pegaPerfilGeral( $_SESSION['usucpf'], $_SESSION['sisid'] );

$somenteLeitura = 'N';

if( $esdid == AGUARDANDO_PAGAMENTO_EVENTO_WF && $permitEdicao == '' ){
	$somenteLeitura = 'S';
}elseif( in_array(PERFIL_SUPER_USUARIO, $perfil) && $permitEdicao == '' ){
	$somenteLeitura = 'S';
}else{
	$somenteLeitura = 'N';	
}


?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form action="" method="POST" name="formulario">
	<input type="hidden" name="formulario" id="formulario" value="1" />
	<input type="hidden" name="ureid" value="<?=$rsDadosEvento[0]['ureid']?>">
	<input type="hidden" name="dpaid" value="<?php echo $obDocumentoPagamento->dpaid; ?>"/>
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<!-- CAMPOS VISUALIZAÇÃO - ORDEM DE SERVIÇO -->
		<tr>
			<td  class="SubTituloDireita" colspan="2">&nbsp;</td>
			<td rowspan="9">
				<?php wf_desenhaBarraNavegacao( $docid , array( '' => '' ) ); ?>
			</td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">Valor da Ordem de Serviço: </td>
			<td>
				<?php 
					echo campo_texto( 'osecustofinal', 'N', 'N', '', 20, 20, '','','right' ,'' , '', 'id="osecustofinal"','',$rsDadosEvento[0]['osecustofinal'],''); 
				?> 
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Justificativa da ordem de serviço:</td>
			<td>
				<?php
					echo campo_textarea( 'dpaobs', 'N', 'N', '', 120, 4, 1500 , '', 0, '', false, null, $rsDadosEvento[0]['oseobsos'] ); 
				?> 
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de registro de pagamento:</td>
			<td>
				<?php 
					if( $rsDadosEvento[0]['dpadataemissao'] != '' ){
						$dpadataemissao = $rsDadosEvento[0]['dpadataemissao'];
					}else{
						$dpadataemissao = date("m").'/'.date("d").'/'.date("Y");
					}
	       			echo campo_data2( 'dpadataemissao','N', 'N', 'Data de registro de pagamento', 'S','','', $dpadataemissao);
				?> 
			</td>
		</tr>
		<!-- FIM - CAMPOS VISUALIZAÇÃO - ORDEM DE SERVIÇO -->
		<tr>
			<td class="subtitulodireita">Tipo de Documento:</td>
			<td>
				<?php 
	                $sql = "SELECT
								tdpid as codigo,
								tdpdescricao as descricao
							FROM evento.tipodocumentopagamento "; 
	                $tdpid = $obDocumentoPagamento->tdpid;
	             	$db->monta_combo('tdpid', $sql, $somenteLeitura, "Selecione...", '', '', '', '260', 'S', '');
	             ?>     
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor do pagamento:</td>
			<td>
				<?php
					if($obDocumentoPagamento->dpavalor){
						$dpavalor = number_format($obDocumentoPagamento->dpavalor,2,",",".");
					}
					echo campo_texto( 'dpavalor', 'S', $somenteLeitura, '', 20, 20, '###.###.###.###,##','','right' ,'' , '', 'id="dpavalor"','','','this.value=mascaraglobal(\'###.###.###.###,##\',this.value);'); 
				?> 
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Número do documento:</td>
			<td>
				<?php
					$dpanumero = $obDocumentoPagamento->dpanumero; 
					echo campo_texto( 'dpanumero', 'S', $somenteLeitura, '', 20, 20, '', '', '', '','', 'id="dpanumero"'); 
				?> 
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Observação:</td>
			<td>
				<?php
					$dpaobs = $obDocumentoPagamento->dpaobs; 
					echo campo_textarea( 'dpaobs', 'S', $somenteLeitura, '', 60, 4, 1500 , '' ); 
				?> 
			</td>
		</tr>
		<tr>
			 <td class="SubTituloDireita" colspan="2" align="center" style="text-align:center"> 
				<input type="button" name="btnGravar" value="Salvar" id="btnGravar" onclick="gravarPag();" <?php echo $somenteLeitura == 'N' ? 'disabled="disabled"' : '' ?> >
				<!--  
				<input type="button" value="Incluir Novo" onclick="window.location.href='evento.php?modulo=principal/cadDocPagamento&acao=A';" <?php echo $somenteLeitura == 'N' ? 'disabled="disabled"' : '' ?> >
				 -->
	         </td>
		</tr>

	</table>
</form>


<?php

	$acao = "'
		<center>
			<a href=\"evento.php?modulo=principal/cadDocPagamento&acao=A&dpaid='|| dp.dpaid ||'\">
				<img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></a>&nbsp;
			<a style=\"margin: 0 -5px 0 5px;\" style=\"cursor:hand\" href=\"#\" onclick=\"excluirDocPagamento(' || dp.dpaid || ');\">
				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>
		</center>'";
	 
    if($somenteLeitura == 'N'){
    	$acao = "''";
    }
    	
	$sql = "
		Select	$acao as acao,	
				--dp.eveid,
				tdp.tdpdescricao,
				to_char(dp.dpadataemissao, 'DD/MM/YYYY') as dpadataemissao,
				dp.dpavalor,		 
				dp.dpanumero,			 
				dp.dpaobs
			FROM evento.documentopagamento dp
			INNER JOIN evento.tipodocumentopagamento tdp on dp.tdpid = tdp.tdpid
			Where dp.eveid = ".$_SESSION['eveid'];
	
	$cabecalho = array("Ação","Tipo de Documento","Data de Emissão","Valor","Número","Observação");
	$db->monta_lista($sql,$cabecalho,200,10,'N','center','');
?>
<script type="text/javascript">
	function excluirDocPagamento(dpaid){
		if(confirm('Deseja excluir este registro.')){
			window.location.href = "evento.php?modulo=principal/cadDocPagamento&acao=A&dpaidExcluir="+dpaid;
			return true;
		} else {
			return false;
		}
	}
	
	function gravarPag()
	{
		if(document.formulario.tdpid.value==''){
			alert('O Campo "Tipo de Documento" é Obrigatório');
			document.formulario.tdpid.focus();
			return false;
		}
		if(document.formulario.dpavalor.value==''){
			alert('O Campo "Valor" é Obrigatório');
			document.formulario.dpavalor.focus();
			return false;
		}
		if(document.formulario.dpadataemissao.value==''){
			alert('O Campo "Data da Emissão" é Obrigatório');
			document.formulario.dpadataemissao.focus();
			return false;
		}
		if(document.formulario.dpanumero.value==''){
			alert('O Campo "Número" é Obrigatório');
			document.formulario.dpanumero.focus();
			return false;
		}

		document.formulario.osecustofinal.disabled = false; 
		
		document.formulario.submit();
	}
</script>
