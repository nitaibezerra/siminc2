<?php
require_once APPRAIZ . "includes/classes/dateTime.inc";

function mensagemAcossiacao(){
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloCentro" align="center"><font color="red"><?php echo 'É necessário Associar a uma Unidade' ?></font></td>
	</tr>
</table>
	<?php
	die;
}

/*
 * Montar árvore
 */
function montarArvore($_tartarefa = null, $boCarregaLinkAjax = false, $boSomenteTabela = false){
	global $db;
	
	echo "<div id=\"lista\">";
	echo "<table id=\"tabela_tarefa\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellpadding=\"3\" align=\"center\">";
	echo "<tr style=\"background-color: #e0e0e0\">
				<td style=\"font-weight:bold; text-align:center; width:5%;\">Ação</td>
				<td style=\"font-weight:bold; text-align:center; width:5%;\">Prioridade</td>			
				<td style=\"font-weight:bold; text-align:center;\">Título</td>
				<td style=\"font-weight:bold; text-align:center; width:30%;\">Responsável</td>
				<td style=\"font-weight:bold; text-align:center; width:10%;\">Situação</td>
				<td style=\"font-weight:bold; text-align:center; width:10%;\">Prazo de Atendimento</td>			
				<td style=\"font-weight:bold; text-align:center; width:50px;\">Ordem</td>			
			</tr>
		  ";
	echo "</table>
		  </div>";
	if(!$boSomenteTabela){
		echo "<script type=\"text/javascript\">
				montaPai('$_tartarefa', '', '$boCarregaLinkAjax');
			  </script>"; 
	}

}

