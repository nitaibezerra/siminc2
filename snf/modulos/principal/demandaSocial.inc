<?php
//Dados Curso POPUP
if ($_REQUEST['dadosCursoPopup']) {
	header('content-type: text/html; charset=ISO-8859-1');
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		  '; 
	dadosCursoPopup($_GET['curid']);
	exit;
}



if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}
if($_POST['requisicao'] == "pesquisarCursos"){
	extract($_POST);
}


include  APPRAIZ."includes/cabecalho.inc";
require_once APPRAIZ . "www/includes/webservice/cpf.php";
?>
<br />
<?php $db->cria_aba($abacod_tela,$url,''); ?>
<?php monta_titulo( 'Demanda Social' , obrigatorio()." Indica campos obrigatórios.");  ?>

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
	
	function importarEscolasPDEInterativo(estuf,muncod){
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
	
	function verTodosCursos(){
		jQuery("[name='mais_solicitados']").val("");
		jQuery("[name='ateid']").val("");
		jQuery("[name='curid']").val("");
		jQuery("[name='requisicao']").val("pesquisarCursos");
		jQuery("[name='formulario_pesquisa']").submit();
	}
		
	function expandirEscolas(curid,ano,salva_prioridade){
		//var NovaOrdem 	= jQuery("#cmb_ordem_curid_" + curid + "_pcfid_" + ano).val();
		//var Antigaordem = jQuery("#hdn_ordem_curid_" + curid + "_pcfid_" + ano).val();
		//if(!jQuery("#td_curid_" + curid).html() || NovaOrdem != Antigaordem){
		if(!jQuery("#td_curid_" + curid).html()){
			jQuery("#tr_curid_" + curid).show();
			jQuery("#img_mais_curid_" + curid).hide();
			jQuery("#img_menos_curid_" + curid).show();
			jQuery("#td_curid_" + curid).html("<center>Carregando...</center>");
			var estuf 	= jQuery("[name='estuf']").val();
			var muncod 	= jQuery("[name='muncod']").val();
			var modid 	= jQuery("[name='modid']").val();
			var ncuid 	= jQuery("[name='ncuid']").val();
			var pcfid 	= jQuery("[name='pcfid']").val();
			jQuery.ajax({
				type: "POST",
				url: 'snf.php?modulo=principal/listaEscolas&acao=A',
				data: "requisicao=exibirEscolasDemandaSocial&curid=" + curid + "&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&pcfid=" + pcfid + "&modid=" + modid,
				success: function(msg){

					jQuery("#td_curid_" + curid).html(msg);
						var vagas_permitidas = jQuery('#vagas_permitidas_'+curid).val();
						var vagas_informadas = jQuery('#vagas_informadas_'+curid).val();
						var saldo_vagas = parseInt(vagas_permitidas)-parseInt(vagas_informadas);

						if(saldo_vagas > 0){
								jQuery('#tb_vagas_curso_'+curid).show();
								}
							jQuery('#saldo_vagas_'+curid).val(saldo_vagas);
				}
			});					 
			}else{
				jQuery("#tr_curid_" + curid).show();
				jQuery("#img_mais_curid_" + curid).hide();
				jQuery("#img_menos_curid_" + curid).show();
			}
	}
	
	function esconderEscolas(curid){
		jQuery("#tr_curid_" + curid).hide();
		jQuery("#img_mais_curid_" + curid).show();
		jQuery("#img_menos_curid_" + curid).hide();
	}
	
	function cursosMaisSolicitados(){
		jQuery("[name='mais_solicitados']").val("1");
		jQuery("[name='requisicao']").val("pesquisarCursos");
		jQuery("[name='formulario_pesquisa']").submit();
	}
	
	function salvarDemandaSocial(){
		var demanda 			= jQuery("[name^='vagas_demanda_social_previstas[']").serialize();
		var estuf 				= jQuery("[name='estuf']").val();
		var muncod 				= jQuery("[name='muncod']").val();
		//var modid 				= jQuery("[name='modid']").val();
		var ncuid 				= jQuery("[name='ncuid']").val();
		var cdscpf 				= jQuery("[name^='cdscpf[']").serialize();
		var modid 				= jQuery("[name^='modid[']").serialize();
		var pdaid 				= jQuery("[name^='pdaid[']").serialize();		
		var cdsnome 			= jQuery("[name^='cdsnome[']").serialize();
		var cdsemail 			= jQuery("[name^='cdsemail[']").serialize();
		var cdsdddtelefonefixo 	= jQuery("[name^='cdsdddtelefonefixo[']").serialize();
		var cdstelefonefixo 	= jQuery("[name^='cdstelefonefixo[']").serialize();
		var cdstelefonecelular 	= jQuery("[name^='cdstelefonecelular[']").serialize();
		
		//candidatos = cdscpf+'&'+cdsnome+'&'+cdsemail+'&'+cdstelefonefixo+'&'+cdsdddtelefonefixo+'&'+cdstelefonecelular+'&'+modid+'&'+pdaid;
		candidatos = cdscpf+'&'+cdsnome+'&'+cdsemail+'&'+cdstelefonefixo+'&'+cdsdddtelefonefixo+'&'+cdstelefonecelular+'&'+pdaid;
		
		jQuery.ajax({
			type: "POST",
			url: 'snf.php?modulo=principal/listaEscolas&acao=A',
			//data: "requisicao=salvarDemandaSocial&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&modid=" + modid + "&" + demanda + "&" + candidatos,
			data: "requisicao=salvarDemandaSocial&estuf=" + estuf + "&muncod=" + muncod + "&ncuid=" + ncuid + "&" + demanda + "&" + candidatos,
			success: function(msg){
						if(msg == "ok"){
					   		alert('Dados atualizados com sucesso!');
					   	}
					 }
		});
	}
	
	function inserirVaga(curid){
	
		var vagas_permitidas = jQuery('#vagas_permitidas_'+curid).val();
		
		var vagas_informadas = 0;
			jQuery.each(jQuery("[name^='vagas_demanda_social_previstas["+curid+"]']"),function(i,v){
				vagas_informadas += parseInt(vagas_informadas)+parseInt(v.value); 
			});

		var vagas_solicitadas = 0;
			jQuery.each(jQuery("[name^='vagas_demanda_social_solicitadas["+curid+"]']"),function(i,v){
				vagas_solicitadas += parseInt(vagas_solicitadas)+parseInt(v.value); 
			});
			
			
		if(vagas_informadas < vagas_solicitadas){
			alert('Não é possivel inserir mais alunos, pois a quantidade de Vagas Autorizadas (Plano Autorizado) é menor que a quantidade de Vagas Solicitadas (Plano da Escola).');
			return false;
		}
			
			
		var conteudo = jQuery('#tb_vagas_curso_'+curid+' tbody');
		
		//var saldo_vagas = jQuery('#saldo_vagas_'+curid).val();
		
		var totlinha = conteudo.find('tr').length;
		if(!totlinha) totlinha = 0;
		 
		vagas_informadas = vagas_informadas + parseInt(totlinha);
		
		if(vagas_permitidas > vagas_informadas){
		
			var clonar = conteudo.find('tr').first().clone();
			conteudo.append(clonar);
			
			jQuery("[name^='cdscpf["+curid+"][']").last().val('');
			jQuery("[name^='cdscpf["+curid+"][']").last().attr('id',curid + '_' + (conteudo.find('tr').length -1) );
			jQuery("[name^='modid["+curid+"][']").last().val('');
			jQuery("[name^='pdaid["+curid+"][']").last().val('');
			jQuery("[name^='cdsnome["+curid+"][']").last().val('');
			jQuery("[name^='cdsemail["+curid+"][']").last().val('');
			jQuery("[name^='cdsdddtelefonefixo["+curid+"][']").last().val('');
			jQuery("[name^='cdstelefonefixo["+curid+"][']").last().val('');
			jQuery("[name^='cdstelefonecelular["+curid+"][']").last().val('');			
			jQuery("[name^='cdsnome["+curid+"][']").last().parent().find('label').html('');
			
		}else{
		
			alert('Não existem mais vagas disponíveis para este curso!');
		}		
	}
	
	
	function excluirVaga(obj){
		conteudo = jQuery(obj).parent().parent().parent();		
		if(conteudo.find('tr').length > 1){
			jQuery(obj).parent().parent().remove();
			
			var saldo = parseInt(jQuery('#saldo_vagas_'+this.id).val())+1;
			jQuery('#saldo_vagas_'+this.id).val(saldo);
		}
	}
	
	jQuery(function(){
		jQuery('input.classcpf').live('blur',function(){
		
			var comp  = new dCPF();
			var td 	  = jQuery(this).parent('td').next();
			var nome  = td.find('input');
			var label = td.find('label');
			
			comp.buscarDados( jQuery(this).val() );			
			if (comp.dados.no_pessoa_rf != ''){
				nome.val(comp.dados.no_pessoa_rf);
				label.html(comp.dados.no_pessoa_rf);
				nome.attr("readonly","readonly");				
				var saldo = parseInt(jQuery('#saldo_vagas_'+this.id).val())-1;
				jQuery('#saldo_vagas_'+this.id).val(saldo);
			}else{
				var saldo = parseInt(jQuery('#saldo_vagas_'+this.id).val())+1;
				jQuery('#saldo_vagas_'+this.id).val(saldo);
				nome.val('');
				label.html('');
			}
	
		});
		
		jQuery("[name^='vagas_demanda_social_previstas[']").live('blur', function(){
			var param = this.id.split("_");
			
			var curid = param[0];
			var pcfid = param[1];
			var pdeid = param[2];
			
			var vagas_permitidas = jQuery('#vagas_permitidas_'+curid).val();
			
			if(vagas_permitidas <= 0){
				alert('Não é possivel inserir valor, pois a Qtde Permitida para este curso é 0 (zero).');
				jQuery('#'+curid+'_'+pcfid+'_'+pdeid).val('');
				return false;
			}
			
			var vagas_solicitadas = jQuery('#vs_'+curid+'_'+pcfid+'_'+pdeid).val();
			var vagas_autorizadas = jQuery('#'+curid+'_'+pcfid+'_'+pdeid).val();
			
			
			var vagas_informadas = 0;
			jQuery.each(jQuery("[name^='vagas_demanda_social_previstas["+curid+"]']"),function(i,v){
				if(v.value) vagas_informadas += parseInt(vagas_informadas)+parseInt(v.value); 
			});
			
			//alert(vagas_informadas);
			
			var totlinhas = jQuery('#tb_vagas_curso_'+curid+' > tbody > tr').size();
			if(!totlinhas) totlinhas = 0;
			
			vagas_informadas = vagas_informadas + parseInt(totlinhas); 
			
			var saldo_vagas = parseInt(vagas_permitidas)-parseInt(vagas_informadas);
			jQuery('#saldo_vagas_'+curid).val(saldo_vagas);
			
			//alert('saldo_vagas = ' + saldo_vagas);
			
			if(saldo_vagas >= 0){
				jQuery('#tb_vagas_curso_'+curid).show();
				
				if(vagas_autorizadas > vagas_solicitadas){
					alert('A quantidade de Vagas Autorizadas (Plano Autorizado) não pode ultrapassar a quantidade de Vagas Solicitadas (Plano da Escola).');
					jQuery('#'+curid+'_'+pcfid+'_'+pdeid).val('');
				}
				
			}else{
				alert("É necessário excluir "+(parseInt(saldo_vagas)*-1)+" aluno(s) para aumentar a quantidade de Vagas Autorizadas");
				var x = jQuery('#'+curid+'_'+pcfid+'_'+pdeid).val();
				var xtotal = parseInt(saldo_vagas) + parseInt(x);
				jQuery('#'+curid+'_'+pcfid+'_'+pdeid).val(xtotal);
				//jQuery('#tb_vagas_curso_'+curid).hide();
			}
		});
	});

	function verificaCPFDuplicado(obj, curid){
		var num = jQuery("[name='cdscpf[" + curid + "][]'][value='" + obj.value + "']").length;
		if(num > 1){
			jQuery("#" + obj.id).val("");
			alert("Candidato já cadastrado, por favor insira outro CPF");
		}
	}
	
	$(document).ready(function(){
		$('#salvar').click(function(){

			if( document.formulario.pdaid.value == '' ){
				alert( "O campo é obrigatório." );
				return false;
			}
			if( document.formulario.modid.value == '' ){
				alert( "O campo é obrigatório." );
				return false;
			}
			if( document.formulario.cdsdddtelefonefixo.value == '' ){
				alert( "O campo é obrigatório." );
				return false;
			}
			if( document.formulario.cdstelefonefixo.value == '' ){
				alert( "O campo é obrigatório." );
				return false;
			}
			if( document.formulario.cdstelefonecelular.value == '' ){
				alert( "O campo é obrigatório." );
				return false;
			}
			
			selectAllOptions( document.formulario.pdaid );
			selectAllOptions( document.formulario.modid ); 
			selectAllOptions( document.formulario.cdsdddtelefonefixo );
			selectAllOptions( document.formulario.cdstelefonefixo );
			selectAllOptions( document.formulario.cdstelefonecelular );
			
			$('#requisicao').val('enviarcurso');
			$('#formulario').submit();
			});
		});
		
function selecionaEstado(estuf){
	location.href = '?modulo=principal/demandaSocial&acao=A&estuf_selecionado=' + estuf;
}		
</script>
<div id="teste"></div>
<?php 
$estuf_selecionado		 			= (isset($_GET['estuf_selecionado']) ? $_GET['estuf_selecionado'] : $_SESSION['snf']['perfil']['estuf']);
$_SESSION['snf']['perfil']['estuf'] = $estuf_selecionado;
$estuf 					 			= $estuf_selecionado;
$arrTravaFiltro['estuf'] 			= $estuf_selecionado;

if ( verificaPerfil( array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR) ) ){
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2" class="SubtituloTabela bold" >Selecione um Estado</td>
	</tr>
	<tr class="center SubtituloDireita" >
		<td colspan="2">
		<?php 
		$sql = "SELECT
					DISTINCT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				ORDER BY
					codigo, descricao";
		$db->monta_combo("estuf_selecionado", $sql, "S", "Selecione um estado...", "selecionaEstado", "", "", '', "N");
		?>	
		</td>
	</tr>
</table>
<?php 
	if ( empty($estuf_selecionado) ){
		die("<script>jQuery('#aguarde').hide();</script>");
	}
}else{
	$arrEstufMuncod = recuperaEstadoMunicipioPerfil(); 
	$arrEstufMuncod ? extract($arrEstufMuncod) : "";
	verificaImportacaoEscolas($arrEstufMuncod);
	$arrTravaFiltro['estuf'] = $arrEstufMuncod['estuf'];
	$arrTravaFiltro['muncod'] = $arrEstufMuncod['muncod']; 
}
?>
<?php 
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
?>

