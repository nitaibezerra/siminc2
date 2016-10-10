<?php 

$arEsdids = Array(WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO,
				  WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO,
				  WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO,
				  WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO);

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
				  
function tramitarSolicitacoes($dados){
	
	global $db;
	
//	ver($dados,d);
	
	if($dados['ordembancariaTodos']||is_array($dados['ordembancaria'])){
		
		$sql = '';
		
		foreach( $dados['docid'] as $solid => $docid){
			
			$ob = $dados['ordembancariaTodos'] ? trim($dados['ordembancariaTodos']) : trim($dados['ordembancaria'][$solid]);
			
			if( $ob != '' ){
				$sql .= "UPDATE evento.solicitacaodiaria SET
							solordembancaria = ".$ob."
						WHERE
							solid = ".$solid.";";
			}
			
		}
		
		if( $sql != '' ){
			if(!$db->executar($sql)){
				$db->rollback();
				echo "<script>
						alert('Ordem Bancária em Branco.');
						window.history.back(-1); 
					  </script>";
			}
		}
	}
	
	foreach( $dados['docid'] as $solid => $docid){
//			ver($docid, $dados['aedid'],d);
			$_SESSION['evento']['solid'] = $solid;
			wf_alterarEstado( $docid, $dados['aedid'], 'tramita em lote', array( 'solid' => $solid ) );
	}
	unset($_SESSION['evento']['solid']);
	
	$db->commit();
	$db->sucesso('principal/tramitaSolicitacaoDiarias','');
}

function montaAcoes($dados){
	
	global $db;
	
	$sql = 'SELECT 
				aedid as codigo,
				aeddscrealizar as descricao
			FROM 
				workflow.acaoestadodoc
			WHERE
				esdidorigem = '.$dados['esdid'];
	
	$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
}