function blocoDadosTarefa(&$obTarefa, &$arCadTarefa = null, &$instituicoesSelecionadas = null){
	global $db;

	if($_SESSION['tarefa']['boPerfilSuperUsuario']){
		$habilitado = 'S';
	} else {
		$habilitado = $_SESSION['tarefa']['boPerfilGerente'];
		if($habilitado == 'N'){
			$disabled = 'disabled="disabled"';
		}
	}

?>
<tr>
	<td align="left" colspan="2"><b>Dados da Tarefa</b></td>
</tr>
<tr>
	<td class="SubTituloDireita">Título:</td>
	<td><?$tartitulo = $obTarefa->tartitulo;?> <?= campo_texto( 'tartitulo', 'S', $habilitado, 'Título', 60, 255, '', '','','','','id="tartitulo"', '', $tartitulo); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Modalidade:</td>
	<td><?php
	$sql = "select tmdid, tmddescricao from tarefa.tipomodalidade";
	$arTipoModalidade = $db->carregar($sql);
	$arTipoModalidade = (is_array($arTipoModalidade)) ? $arTipoModalidade : array();
	foreach($arTipoModalidade as $modalidade){
		if($obTarefa->tmdid == $modalidade['tmdid']){
			$ckecked = "checked=\"checked\"";
		}
		echo "<input type=\"radio\" $var $ckecked id=\"{$modalidade['tmddescricao']}\" title=\"Modalidade\" name=\"tmdid\" value=\"{$modalidade['tmdid']}\" align=\"bottom\"><label for=\"{$modalidade['tmddescricao']}\">{$modalidade['tmddescricao']}</label>";
		$ckecked = "";
	}
	?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Tema:</td>
	<td><?$tartema = $obTarefa->tartema; ?> <?= campo_texto( 'tartema', 'S', $habilitado, 'Tema', 60, 50, '', '','','','','id="tartema"', '', $tartema); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Descrição:</td>
	<td><?$tardsc = $obTarefa->tardsc; ?> <?= campo_textarea( 'tardsc', 'N', $habilitado, 'Descrição ', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição', $tardsc ); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Número SIDOC:</td>
	<?$tarnumsidoc= $obTarefa->tarnumsidoc; ?>
	<?$tartiponumsidoc= $obTarefa->tartiponumsidoc; ?>
	<input type="hidden" name="hid_tarnumsidoc" value="<?=$tarnumsidoc?>" id="hid_tarnumsidoc">
	<input type="hidden" name="hid_tartiponumsidoc" value="<?=$tartiponumsidoc?>" id="hid_tartiponumsidoc">
	<td>
		<input type="radio" name="tartiponumsidoc" id="tartiponumsidoc" value="D" <?if( $tartiponumsidoc == 'D' ) echo 'checked=checked'; ?> onchange="mudarTipoSIDOC('D'); verificaSIDOC(this.value);"> &nbsp;Documento
		&nbsp;&nbsp;&nbsp;&nbsp; 
		<input type="radio" name="tartiponumsidoc" id="tartiponumsidoc" value="P" <?if( $tartiponumsidoc == 'P' ) echo 'checked=checked'; ?> onchange="mudarTipoSIDOC('P'); verificaSIDOC(this.value);"> &nbsp;Processo<br>
		<div id="div_numerosidoc"></div>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Data do Recebimento:</td>
	<td><?php 
	$tardatarecebimento = ($obTarefa->tardatarecebimento) ? $obTarefa->tardatarecebimento : date("Y/m/d");
	if($arCadTarefa['tardatarecebimento']){
		$obData = new Data();
		$tardatarecebimento = $obData->formataData($tardatarecebimento,"YYYY-mm-dd");
	}
	?> <?= campo_data2( 'tardatarecebimento','S', $habilitado, 'Data do Recebimento', 'S', '', 'validaDataRecebimento(this)', $tardatarecebimento ); ?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" align="right">Data de Início:</td>
	<td><?php 
	$tardatainicio = ($obTarefa->tardatainicio) ? $obTarefa->tardatainicio : date("Y/m/d");
	if($arCadTarefa['tardatainicio']){
		$obData = new Data();
		$tardatainicio = $obData->formataData($tardatainicio,"YYYY-mm-dd");
	}
	?> <?= campo_data2( 'tardatainicio','S', 'N', 'Data de Início', 'S', '', '', $tardatainicio ); ?>
	</td>
</tr>
<tr>
	<td align='right' class="SubTituloDireita" valign="top">Instituição	Avaliada: 
		<input type="hidden" id="instituicao_campo_flag" name="instituicao_campo_flag" value="<?= $instituicoesSelecionadas; ?>" />
	</td>
	<td>
	<div id="instituicao_campo_on"><? 
	//				$sql_combo = "SELECT e.entid as codigo, ee.entcodent || ' - ' || e.entnome || ' - ' || eend.estuf || ' - ' || m.mundescricao as descricao
	//								  FROM entidade.entidade e
	//								  inner join entidade.funcaoentidade fe on e.entid = fe.entid
	//								  inner join entidade.funcao f on fe.funid = f.funid
	//								  inner join entidade.entidadedetalhe ee on e.entcodent = ee.entcodent
	//								  inner join entidade.endereco eend on eend.entid = e.entid
	//								  inner join territorios.municipio m on eend.muncod = m.muncod
	//							  where fe.funid in (3,4,12,18)
	//								--limit 10";
/*	$sql_combo = "SELECT  iescodigo as codigo , iesnome  || ' - ' || iessigla || ' - ' || iesuf || ' - ' || iesmunicipio as descricao
 	 			  FROM ies.ies ORDER BY iesnome";
*/
	$arFiltro = array(	array("codigo"    => 'iescodigo',
	  							 		     "descricao" => 'Código',
	  									     "tipo" 	 => 1 ),
						array("codigo"    => 'iesnome',
						  		 		     "descricao" => 'Descrição',
						  				     "tipo" 	 => 0 ),
						array("codigo"    => 'iesuf',
						  		 		     "descricao" => 'UF',
						  				     "tipo" 	 => 0 ),
						array("codigo"    => 'iesmunicipio',
						  		 		     "descricao" => 'Cidade',
						  				     "tipo" 	 => 0 )
	);

	//Código existente
	/*if(is_array($arCadTarefa['instituicoes']) && $arCadTarefa['instituicoes'][0]){
		$stInstituicoes = implode(',',$arCadTarefa['instituicoes']);
		//$instituicoes = $db->carregar("SELECT e.entid as codigo, e.entnome as descricao FROM entidade.entidade e where e.entid in ({$stInstituicoes}) ");
		$instituicoes = $db->carregar("SELECT DISTINCT iescodigo as codigo , iesnome as descricao, iessigla, iesuf, iesmunicipio FROM ies.ies  ");
	} elseif($obTarefa->tarid){
		$instituicoes = $obTarefa->recuperaInstituicoesPorTarid($obTarefa->tarid);
	}*/
	
	//Código acima modificado por Marcelo Santos
	if($obTarefa->tarid){
		//$stInstituicoes = implode(',',$arCadTarefa['instituicoes']);
		//$instituicoes = $db->carregar("SELECT e.entid as codigo, e.entnome as descricao FROM entidade.entidade e where e.entid in ({$stInstituicoes}) ");
		$instituicoes = $obTarefa->tarid;
	} else {
		$instituicoes = $instituicoesSelecionadas;
	}
	
	//dbg($instituicoes,1);
	
	//combo_popup( 'instituicoes', $sql_combo, 'Selecione a(s) Instituição(ões)', '400x400', 0, array(), '', $habilitado, true, true, 10, 400, null, null, false, $arFiltro, $instituicoes, false );

	$img 		= "/imagens/gif_inclui.gif";
	$jsFunction = "onclick=\"return abrirPopupInstituicao('checkbox');\"";
	?> <span style="cursor: pointer" <?=$jsFunction; ?>
		id="linkInserirInstituicao"><img src="<?=$img;?>" align="absmiddle"
		style="border: none" /> Adicionar Instituição</span> <br>
	<br>

	<div id="div_tabela_instituicao"><?php
		//Código acima modificado por Marcelo Santos
		if($obTarefa->tarid){
			//$stInstituicoes = implode(',',$arCadTarefa['instituicoes']);
			//$instituicoes = $db->carregar("SELECT e.entid as codigo, e.entnome as descricao FROM entidade.entidade e where e.entid in ({$stInstituicoes}) ");
			$instituicoes = $obTarefa->tarid;
			echo listaInstituicoes('id',$instituicoes);
		} else {
			$instituicoes = $instituicoesSelecionadas;
			echo listaInstituicoes('inst',$instituicoes);
		}
?>
	</div>
	
	</div>
	</td>
</tr>
<script type="text/javascript">
	loadMask();
	listarIescodigo();
	function loadMask(){
		var hid_tarnumsidoc = document.getElementById('hid_tarnumsidoc'); 
		top.boSIDOC = 'f';
		hid_tartiponumsidoc = document.getElementById('hid_tartiponumsidoc'); 
		verificaSIDOC(hid_tartiponumsidoc.value);
		if( hid_tartiponumsidoc.value != '' ){ 
			numsidoc = document.getElementById('numsidoc');
			numsidoc.value = hid_tarnumsidoc.value;
			if( hid_tartiponumsidoc.value == 'D' ){
				numsidoc.value = mascaraglobal('######/####-##', hid_tarnumsidoc.value ); 
				//numsidoc.maxlength = '14';
			}else if(hid_tartiponumsidoc.value == 'P' ){
				numsidoc.value = mascaraglobal('#####.######/####-##', hid_tarnumsidoc.value);
				//numsidoc.maxlength = '20';
			}
		}
	}
	function verificaSIDOC(tipo){
		if( !tipo ){
			return false;
		}  
		var div   = document.getElementById('div_numerosidoc');
		var tarnumsidoc = document.getElementById('hid_tarnumsidoc');
		var input = document.createElement("input");
		if( top.boSIDOC == 'f'){
			var input_;
			if( tipo == 'D' ){
				tarnumsidoc.value = mascaraglobal('######/####-##', tarnumsidoc.value ); 
				input_ = "&nbsp;<input type=\"text\" value=\""+tarnumsidoc.value+"\" size=\"25\" name=\"tarnumsidoc\" id=\"tarnumsidoc\" onkeydown=\"verificaQtdSIDOC(this.id, 'D');\" onchange=\"this.value=mascaraglobal('######/####-##',this.value);\"  onkeyup=\"this.value=mascaraglobal('######/####-##',this.value);\">";			
			}else
			if(tipo == 'P'){
				tarnumsidoc.value = mascaraglobal('#####.######/####-##', tarnumsidoc.value);
				input_ = "&nbsp;<input type=\"text\" value=\""+tarnumsidoc.value+"\" size=\"25\" name=\"tarnumsidoc\" id=\"tarnumsidoc\" onkeydown=\"verificaQtdSIDOC(this.id, 'P');\"  onchange=\"this.value=mascaraglobal('#####.######/####-##',this.value); \" onkeyup=\"this.value=mascaraglobal('#####.######/####-##',this.value);\">";
			}
			div.innerHTML = input_;
		}else{
			var old_input	= document.getElementById('hid_tarnumsidoc');
			var input		= document.getElementById('tarnumsidoc');
			var radio		= document.getElementById('tartiponumsidoc'); 
			if( tipo == 'D' ){		
				input.value = mascaraglobal('######/####-##', input.value ); 
				input.setAttribute('onkeyup', this.value=mascaraglobal('######/####-##',this.value) );
			}else if( tipo == 'P' ){	
				input.value = mascaraglobal('#####.######/####-##', input.value); 
				input.setAttribute('onkeyup', this.value=mascaraglobal('#####.######/####-##',this.value) );
			}	
		}  
		top.boSIDOC = 't';
	}
	function mudarTipoSIDOC(tipo){  
		top.selecSIDOC = tipo;
	}
	function verificaQtdSIDOC(id,tipo){
		
		if(! top.selecSIDOC ){
			return false;
		}
		if( !id ){
			return false;
		}  
		top.boSIDOC = 'f';
		if( top.selecSIDOC == 'D' ){
			if( document.getElementById(id).value.length > 14 ){ 
				document.getElementById(id).value =document.getElementById(id).value.substr(0, 14); 
				verificaSIDOC('D');
 				return false;
			}
		}else if( top.selecSIDOC == 'P' ){ 
			if( document.getElementById(id).value.length > 20 ){ 
				document.getElementById(id).value =document.getElementById(id).value.substr(0, 20); 
				verificaSIDOC('P');
 				return false;
			}
		} 
	}
	function abrirPopupInstituicao(tipo){ 
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: '',   
			onComplete: function(r)
			{ 
				window.open('tarefa.php?modulo=principal/popupInstituicoes&acao=A&type='+tipo,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=500');
		 
			}
		});
	}
	function excluiIescodigo( codigo ){
		var req = new Ajax.Request('ajax.php', {
							        method:     'post',
							        parameters: '&iescodDel=' + codigo + '&tipo=excluirIescodigo',
							        onComplete: function (res)
							        {
							        	document.getElementById('div_tabela_instituicao').innerHTML = res.responseText; 
							        }
							  });
	}
	function listarIescodigo(){
		var req = new Ajax.Request('ajax.php', {
							        method:     'post',
							        parameters: 'tipo=listarIescodigo',
							        onComplete: function (res)
							        {
							        	document.getElementById('div_tabela_instituicao').innerHTML = res.responseText; 
							        }
							  });
	}
	<!--
		function validaDataRecebimento(obj){
			var data1 = obj.value;
			var data2 = $('tardatainicio').value;
			
			data1 = parseInt( data1.split( "/" )[2].toString() + data1.split( "/" )[1].toString() + data1.split( "/" )[0].toString() );
			data2 = parseInt( data2.split( "/" )[2].toString() + data2.split( "/" )[1].toString() + data2.split( "/" )[0].toString() );
			
			if ( data1 > data2 ) {
			    alert('A Data do Recebimento não pode ser maior que a data de início');
				obj.value = "";
				obj.focus();
			}
		}
	//-->
	</script>
	<?php
}