<form name="formulario_pesquisa" method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="mais_solicitados" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2" class="SubtituloTabela bold" >Filtros</td>
		</tr>
		<tr>
			<td class="subtituloDireita" width="220">Orientações</td>
			<td>		  
			  Os cursos poderão contemplar também, a demanda social identificada na comunidade escolar, visando apoiar os sistemas de ensino para o desenvolvimento inclusivo das escolas brasileiras.

			  <p>Por demanda social compreende-se a participação dos demais integrantes da comunidade escolar, assim como, dos gestores públicos das áreas que atuam em conjunto com a educação.</p>
			
			  O preenchimento da demanda social é facultado à escola e à Secretaria de Educação, que deve validar a proposta final.
			  
			  <p>Este ano, só é possível modificar,  priorizar, validar e indicar novos nomes para o período 2012/2013.</p>
			</td>
		</tr>
		<tr style="display:none" >
			<td width="25%" class="SubtituloDireita" >Estado:</td>
			<td>
				<?php $sql = "SELECT estuf AS codigo, estdescricao AS descricao
								FROM territorios.estado
							ORDER BY estdescricao" 
				?>
				<?php $db->monta_combo("estuf", $sql,($arrTravaFiltro['estuf'] ? "N" : "S"), "Selecione...", "filtraCidade", "", "", "", "", "", "", $estuf) ?>
			</td>
		</tr>
		<?php $arrPerfil = pegaPerfilGeral();
				if(in_array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil) || 
				   in_array(PERFIL_EQUIPE_MUNICIPAL,$arrPerfil)){
					$visivel = "none";
				}else{
					$visivel = "";
			  	}
		?>
		<tr style="display:<?php echo $visivel ?>" >
			<td class="SubtituloDireita" >Município:</td>
			<td id="td_muncod" >
			<?php $sql = "SELECT muncod AS codigo, mundescricao AS descricao
							FROM territorios.municipio
				   		   WHERE estuf = '$estuf'
			   			ORDER BY mundescricao";

				  if(!$estuf){
				  	$db->monta_combo("muncod",$sql,"N","Selecione...","","","","200","N","","",$muncod);
				  }else{
			   	  	$db->monta_combo("muncod", $sql, ($arrTravaFiltro['muncod'] ? "N" : "S"), "Selecione...", "", "", "", "200", "N", "", "", $muncod);
				  }
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Área Temática:</td>
			<td>
				<?php $sql = "SELECT ateid as codigo, atedesc as descricao 
								FROM catalogocurso.areatematica
							   WHERE atestatus = 'A'
							ORDER BY atedesc" 
				?>
				<?php $db->monta_combo("ateid",$sql,"S","Selecione...","filtraCurso","","","","N","","",$ateid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Cursos:</td>
			<td id="td_curid" >
			<?php if(!$ateid){
					$sql = "SELECT 1 AS codigo, 2 AS descricao";
				  }else{
					$sql = "SELECT curid AS codigo, curdesc AS descricao
							  FROM catalogocurso.curso
							 WHERE curstatus = 'A'
							   AND ateid = $ateid
						  ORDER BY curdesc";
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
			<td><?php 
					if(!$pcfid) 
						$pcfid = 1;
					$sql = "
						Select	pcfid AS codigo, 
								pcfano AS descricao
						From pdeinterativo.periodocursoformacao
						Where pcfstatus = 'A'
						Order by pcfano
					"; 
				?>
				<?php $arrPeriodo[0] = array("codigo" => "1,2", "descricao" => "2012/2013") ?>
				<?php $arrPeriodo[1] = array("codigo" => "3", "descricao" => "2014") ?>
				<?php $arrPeriodo[2] = array("codigo" => "4", "descricao" => "2015") ?>
				<?php $arrPeriodo[3] = array("codigo" => "10", "descricao" => "2016") ?>
				<?php $db->monta_combo("pcfid",$arrPeriodo,"S","Selecione...","","","","","S","","",$pcfid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Modalidade:</td>
			<td><?php $sql = "SELECT modid AS codigo, moddesc AS descricao
								FROM catalogocurso.modalidadecurso
							   WHERE modstatus = 'A'
							ORDER BY moddesc" 
				?>
				<?php $db->monta_combo("modid",$sql,"S","Selecione...","","","","","N","","",$modid) ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Nível:</td>
			<td><?php $sql = "SELECT ncuid AS codigo, ncudesc AS descricao
								FROM catalogocurso.nivelcurso
							   WHERE ncustatus = 'A'
							ORDER BY ncudesc" 
				?>
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
				$arrDocids = exibeDemandaSocial($estuf,$muncod);
				$docid = pegaDocid($muncod, $estuf);
			?>
		</td>
		<td valign="top" >
			<?php 
			if($arrDocids){
				include APPRAIZ ."includes/workflow.php";
				//$dados_wf = array("estuf" => $estuf, "muncod" => $muncod, "pcfid" => $pcfid, "docid_usado" => $arrDocids[0]);
				//$docid = $arrDocids[0];
				$dados_wf = array("estuf" => $estuf, "muncod" => $muncod, "esfera"=> $esfera);
				//wf_desenhaBarraNavegacao($docid, $dados_wf);
				$estadoAtual = pegaEstadoAtual($docid);
				$_SESSION['estadoAtual'] = $estadoAtual;
			}
			?>
		</td>
	</tr>
</table>
<?php //if($estadoAtual['esdid'] != WORKFLOW_SNF_ESTADO_EM_ANALISE_SECRETARIA): 
if($pcfid != 1 && $pcfid != 2):
?>
	<script>
		jQuery("#tbl_btn_salvar").remove();
	</script>
<?php endif; ?>