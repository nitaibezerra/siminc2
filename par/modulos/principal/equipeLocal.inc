<?php
/* TRECHO DO POPUP DO HISTORICO DE MODIFICACAO */

if($_REQUEST['hdpid']) {
	
$arrDados = $db->pegaLinha("
				SELECT	hdpdados, 
						usu.usunome, 
						to_char(hpddatainsercao,'dd/mm/YYYY HH24:MI') as hpddatainsercao 
				FROM par.historicodadospar hdp 
				INNER JOIN seguranca.usuario usu ON usu.usucpf = hdp.usucpf 
				WHERE hdpid='".$_REQUEST['hdpid']."'
			");
$dados = unserialize($arrDados['hdpdados']);

$sql = "SELECT sgmid as codigo, sgmdsc as descricao FROM par.dadosunidadesegmento ORDER BY sgmdsc ASC";
$segmentos = $db->carregar($sql);
if($segmentos[0]) {
	foreach($segmentos as $seg) {
		$_segmentos[$seg['codigo']] = $seg['descricao'];
	}
}



if($dados['duncpf']) {
	foreach($dados['duncpf'] as $key => $cpf) {
		$htmltable .= "<tr>
							<td align=center>".$cpf."</td>
							<td align=center>".$dados['dunnome'][$key]."</td>
							<td align=center>".$dados['dunfuncao'][$key]."</td>
							<td align=center>".$_segmentos[$dados['sgmid'][$key]]."</td>
							<td align=center>".$dados['dunemail'][$key]."</td>
					   </tr>";
	}
}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr><td class="SubTituloCentro" colspan="5">DADOS MODIFICADOS POR <?=$arrDados['usunome'] ?>, <?=$arrDados['hpddatainsercao'] ?></td></tr>
<tr>
				<td class="SubTituloCentro">CPF</td>	
				<td class="SubTituloCentro">Nome</td>	
				<td class="SubTituloCentro">Função/Cargo</td>	
				<td class="SubTituloCentro">Segmento</td>	
				<td class="SubTituloCentro">E-mail</td>	
</tr>
<?=$htmltable ?>
<tr><td class="SubTituloCentro" colspan="5"><input type="button" name="fechar" value="Fechar" onclick="window.close();"></td></tr>
</table>	
<?
exit;
}

/* FIM - TRECHO DO POPUP DO HISTORICO DE MODIFICACAO */



require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . "www/includes/webservice/cpf.php";

if($_POST['requisicao'] == 'excluirDadosUnidade'){
	ob_clean();
	extract($_POST);
	if($dunid){
		$obDadosUnidade = new DadosUnidade($dunid);
		$obDadosUnidade->excluir($dunid);
		if($obDadosUnidade->commit()){
			echo 'sucesso';		
		} else {
			$obDadosUnidade->rollback();
		}
		unset($obDadosUnidade);		
	}
	die;
}

validaSessaoPar( $_SESSION['par']['inuid'] );

$inuid = $_SESSION['par']['inuid'];

$oDadosUnidade = new DadosUnidade();
$oDadosUnidade->recuperarHistoricoEntidadeDadosUnidade($_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid'], true);
//$boBotaoVoltar = $oDadosUnidade->verificaAbasDesatualizada($inuid, $_SESSION['par']['muncod'], $_SESSION['par']['estuf'], $_SESSION['par']['itrid']);

if($_POST['formulario']){
	$oDadosUnidade->deletarDadosPorDutid($_SESSION['par']['inuid'], EQUIPE_LOCAL);
	
	for ($i = 0; $i < count($_POST['duncpf']); $i++) {
		if($_POST['duncpf'][$i]){
			if(!$_POST['sgmid'][$i]){
				$msg = 'O campo Segmento é obrigatório.\n';
				$boSalvou = false;
			} elseif($_POST['sgmid'][$i] == 10){
				if(!$_POST['dunsegmento'][$i]){
					$msg = 'O campo Segmento é obrigatório.\n';
					$boSalvou = false;
				}
				
			}
						
			if(!$_POST['dunfuncao'][$i]){
				$msg .= 'O campo Função é obrigatório.\n';
				$boSalvou = false;		
			}
			if(!$_POST['dunemail'][$i]){
				$msg .= 'O campo Email é obrigatório.\n';
				$boSalvou = false;		
			}
			
			if($msg){
				break;
			}
			
			$oDadosUnidade->dunid 		= null;
			$oDadosUnidade->inuid 		= $_SESSION['par']['inuid'];
			$oDadosUnidade->sgmid 		= $_POST['sgmid'][$i];
			$oDadosUnidade->dundata 	= date('Y-m-d H:i:s');
			$oDadosUnidade->usucpf 		= $_SESSION['usucpf'];
			$oDadosUnidade->dutid 		= EQUIPE_LOCAL;
			$oDadosUnidade->dunfuncao 	= $_POST['dunfuncao'][$i];
			$oDadosUnidade->duncpf 		= substr(str_replace(array(".","-"),"",$_POST['duncpf'][$i]), 0, 11);
			$oDadosUnidade->dunemail 	= $_POST['dunemail'][$i];
			$oDadosUnidade->dunnome 	= $_POST['dunnome'][$i];
			$oDadosUnidade->dunsegmento	= $_POST['dunsegmento'][$i];
			$oDadosUnidade->salvar();			
			$boSalvou = true;
			
		}
	}
	
	$sql = "INSERT INTO par.historicodadospar(
            	muncod, estuf, hdplink, hdpdados, usucpf, hpddatainsercao)
    			VALUES (".(($_SESSION['par']['muncod'])?"'".$_SESSION['par']['muncod']."'":"NULL").", 
    					".(($_SESSION['par']['estuf'])?"'".$_SESSION['par']['estuf']."'":"NULL").", 
    					'".$_SERVER['REQUEST_URI']."', '".serialize($_REQUEST)."', '".$_SESSION['usucpf']."',
    					NOW());";
		
	$db->executar($sql);
	$db->commit();
	
	if($boSalvou){
		$oDadosUnidade->commit();
		echo "<script>
				alert('Operação realizada com sucesso');
				window.location.href = 'par.php?modulo=principal/equipeLocal&acao=A';
			  </script>";
		exit;
	} else {
		$oDadosUnidade->rollback();
		echo "<script>
				alert('$msg');
				window.location.href = 'par.php?modulo=principal/equipeLocal&acao=A';
			  </script>";
		exit;
	}
}


# se for municipal, recupera cpf do dirigente
/*if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
	$cpfDirigente = $oDadosUnidade->recuperarCpfDirigente($_SESSION['par']['estuf'],$_SESSION['par']['muncod']);
}*/

$arDadosUnidade = $oDadosUnidade->recuperarDadosPorInuid($inuid, EQUIPE_LOCAL);
$arSegmentos 	= $oDadosUnidade->recuperarSegmentos();
$arSegmentos 	= ($arSegmentos) ? $arSegmentos : array();

print "<br />";

print carregaAbaDadosUnidade("/par/par.php?modulo=principal/equipeLocal&acao=A");

monta_titulo( "Dados da unidade", "Equipe local" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];

//Pega os perfis do usuário
$arrPerfil = pegaPerfilGeral();
//Desabilita as opções de salvar para os perfis de consulta

#Estrutura condicional mudada em 04/06/2012, para que seja habilitando o questionário somente para os perfis que tem essa permissão, permissão para edição.
#Habilitando ou desabilitando os botões "Salvar anterior, Salvar e Salvar próximo".  
#O valor "false" habilita o quetionário.
#O valor "true" desabilita o questionário.
if(	in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arrPerfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arrPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrPerfil)	){
	$boFormDesabilitado = false;
}else{
	$boFormDesabilitado = true;
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<?php if( $_SESSION['par']['atualizacao'] ){ ?>
	<tr>
		<td colspan="2">
			<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="60%" class="tabela">
				<tbody>
					<tr bgcolor="#cccccc">
						<td>
							<div style="float: left; width:100%; text-align: center;">
								<b>ATUALIZAÇÃO DO PAR</b><BR>
								<input value="Retornar para a Atualização do PAR" type="button" id="atualizacaopar" onclick="javascript:window.location='par.php?modulo=principal/dadosUnidadeAtualizacaoPAR&acao=A'">
							</div>
						</td>
					</tr>
				</tbody>
			</table>
		</td>
	</tr>
	<? } ?>
	<tr>
		<td style="color: blue; font-size: 22px">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore">
				<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
			</a>
		</td>
		<td style="color:blue; font-size:12px; text-align:right">
			<a href="par.php?modulo=principal/planoTrabalho&acao=A">Voltar à Árvore |</a>
				<label style="color:black; text-align:center">
					<b>Dados da Unidade</b>
				</label>
			<a href="par.php?modulo=principal/listaObrasParUnidade&acao=A">| Lista de Obras </a>
			<a href="par.php?modulo=principal/questoesPontuais&acao=A">| Questões Pontuais</a>
		</td>
	</tr>
</table>

<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="inuid" value="<?php echo $_SESSION["inuid"]; ?>"/>

	<table id="tabelaEquipeLocal" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Integrantes</td>	
			</tr>
			<tr align="center">
				<td width="3%">&nbsp;</td>	
				<td width="12%">CPF</td>	
				<td width="15%">Nome</td>	
				<td width="15%">Função/Cargo</td>	
				<td width="30%">Segmento</td>	
				<td width="20%">E-mail</td>	
			</tr>	
		</thead>
		<tbody id="tbodyTabela">
		<?php $arDadosUnidade = $arDadosUnidade ? $arDadosUnidade : array(array(1)); ?>
		<?php $count = 1 ?>
		<?php foreach ($arDadosUnidade as $dados) : ?>
			<?php extract($dados);?>	
			<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
				<td>
					<?php if($_SESSION['par']['boPerfilConsulta']):?>
						<img alt="Excluir" title="Excluir" src="/imagens/excluir_01.gif" style="margin-left:5px; cursor:pointer">
					<?php else:?>
						<img class="removeItens" id="<?php echo $dunid; ?>" alt="Excluir" title="Excluir" src="/imagens/excluir.gif" style="margin-left:5px; cursor:pointer">
					<?php endif;?>
					<input value="1" name="ardun[]" type="hidden" />
				</td>
				<td>
					<input value="<?php echo $dunid; ?>" name="dunid[]" type="hidden" />
					<!--<input onchange="procurarNome( this, 1 )" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this); this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this); validarduncpf( this );" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo $duncpf; ?>" name="duncpf[]" type="text" />
					
						<img title="Indica campo obrigatório." src="../imagens/obrig.gif">
					-->
					<input onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo $duncpf; ?>" name="duncpf[]" id="duncpf[]" type="text" />
					<img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				</td>
				<td>
					<input value="<?php echo $dunnome; ?>" name="dunnome[]" id="dunnome[]" class="dunnome" type="hidden" /><label><?php echo $dunnome; ?></label>
				</td>
				<td>
					<input maxlength="254" size="30" style="width: 30ex;" value="<?php echo $dunfuncao; ?>" name="dunfuncao[]" type="text" class="normal">
					<img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				</td>
				<td>
					<?php //echo $db->monta_combo( 'sgmid[]', $arSegmentos, 'S', 'Selecione...', 'alternarExibicaoSegmento', '', '', '', 'S','',false,$sgmid); ?>
					
					<select class="CampoEstilo exibicaoSegmento" name="sgmid[]" style="width: 56ex;">
						<option value="">Selecione</option>
						<?php 
							foreach ($arSegmentos as $segmento) {
								echo "<option value='" . $segmento['codigo'] . "' " . (($segmento['codigo'] == $sgmid) ? "selected=\"selected\"" : "")  . ">" . $segmento['descricao'] . "</option>";
							}
						?>
					</select>
					<input size="50" class="objetoOculto" style="width: 50ex; display: none;" value="<?php echo $dunsegmento; ?>" name="dunsegmento[]" type="text">
					<img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
					<a class="exibicaoSegmento" style="color:blue; margin-left: 20px; width: 50ex; display: none; cursor: pointer;" >Exibir Opções</a>
				</td>
				<td>
					<input size="30" style="width: 33ex;" value="<?php echo $dunemail; ?>" name="dunemail[]" type="text" class="normal">
					<img src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				</td>
			</tr>
			<?php $count++ ?>
		<?php endforeach; ?>
		</tbody>
	</table>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">		
		<tr>
			<td style="">
				<?php if($_SESSION['par']['boPerfilConsulta']):?>
					<span style="margin-left:5px; cursor:pointer">
						<img src="/imagens/gif_inclui_d.gif" align="top" style="border: none" />
						Inserir Integrante
					</span>
				<?php else:?>
					<span class="add" style="margin-left:5px; cursor:pointer">
						<img src="/imagens/gif_inclui.gif" align="top" style="border: none" />
						Inserir Integrante
					</span>
				<?php endif;?>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0" align="center">
			<td>
				<?php if($_SESSION['par']['itrid'] == 1): ?>
					<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='par.php?modulo=principal/orgaoEducacao&acao=A&funid=25'" >
				<?php else: ?>
					<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='par.php?modulo=principal/orgaoEducacao&acao=A&funid=15'" >
				<?php endif; ?>
				<?php if(!$_SESSION['par']['boPerfilConsulta']):?>
					<input type='submit' class="botao" name='cadastra' value='Salvar' />
				<?php endif; ?>
					<!-- input type='button' class="botao" name='fecha' value='Voltar' onclick="fechar();" / -->
					<input type="button" class="botao" name="proximo" value='Próximo' onclick="window.location='par.php?modulo=principal/comiteLocal&acao=A'" >
			</td>
		</tr>
	</table>
	<?php 
	//$arHistoricoIntegrantes = $oDadosUnidade->recuperarHistorico($_SESSION['par']['inuid'], EQUIPE_LOCAL);
	?>
	<?php //if(count($arHistoricoIntegrantes)): ?>
	<!--<table align="center" width="95%;" style="margin-top:10px">
		<tr bgcolor="#c0c0c0">
			<td colspan="2" align="center"><b>Últimas alterações:</b></td>
		</tr>		
		<tr>
			<td align="left" width="100">
				Integrantes:
			</td>
			<td align="left">
				<?php //echo $arHistoricoIntegrantes['dunnome'] ?> em 
				<?php //echo formata_data($arHistoricoIntegrantes['dundata']) ?>
			</td>
		</tr>				
	</table>
	--><?php //endif; ?>