function blocoDadosAtendimento(&$obTarefa, $boMensagemObrig = 'N', $boCadAcompanhamento = false, $boCadTarefa = false, $acodsc = '', $arCadTarefa = array(), $boCadAtividade = false, $db = false){
	if(!$db){
		global $db;
	}

	if($_SESSION['tarefa']['boPerfilSuperUsuario']){
		$habilitado = 'S';
	} else {
		$habilitado = $_SESSION['tarefa']['boPerfilGerente'];
		if($habilitado == 'N'){
			$disabled = 'disabled="disabled"';
		}
	}
	if($boCadAcompanhamento){ # se for estiver no cadastro de tarefa, não monta tabela
		?>
<!-- DADOS DO ATENDIMENTO -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
}
?>
	<tr>
		<td align="left" colspan="2"><b>Dados do Atendimento</b></td>
	</tr>
	<?php
	if($boCadAcompanhamento){
		?>
	<tr>
		<td class="SubTituloDireita" align="right" valign="top" width="300px">
		<?php
		$boAtividade = $obTarefa->boAtividade();
		if($boAtividade){
			echo 'Atividade';
		} else {
			echo 'Tarefa';
		}
		?></td>
		<td><?php
		echo "<b>".$obTarefa->tartitulo."</b><br />".$obTarefa->tardsc;
		?></td>
	</tr>
	<?php
}
?>
	<tr>
		<td class="SubTituloDireita">Prioridade:</td>
		<td><?php
		$arPrioridade = array();
		$arPrioridade['N'] = "Normal";
		$arPrioridade['A'] = "Alta";
		$arPrioridade['U'] = "Urgente";
		foreach($arPrioridade as $valor=>$prioridade){
			if($obTarefa->tarprioridade == $valor){
				$ckecked = "checked=\"checked\"";
			} else {
				if($valor == 'N'){
					$ckecked = "checked=\"checked\"";
				}
			}
			echo "<input type=\"radio\" $disabled $ckecked id=\"{$prioridade}\" name=\"tarprioridade\" title=\"Prioridade\" value=\"{$valor}\" align=\"bottom\"><label for=\"{$prioridade}\">{$prioridade}</label>";
			$ckecked = "";
		}
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Setor de Origem:</td>
		<td><?php
		if($_SESSION['tarefa']['boPerfilSuperUsuario']){
			if($obTarefa->unaidsetororigem){
				$unaidsetororigem = $obTarefa->unaidsetororigem;
			}
			$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade ORDER BY unasigla";
			$db->monta_combo( "unaidsetororigem", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'unaidsetororigem',false,$unaidsetororigem,'Setor de Origem');
		} else {
			$unaidsetororigem = $_SESSION['tarefa']['unaid'];
			if($obTarefa->unaidsetororigem){
				$unaidsetororigem = $obTarefa->unaidsetororigem;
			}
			$sql = "SELECT unasigla ||' - '|| unadescricao as descricao FROM tarefa.unidade where unaid = {$unaidsetororigem}";
			echo $db->pegaUm($sql);
			echo "<input type=\"hidden\" value=\"{$unaidsetororigem}\" name=\"unaidsetororigem\" id=\"unaidsetororigem\">";
		}
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Setor Responsável:</td>
		<td><?php 
		$unaidsetorresponsavel = $obTarefa->unaidsetorresponsavel;

		if(!$unaidsetorresponsavel)
		$unaidsetorresponsavel = $unaidsetororigem;
			
		$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade ORDER BY unasigla";
		$db->monta_combo( "unaidsetorresponsavel", $sql, $habilitado, 'Selecione...', 'filtraSetorRespon', '', '', '', 'S', 'unaidsetorresponsavel',false,$unaidsetorresponsavel,'Setor de Responsável');
		?> <input type="hidden" value="<?php echo $unaidsetorresponsavel; ?>"
			name="unaidsetorresponsavelAnterior"
			id="unaidsetorresponsavelAnterior"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Responsável pela Tarefa:</td>
		<td id="td_usucpfresponsavel2"><?php 
		if ($obTarefa->unaidsetorresponsavel && $obTarefa->usucpfresponsavel) {
			if($_SESSION['tarefa']['boPerfilSuperUsuario']){
				$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao
							from tarefa.usuarioresponsabilidade ur
   							inner join seguranca.usuario u on ur.usucpf = u.usucpf
   							where ur.rpustatus = 'A' order by u.usunome";
			} else {
				$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao
								from tarefa.usuarioresponsabilidade ur
	   							inner join seguranca.usuario u on ur.usucpf = u.usucpf
	   							where ur.unaid = {$obTarefa->unaidsetorresponsavel} and ur.rpustatus = 'A' order by u.usunome ";
			}
			$usucpfresponsavel = $obTarefa->usucpfresponsavel;
			$db->monta_combo('usucpfresponsavel', $sql, $habilitado, "Selecione...", '', '', '', '400', 'N', 'usucpfresponsavel',false,$usucpfresponsavel,'Responsável pela Tarefa');
		} else {
			$db->monta_combo('usucpfresponsavel', array(), $habilitado, "Selecione um Setor Responsável", '', '', '', '400', 'N', 'usucpfresponsavel',false,null,'Responsável pela Tarefa');
		}
		?></td>
		<input type="hidden" value="<?php echo $usucpfresponsavel; ?>"
			name="usucpfresponsavelAnterior" id="usucpfresponsavelAnterior">
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Prazo para Atendimento:</td>
		<td><?php 
		$obData = new Data();
		$tardataprazoatendimento  = $obTarefa->tardataprazoatendimento;
		if($tardataprazoatendimento){
			$tardataprazoatendimentoAnterior  = $obData->formataData($tardataprazoatendimento,"dd/mm/YYYY");
		}
		if($arCadTarefa['tardataprazoatendimento']){
			$obData = new Data();
			$tardataprazoatendimento = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
		}

		echo campo_data2( 'tardataprazoatendimento','S', $habilitado, 'Prazo para Atendimento', 'S','', 'verificaDataPaiEDataFilha(this, $(\'tarid\').value, 1);', $tardataprazoatendimento );
		?> <input type="hidden"
			value="<?php echo $tardataprazoatendimentoAnterior; ?>"
			name="tardataprazoatendimentoAnterior"
			id="tardataprazoatendimentoAnterior"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">Situação da Tarefa:</td>
		<td><?php 
		$sql = "SELECT
                            sitid AS codigo, 
                            sitdsc AS descricao
                        FROM
                            tarefa.situacaotarefa order by codigo";
			
		$sitid = $obTarefa->sitid;
		$tarid = $obTarefa->tarid;
		$db->monta_combo('sitid', $sql, $habilitado, "", '', '', '', '200', 'S', 'sitid',false,$sitid,'Situação da Tarefa');
		?> <input type="hidden" value="<?php echo $sitid; ?>"
			name="sitidAnterior" id="sitidAnterior"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Dependência Externa:</td>
		<td><?$tardepexterna = $obTarefa->tardepexterna; ?> <?= campo_texto( 'tardepexterna', 'N', '', 'Dependência Externa', 60, 100, '', '','','','','id="tardepexterna"', '', $tardepexterna); ?>
		<input type="hidden" value="<?php echo $tardepexterna; ?>"
			name="tardepexternaAnterior" id="tardepexternaAnterior"></td>
	</tr>
	<tr>
		<td align="left" colspan="2"><b>Mensagem</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Mensagem:</td>
		<td><?= campo_textarea( 'acodsc', $boMensagemObrig, '', 'Mensagem ', 80 , 8, 1500, '', 0, '', false, 'Mensagem' ); ?>
		</td>
	</tr>
	<?
	if($boCadAcompanhamento){
		?>
</table>
		<?
}
}