function listaSolicitacoes( $dados = null ){
	
	global $db;
	
	$notUngcod = Array();
	$trava = false;
	if(!$db->testa_superuser()){
		
		$arPerfisSolicitacaoAjudaCusto = Array(EVENTO_PERFIL_SOLICITADOR, 
						 	 EVENTO_PERFIL_VALIDADOR,
						 	 EVENTO_PERFIL_AGENTE_FINANCEIRO,
						 	 EVENTO_PERFIL_AUTORIZADOR
							);
		
		$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
		if( possuiPerfil( $arPerfisSolicitacaoAjudaCusto ) ){
			
			
			$sql = "SELECT DISTINCT 
						p.ungcod,
						emu.empcod 
					FROM 
						evento.usuarioresponsabilidade ur 
					INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
					INNER JOIN evento.empenho_unidadegestora emu ON emu.ungcod = ur.ungcod
					INNER JOIN evento.empenho_responssavel   urp ON urp.emuid  = emu.emuid
					WHERE
						ur.rpustatus = 'A' and
						ur.usucpf = '".$_SESSION['usucpf']."' and
						ur.pflcod IN (".implode(',',$perfil).") and
						ur.prsano = '".$_SESSION['exercicio']."'";
			$notUngcod = $db->carregarColuna($sql);
			$notEmpcod = $db->carregarColuna($sql,2);
			if(!is_array($notUngcod)){
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
	
	$arPerf = Array(EVENTO_PERFIL_VALIDADOR,
					EVENTO_PERFIL_AGENTE_FINANCEIRO,
					EVENTO_PERFIL_AUTORIZADOR);
//	ver($dados,d);				
	if( is_array($dados) ){
		
		if( ($perfil[0] == EVENTO_PERFIL_SOLICITADOR && !$db->testa_superuser() ) || $dados['esdid'] == EVENTO_PERFIL_WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAOSOLICITADOR )		 	 
										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO.''); $esdid = WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO; }
		if( ($perfil[0] == EVENTO_PERFIL_VALIDADOR && !$db->testa_superuser() ) || $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO )		 	 
										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO.''); $esdid = WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO; }
		if( ($perfil[0] == EVENTO_PERFIL_AUTORIZADOR && !$db->testa_superuser() ) || $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO )		 	 
										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO.''); $esdid = WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO; }
		if( ($perfil[0] == EVENTO_PERFIL_AGENTE_FINANCEIRO && !$db->testa_superuser() ) || $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO )		 	 
										 { array_push($where, 'esd.esdid = '.WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO.''); 
										   $coluna = "'<input type=\"text\" class=\"normal ob\" id=\"ordembancaria'||sol.solid||'\" 
										   					  onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" 
										   					  onfocus=\"MouseClick(this);this.select();\" onmouseover=\"MouseOver(this);\" 
										   					  value=\"\" maxlength=\"12\" size=\"16\" name=\"ordembancaria['||sol.solid||']\" 
										   					  style=\"text-align:;\">
										   			   <input type=\"hidden\" class=\"solid\" value=\"'||sol.solid||'\"/>' as ordembanc,";
										   $esdid  = WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO; }
		if( $dados['esdid'] )			 { array_push($where, 'esd.esdid = '.$dados['esdid']); }
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
		if($dados['esdid'])				 { array_push($where, 'esd.esdid = '.$dados['esdid'].''); }
		if($dados['comp']=='S')			 { array_push($where, 'sol.solcomplemento is not null'); }
	}
	
	$sql = "SELECT DISTINCT
				'<div align=\"center\">
					<input type=\"checkbox\" class=\"doc\" id=\"docid'||sol.solid||'\" name=\"docid['||sol.solid||']\" value=\"'||sol.docid||'\" />
				</div>'  as mais,
				to_char(sol.soldatainclusao,'DD/MM/YYYY') as datainclusao,
				CASE WHEN usucpf is not null
					THEN replace(to_char(usucpf::bigint, '000:000:000-00'), ':', '.')
					ELSE solnumdocestrangeiro||' '
				END as id,
				'<a onclick=\"windowOpen(\'evento.php?modulo=principal/impressaoSolicitacaoDiarias&acao=A&solid='||sol.solid||'\',
	                      \'selecionaMunicipios\', \'height=800,width=1024,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes\')\" style=\"color:blue\">'||solnome||'</a>' as solnome,  
				solemail,
				solbanco||' ' as banco, 
				solagencia||' ' as agencia, 
				solconta||' ' as conta, 
				CASE WHEN solcodigoscdb = 'NULL'
					THEN ''
					ELSE solcodigoscdb
				END AS codigoscdb,
				ung.ungdsc,
				sol.empcod,
				esd.esddsc,
				".$coluna."
				val.valor as val,
				round(soldeducaoalim+(soldeducaotransp*soltotdias),2) as deducao,
				round(val.valor-(soldeducaoalim+(soldeducaotransp*soltotdias)),2) as totres
			FROM 
				evento.solicitacaodiaria sol
			INNER JOIN workflow.documento 	   	      	  doc ON doc.docid  = sol.docid
			INNER JOIN workflow.estadodocumento 	      esd ON esd.esdid  = doc.esdid
			INNER JOIN evento.empenho_unidadegestora      emu ON emu.empcod = sol.empcod
			INNER JOIN public.unidadegestora 	      	  ung ON ung.ungcod = sol.ungcod
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
				".(count($notUngcod)>0 ? "ung.ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
				".(count($notEmpcod)>0 ? "sol.empcod IN ('".implode('\',\'', $notEmpcod)."') AND" : "")."
				".($trava ? "1=0 AND" : "")."
				".implode(' AND ',$where)."
			ORDER BY
				2,4
				";