</form>	

<script type="text/javascript">

jQuery.noConflict();

//jQuery('input').disable();

jQuery(document).ready(function(){

	jQuery('#formulario').submit(function(){

		var boSubmeter = true;
		var boDuplicado = false;
		if(jQuery("input.[name=duncpf[]]").length > 0){
			jQuery("input.[name=duncpf[]]").each(function(){
				var cpfTemp = '';
				//var id = '';
				if(jQuery(this).val()){
					cpfTemp = jQuery(this);
					//id = jQuery(this).attr('id');
					//jQuery("input.[name=duncpf[]]").not('#'+id).each(function(){
					jQuery("input.[name=duncpf[]]").not(cpfTemp).each(function(){
						if(jQuery(this).val() == cpfTemp.val()){
							alert('Existe CPF duplicado.');
							cpfTemp.val('');
							cpfTemp.parent().parent().find("input[name=dunnome[]]").val('');
							cpfTemp.parent().parent().find("label").html('');
							cpfTemp.focus();
							boDuplicado = true;
							return false;
						}
					});
				}else{
					alert('O campo CPF é obrigatório.');
					jQuery(this).focus();
					return false;
				}
				if(boDuplicado){
					return false;
				}
			});
		}
		
		if(boDuplicado){
			return false;
		}

		jQuery("input.[name=dunnome[]]").each(function(){
			if(jQuery(this).val() == ''){
				alert('O campo Nome é obrigatório.');
				jQuery(this).focus();
				boSubmeter = false;
				return false;
			}
		});

		jQuery("input.[name=dunfuncao[]]").each(function(){
			if(jQuery(this).val() == ''){
				alert('O campo Função/Cargo é obrigatório.');
				jQuery(this).focus();
				boSubmeter = false;
				return false;
			}
			if(jQuery(this).val().length > 250){
				alert('O campo Função/Cargo tem que ter até 250 caracteres.');
				jQuery(this).focus();
				boSubmeter = false;
				return false;
			}
		});

		jQuery("select.[name=sgmid[]]").each(function(i){
			if(jQuery(this).val() == ''){
				alert('O campo Segmento é obrigatório.');
				jQuery(this).focus();
				boSubmeter = false;
				return false;
			} else if (jQuery(this).val() == 10){
				if(jQuery("input[name=dunsegmento[]]")[i].value == ''){
					alert('O campo Segmento é obrigatório.');
					jQuery(this).focus();
					boSubmeter = false;
					return false;
				}
			}
		});

		jQuery("input.[name=dunemail[]]").each(function(){
			if(jQuery(this).val() == ''){
				alert('O campo E-mail é obrigatório.');
				jQuery(this).focus();
				boSubmeter = false;
				return false;
			}
			if(!validaEmail(jQuery(this).val())){
				alert('E-mail inválido.');
				jQuery(this).focus();
				boSubmeter = false;
				return false;
			}
		});
		
		return boSubmeter;

	});

	jQuery('a.exibicaoSegmento').live('click',function(){
		var td 			= jQuery(this).parent('td');
		var sgmid       = td.find('select');
		var dunsegmento = td.find('input');
		var link        = td.find('a');
		
		dunsegmento.hide();
		link.hide();
		sgmid.show();
		sgmid.val('');
	});

	// IE8 DOES NOT SUPPORT THE CHANGE EVENT WHILE USING LIVE
	//var evt = jQuery.browser.msie ? "click" : "change";
	
	var is_chrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1;
	
	if(jQuery.browser.msie){
		var evt = "click";
	}
	if(jQuery.browser.mozilla){
		var evt = "change";
	}
	if(is_chrome){
		var evt = "blur";
	}
		
	jQuery('select.exibicaoSegmento').live(evt,function(){
		
		if(evt=='click'){
			jQuery(this).change();
		}
				
		var td 			= jQuery(this).parent('td');
		var sgmid       = td.find('select');
		var dunsegmento = td.find('input');
		var link        = td.find('a');
	
		var objAtual = td.find('select option:selected');
		var id_option_selecionado = td.find('select option:selected').attr('id');
		if( sgmid.val() == 10 ){
			dunsegmento.show();
			dunsegmento.attr('class','');
			link.show();
			sgmid.hide();
			link.attr('class','exibicaoSegmento');
		} else if (sgmid.val() == 1){
			jQuery("select[name=sgmid[]] option:selected").not(objAtual).each(function(){
				if(jQuery(this).val() == 1){
					alert('Não é possível cadastrar dois Dirigentes.');
					sgmid.val('');
					return false;
				}
			});
		}
	});

	if(jQuery("select.[name=sgmid[]]").length > 0){
		jQuery("select.[name=sgmid[]] option:selected").each(function(){
			if(jQuery(this).val() == 10){
				jQuery('.exibicaoSegmento').change();
			}
		});
	}
	
	if(jQuery("input.[name=duncpf[]]").length > 0){
		jQuery("input.[name=duncpf[]]").each(function(){
			if(jQuery(this).val() != ''){
				jQuery(this).val(mascaraglobal('###.###.###-##',jQuery(this).val()));
			}
		});
	}
	
	jQuery('.add').click(function(){
		var $campos = jQuery('#tbodyTabela'),
				$tr = $campos.find('tr:first').clone();
				//var id_cpf = $tr.find("input.[name=duncpf[]]").attr('id');
				//var tamanho = id_cpf.length;
				//var novo_idcpf = parseInt(id_cpf.substring(4,tamanho))+1;
				//$tr.find("input.[name=duncpf[]]").attr('id','cpf_'+novo_idcpf);

				// sempre colocar o select e não o input de outros
				$tr.find('td:last').prev().find('select[name=sgmid[]]').css('display', 'inline');
				$tr.find('td:last').prev().find('input[name=dunsegmento[]]').css('display', 'none');
				$tr.find('td:last').prev().find('a.exibicaoSegmento').css('display', 'none');
				
				$tr.find("input").val("");
				$tr.find("select").val("");
				$tr.find("label").html("");
			$campos.append($tr);
		return false;
	});
	
	jQuery('.removeItens').live('click',function(){
		var filhos = jQuery('#tbodyTabela').children().length;
		if (filhos >= 1) {
			var id = jQuery(this).attr('id');
			var tr = jQuery(this).closest('tr');
			var param = new Array();
				param.push({name : 'requisicao', value : 'excluirDadosUnidade'}, 
						   {name : 'dunid', value : id}
				);
				jQuery.ajax({
					type	 : "POST",
					url		 : "par.php?modulo=principal/equipeLocal&acao=A",
					data	 : param,
					async    : false,
					success	 : function(data){
									if(trim(data) == 'sucesso'){
										if( filhos == 1 ){
											jQuery('.add').trigger('click');
										}
										tr.remove();
									}
							   }
					 });
		}
		return false;
	});
	
	jQuery('input.classcpf').live('blur',function(){

		if(evt=='click'){
			jQuery(this).change();
		}

		if( !validar_cpf( jQuery(this).val()  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			jQuery(this).val('');
			jQuery(this).parent().parent().find("input[name=dunnome[]]").val('');
			jQuery(this).parent().parent().find("label").html('');
			return false;	
		}
		
		var comp  = new dCPF();
		var td 	  = jQuery(this).parent('td').next();
		var nome  = td.find('input');
		var label = td.find('label');
		
		comp.buscarDados( jQuery(this).val() );
		if (comp.dados.no_pessoa_rf != ''){
			nome.val(comp.dados.no_pessoa_rf);
			label.html(comp.dados.no_pessoa_rf);
			nome.attr("readonly","readonly");
		}

	});
	
	jQuery('input.classcpf').live('blur',function(){
		var cpfAtual = jQuery(this);
		var idCpfAtual = jQuery(this).attr('id');
		var valorCpfAtual = jQuery(this).val();
		var boDuplicado = false;

		jQuery("input.[name=duncpf[]]").each(function(){
			if(jQuery(this).val()){
				//jQuery("input.[name=duncpf[]]").not('#'+idCpfAtual).each(function(){
				jQuery("input.[name=duncpf[]]").not(cpfAtual).each(function(){
					if(jQuery(this).val() && (jQuery(this).val() == valorCpfAtual)){
						alert('CPF duplicado.');
						cpfAtual.val('');
						cpfAtual.parent().parent().find("input[name=dunnome[]]").val('');
						cpfAtual.parent().parent().find("label").html('');
						cpfAtual.focus();
						boDuplicado = true;
						return false;
					}
				});
			}
			if(boDuplicado){
				return false;
			}
		});

	});
	
	<?php if($boFormDesabilitado): ?>
		jQuery('#entnumcpfcnpj').attr("disabled", true);
		jQuery('[name=cadastra],[class=removeItens],[class=add]').remove();
		jQuery('input').attr("readonly", true);
		jQuery('select').attr("disabled", true);
		jQuery('textarea').attr("readonly", true);
		jQuery('#formulario').attr("onsubmit","return false");
		jQuery('#formulario').submit(function(){
			alert('Ação Indisponível!');
			return false;
		});
	<?php endif; ?>

});


function verModificacao(hdpid){
	window.open('par.php?modulo=principal/equipeLocal&acao=A&hdpid='+hdpid, 'blank', 'height=600,width=750,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
}


</script>
<?

monta_titulo( "Histórico de modificações", "Equipe Local" );
							 
$sql = "SELECT '<center><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"verModificacao(\''||hdp.hdpid||'\');\"></center>' as acao, usu.usunome, to_char(hdp.hpddatainsercao, 'dd/mm/YYYY HH24:MI') as hpddatainsercao 
		FROM par.historicodadospar hdp
		LEFT JOIN seguranca.usuario usu ON usu.usucpf = hdp.usucpf 
		WHERE hdplink='/par/par.php?modulo=principal/equipeLocal&acao=A' ".(($_SESSION['par']['itrid']==2)?"AND hdp.muncod='".$_SESSION['par']['muncod']."' ":"").(($_SESSION['par']['itrid']==1)?"AND hdp.estuf='".$_SESSION['par']['estuf']."' AND hdp.muncod IS NULL":"");

$cabecalho = array("&nbsp;","Nome","Data alteração");
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);


?>