function blocoDadosSolicitante(&$obTarefa, &$arCadTarefa = null){
	global $db;
	if($_SESSION['tarefa']['boPerfilSuperUsuario']){
		$habilitado = 'S';
	} else {
		$habilitado = $_SESSION['tarefa']['boPerfilGerente'];
	}
	?>
<!-- DADOS DO SOLICITANTE -->
<tr>
	<td align="left" colspan="2"><b>Dados do(s) Solicitante(s) </b></td>
</tr>
<tr>
	<td class="SubTituloDireita">Solicitante(s):</td>
	<td><?php
	if($habilitado == 'S'){
		$jsFunction = "onclick=\"return abrirPopupSolicitante('','".$obTarefa->tarid."');\"";
		$img 		= "/imagens/gif_inclui.gif";
	} else {
		$jsFunction = "";
		$img 		= "/imagens/gif_inclui_d.gif";
	}
	?> <span style="cursor: pointer" <?=$jsFunction; ?>
		id="linkInserirSolicitante"><img src="<?=$img;?>" align="absmiddle"
		style="border: none" /> Adicionar Solicitante</span> <br>
	<br>
	<div id="div_tabela_solicitante">
	<table class="tabela_listagem" width="600px" id="listaSolicitante">
		<tr>
			<th>Ação</th>
			<th>Entidade/Unidade</th>
			<th>Nome do Solicitante</th>
		</tr>
		<?php
		$obSolicitante = new Solicitante();
		$taridAterior = $_SESSION['arSolicitante'][0][0]['tarid'];
		if(!$_SESSION['tarid'] && $_GET['acao'] == 'I' && !$arCadTarefa ){
			$_SESSION['arSolicitante'] = array();
		}
			
		if(isset($_SESSION['arSolicitante']) && is_array($_SESSION['arSolicitante']) && $_SESSION['arSolicitante'][0][0]['solnome']){
			foreach($_SESSION['arSolicitante'] as $arSolicitante){
				echo listaSolicitantes($arSolicitante, $obTarefa->tarid);
			}
		} else {
			if($obTarefa->tarid){
				$_SESSION['arSolicitante'] = array();
				$arSolicitante = $obSolicitante->carregaPorTarefa($obTarefa->tarid);
				$arSolicitante = ($arSolicitante) ? $arSolicitante : array();
				$_SESSION['arSolicitante'][0] = $arSolicitante;
				foreach($_SESSION['arSolicitante'] as $arSolicitante){
					echo listaSolicitantes($arSolicitante, $obTarefa->tarid);
				}
			} else {
				$_SESSION['arSolicitante'] = array();
			}
		}

		?>
	</table>
	</div>
	</td>
</tr>
		<?php
}

