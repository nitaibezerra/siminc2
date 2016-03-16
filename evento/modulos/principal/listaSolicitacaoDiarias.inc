<?php 
function toltipvalor( $dados ){
	
	global $db;
	
	$sql = "SELECT DISTINCT	
				val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)) as valorcomp,
				valorant,
				val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)) - valorant as valorpagar
			FROM 
				evento.solicitacaodiaria sol
			INNER JOIN evento.itinerariosolicitacaodiaria iti ON iti.solid  = sol.solid AND iti.itistatus = 'A'
			INNER JOIN (
				SELECT DISTINCT
					solid,
					sum(
						(itidias*(itivalordiaria*(CASE WHEN itimeiadiaria THEN 0.5 ELSE 1 END)
							      )
						)+
						(itivaloradicionalembarque
						)+
						(CASE WHEN itiultimo THEN (itivalordiaria*0.5) ELSE 0 END
						)+
						(itidiasviajem*itivalordiaria)
					) as valor
				FROM
					evento.itinerariosolicitacaodiaria
				WHERE
					itistatus = 'A'
				GROUP BY solid) val ON val.solid = sol.solid
			INNER JOIN (
					SELECT DISTINCT	
						val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)) as valorant,
						sol.solid
					FROM 
						evento.solicitacaodiaria sol
					INNER JOIN evento.itinerariosolicitacaodiaria iti ON iti.solid  = sol.solid AND iti.itistatus = 'A'
					INNER JOIN (
						SELECT DISTINCT
							solid,
							sum(
								(itidias*(itivalordiaria*(CASE WHEN itimeiadiaria THEN 0.5 ELSE 1 END)
									      )
								)+
								(itivaloradicionalembarque
								)+
								(CASE WHEN itiultimo THEN (itivalordiaria*0.5) ELSE 0 END
								)+
								(itidiasviajem*itivalordiaria)
							) as valor
						FROM
							evento.itinerariosolicitacaodiaria
						WHERE
							itistatus = 'A'
						GROUP BY solid) val ON val.solid = sol.solid ) ant ON ant.solid = sol.solcomplemento
			WHERE
				sol.solid = ".$dados['solid'];
	$info = $db->pegaLinha($sql);
	?>
