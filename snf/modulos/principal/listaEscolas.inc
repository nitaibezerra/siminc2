<?php
	//error_reporting(-1);
	//Dados Curso POPUP
	if ($_REQUEST['dadosCursoPopup']) {
		header('content-type: text/html; charset=ISO-8859-1');
		echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/><link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
		dadosCursoPopup($_GET['curid']);
		exit;
	}
	
	if($_REQUEST['vagas'] == 'disponivel'){
		extract($_POST);
		
		$stWhere = '';
		if($muncod){
			$stWhere .= " AND muncod = '{$muncod}' ";
		}
			
		/*$curid = str_replace('null,', '', $curid);
		$pdeid = str_replace('null,', '', $pdeid);

		$arPriid = array();		
		if(($curid && $curid != 'null') && ($pdeid && $pdeid != 'null')){
			$arCurid = explode(',', $curid);
			$arPdeid = explode(',', $pdeid);
			if(count($arCurid)){
				foreach($arCurid as $k => $v){
					if($arCurid && $arPdeid[$k] != ''){
						$sql = "Select priid From snf.prioridadecursoescola Where curid in ({$v}) and pdeid in ({$arPdeid[$k]})";
						$arPriid[] = $db->pegaUm($sql);
					}
				}
				print_r($arPriid);
			}
		}

		if(count($arPriid)){
			$stWhere .= " AND priid not in (".implode(',', $arPriid).") "; 
		}*/
		
		$sql = "
			Select coalesce(sum(privagasprevistas),0) as vagas 
			From snf.prioridadecursoescola
			Where estuf = '{$estuf}' and pdiesfera = ".($muncod ? "'Municipal'" : "'Estadual'")." and pristatus = 'A' and pcfid in (1,2)
		".$stWhere;
		
		//die($sql);
		$rs = 0;
		$rs = $db->pegaUm($sql);
		//echo 'VD: '.$_SESSION['vagasDisponiveis'].' - VS: '.$_SESSION['vagasSolicitadas'].' - sql: ';
		echo $rs;
		exit;
	}
	
	if($_REQUEST['vagas'] == 'nulo'){
		
		extract($_POST);
		$stWhere = '';
		if($muncod){
			$stWhere .= " AND pce.muncod = '{$muncod}' ";
		}
		
		$sql = "
			Select	pce.pdenome,
					cur.curdesc
			From snf.prioridadecursoescola pce
			Left Join catalogocurso.curso cur on cur.curid = pce.curid
			Where pce.estuf = '$estuf' and pcfano in (2012,2013) and pce.pdiesfera = ".($muncod ? "'Municipal'" : "'Estadual'")." 
			and pce.privagasprevistas is null and pce.pristatus = 'A' and pce.pcfid in (1,2)
			{$stWhere}
		";
		$rsEscolas = $db->carregar($sql);
		
		if($rsEscolas){
			$stEscolas = array();
			foreach($rsEscolas as $dados){
				$stEscolas[] = $dados['pdenome']." ({$dados['curdesc']})";
			}
			$stEscolas = implode(', ',$stEscolas);
		}else{
			$stEscolas = 'false';
		}
		echo $stEscolas;
		exit;
	}
	
	if($_REQUEST['requisicao']){
		$_REQUEST['requisicao']();
	}
	
	if($_POST['requisicao'] == "pesquisarCursos"){
		extract($_POST);
	}
	
	include  APPRAIZ."includes/cabecalho.inc";
	
	$sql = "select muncod, estuf from snf.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}'";
	$rsResponsabilidade = $db->pegaLinha($sql);
	
	// Monta filtros
	$arWhere = array();
	
	if(checkPerfil(array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PERFIL_EQUIPE_MUNICIPAL)) && $rsResponsabilidade['muncod']){
		array_push($arWhere, "pd.muncod = '{$rsResponsabilidade['muncod']}'");
		array_push($arWhere, "pd.pdiesfera = 'Municipal'");
		$_SESSION['snf']['esfera'] = 'Municipal';
	}elseif(checkPerfil(array(PERFIL_EQUIPE_ESTADUAL_APROVACAO, PERFIL_EQUIPE_ESTADUAL)) && $rsResponsabilidade['estuf']){
		array_push($arWhere, "pd.estuf = '{$rsResponsabilidade['estuf']}'");
		array_push($arWhere, "pd.pdiesfera = 'Estadual'");
		$_SESSION['snf']['esfera'] = 'Estadual';
	}