function cabecalhoTarefa($tarid,$db = false){
	if(!$db){
		global $db;
	}

	$obTarefa  = new Tarefa();
	$tartarefa = $obTarefa->pegaTartarefaPorTarid($tarid);
	$obTarefa  = new Tarefa($tartarefa);
	$_SESSION['dados_tarefa']['tarid'] = $obTarefa->tarid;
	$obData = new Data();
	$_SESSION['dados_tarefa']['tardataprazoatendimento'] = $obData->formataData($obTarefa->tardataprazoatendimento,"dd/mm/YYYY");
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td align="left" colspan="4"><b>Dados da Tarefa</b></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="300px">Número SIDOC:</td>
		<td width="230px">
		<!--  <input type="text" size="50" name="nrsidoc" readonly id="nrsidoc" style="border:0;background-color:f5f5f5;font: 12px;"></input>--> 
			<?php 
				$tarnumsidoc= $obTarefa->tarnumsidoc;
				$tartiponumsidoc= $obTarefa->tartiponumsidoc;

				if( $tartiponumsidoc == 'D' )
					echo substr($tarnumsidoc,0,6)."/".substr($tarnumsidoc,6,4)."-".substr($tarnumsidoc,10,2);
				else
					//$mask = "#####.######/####-##";*/
					echo substr($tarnumsidoc,0,5).".".substr($tarnumsidoc,5,6)."/".substr($tarnumsidoc,11,4)."-".substr($tarnumsidoc,15,2);
						
//				echo "<script>document.getElementById('nrsidoc').value = mascaraglobal('$mask',$tarnumsidoc);</script>";
				
			?>
		</td>
		<td class="SubTituloDireita" width="150" colspan="2"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="300px">Código da Tarefa:</td>
		<td width="230px"><?php echo $obTarefa->tarid; ?></td>
		<td class="SubTituloDireita" width="150">Título:</td>
		<td><?php echo $obTarefa->tartitulo; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Modalidade:</td>
		<td><?php 
		if($obTarefa->tmdid){
			$tmddescricao = $db->pegaUm("SELECT tmddescricao FROM tarefa.tipomodalidade WHERE tmdid = {$obTarefa->tmdid}");
			echo $tmddescricao;
		}
		?></td>
		<td class="SubTituloDireita" width="150">Tema:</td>
		<td><?php echo $obTarefa->tartema; ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Solicitantes:</td>
		<td><?php 
		$solicitantesTemp = "";
		$arSolicitantes = $obTarefa->recuperaSolicitantesPorTarid($obTarefa->tarid);
		foreach($arSolicitantes as $solicitantes){
			$solicitantesTemp .= $solicitantes['solnome'].", ";
		}
		if($solicitantesTemp){
			echo substr($solicitantesTemp,0,strlen($solicitantesTemp)-2);
		}
		?></td>
		<td class="SubTituloDireita">Instituições Avaliadas:</td>
		<td><?php 
		$arInstituicoes = $obTarefa->recuperaInstituicoesPorTarid($obTarefa->tarid);
		$instituicoesTemp = "";
		$arInstituicoes = ($arInstituicoes) ? $arInstituicoes : array();
		foreach($arInstituicoes as $instituicoes){
			$instituicoesTemp .= $instituicoes['descricao'].", ";
		}
		if($instituicoesTemp){
			echo substr($instituicoesTemp,0,strlen($instituicoesTemp)-2);
		}
		?></td>
	</tr>
</table>
		<?php
}

function listaAtendimento($tarid, $db = false){
	if(!$db){
		global $db;
	}

	$sql = "SELECT
			 to_char(a.acodata, 'DD/MM/YYYY HH24:MI:SS') AS data,
			 u.usunome,
			 a.acodsc
			FROM
			 tarefa.acompanhamento AS a 
			 LEFT JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf
		    WHERE
		     a.tarid = '{$tarid}' AND 
		     a.acostatus = 'A' 
		    ORDER BY
		     a.acoid DESC";
	 
	$arDados = $db->carregar($sql);
	?>
<table class="listagem" width="95%" align="center" border="0"
	cellpadding="2" cellspacing="0">
	<thead>
		<tr>
			<td width="27%" class="title"
				style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"
				valign="top"><strong>Autor / Postado</strong></td>
			<td class="title"
				style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);"
				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"
				valign="top"><strong>Mensagem</strong></td>
		</tr>
	</thead>
	<?
	$i = 0;
	if($arDados[0]){
		foreach($arDados as $dados){
			if(($i % 2) == 1) {
				$fundo="#F7F7F7";
			} else {
				$fundo="#FFFFFF";
			}
			?>
	<tr onmouseover="this.bgColor='#ffffcc';"
		onmouseout="this.bgColor='<?=$fundo ?>';" bgcolor="<?=$fundo ?>">
		<td valign="top"><b><?php echo $dados['usunome'];?></b><br />
		<?php echo $dados['data'];?></td>
		<td><?php echo $dados['acodsc'];?></td>
	</tr>
	<?
	$i++;
}
} else {

	?>
	<tr onmouseover="this.bgColor='#ffffcc';"
		onmouseout="this.bgColor='<?=$fundo ?>';" bgcolor="<?=$fundo ?>">
		<td valign="top" colspan="2" align="center"><font color="#FF0000">Não
		foi encontrado nenhum registro.</font></td>
	</tr>
	<?
}
?>
</table>
<?php
exit;
}