//	ver($sql,d);
	$arCabecalho = Array("&nbsp","Data de Inclusão", "CPF/Número de Documento", "Nome","E-mail","Banco","Agência", "Conta Corrente", "Código SCDP", "Unidade Gestora", "Empenho", "Situação", "Valor Itinerário (R$)", "Deduções (R$)", "Valor Total da Ajuda de Custo (R$)");
	$arTipo      = Array("","","","","","","","","","","","","numeric","numeric","numeric"); 
	if( ($perfil[0] == EVENTO_PERFIL_AGENTE_FINANCEIRO && !$db->testa_superuser() ) || $dados['esdid'] == WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO )		 	 
	{  
			$arCabecalho = Array("&nbsp","Data de Inclusão", "CPF/Número de Documento", "Nome","E-mail","Banco","Agência", "Conta Corrente", "Código SCDP", "Unidade Gestora", "Empenho", "Situação", "Ordem Bancária", "Valor Itinerário (R$)", "Deduções (R$)", "Valor Total da Ajuda de Custo (R$)");
			$arTipo      = Array("","","","","","","","","","","","","","numeric","numeric","numeric");  
	}
	unset($_REQUEST['req']);
	unset($_POST['req']);
	
//	$db->monta_lista( $sql, $arCabecalho, 100, 10, 'N', '');
	$linhas = $db->carregar($sql);
	
	$table = '<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
				<tr style="background-color: rgb(230,230,230)">';
	foreach($arCabecalho as $cabecalho){
		$table .= '<td valign="top" bgcolor="" align="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#c0c0c0\';" 
					    style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" 
					    class="title"><strong>'.$cabecalho.'</strong>
					</td>';
	}
	
	$table .= '</tr>';
	if(is_array($linhas)){
		foreach($linhas as $k=>$linha){
			
			if($k%2==0){
				$cor = '#F7F7F7';
			}else{
				$cor = '';
			}
			
			$table .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
			
			$y = 0;
			foreach($linha as $coluna){
				if($arTipo[$y]=='numeric'){
					$estilo = 'align="right" style="color:blue"';
					$coluna = formata_valor($coluna);
				}else{
					$estilo = 'align="left"';
					$coluna = $coluna;
				}
				$table .= ' <td '.$estilo.' title="'.$arCabecalho[$y].'">
								'.$coluna.'
							</td>';
				$y++;
			}
			
			$table .= '</tr>';
		}
	}else{
		$table .= '<tr bgcolor="#F7F7F7" onmouseout="this.bgColor=\'#F7F7F7\';" onmouseover="this.bgColor=\'#ffffcc\';">
						<td align="center" style="color:red" colspan="'.count($arCabecalho).'">
							Nenhum registro encontrado.
						</td>
					</tr>';
	}

	$table .= '</table>';

	echo $table;
}

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

$notUngcod = Array();
$esdid = Array();
if(!$db->testa_superuser()){
	
	$arPerfisSolicitacaoAjudaCusto = array(EVENTO_PERFIL_SOLICITADOR, 
					 	 EVENTO_PERFIL_VALIDADOR,
					 	 EVENTO_PERFIL_AGENTE_FINANCEIRO,
					 	 EVENTO_PERFIL_AUTORIZADOR
						);
	
//	$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
	$sql = "select 
				p.pflcod 
			from 
				seguranca.perfilusuario pu 
			inner join seguranca.perfil p on pu.pflcod = p.pflcod 
			where 
				pu.usucpf = '".$_SESSION['usucpf']."' 
				and p.pflstatus = 'A' 
				and p.sisid = ".$_SESSION['sisid']."
				AND p.pflcod in (".implode(',',$arPerfisSolicitacaoAjudaCusto).");";
	$perfil = $db->carregarColuna($sql);
	
	if( is_array($perfil) ){
		
		if( in_array(EVENTO_PERFIL_SOLICITADOR,$perfil) )		 	 
										 { array_push($esdid, WF_SOLICITAÇÃO_DIARIAS_EM_SOLICITACAO); }
		if( in_array(EVENTO_PERFIL_VALIDADOR,$perfil) )		 	 
										 { array_push($esdid, WF_SOLICITAÇÃO_DIARIAS_EM_VERIFICACAO); }
		if( in_array(EVENTO_PERFIL_AUTORIZADOR,$perfil) )		 	 
										 { array_push($esdid, WF_SOLICITAÇÃO_DIARIAS_EM_AUTORIZACAO); }
		if( in_array(EVENTO_PERFIL_AGENTE_FINANCEIRO,$perfil) )		 	 
										 { array_push($esdid, WF_SOLICITAÇÃO_DIARIAS_EM_PAGAMENTO); }
	}
	
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
	}else{
		echo "<script>
				alert('Acesso negado.');
				window.history.back(-1); 
			  </script>";
	}
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$abas = Array(Array('descricao'=>'Lista de Solicitação de Diarias',
			  		'link'     =>'evento.php?modulo=principal/listaSolicitacaoDiarias&acao=A'),
			  Array('descricao'=>'Solicitações de Diárias - Escolha de Empenho ',
		  			'link'     =>'evento.php?modulo=principal/empenhoSolicitacaoDiarias&acao=A'),
			  Array('descricao'=>'Solicitações de Diárias - Tramitação em Lotes',
		  			'link'     =>'evento.php?modulo=principal/tramitaSolicitacaoDiarias&acao=A')
				);