?>

<?php
	$arrPerfil = pegaPerfilGeral();
	if(in_array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil) || in_array(PERFIL_EQUIPE_MUNICIPAL,$arrPerfil)){
		$visivel = "none";
	}else{
		$visivel = "";
	}
?>
<br/>

<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

	function popupCurso(curid){
		var formulario = document.formulario_pesquisa;
		formulario.target = 'popupCurso';
		var janela = window.open( 'snf.php?modulo=principal/demandaSocial&acao=A&dadosCursoPopup=1&curid='+curid, 'popupCurso', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
		//formulario.submit();
	}

	function filtraCidade(estuf){
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
			   data: "requisicao=filtraCidade&estuf=" + estuf + "&obrigatorio=N",
			   success: function(msg){
			   		jQuery("#td_muncod").html(msg);
			   }
		});
	}
	
	function filtraCurso(ateid){
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
			   data: "requisicao=filtraCurso&ateid=" + ateid + "&obrigatorio=N",
			   success: function(msg){
			   		jQuery("#td_curid").html(msg);
			   }
		});
	}
	
	function importarEscolasPDEInterativo(estuf, muncod){
		jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
			   data: "requisicao=importarEscolas&estuf=" + estuf + "&muncod=" + muncod,
			   success: function(msg){
			   		if(msg == "ok"){
			   			window.location.href = window.location;
			   		}else{
			   			alert('Não foi possível importar as escolas.');
			   			//jQuery("#teste").html(msg);
			   		}
			   }
		});
	}
	
	function pesquisarCursos(){
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios.');
				this.focus();
				return false;
			}
		});

		if(erro == 0){
			jQuery("[name='mais_solicitados']").val("");
			jQuery("[name='requisicao']").val("pesquisarCursos");
			jQuery("[name='formulario_pesquisa']").submit();
		}
	}
	
	function verTodosCursos()
	{
		jQuery("[name='mais_solicitados']").val("");
		jQuery("[name='ateid']").val("");
		jQuery("[name='curid']").val("");
		jQuery("[name='requisicao']").val("pesquisarCursos");
		jQuery("[name='formulario_pesquisa']").submit();	
	}
		
	function expandirEscolas(curid, modalidade, pcfano)
	{
		//if(!jQuery("#td_curid_" + curid).html() || NovaOrdem != Antigaordem){
		if(!jQuery("#td_curid_" + curid+modalidade).html()){
			jQuery("#tr_curid_" + curid+modalidade).show();
			jQuery("#img_mais_curid_" + curid+modalidade).hide();
			jQuery("#img_menos_curid_" + curid+modalidade).show();
			jQuery("#td_curid_" + curid+modalidade).html("<center>Carregando...</center>");
			var estuf 	= jQuery("[name='estuf']").val();
			var muncod 	= jQuery("[name='muncod']").val();
			var modid 	= jQuery("[name='modid']").val();
			var ncuid 	= jQuery("[name='ncuid']").val();
			var pcfid 	= jQuery("[name='pcfid']").val();
			jQuery.ajax({
					   type: "POST",
					   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
					   data: "requisicao=exibirEscolasPorCurso&curid=" + curid + "&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&pcfid=" + pcfid + "&modid=" + modalidade,
					   success: function(msg){
					   		jQuery("#td_curid_" + curid+modalidade).html(msg);
					   		/*
					   		if(salva_prioridade){
								var prioridade = 1;
								jQuery("[name^='prioridade[" + curid + "][" + pcfid + "][']").each(function(i,j) {
									jQuery(j).val(prioridade);
									prioridade++;
								});
								salvarPrioridade(1);
					   		}
					   		*/
					   }
					 });
		}else{
			jQuery("#tr_curid_" + curid+modalidade).show();
			jQuery("#img_mais_curid_" + curid+modalidade).hide();
			jQuery("#img_menos_curid_" + curid+modalidade).show();
		}		
	}
	
	function esconderEscolas(curid)
	{
		jQuery("#tr_curid_" + curid).hide();
		jQuery("#img_mais_curid_" + curid).show();
		jQuery("#img_menos_curid_" + curid).hide();
	}
	
	function excluirEscola(priid,curid)
	{
		var tr = jQuery("#img_priid_" + priid).parent().parent();
		var tabela = tr.parent();
		
		if(confirm("Deseja realmente remover esta escola?")){
			jQuery.ajax({
				   type: "POST",
				   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
				   data: "requisicao=excluirEscola&priid=" + priid,
				   success: function(msg){
				   		if(msg == "ok"){
				   			tr.remove();
				   			var row_index = 0;
							jQuery(tabela).find('tr').each(function(i,j) {
								if(row_index % 2 == 1){
									jQuery(j).attr("bgcolor","#F7F7F7");
									jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","#F7F7F7") });
									jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
								}else{
									jQuery(j).attr("bgcolor","");
									jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","") });
									jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
								}
								row_index++;
							});
							var prioridade = 1;
							jQuery("[name^='prioridade[" + curid + "][']").each(function(i,j) {
								jQuery(j).val(prioridade);
								prioridade++;
							});
							salvarPrioridade(1);
				   		}else{
				   			alert('Não foi possível excluir a escola!');
				   		}
				   }
			});
		}
		
	}
	
	function excluirCurso(curid)
	{
		var tr = jQuery("#img_excluir_curid_" + curid).parent().parent();
		var tabela = tr.parent();
		
		var estuf 	= jQuery("[name='estuf']").val();
		var muncod 	= jQuery("[name='muncod']").val();
		var modid 	= jQuery("[name='modid']").val();
		var pcfid 	= jQuery("[name='pcfid']").val();
		var ncuid 	= jQuery("[name='ncuid']").val();
		
		if(confirm("Deseja realmente remover este curso e todas as escolas vinculadas?")){
			jQuery.ajax({
			   type	: "POST",
			   url	: 'snf.php?modulo=principal/listaEscolas&acao=A',
			   data	: "requisicao=excluirCurso&curid=" + curid + "&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&pcfid=" + pcfid + "&modid=" + modid,
			   success: function(msg){
		   		if(msg == "ok"){
			   			tr.remove();
			   			jQuery("#tr_curid_" + curid).remove();
			   			var row_index = 0;
						jQuery(tabela).find('tr').each(function(i,j) {
							if(row_index % 2 == 1){
								jQuery(j).attr("bgcolor","#F7F7F7");
								jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","#F7F7F7") });
								jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
							}else{
								jQuery(j).attr("bgcolor","#FFFFFF");
								jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","#FFFFFF") });
								jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
							}
							if( !jQuery(j).attr("id")) {
								row_index++;
							}
						});
			   		}else{
			   			alert('Não foi possível excluir a escola!');
			   		}
			   }
			});
		}
	}
	
	function cursosMaisSolicitados()
	{
		jQuery("[name='mais_solicitados']").val("1");
		jQuery("[name='requisicao']").val("pesquisarCursos");
		jQuery("[name='formulario_pesquisa']").submit();
	}
	
	function alteraPrioridadeCursoCombo(curid,ano)
	{		
		var ordem = jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + ano).val();
		var estuf 	= jQuery("[name='estuf']").val();
		var muncod 	= jQuery("[name='muncod']").val();
		var modid 	= jQuery("[name='modid']").val();
		var ncuid 	= jQuery("[name='ncuid']").val();
		var pcfid 	= jQuery("[name='pcfid']").val();
		jQuery("#td_curid_" + curid).html("<center>Carregando...</center>");
		jQuery.ajax({
				   type: "POST",
				   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
				   data: "requisicao=exibirEscolasPorCurso&curid=" + curid + "&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&pcfid=" + pcfid + "&modid=" + modid + "&ordem=" + ordem + "&ordem_ano=" + ano,
				   success: function(msg){
				   		jQuery("#td_curid_" + curid).html(msg);
						var prioridade = 1;
						jQuery("[name^='prioridade[" + curid + "][" + ano + "][']").each(function(i,j) {
							jQuery(j).val(prioridade);
							prioridade++;
						});
						salvarPrioridade(1);
				   }
				 });
	}
	
	function salvaTipoOrdem(curid, pcfid)
	{
		var NovaOrdem 	= jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + pcfid).val();
		var Antigaordem = jQuery("#hdn_ordem_curid_" + curid + "_pcfid_" + pcfid).val();
		if(NovaOrdem != Antigaordem){
			var estuf 	= jQuery("[name='estuf']").val();
			var muncod 	= jQuery("[name='muncod']").val();
			var modid 	= jQuery("[name='modid']").val();
			var ncuid 	= jQuery("[name='ncuid']").val();
			var ordem	= jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + pcfid).val();
			jQuery.ajax({
			   type: "POST",
			   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
			   data: "requisicao=salvarTipoOdemCurso&curid=" + curid + "&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&pcfid=" + pcfid + "&modid=" + modid + "&ordem=" + ordem
			 });
		}
	}
	
	function alteraPrioridade(curid,pdeid,pcfid)
	{
		var erro = 0;
		jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + pcfid).val("personalizado");
		salvaTipoOrdem(curid,pcfid);
		var qtde_escolas = jQuery("[name^='prioridade[" + curid + "][" + pcfid + "]']").length;
		var valor = jQuery("[name='prioridade[" + curid + "][" + pcfid + "][" + pdeid + "]']").val();
		if(valor == 0){
			jQuery("[name='prioridade[" + curid + "][" + pcfid + "][" + pdeid + "]']").val("");
			alert('A prioridade não pode ser 0.');
			var prioridade = 1;
			jQuery("[name^='prioridade[" + curid + "][" + pcfid + "][']").each(function(i,j) {
				jQuery(j).val(prioridade);
				prioridade++;
			});
			erro = 1;
			return false;
		}
		if(valor > qtde_escolas){
			jQuery("[name='prioridade[" + curid + "][" + pcfid + "][" + pdeid + "]']").val("");
			alert('A prioridade não pode ser maior que ' + qtde_escolas + ".");
			var prioridade = 1;
			jQuery("[name^='prioridade[" + curid + "][" + pcfid + "][']").each(function(i,j) {
				jQuery(j).val(prioridade);
				prioridade++;
			});
			erro = 1;
			return false;
		}
		if(erro == 0){
			var row = jQuery("[name='prioridade[" + curid + "][" + pcfid + "][" + pdeid + "]']").parent().parent().parent();
			var row_index = row.index() + 1;
			var tabela = row.parent();
			if(valor > row_index){
				row.insertAfter(jQuery(tabela).find('tr')[valor - 1]);
			}if(valor < row_index){
				row.insertBefore(jQuery(tabela).find('tr')[valor - 1]);
			}
			
			var row_index = 0;
			jQuery(tabela).find('tr').each(function(i,j) {
				if(row_index % 2 == 1){
					jQuery(j).attr("bgcolor","#F7F7F7");
					jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","#F7F7F7") });
					jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
				}else{
					jQuery(j).attr("bgcolor","");
					jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","") });
					jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
				}
				row_index++;
			});
			var prioridade = 1;
			jQuery("[name^='prioridade[" + curid + "][" + pcfid + "][']").each(function(i,j) {
				jQuery(j).val(prioridade);
				prioridade++;
			});
			return false;
		}
	}
	
	function aumentarPrioridade(curid,pdeid,pcfid)
	{
		var row = jQuery("[name='prioridade[" + curid + "][" + pcfid + "][" + pdeid + "]']").parent().parent().parent();
		jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + pcfid).val("personalizado");
		salvaTipoOrdem(curid,pcfid);
		var tabela = row.parent();
		if (row.prev().length > 0) {
			row.insertBefore(row.prev());
		}
		var row_index = 0;
		jQuery(tabela).find('tr').each(function(i,j) {
			if(row_index % 2 == 1){
				jQuery(j).attr("bgcolor","#F7F7F7");
				jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","#F7F7F7") });
				jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
			}else{
				jQuery(j).attr("bgcolor","");
				jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","") });
				jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
			}
			row_index++;
		});
		var prioridade = 1;
		jQuery("[name^='prioridade[" + curid + "][" + pcfid + "][']").each(function(i,j) {
			jQuery(j).val(prioridade);
			prioridade++;
		});
		return false;
	}
	
	function reduzirPrioridade (curid,pdeid,pcfid)
	{
		var row = jQuery("[name='prioridade[" + curid + "][" + pcfid + "][" + pdeid + "]']").parent().parent().parent();
		jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + pcfid).val("personalizado");
		salvaTipoOrdem(curid,pcfid);
		var tabela = row.parent();
		if (row.next().length > 0) {
			row.insertAfter(row.next());
		}
		var row_index = 0;
		jQuery(tabela).find('tr').each(function(i,j) {
			if(row_index % 2 == 1){
				jQuery(j).attr("bgcolor","#F7F7F7");
				jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","#F7F7F7") });
				jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
			}else{
				jQuery(j).attr("bgcolor","");
				jQuery(j).mouseout(function(){ jQuery(j).attr("bgcolor","") });
				jQuery(j).mouseover(function(){ jQuery(j).attr("bgcolor","#ffffcc") });
			}
			row_index++;
		});
		var prioridade = 1;
		jQuery("[name^='prioridade[" + curid + "][" + pcfid + "][']").each(function(i,j) {
			jQuery(j).val(prioridade);
			prioridade++;
		});
		return false;
	}
	
	function salvarPrioridade(alerta){
		
		var vagas_previstas = jQuery("[name^='vagas_previstas[']").serialize();
		var prioridade = jQuery("[name^='prioridade[']").serialize();
		var pcfano  = jQuery("[name^='pcfano[']").serialize();
		var modid 	= jQuery("[name^='modid[']").serialize();
		
		var estuf 	= jQuery("[name='estuf']").val();
		var muncod 	= jQuery("[name='muncod']").val();
		var ncuid 	= jQuery("[name='ncuid']").val();

		var curid = null, pdeid = null, vagas_solicitadas = 0;
		
		jQuery.each(jQuery("[name^='vagas_previstas']"), function(i,v){
			
			arIds = v.id.split('_');

			curid += ','+arIds[1];
			pdeid += ','+arIds[3];
			
			vagas_solicitadas = parseInt(vagas_solicitadas)+parseInt(v.value);
		});
		
		jQuery.ajax({
			url : '?modulo=principal/listaEscolas&acao=A',
			type: 'post',
			data: 'vagas=disponivel&estuf='+estuf+'&muncod='+muncod+'&curid='+curid+'&pdeid='+pdeid,
			success: function(e){
				
				vagas_solicitadas = parseInt(vagas_solicitadas)+parseInt(e);
				//vagas_solicitadas = parseInt(e);
				
				//if(vagas_solicitadas > jQuery('#vagas_disponiveis_id').val() ){
					
					//alert('O numero de vagas autorizadas ('+vagas_solicitadas+') é maior que o numero de vagas previstas ('+jQuery('#vagas_disponiveis_id').val()+')!');
					//return false;	
					
				//}else{

					jQuery.ajax({
						url: '?modulo=principal/listaEscolas&acao=A',
						type: 'post',
						data: 'vagas=nulo&estuf='+estuf+'&muncod='+muncod,
						success: function(e){
							if(e != 'false'){
								var stEscolas = '';
								arEscolas = e.split(',');
								
								jQuery.each(arEscolas, function(i,v){
									stEscolas += v+'\n';
								});
								
								//if(confirm('Existem escolas com vagas em branco: \n\n '+stEscolas+' \n Deseja continuar?')){
									jQuery.ajax({
										   type: "POST",
										   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
										   data: "requisicao=salvarPrioridadeCursos&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&" + modid + "&" + prioridade + "&" + vagas_previstas + '&' + pcfano,
										   success: function(msg){
											   if(msg == "ok"){
										   			if(!alerta){
										   				alert('Dados atualizados com sucesso!');
										   				var formulario = document.formulario_pesquisa;
										   				formulario.submit();
										   			}
										   		}
										   }
									 });
								//}
								if(stEscolas){
									//jAlert('Existem escolas com vagas em branco: \n\n '+stEscolas, 'Alert Dialog');
									alert('Existem escolas com vagas em branco: \n\n '+stEscolas+' \n');
								}
							}else{
								jQuery.ajax({
									   type: "POST",
									   url: 'snf.php?modulo=principal/listaEscolas&acao=A',
									   data: "requisicao=salvarPrioridadeCursos&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&" + modid + "&" + prioridade + "&" + vagas_previstas + '&' + pcfano,
									   success: function(msg){									
									   		if(msg == "ok"){
									   			if(!alerta){
									   				alert('Dados atualizados com sucesso!');
									   				var formulario = document.formulario_pesquisa;
									   				formulario.submit();
									   			}
									   		}
									   }
								 });
							}					
						}
					});					
				//}
			}
		});				
	}

	function verificaTotal(curid, pcfid, pdeid)
	{
		var vagas = document.getElementById('vagas_'+curid+'_'+pcfid+'_'+pdeid).value;
		var previstas = document.getElementById('previstas_'+curid+'_'+pcfid+'_'+pdeid).value;
		
		if(!vagas) vagas = 0;
		if(!previstas) previstas = 0;
		
		if(parseInt(previstas) > parseInt(vagas)){
			alert(' "O Plano Autorizado", não pode exceder o total do "Plano da Escola"!');
			document.getElementById('previstas_'+curid+'_'+pcfid+'_'+pdeid).value = '';
			return false;
		}
	}