function listaSolicitantes(&$arSolicitante,$tarid){
	global $db;

	if($_SESSION['tarefa']['boPerfilSuperUsuario']){
		$habilitado = 'S';
	} else {
		$habilitado = $_SESSION['tarefa']['boPerfilGerente'];
	}
 	
	foreach($arSolicitante as $posicao => $solicitante){
		if($habilitado == 'S'){
			$onClickAlterar = "onclick=\"abrirPopupSolicitante('$posicao', '$tarid');\"";
			$imgAlterar 	= "src=\"/imagens/alterar.gif\"";
			$onClickExcluir = "onclick=\"excluirSolicitante($posicao);\"";
			$imgExcluir 	= "src=\"/imagens/excluir.gif\"";
		} else {
			$onClickAlterar = "";
			$imgAlterar 	= "src=\"/imagens/alterar_01.gif\"";
			$onClickExcluir = "";
			$imgExcluir 	= "src=\"/imagens/excluir_01.gif\"";
		}
		$unidadeentidade = "";
		
		if($solicitante['unaid'] && !$solicitante['entid']){
			$unidadeentidade = $db->pegaUm("select unasigla||' - '||unadescricao as unidade from tarefa.unidade where unaid = {$solicitante['unaid']}");
		} elseif(!$solicitante['unaid'] && $solicitante['entid']){
			$unidadeentidade = $db->pegaUm("select entnome from entidade.entidade where entid = {$solicitante['entid']}");
		} elseif(!$solicitante['unaid'] && !$solicitante['entid'] && $solicitante['iesid']){
			$unidadeentidade = $db->pegaUm("select iescodigo ||' - '|| iessigla ||' - '|| iesnome from ies.ies where iesid = {$solicitante['iesid']}");
		}else{	
			$unidadeentidade = "<font color='red'><center>-</center></font>";
		}
		echo "<tr id=\"tr_sol$posicao\">";
		echo "<td width='50px' align='center'><a style=\"margin: 0 -5px 0 5px;\" href=\"#\" $onClickAlterar ><img $imgAlterar border=0 title=\"Alterar\"></a>&nbsp;<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" $onClickExcluir ><img $imgExcluir border=0 title=\"Excluir\"></a></td>";
		echo "<td width='400px'>$unidadeentidade</td>";
		echo "<td width='250px'>{$solicitante['solnome']}</td>";
		echo "</tr>";
	}
}


function listaInstituicoes($tipo,$codigo){
	global $db;
	if(!empty($codigo)){
		if($tipo=='id'){
			$sql = "select iesid from tarefa.instituicaorelacionada where tarid = $codigo";
		}
		else {
			$sql = "select iesid from ies.ies where iesid in ($codigo)";
		}
		$rs = $db->carregar( $sql );

		if($_SESSION['tarefa']['boPerfilSuperUsuario']){
			$habilitado = 'S';
		} else {
			$habilitado = $_SESSION['tarefa']['boPerfilGerente'];
		}
		echo "<table class=\"tabela_listagem\" width=\"600px\" id=\"listaInstituicao\">
	          		  	<tr>
	          		  		<th>Ação</th>
	          		  		<th>Sigla</th>
	          		  		<th>Instituição</th>
	          		  		<th>UF</th>
	          		  	</tr>";
	          		  	 
		if($rs){
			foreach($rs as $dados){
		
					$sql = "select iesid, iescodigo, iessigla, iesnome, iesuf FROM ies.ies where iesid = ".$dados['iesid'];
					$rs = $db->carregar( $sql );
					
					echo "<tr>
							<td><img src=\"/imagens/excluir.gif\" onclick=\"excluiIescodigo(".$rs[0]['iesid'] .");\" border=0 style=\"cursor: pointer;\"></img> </td>
							<td> ".$rs[0]['iessigla'] ."</td>
							<td> ".$rs[0]['iesnome'] ."</td>
							<td> ".$rs[0]['iesuf'] ."</td>
						  </tr>"; 
			} 
		}
		echo "</table>";
	}
}



function boExisteTarefa( $tarid, $boMensagem = false){
	global $db;
	$tarefa = "";

	if($tarid){
		$tarefa = $db->pegaUm("SELECT tarid FROM tarefa.tarefa WHERE tarid = {$tarid}");
		if( !$tarefa && $boMensagem){
			echo "<script>
					alert('A Tarefa / Atividade informada não existe!');
					history.back(-1);
				  </script>";
			die;
		} else {
			return true;
		}
	}
}

function pegaPerfil($usucpf){
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p 
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE p.sisid = '{$_SESSION['sisid']}'
			AND 
			pu.usucpf = '$usucpf'";


	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}

function arrayPerfil(){
	global $db;

	$sql = sprintf("SELECT
					 pu.pflcod
					FROM
					 seguranca.perfilusuario pu
					 INNER JOIN seguranca.perfil p ON p.pflcod = pu.pflcod AND
					 	p.sisid = 58
					WHERE
					 pu.usucpf = '%s'
					ORDER BY
					 p.pflnivel",
	$_SESSION['usucpf']);
	return (array) $db->carregarColuna($sql,'pflcod');
}

function possuiPerfil( $pflcods ){

	global $db;

	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	} else {
		$pflcods = array( (integer) $pflcods );
	} if ( count( $pflcods ) == 0 ) {
		return false;
	}
	$sql = "select
				count(*)
		from seguranca.perfilusuario
		where
			usucpf = '" . $_SESSION['usucpf'] . "' and
			pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
}

/**
 * Função que retorna o array para montar as abas do Acompanhamento
 *
 * @return array
 *
 */
function carregaAbasAcompanhamento($pagina,$ptaid='',$pacid='') {
	global $db;

	switch($pagina) {

		case 'cadAcompanhamento':
			$menu = array(
			0 => array("id" => 1, "descricao" => "Atendimento", "link" => "/tarefa/tarefa.php?modulo=principal/cadAcompanhamento&acao=A"),
			1 => array("id" => 2, "descricao" => "Restrição",   "link" => "/tarefa/tarefa.php?modulo=principal/cadRestricao&acao=A")
			);
			break;
				
		case 'cadRestricao':
			$menu = array(
			0 => array("id" => 1, "descricao" => "Atendimento", "link" => "/tarefa/tarefa.php?modulo=principal/cadAcompanhamento&acao=A"),
			1 => array("id" => 2, "descricao" => "Restrição",   "link" => "/tarefa/tarefa.php?modulo=principal/cadRestricao&acao=A")
			);
			break;
	}

	$menu = $menu ? $menu : array();
	 
	return $menu;
}