<table width="300px">
	<tr>
		<td class="SubTituloCentro" colspan="2" >
			Resumo Complemento 
		</td>
	</tr>
	<tr>
		<td width="70%">
			Valor do Complemento: 
		</td>
		<td>
			R$ <?=number_format($info['valorcomp'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td>
			Valor da Solicitação Complementada:
		</td>
		<td style="color:red">
			- R$ <?=number_format($info['valorant'],2,',','.'); ?>
		</td>
	</tr>
	<tr>
		<td>
			Valor a pagar:
		</td>
		<td>
			R$ <?=number_format($info['valorpagar'],2,',','.'); ?>
		</td>
	</tr>
</table>
	<?php 
}

function montacomboEmpenho($dados){
	
	global $db;
	
	$sql = "SELECT DISTINCT
				empcod as codigo,
				empcod as descricao
			FROM
				evento.empenho_unidadegestora
			WHERE
				ungcod = '".$dados['ungcod']."'";
	
	echo $db->monta_combo('empcod',$sql,'S','Selecione...','','','','','S', 'empcod');
	
}

function listaSolicitacoes( $dados = null ){
	
	global $db;
	
	$notUngcod = Array();
	$notUnicod = Array();
	$trava = false;
	if(!$db->testa_superuser()){
		
		$arPerfisSolicitacaoAjudaCusto = Array(EVENTO_PERFIL_SOLICITADOR, 
						 	 EVENTO_PERFIL_VALIDADOR,
						 	 EVENTO_PERFIL_AGENTE_FINANCEIRO,
						 	 EVENTO_PERFIL_AUTORIZADOR,
					 		 EVENTO_PERFIL_ADMINISTRADOR_DIARIAS
							);
		
		$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
		if( possuiPerfil( $arPerfisSolicitacaoAjudaCusto ) ){
			
			$sql = "SELECT DISTINCT 
				p.ungcod,
				eug.empcod
			FROM 
				evento.usuarioresponsabilidade ur 
			INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
			INNER JOIN evento.empenho_unidadegestora eug ON eug.ungcod = ur.ungcod
			WHERE
				ur.rpustatus = 'A' and
				ur.usucpf = '".$_SESSION['usucpf']."' and
				ur.pflcod IN (".implode(',',$perfil).") and
				ur.prsano = '".$_SESSION['exercicio']."'";

			$notUngcod = $db->carregarColuna($sql);
			array_push($notUngcod, '00000000');
			$notEmpcod = $db->carregarColuna($sql,'empcod');
			array_push($notEmpcod, '00000000');
			
			$sql = "SELECT DISTINCT 
						p.unicod,
						eug.empcod
					FROM 
						evento.usuarioresponsabilidade ur 
					INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
					INNER JOIN evento.empenho_unidadegestora eug ON eug.ungcod = ur.ungcod
					WHERE
						ur.rpustatus = 'A' and
						ur.usucpf = '".$_SESSION['usucpf']."' and
						ur.pflcod IN (".implode(',',$perfil).") and
						ur.prsano = '".$_SESSION['exercicio']."'";

			$notUnicod = $db->carregarColuna($sql);
			array_push($notUngcod, '00000000');
			if($notEmpcod[0]=='00000000'){
				$notEmpcod = $db->carregarColuna($sql,'empcod');
				array_push($notEmpcod, '00000000');
			}
			
			if(!is_array($notUngcod)&&!is_array($notUnicod)){
				$trava = true;
			}
		}else{
			echo "<script>
					alert('Acesso negado.');
					window.history.back(-1); 
				  </script>";
		}
	}

	$where = Array('solstatus = \'A\'','solanoexercicio = '.$_SESSION['exercicio']);
	
	$esdids = array();
	if(is_array($perfil)){
//		if(in_array(EVENTO_PERFIL_SOLICITADOR,$perfil)){ array_push($esdids,WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO); }
		if(in_array(EVENTO_PERFIL_VALIDADOR,$perfil)){ array_push($esdids,WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO); }
		if(in_array(EVENTO_PERFIL_AUTORIZADOR,$perfil)){ array_push($esdids,WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO); }
		if(in_array(EVENTO_PERFIL_AGENTE_FINANCEIRO,$perfil)){ array_push($esdids,WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO,WF_SOLICITAÇÃO_DIARIAS_PAGO); }
		if(in_array(EVENTO_PERFIL_ADMINISTRADOR_DIARIAS,$perfil)){ $esdids = array(); }
		if($esdids[0]!=''){
			array_push($where, 'esd.esdid in ('.implode(',',$esdids).')');
		}
	}
					
	if( is_array($dados) ){
		
		if( $dados['cpf'] )				 { array_push($where, 'usucpf = \''.str_replace(Array('.','-'),Array('',''),$dados['cpf']).'\''); }
		if( $dados['numdocestrangeiro'] ){ array_push($where, 'solnumdocestrangeiro = \''.$dados['numdocestrangeiro'].'\''); }
		if( $dados['nome'] )			 { array_push($where, 'upper(solnome) like upper(\'%'.$dados['nome'].'%\')'); }
		if( $dados['email'] )			 { array_push($where, 'solemail = \''.$dados['email'].'\''); }
		if( $dados['aux'] )				 { array_push($where, 'sol.tpaid = '.$dados['aux']); }
		if( $dados['banco'] )			 { array_push($where, 'solbanco = \''.$dados['banco'].'\''); }
		if( $dados['agencia'] )			 { array_push($where, 'solagencia like \''.$dados['agencia'].'%\''); $dados['digitoagencia'] = $dados['digitoagencia'] ? $dados['digitoagencia'] : 'X'; }
		if( $dados['conta'] )			 { array_push($where, 'solconta like \''.$dados['conta'].'%\''); $dados['digitoconta'] = $dados['digitoconta'] ? $dados['digitoconta'] : 'X'; }
		if( $dados['codSCDP'] )		 	 { array_push($where, 'solcodigoscdb = \''.$dados['codSCDP'].'\''); }
		if( strlen($dados['ungcod']) == 6 )		 	 
										 { array_push($where, 'ung.ungcod = \''.$dados['ungcod'].'\''); }
		if( strlen($dados['ungcod']) == 5 )		 	 
										 { array_push($where, 'uni.unicod = \''.$dados['ungcod'].'\''); }
		if( $dados['empcod'] )		 	 { array_push($where, 'emu.empcod = \''.$dados['empcod'].'\''); }
//		if( $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO )		 	 
//										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO.''); }
//		if( $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO )		 	 
//										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO.''); }
//		if( $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO )		 	 
//										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO.''); }
//		if( $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO )		 	 
//										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO.''); }
		if($dados['esdid'])				 { array_push($where, 'esd.esdid = '.$dados['esdid'].''); }
		if($dados['comp']=='S')			 { array_push($where, 'sol.solcomplemento is not null'); }
	}
	
	$sql = "SELECT DISTINCT
				'<div align=\"center\">
					<img class=\"imprimir\" border=\"0\" title=\"Imprimir solicitação.\" src=\"../imagens/print.gif\"  id=\"'||iti.solid||'\" />
				</div>'  as mais,
				CASE WHEN solcomplemento IS NOT NULL
					THEN '  <div align=\"center\">
								<img border=\"0\" width=\"20px\" title=\"Solicitação Complementar.\" src=\"../imagens/conveniada.png\"/>
							</div>'
					ELSE ''
				END as complemento,
				to_char(sol.soldatainclusao,'DD/MM/YYYY') as datainclusao,
				CASE WHEN usucpf is not null
					THEN replace(to_char(usucpf::bigint, '000:000:000-00'), ':', '.')
					ELSE solnumdocestrangeiro||' '
				END as id,
				'<a href=\"evento.php?modulo=principal/solicitacaoDiarias&acao=A&solid='||sol.solid||'\" style=\"color:blue\">'||solnome||'</a>' as solnome,  
				solemail,
				solbanco||' ' as banco, 
				solagencia||' ' as agencia, 
				solconta||' ' as conta, 
				CASE WHEN solcodigoscdb = 'NULL'
					THEN ''
					ELSE solcodigoscdb
				END AS codigoscdb,
				CASE WHEN ung.ungdsc is not null
					THEN ung.ungdsc
					ELSE uni.unidsc
				END as unidade, 
				sol.empcod,
				esd.esddsc,
				val.valor as val,
				soldeducaoalim+(soldeducaotransp*soltotdias) as deducao,
				CASE WHEN solcomplemento IS NOT NULL THEN
'<div border=\"0\"  style=\"color:blue\" align=\"right\" onmouseover=\"SuperTitleAjax(\''evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A&req=toltipvalor&solid=' || sol.solid || ')\" onmouseout=\"SuperTitleOff( this );\" >' || replace(replace(replace(to_char(val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)),'999,999,999,999,999.99'),'.',':'),',','.'),':',',') || '</div>'
                                ELSE
                                    (val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)))::varchar
				END as totres
			FROM 
				evento.solicitacaodiaria sol
			INNER JOIN workflow.documento 	   	      	  doc ON doc.docid  = sol.docid
			INNER JOIN workflow.estadodocumento 	      esd ON esd.esdid  = doc.esdid
			INNER JOIN evento.empenho_unidadegestora      emu ON emu.empcod = sol.empcod
			LEFT  JOIN public.unidadegestora 	      	  ung ON ung.ungcod = sol.ungcod
			LEFT  JOIN public.unidade		 	      	  uni ON uni.unicod = sol.unicod
			INNER JOIN evento.itinerariosolicitacaodiaria iti ON iti.solid  = sol.solid AND iti.itistatus = 'A'
			INNER JOIN (
				SELECT DISTINCT
					solid,
					sum(
						(itidias*(itivalordiaria*(CASE WHEN itimeiadiaria THEN 0.5 ELSE 1 END)
							      )
						)+
						(itivaloradicionalembarque
						)+
						(CASE WHEN itiultimo THEN (itivalordiaria*0.5) ELSE 0 END
						)+
						(itidiasviajem*itivalordiaria)
					) as valor
				FROM
					evento.itinerariosolicitacaodiaria
				WHERE
					itistatus = 'A'
				GROUP BY solid) val ON val.solid = sol.solid
			WHERE
				(".(count($notUngcod)>0 ? "ung.ungcod IN ('".implode('\',\'', $notUngcod)."') OR" : "1=1 OR")."
				".(count($notUnicod)>0 ? "uni.unicod IN ('".implode('\',\'', $notUnicod)."') " : "1=1").") AND
				".(count($notEmpcod)>0 ? "sol.empcod IN ('".implode('\',\'', $notEmpcod)."') AND" : "")."
				".($trava ? "1=0 AND" : "")."
				".implode(' AND ',$where)." 
			ORDER BY
				2, 4
				";