function selecionaEstado(estuf){
	location.href = '?modulo=principal/listaEscolas&acao=A&estuf_selecionado=' + estuf;
}		

</script>

<div id="teste"></div>
<?php 
$estuf_selecionado		 			= (isset($_GET['estuf_selecionado']) ? $_GET['estuf_selecionado'] : $_SESSION['snf']['perfil']['estuf']);
$_SESSION['snf']['perfil']['estuf'] = $estuf_selecionado;
$estuf 					 			= $estuf_selecionado;
$arrTravaFiltro['estuf'] 			= $estuf_selecionado;

if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) ){
	$sql = "SELECT
				DISTINCT
				estuf AS codigo,
				estuf || ' - ' || estdescricao AS descricao
			FROM
				territorios.estado
			ORDER BY
				codigo, descricao";

	$htmlComboEstado = ' 	
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td colspan="2" class="SubtituloTabela bold" >Selecione um Estado</td>
			</tr>
			<tr class="center SubtituloDireita" >
				<td colspan="2">' 
					.
					$db->monta_combo("estuf_selecionado", $sql, "S", "Selecione um estado...", "selecionaEstado", "", "", '', "N", "", true)
					. '		
				</td>
			</tr>
		</table>';
					
	if ( empty($estuf_selecionado) ){
		echo $htmlComboEstado;
		die("<script>jQuery('#aguarde').hide();</script>");
	}
}else{
	$arrEstufMuncod = recuperaEstadoMunicipioPerfil();
	$arrEstufMuncod ? extract($arrEstufMuncod) : "";
	verificaImportacaoEscolas($arrEstufMuncod);
	$arrTravaFiltro['estuf'] = $arrEstufMuncod['estuf']; 
	$arrTravaFiltro['muncod'] = $arrEstufMuncod['muncod'];
}	
#********************************************************************************************
#@ Quarda em sess?o estado, munic?pio e esfera para uso em outros arquivos e fun??es.       *
#@ Exemplo arquivo sugestaoCurso - fun??o enviarcurso().									*
#********************************************************************************************
	$_SESSION['snf']['mun_cod'] = $arrTravaFiltro['muncod'];
	$_SESSION['snf']['est_uf'] =  $arrTravaFiltro['estuf'];
	