function pegaUnidadeAssociada($perfil){
	global $db;

	$sql = "SELECT unaid FROM tarefa.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '{$perfil}' ";
	$unidade = $db->pegaUm($sql);

	if($unidade){
		return $unidade;
	} else {
		return "Não existe Unidades atribuidas ao perfil para este CPF: {$_SESSION['usucpf']}.";
	}

	return false;
}

function pegaSetorUsuarioLogado($perfil){
	global $db;

	$sql = "SELECT unaid FROM tarefa.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and pflcod = '".$perfil."' ";
	$unidade = $db->pegaUm($sql);

	if($unidade){
		return $unidade;
	}

	return false;
}

function verificaSessao($boVerificaTarid = false){
	if($boVerificaTarid){
		if(!$_SESSION['dados_tarefa']['tarid']){
			echo "<script>
					alert('Sua sessão expirou.');
					window.location.href = 'tarefa.php?modulo=inicio&acao=C';
				  </script>";
			die;
		}
	}
	if((!$_SESSION['tarefa']['unaid']
	|| !$_SESSION['tarefa']['setorUsuarioLogado']
	|| !$_SESSION['tarefa']['boPerfilGerente']) && (
	!$_SESSION['tarefa']['boPerfilSuperUsuario'])
	){
		echo "<script>
				alert('Sua sessão expirou.');
				window.location.href = 'tarefa.php?modulo=inicio&acao=C';
			  </script>";
		die;
	}

	return false;
}