//	ver($sql);
	$arCabecalho = array("&nbsp","&nbsp","Data de Inclusão", "CPF/Número de Documento", "Nome","E-mail","Banco","Agência", "Conta Corrente", "Código SCDP", "Unidade Gestora/Orçamentária", "Empenho", "Situação", "Valor Itinerário (R$)", "Deduções (R$)", "Valor Total da Ajuda de Custo (R$)"); 
	unset($_REQUEST['req']);
	unset($_POST['req']);
	
	$db->monta_lista( $sql, $arCabecalho, 20, 10, 'N', '');
	
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

$notUngcod = Array();
$notUnicod = Array();
if(!$db->testa_superuser()){
	
	$arPerfisSolicitacaoAjudaCusto = array(EVENTO_PERFIL_SOLICITADOR, 
					 	 EVENTO_PERFIL_VALIDADOR,
					 	 EVENTO_PERFIL_AGENTE_FINANCEIRO,
					 	 EVENTO_PERFIL_AUTORIZADOR,
					 	 EVENTO_PERFIL_ADMINISTRADOR_DIARIAS
						);
	
	$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
	if( possuiPerfil( $arPerfisSolicitacaoAjudaCusto ) ){
		
		
		$sql = "SELECT DISTINCT 
					p.ungcod 
				FROM 
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidadegestora p on
					ur.ungcod = p.ungcod
				WHERE
					ur.rpustatus = 'A' and
					ur.usucpf = '".$_SESSION['usucpf']."' and
					ur.pflcod IN (".implode(',',$perfil).") and
					ur.prsano = '".$_SESSION['exercicio']."'";
		$notUngcod = $db->carregarColuna($sql);
		array_push($notUngcod,'000000');
		
		$sql = "SELECT DISTINCT 
					p.unicod as codigo, 
					p.unicod || ' - ' || p.unidsc as descricao
				FROM 
					evento.usuarioresponsabilidade ur
				INNER JOIN public.unidade p ON ur.unicod = p.unicod
				WHERE
					ur.rpustatus = 'A' and
					ur.usucpf = '".$_SESSION['usucpf']."' and
					ur.pflcod in ( ".implode(',',$perfil)." ) and
					ur.prsano = '".$_SESSION['exercicio']."'";
		
		$notUnicod = $db->carregarColuna($sql);
		array_push($notUnicod,'00000');
	}else{
		echo "<script>
				alert('Acesso negado.');
				window.history.back(-1); 
			  </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$abas = Array(Array('descricao'=>'Lista de Solicitação de Diárias',
			  		'link'     =>'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A'),
			  Array('descricao'=>'Solicitações de Diárias - Escolha de Empenho ',
		  			'link'     =>'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A'));
if(possuiPerfil( Array(EVENTO_PERFIL_AGENTE_FINANCEIRO,EVENTO_PERFIL_ADMINISTRADOR_DIARIAS) )){
	array_push($abas, Array('descricao'=>'Solicitações de Diárias - Tramitação em Lotes',
		  				   'link'     =>'evento.php?modulo=principal/tramitaSolicitacaoDiarias&acao=A')
			 );
}
$url = 'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A';

echo montarAbasArray($abas, $url);
monta_titulo( 'Lista de Solicitações de Ajuda de Custo', '' );

?>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function() {

	jQuery('#trocacpf').click(function(){
		jQuery('#trcpf').hide();
		jQuery('#trnumdoc').show();
	});

	jQuery('#trocanumdoc').click(function(){
		jQuery('#trnumdoc').hide();
		jQuery('#trcpf').show();
	});

	jQuery('#inserir').click(function(){
		window.location = 'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A';
	});

	jQuery('#limpar').click(function(){
		jQuery('.normal').each(function(){
			jQuery(this).val('');
		});
		jQuery('.CampoEstilo').each(function(){
			jQuery(this).val('');
		});
		jQuery('#pesquisar').click();
	});

	jQuery('.imprimir').live('click',function(){
	    windowOpen('evento.php?modulo=principal/impressaoSolicitacaoDiarias&acao=A&solid='+jQuery(this).attr('id'),
	                      'selecionaMunicipios', 'height=800,width=1024,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
	});


	jQuery('#pesquisar').click(function(){
		
		jQuery(this).attr('disabled',true);

		if( !validar_cpf( jQuery('#cpf').val() ) && jQuery('#cpf').val() != '' ){
			alert('Cpf inválido!');
			jQuery(this).focus();
			jQuery(this).attr('disabled',false);
			return false;
		}

		if( !validaEmail(jQuery('#email').val()) && jQuery('#email').val() != '' ){
			alert('E-mail inválido');
			jQuery(this).attr('disabled',false);
			jQuery('#email').focus();
			return false;
		}
		
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=listaSolicitacoes&"+jQuery('#formListaSolDia').serialize(),
			async: false,
			success: function(msg){
				jQuery('#listaSolicitacoes').html(msg);
				jQuery('#pesquisar').attr('disabled',false);
			}
		});
	});

	jQuery('#ungcod').change(function(){

		var ungcod = jQuery(this).val();

		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=montacomboEmpenho&ungcod="+ungcod,
			async: false,
			success: function(msg){
				jQuery('#tdEmpenho').html(msg);
			}
		});
	});
	
});