#@ FIm



	if(!$_POST['requisicao']){
		$_POST['pcfid'] = $_SESSION['snf']['formulario']['pcfid'] ? $_SESSION['snf']['formulario']['pcfid'] : false;
		$_POST['ateid'] = $_SESSION['snf']['formulario']['ateid'] ? $_SESSION['snf']['formulario']['ateid'] : false;
		$_POST['curid'] = $_SESSION['snf']['formulario']['curid'] ? $_SESSION['snf']['formulario']['curid'] : false;
		$_POST['modid'] = $_SESSION['snf']['formulario']['modid'] ? $_SESSION['snf']['formulario']['modid'] : false;
		$_POST['ncuid'] = $_SESSION['snf']['formulario']['ncuid'] ? $_SESSION['snf']['formulario']['ncuid'] : false;
		extract($_POST);
	}else{
		$_SESSION['snf']['formulario']['pcfid'] = $_POST['pcfid'];
		$_SESSION['snf']['formulario']['ateid'] = $_POST['ateid'];
		$_SESSION['snf']['formulario']['curid'] = $_POST['curid'];
		$_SESSION['snf']['formulario']['modid'] = $_POST['modid'];
		$_SESSION['snf']['formulario']['ncuid'] = $_POST['ncuid'];
	}
	
	$muncod = $_SESSION['snf']['mun_cod'];
	$estuf = $_SESSION['snf']['est_uf'];
	$esfera = $_SESSION['snf']['esfera'];	

	$db->cria_aba($abacod_tela,$url,''); 
	
	monta_titulo( 'Lista de Escolas' , obrigatorio()." Indica campos obrigatórios.");  
	
	cabecalhoListaEscolas($estuf, $muncod, $esfera);		
	
	echo $htmlComboEstado;