function dadosRetricao($db = null, &$tarid){
	if(!$db){
		global $db;
	}
	?>
<div id="divRestricao"><input type="hidden" name="boRestricao"
	id="boRestricao" value="1" /> <!-- script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
		<script language="JavaScript">
	//Editor de textos
	tinyMCE.init({
		theme : "advanced",
		mode: "specific_textareas",
		editor_selector : "text_editor_simple",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,link,bold,italic,underline,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		width : "450px",
		entity_encoding : "raw"
		});
	</script -->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center" border="0" width="95%">
	<tr>
		<td class="SubTituloDireita" align="right" valign="top" width="300px">
		<?php

		$obTarefa = new Tarefa($tarid);
		$boAtividade = $obTarefa->boAtividade();
		if($boAtividade){
			echo 'Atividade';
		} else {
			echo 'Tarefa';
		}
		?></td>
		<td><?php
		echo "<b>".$obTarefa->tartitulo."</b><br />".$obTarefa->tardsc;
		?></td>
	</tr>
	<tr>
		<td align="left"><b>Dados da Restrição</b></td>

	</tr>
	<tr>
		<td align='right' class="SubTituloDireita"
			style="vertical-align: top;" width="25%">Restrição:</td>
		<td><!--  textarea name="resdescricao" id="resdescricao" rows="10" cols="70" class="text_editor_simple"></textarea -->
		<?php echo campo_textarea( 'resdescricao', 'N', 'S', 'Restrição', 70, 8, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Restrição' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita"
			style="vertical-align: top;">Providência:</td>
		<td><?= campo_textarea( 'resmedida', 'N', 'S', '', 80, 3, 250 ); ?></td>
	</tr>
	<tr style="background-color: #cccccc">
		<td align='right' style="vertical-align: top;">&nbsp;</td>
		<td><input type="button" name="botao" value="Salvar"
			onclick="gravarRestricao('', <?= $obTarefa->_tartarefa ?>)" /></td>
	</tr>
</table>


<div id="divListaRestricao"><?php echo listaRetricao($db, $tarid); ?></div>
</div>
		<?
		die;
}


function listaRetricao($db, &$tarid){
	if(!$db){
		global $db;
	}

	$obTarefa = new Tarefa($tarid);

	$sql = "select
				r.resid, 
				to_char(r.resdata, 'DD/MM/YYYY') as data, 
				r.resdescricao, 
				r.usucpf, 
				r.resmedida,
				r.ressolucao,
				u.usunome,
				u.usufoneddd as dddresponsavel,
				u.usufonenum as telefoneresponsavel,
				tu.unadescricao
			from tarefa.restricao r 
				inner join seguranca.usuario u on r.usucpf = u.usucpf
				inner join tarefa.unidadeusuario uu on r.usucpf = uu.usucpf
				inner join tarefa.unidade tu on uu.unaid = tu.unaid
			where r.resstatus = 'A' and tarid = {$obTarefa->tarid}";
	$arRestricao = $db->carregar($sql);
	$arRestricao = ($arRestricao) ? $arRestricao : array();
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center" border="0" width="95%">
	<tbody>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align: top; width: 25%">&nbsp;</td>
			<td align='left' style="vertical-align: top;"><b>Restrição</b> <img
				src="../imagens/restricao.png" border="0" align="absmiddle"
				style="margin: 0 3px 0 3px;" /></td>
		</tr>
		<?php foreach ($arRestricao as $restricao){ ?>
		<tr>
			<td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Descrição:</td>
			<td id="celDescricao_<?= $restricao['resid'] ?>"
				name="celDescricao_<?= $restricao['resid'] ?>"><input type="hidden"
				id="hiddenDesc[<?= $restricao['resid'] ?>]"
				name="hiddenDesc[<?= $restricao['resid'] ?>]"
				value="<?= $restricao['resdescricao'] ?>" />
			<div id="divDesc1_<?= $restricao['resid'] ?>" style="display: none">
			<textarea name="resdescricao_<?= $restricao['resid'] ?>"
				id="resdescricao_<?= $restricao['resid'] ?>" rows="5" cols="70"
				class="text_editor_simple"><?= $restricao['resdescricao'] ?></textarea>
			</div>
			<div id="divDesc2_<?= $restricao['resid'] ?>" style="display: ''"><?= $restricao['resdescricao'] ?>
			</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Data:</td>
			<input type="hidden" id="hiddenData[<?= $restricao['resid'] ?>]"
				name="hiddenData[<?= $restricao['resid'] ?>]"
				value="<?= $restricao['data'] ?>" />
			<td id="celData_<?= $restricao['resid'] ?>"
				name="celData_<?= $restricao['resid'] ?>"><?= ( $restricao['data'] ); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Autor:</td>
			<td>
			<div><!-- img onclick="enviar_email( '<?= $restricao['usucpf'] ?>' );" title="enviar e-mail" src="../imagens/email.gif" align="absmiddle" style="border:0; cursor:pointer;"/ -->
			<?= $restricao['usunome'] ?></div>
			<div style="color: #959595;"><?= $restricao['unadescricao'] ?> - Tel:
			(<?= $restricao['dddresponsavel'] ?>) <?= $restricao['telefoneresponsavel'] ?></div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Restrição
			superada?</td>
			<td><label title="indica que a restrição está superada"> <input
				type="radio" name="ressolucao_<?= $restricao['resid'] ?>" value="t"
				<?= $restricao['ressolucao'] == 't' ? 'checked="checked"' : '' ?> />
			Sim </label> &nbsp;&nbsp; <label
				title="indica que a restrição não está superada"> <input
				type="radio" name="ressolucao_<?= $restricao['resid'] ?>" value="f"
				<?= $restricao['ressolucao'] == 'f' ? 'checked="checked"' : '' ?> />
			Não </label></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="vertical-align: top; width: 25%;">Providência:</td>
			<td><?php
			$resmedida = $restricao["resmedida"];
			echo campo_textarea( 'resmedida_'.$restricao['resid'], 'N', 'S', 'Providência ', 70, 2, 250, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Providência', $resmedida );
			?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align: top; width: 25%">&nbsp;</td>
			<td><?php
			if( $restricao['usucpf'] == $_SESSION['usucpf'])
			{
				?> <input type="button" name="bntAltera_<?= $restricao['resid'] ?>"
				id="bntAltera_<?= $restricao['resid'] ?>" value="Alterar"
				onclick="alteraCampoDescricao(<?= $restricao['resid'] ?>);" /> <?php
}
?> <input type="button" name="botao" value="Salvar"
				onclick="gravarRestricao(<?= $restricao['resid']?>, <?= $obTarefa->_tartarefa ?>); return void(0);" />
			<input type="button" name="botao" value="Excluir"
				onclick="excluirRestricao(<?= $restricao['resid']?>, <?= $obTarefa->tarid ?> ); return void(0);" />
			</td>
		</tr>
		<?php } ?>
	</tbody>
</table>
		<?
}

function enviarEmailTarefa($usrcpf, $email, $arAcompanhamento, $tituloTarefa, $idTarefa){
	global $db;
	$sql = "select usunome from seguranca.usuario where usucpf = '$usrcpf'";
	$usrnome = $db->pegaUm($sql);
	
	$arrEmails = array();

	//$arrEmails = array($email);
	$remetente = array('nome'=>REMETENTE_NOME, 'email'=>REMETENTE_EMAIL);
	
	$assunto  = "[SIMEC] Tarefa / Atividade cadastrada no SIMEC [".$usrnome."] nº ".$idTarefa." - Módulo de Gestão de Tarefa";

	$conteudo = "
		<font size='2'><b>Tarefa / Atividade:</b> [".$usrnome."] nº ".$idTarefa."<font>
		<br><br>
		<b><font size='2'>Segue abaixo os logs e mensagens desta tarefa.<font></b>
		<br><br>
		";
	
	$conteudo .= "<table width=\"95%\" align=\"center\" border=\"1\" cellpadding=\"2\" cellspacing=\"0\">
						<tr>
							<td width=\"27%\" valign=\"top\" bgcolor=\"\">
								<strong>Autor / Postado</strong>
							</td>
							<td valign=\"top\">
								<strong>Mensagem</strong>
							</td>
						</tr>
		";
	$arAcompanhamento = ($arAcompanhamento) ? $arAcompanhamento : array();
	foreach ($arAcompanhamento as $acompanhamento){
		//$conteudo .= $acompanhamento['acodsc']."<br /><br /><br />";
		$conteudo .= "<tr>
							<td valign=\"top\"><b>{$acompanhamento['usunome']}</b><br />{$acompanhamento['data']}</td>
							<td>{$acompanhamento['acodsc']}</td>
						  </tr>";
	}
	$conteudo .="
			</table>
		<br>	
		<a href=\"http://simec.mec.gov.br\">Clique Aqui para acessar o SIMEC.</a>
		<br>	
		<br>		
		Atenciosamente,
		<br>	
		{$remetente['nome']}";

		enviar_email($remetente, $email, $assunto, $conteudo, $arrEmails );
}
/*
 * Joga na sessão os dados das instituições cadastradas da tarefa
 */
function getInstituicaoCadastrada(){
	global $db;
	if($_SESSION['dados_tarefa']['tarid']){
		$sql = "SELECT iesid, tarid FROM tarefa.instituicaorelacionada WHERE tarid = ".$_SESSION['dados_tarefa']['tarid'];
		$rs  = $db->carregar( $sql );
		if( is_array( $rs ) ){
			foreach( $rs as $d ){
				array_push( $_SESSION['iescodSession'], $d['iesid'] );
			}
		}
		$_SESSION['iescodSession'] = array_unique( $_SESSION['iescodSession'] );
		return true;
	}
}

function marcarInstituicaoCadastrada(){
	if( is_array( $_SESSION['iescodSession'] ) ){
		foreach( $_SESSION['iescodSession'] as $i ){
			?>
				<script>
				if( document.getElementById('iesid_<?=$i;?>') ){
					document.getElementById('iesid_<?=$i;?>').checked = 1;
				}
				</script>
			<?
		}
	}
}
function marcarInstituicaoEntidade(){
	if( $_SESSION['iescodSessionEntidade'] != '' ){
		?>
			<script> 
			if( document.getElementById('iesid_<?=$_SESSION['iescodSessionEntidade'];?>') ) { 
				document.getElementById('iesid_<?=$_SESSION['iescodSessionEntidade'];?>').checked = 1;
			}
			</script>
		<?
	}
}
function verificaInstituicaoEntidade(){
	global $db;
	//$sql = "select iesid from tarefa.solicitante h"
}