</script>
<form method="post" name="formListaSolDia" id="formListaSolDia" action="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr id="trcpf">
			<td class="SubTituloDireita">CPF</td>
			<td>
				<?php 
					$cpf = $_POST['cpf']; 
					echo campo_texto( 'cpf', 'N', 'S', '', 16, 15, '###.###.###-##', '','','','','id="cpf"' ); 
				?>
				<a id="trocacpf" style="color: blue">É estrangeiro?</a>
			</td>
		</tr>
		<tr id="trnumdoc" style="display:none">
			<td class="SubTituloDireita">N° do Documento:<br /> (Estrangeiro)</td>
			<td>
				<?php 
					$numdocestrangeiro = $_POST['numdocestrangeiro']; 
					echo campo_texto( 'numdocestrangeiro', 'N', 'S', '', 32, 30, '###.###.###-##', '','','','','id="numdocestrangeiro"' ); 
				?>
				<a id="trocanumdoc" style="color: blue">Não é estrangeiro?</a>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome</td>
			<td>
				<?php 
					$nome = $_POST['nome']; 
					echo campo_texto( 'nome', 'N', 'S', '', 50, 200, '', '','','','','id="nome"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">E-mail</td>
			<td>
				<?php 
					$email = $_POST['email']; 
					echo campo_texto( 'email', 'N', 'S', '', 25, 50, '', '','','','','id="email"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Banco</td>
			<td>
				<?php 
					$banco = $_POST['banco']; 
					echo campo_texto( 'banco', 'N', 'S', '', 5, 3, '', '','','','','id="banco"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Agência</td>
			<td>
				<?php 
					$agencia = $_POST['agencia']; 
//					echo campo_texto( 'agencia', 'N', 'S', '', 10, 6, '####-#', '','','','','id="agencia"' ); 
					echo campo_texto( 'agencia', 'N', $disabled, '', 8, 10, '', '','','','','id="agencia"' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Conta Corrente</td>
			<td>
				<?php 
					$conta = $_POST['conta']; 
//					echo campo_texto( 'conta', 'N', 'S', '', 20, 12, '##.###.###-#', '','','','','id="conta"' );
					echo campo_texto( 'conta', 'N', $disabled, '', 18, 20, '', '','','','','id="conta"' );   
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Código SCDP</td>
			<td>
				<?php 
					$codSCDP = $_POST['codSCDP']; 
					echo campo_texto( 'codSCDP', 'N', 'S', '', 25, 20, '######/##', '','','','','id="codSCDP"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade Gestora/Orçamentária</td>
			<td>
			<?php 
//				// Unidades Gestoras
//				$sql = "SELECT 
//							ungcod as codigo, 
//							ungdsc as descricao
//						FROM 
//							public.unidadegestora
//						WHERE
//							ungcod NOT IN ('152004','152005') AND
//							".(count($notUngcod)>0 ? "ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
//							ungstatus = 'A' 
//						GROUP BY 
//							ungcod, 
//							ungdsc
//						ORDER BY 
//							ungdsc;";
//				$db->monta_combo('ungcod',$sql,'S','Selecione...','','','','','S', 'ungcod');
			?>
			<select style="width: 200px;" class="CampoEstilo" name="ungcod"  id="ungcod">
					<option value="" />Selecione... </option>
					<optgroup label="Unidade Gestora">
						<?php
							$sql = "SELECT 
										ungcod as codigo, 
										ungdsc as descricao
									FROM 
										public.unidadegestora
									WHERE
										ungcod NOT IN ('152004','152005') AND
										".(count($notUngcod)>0 ? "ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
										ungstatus = 'A' 
									GROUP BY 
										ungcod, 
										ungdsc
									ORDER BY 
										ungdsc;";
						?>
						<?php 
							$opt = $db->carregar($sql);
							if( is_array($opt) ){
								foreach( $opt as $tipo ){ 
							?>
						<option value="<?= $tipo['codigo'] ?>"<?=$emp['ungcod'] == $tipo['codigo'] ? 'selectec="selected"' : ''; ?>><?= $tipo['descricao'] ?></option>
						<?php 	}
							} ?>
					</optgroup>
					<optgroup label="Unidade Orçamentária">
						<?php 
							 $sql = "SELECT 
										unicod as codigo, 
										unidsc as descricao
									FROM 
										public.unidade
									WHERE
										unistatus = 'A' AND
										unitpocod = 'U' AND
										".(count($notUnicod)>0 ? "unicod IN ('".implode('\',\'', $notUnicod)."') AND" : "")."
										orgcod = '26000' AND
										unicod != '26101' AND
										unicod ilike '26%'
									GROUP BY 
										unicod, 
										unidsc
									ORDER BY 
										unicod;";
						?>
						<?php 
							$opt = $db->carregar($sql);
							if( is_array($opt) ){
								foreach( $opt as $tipo ){ ?>
						<option value="<?= $tipo['codigo'] ?>"<?=$emp['unicod'] == $tipo['codigo'] ? 'selected="selected"' : ''; ?> ><?= $tipo['descricao'] ?></option>
						<?php 	}
							} ?>
					</optgroup>
				</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Empenho</td>
			<td id="tdEmpenho">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação da Solicitação</td>
			<td>
			<?php 
				// Unidades Gestoras
				$sql = "SELECT
							esdid as codigo,
							esddsc as descricao
						FROM
							workflow.estadodocumento
						WHERE
							tpdid = 42 AND esdid NOT IN (".WF_SOLICITACAO_EXCLUIDA.",".WF_SOLICITACAO_COMPLEMENTADA.")
						ORDER BY 2";
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Foi complementada</td>
			<td>
				<input type="radio" name="comp" value="S"/> Sim &nbsp;
				<input type="radio" name="comp" value="N" checked="checked"/> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" >
			<?php 
				$arPerfisSolicitador = array( EVENTO_PERFIL_SOLICITADOR );
	
				if( possuiPerfil( $arPerfisSolicitador ) ){ ?>
				<div style="float:left">
					<input type="button" id="inserir" value="Inserir Nova Solicitação" />	
				</div>
			<?php }?>
				<input type="button" id="pesquisar" value="Pesquisar" />
				<div style="float:right">
					<input type="button" id="limpar" value="Limpar campos da pesquisa." />	
				</div>	
			</td>
		</tr>
	</table>
</form>
	<div id="listaSolicitacoes" ><?=listaSolicitacoes($_POST); ?></div>