$url = 'evento.php?modulo=principal/tramitaSolicitacaoDiarias&acao=A';

echo montarAbasArray($abas, $url);
monta_titulo( 'Solicitações de Diárias - Tramitação em Lotes', '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function() {

//	var esdid = '<?=$esdid ?>';

	function atualizaOB(){
		jQuery('.solid').each(function(){
			if(jQuery('#docid'+jQuery(this).val()).attr('checked')==true){
				jQuery('#ordembancaria'+jQuery(this).val()).val(jQuery('#ordembancaria').val());
			}else{
				jQuery('#ordembancaria'+jQuery(this).val()).val('');
			}
		});
	}
	
	jQuery('#ordembancaria').attr('disabled',true);
	jQuery('#ordembancaria').removeClass('normal');
	jQuery('#ordembancaria').addClass('disabled');

	jQuery('.obBtn').click(function (){
		if(jQuery(this).val()=='n'){
			jQuery('#ordembancaria').attr('disabled',true);
			jQuery('#ordembancaria').removeClass('normal');
			jQuery('#ordembancaria').addClass('disabled');
			jQuery('.ob').each(function(){
				jQuery(this).attr('disabled',false);
				jQuery(this).addClass('normal');
				jQuery(this).removeClass('disabled');
			});
		}else if(jQuery(this).val()=='s'){
			jQuery('#ordembancaria').attr('disabled',false);
			jQuery('#ordembancaria').addClass('normal');
			jQuery('#ordembancaria').removeClass('disabled');
			jQuery('.ob').each(function(){
				jQuery(this).attr('disabled',true);
				jQuery(this).removeClass('normal');
				jQuery(this).addClass('disabled');
			});
		}
	});

	jQuery('#ordembancaria').keyup(function(){
		atualizaOB();
	});

	jQuery('.doc').live('click',function(){
		atualizaOB();
	});

	jQuery('#esdid').change(function(){

		var esdid = jQuery(this).val();

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
		
		if(esdid==293){
			jQuery('#trOB').show();
		}else{
			jQuery('#trOB').hide();
		}
		jQuery(this).attr('disabled',false);
		jQuery('#req').val('listaSolicitacoes');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "&esdid="+esdid+"&"+jQuery('#formTramitaSolDia').serialize(),
			async: false,
			success: function(msg){
				jQuery('#listaSolicitacoes').html(msg);
			}
		});
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "req=montaAcoes&esdid="+esdid,
			async: false,
			success: function(msg){
				jQuery('#tdAcao').html(msg);
			}
		});
		
	});

	jQuery('#tramita').click(function(){
		var selecionado = false;
		jQuery(this).attr('disabled',true);
		jQuery('.doc').each(function(){
			if(jQuery(this).attr('checked')==true){
				selecionado = true;
			}
		});
		var pendencia = false;
		jQuery('.obrigatorio').each(function(){
			var val = jQuery.trim(jQuery(this).val());
			if( val.length == 0 ){
				jQuery(this).focus();
				pendencia = true;
				jQuery(this).attr('disabled',false);
				return false;
			}
		});
				
		if( pendencia ){alert('Campo obrigatório.');jQuery(this).attr('disabled',false); return false; }
		if( jQuery('#esdid').val()==293 && jQuery('#aedid').val()==761 ){
			var obvazio = false;
			jQuery('.solid').each(function(){
				if(jQuery('#docid'+jQuery(this).val()).attr('checked')==true){
					if( trim(jQuery('#ordembancaria'+jQuery(this).val()).val()) == ''){
						obvazio = true;
						jQuery('#ordembancaria'+jQuery(this).val()).focus();
					};
				}
				if(obvazio){
					jQuery(this).attr('disabled',false);
					return false;
				}
			});
			if( obvazio ){
				jQuery(this).attr('disabled',false);
				alert('Ordem Bancária não informada.');
				return false;
			}
		}
		if(selecionado){
			if(confirm('Deseja '+jQuery('#aedid option:selected').html()+'?')){
				jQuery('#req').val('tramitarSolicitacoes');
				jQuery('#formTramitaSolDia').submit();
			}
			jQuery(this).attr('disabled',false);
			return false;
		}else{
			alert('Selecione pelo menos uma solicitação.');
			jQuery(this).attr('disabled',false);
			return false;
		}
	});

	jQuery('#trocacpf').click(function(){
		jQuery('#trcpf').hide();
		jQuery('#trnumdoc').show();
	});

	jQuery('#trocanumdoc').click(function(){
		jQuery('#trnumdoc').hide();
		jQuery('#trcpf').show();
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

		jQuery('#req').val('listaSolicitacoes');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: jQuery('#formTramitaSolDia').serialize(),
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
<form method="post" name="formTramitaSolDia" id="formTramitaSolDia" action="">
	<input type="hidden" id="req" name="req" value=""/>
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
										orgcod = '". CODIGO_ORGAO_SISTEMA. "' AND
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
			<td class="SubTituloDireita">Foi complementada</td>
			<td>
				<input type="radio" name="comp" value="S"/> Sim &nbsp;
				<input type="radio" name="comp" value="N" checked="checked"/> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" >
				<input type="button" id="pesquisar" value="Pesquisar" />
				<div style="float:right">
					<input type="button" id="limpar" value="Limpar campos da pesquisa." />	
				</div>	
			</td>
		</tr>
		<?php if($db->testa_superuser()){?>
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
							tpdid = 42 AND esdid in (".implode(',',$arEsdids).")
						ORDER BY 2";
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
			?>
			</td>
		</tr>
		<?php }else{?>
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
							tpdid = 42 AND esdid in (".implode(',',$esdid).")
						ORDER BY 2";
				$db->monta_combo('esdid',$sql,'S','Selecione...','','','','','S', 'esdid');
			?>
			</td>
		</tr>
		<?php }?>
		<tr id="trOB" style="display:none" >
			<td class="SubTituloDireita">Ordem bancária para todos?</td>
			<td>
				<input type="radio" class="obBtn" name="obBtn" value="n" checked="checked"/> Não
				<input type="radio" class="obBtn" name="obBtn" value="s"/> Sim
				&nbsp;
				<?php 
					$ordembancaria = $dados[0]['solordembancaria']; 
					echo campo_texto( 'ordembancariaTodos', 'N', 'S', '', 15, 12, '', '','','','','id="ordembancaria"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ações de Tramitação</td>
			<td id="tdAcao">
				<?php 
//				if(!$db->testa_superuser()){
//					$sql = 'SELECT 
//								aedid as codigo,
//								aeddscrealizar as descricao
//							FROM 
//								workflow.acaoestadodoc
//							WHERE
//								esdidorigem in '.$esdid;
//					
//					$db->monta_combo('aedid',$sql,'S','Selecione...','','','','','S', 'aedid');
//				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" id="tdacao" >
				<input type="button" id="tramita" value="Tramitar" />	
			</td>
		</tr>
	</table>
	<div id="listaSolicitacoes" >
	<?php //if($esdid){
		//$dados['esdid'] = $esdid; ?>
<!--		<?php//listaSolicitacoes($dados); ?>-->
	<?php //}?>
	</div>
</form>