?>

<div id="teste"></div>
<form name="formulario_pesquisa" method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="mais_solicitados" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Filtros</td>
		</tr>
		<tr style="display:none" >
			<td width="25%" class="SubtituloDireita" >Estado:</td>
			<td>
				<?php 
					$sql = "
						Select	estuf as codigo,
								estdescricao as descricao
						From territorios.estado
						order by estdescricao
					"; 
				?>
				<?php $db->monta_combo("estuf",$sql,($arrTravaFiltro['estuf'] ? "N" : "S"),"Selecione...","filtraCidade","","","","","","",$estuf) ?>
			</td>
		</tr>
		<tr style="display:<?php echo $visivel ?>" >
			<td class="SubtituloDireita" >Município:</td>
			<td id="td_muncod" >
				<?php
					$sql = "Select	muncod as codigo,
				   					mundescricao as descricao
				   			From territorios.municipio
				   			where estuf = '$estuf'
				   			order by mundescricao";
					if(!$estuf){
						$db->monta_combo("muncod",$sql,"N","Selecione...","","","","200","N","","",$muncod);
					}else{
				   		$db->monta_combo("muncod",$sql,($arrTravaFiltro['muncod'] ? "N" : "S"),"Selecione...","","","","200","N","","",$muncod);
					}
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Área Temática:</td>
			<td>
				<?php $sql = "	select 
									ateid as codigo,
									atedesc as descricao
								from 
									catalogocurso.areatematica
								where 
									atestatus = 'A'
								order by 
									atedesc" ?>
				<?php $db->monta_combo("ateid",$sql,"S","Selecione...","filtraCurso","","","","N","","",$ateid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Cursos:</td>
			<td id="td_curid" >
				<?php if(!$ateid){
					$sql = "	select 
									1 as codigo,
									2 as descricao";
				}else{
					$sql = "	select 
									curid as codigo,
									curdesc as descricao
								from 
									catalogocurso.curso
								where 
									curstatus = 'A'
								and
									ateid = $ateid
								order by 
									curdesc";
				}
				if(!$ateid){
					$db->monta_combo("curid",$sql,"N","Selecione...","","","","200","N","","",$curid);
				}else{
			   		$db->monta_combo("curid",$sql,"S","Selecione...","","","","200","N","","",$curid);
				} ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Período:</td>
			<td>
				<?php 
					if(!$pcfid) 
						$pcfid = 1;
					$sql = "	
						Select	pcfid as codigo,
								pcfano as descricao
						from pdeinterativo.periodocursoformacao
						where pcfstatus = 'A'
						order by pcfano
					"; 
				?>
				<?php $arrPeriodo[0] = array("codigo" => "1,2", "descricao" => "2012/2013") ?>
				<?php $arrPeriodo[1] = array("codigo" => "3", "descricao" => "2014") ?>
				<?php $arrPeriodo[2] = array("codigo" => "4", "descricao" => "2015") ?>
				<?php $arrPeriodo[3] = array("codigo" => "10", "descricao" => "2016") ?>
				<?php $db->monta_combo("pcfid",$arrPeriodo,"S","","","","","","S","pcfid","", $pcfid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Modalidade:</td>
			<td>
				<?php $sql = "	select 
									modid as codigo,
									moddesc as descricao
								from 
									catalogocurso.modalidadecurso
								where 
									modstatus = 'A'
								order by 
									moddesc" ?>
				<?php $db->monta_combo("modid",$sql,"S","Selecione...","","","","","N","","",$modid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Nível:</td>
			<td>
				<?php $sql = "	select 
									ncuid as codigo,
									ncudesc as descricao
								from 
									catalogocurso.nivelcurso
								where 
									ncustatus = 'A'
								order by 
									ncudesc" ?>
				<?php $db->monta_combo("ncuid",$sql,"S","Selecione...","","","","","N","","",$ncuid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloTabela center" colspan="2"  >
				<input type="button" name="btn_pesquisar" value="Pesquisar" onclick="pesquisarCursos()" />
				<input type="button" name="btn_ver_todos" value="Ver Todos" onclick="verTodosCursos()" />
				<input type="button" name="btn_cusros_mais_solicitados" value="Cursos Mais Solicitados" onclick="cursosMaisSolicitados()" />
			</td>
		</tr>
	</table>
</form>
<table class="tabela" bgcolor="#FFFFFF" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td valign="top" width="95%" >
			<?php 
				exibeCursosSNF($estuf, $muncod);
				$docid = pegaDocid($muncod, $estuf);
			?>
		</td>
		<td valign="top" >
			<?php 
				if($docid){
					include APPRAIZ ."includes/workflow.php";
	
					$dados_wf = array("estuf" => $estuf, "muncod" => $muncod, "esfera"=> $esfera, "pcfid"=> $pcfid);
					wf_desenhaBarraNavegacao($docid, $dados_wf);
					$estadoAtual = pegaEstadoAtual($docid);
					$_SESSION['estadoAtual'] = $estadoAtual;
				}
			?>
		</td>
	</tr>
</table>

<input type="hidden" name="vagas_disponiveis" id="vagas_disponiveis_id" value="<?php echo $_SESSION['vagasDisponiveis'] ?>" />
<input type="hidden" name="vagas_solicitadas" id="vagas_solicitadas_id" value="<?php echo $_SESSION['vagasSolicitadas'] ?>" />

<?php //if($estadoAtual['esdid'] != WORKFLOW_SNF_ESTADO_EM_ANALISE_SECRETARIA): 
	if($pcfid != 1 && $pcfid != 2):
?>
	<script>
		jQuery("#tbl_btn_salvar").remove();
	</script>
<?php endif; ?>