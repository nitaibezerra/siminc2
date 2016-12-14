<?php
ini_set("memory_limit", "4096M");
set_time_limit(30000);

function montaTabelaRetorno( $arrResponse = array(), $funcao = '' ){
	global $db;
	
	$html = '<table align="center" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
				<tr>
					<th width="10%">Código</th>
					<th width="30%">Descrição</th>
					<th width="60%">Mensagem Retorno</th>
				</tr>';
	foreach ($arrResponse as $key => $retorno) {
					
		$sql = "select prgcod||'.'||acacod||'.'||unicod||'.'||loccod as codigo, acadsc as descricao
					from elabrev.ppaacao_orcamento 
					where prgano = '{$_SESSION['exercicio']}' and prgcod is not null and acaid = {$retorno['acaid']}
					group by prgcod, acacod, unicod, loccod, acadsc
					order by prgcod||'.'||acacod||'.'||unicod||'.'||loccod|| ' - ' || acadsc";
		$arrAcao = $db->pegaLinha( $sql );
			
		if(!$retorno['retorno']->return->sucesso){
			if( is_array($retorno['retorno']->return->mensagensErro) ){
				$strMensagem = implode('<br>', $retorno['retorno']->return->mensagensErro);
			} else {
				$strMensagem = $retorno['retorno']->return->mensagensErro;
			}
			$mensagem = '<b>Detalhe do erro:</b> <br><br>'.$strMensagem;
			$corTD = 'red';
		} else {
			if( $funcao == 'cadastra' ){
				$mensagem = 'Proposta cadastrada com sucesso!';
				$sql = "UPDATE elabrev.ppaacao_orcamento SET acapropostaenviada = true, acadtenviosiop = now() WHERE acaid = {$retorno['acaid']}";
				$db->executar( $sql );
				$db->commit();
			} else if( $funcao == 'consulta' ){
				$mensagem = 'Consulta realizada com sucesso!';
			} else {
				$mensagem = 'Proposta excluida com sucesso!';
				$sql = "UPDATE elabrev.ppaacao_orcamento SET acapropostaenviada = false, acadtenviosiop = now() WHERE acaid = {$retorno['acaid']}";
				$db->executar( $sql );
				$db->commit();
			}
			$corTD = 'blue';
		}
		$key % 2 ? $cor = "#dedfde" : $cor = "";
		$html.= '<tr bgcolor="'.$cor.'" id="tr_'.$key.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td>'.$arrAcao['codigo'].'</td>
					<td>'.$arrAcao['descricao'].'</td>
					<td style="color: '.$corTD.';">'.$mensagem.'</td>
				</tr>';
	}
	$html.= '</table>';
	return $html;
}

if($_POST['requisicao'] == 'enviasolicitacao') {
	
	include_once APPRAIZ."elabrev/classes/WSQualitativo.class.inc";
	include_once APPRAIZ.'elabrev/classes/SiopQualitativo.class.inc';

	$arrParam = array('usuario' => $_POST['wsusuario'],
							'senha' => $_POST['wssenha'],
							'post' 	=> $_POST
						);
	
	$obResponse = new SiopQualitativo($arrParam);	
	$documento = $_POST['documento'];
	
	if( $documento == 'obterProgramacaoCompletaQualitativo' ){
		//ver($_POST,d);
		$arrResponse = $obResponse->obterProgramacaoCompleta();
		//ver($_POST,$arrResponse,d);
		if( $arrResponse->return->sucesso ){
			$acoesDTO 				= $arrResponse->return->acoesDTO;
			$agendasSamDTO 			= $arrResponse->return->agendasSamDTO;
			$indicadoresDTO 		= $arrResponse->return->indicadoresDTO;
			$iniciativasDTO 		= $arrResponse->return->iniciativasDTO;
			$localizadoresDTO 		= $arrResponse->return->localizadoresDTO;
			$medidasInstitucionaisNormativasDTO = $arrResponse->return->medidasInstitucionaisNormativasDTO;
			$metasDTO	 			= $arrResponse->return->metasDTO;
			$objetivosDTO 			= $arrResponse->return->objetivosDTO;
			$orgaosDTO 				= $arrResponse->return->orgaosDTO;
			$planosOrcamentariosDTO = $arrResponse->return->planosOrcamentariosDTO;
			$programasDTO			= $arrResponse->return->programasDTO;
			$regionalizacoesDTO 	= $arrResponse->return->regionalizacoesDTO;
			
			insereAcoesDTO($acoesDTO);
			insereAgendasSamDTO($agendasSamDTO);
			insereIndicadoresDTO($indicadoresDTO);
			insereIniciativasDTO($iniciativasDTO);
			insereLocalizadoresDTO($localizadoresDTO);
			insereMedidasInstitucionaisNormativasDTO($medidasInstitucionaisNormativasDTO);
			insereObjetivosDTO($objetivosDTO);
			insereMetasDTO($metasDTO);
			insereOrgaosDTO($orgaosDTO);
			inserePlanosOrcamentariosDTO($planosOrcamentariosDTO);
			insereProgramasDTO($programasDTO);
			insereRegionalizacoesDTO($regionalizacoesDTO);

			echo '<script>
						alert("Dados atualizado com sucesso:\n* AcoesDTO: '.(is_array( $acoesDTO ) ? sizeof($acoesDTO) : 0).'\n* AgendasSamDTO: '.(is_array( $agendasSamDTO ) ? sizeof($agendasSamDTO) : 0).'\n* IndicadoresDTO: '.(is_array( $indicadoresDTO ) ? sizeof($indicadoresDTO) : 0).'\n* IniciativasDTO: '.(is_array( $iniciativasDTO ) ? sizeof($iniciativasDTO) : 0).'\n* LocalizadoresDTO: '.(is_array( $localizadoresDTO ) ? sizeof($localizadoresDTO) : 0).'\n* MedidasInstitucionaisNormativasDTO: '.(is_array( $medidasInstitucionaisNormativasDTO ) ? sizeof($medidasInstitucionaisNormativasDTO) : 0).'\n* ObjetivosDTO: '.(is_array( $objetivosDTO ) ? sizeof($objetivosDTO) : 0).'\n* MetasDTO: '.(is_array( $metasDTO ) ? sizeof($metasDTO) : 0).'\n* OrgaosDTO: '.(is_array( $orgaosDTO ) ? sizeof($orgaosDTO) : 0).'\n* PlanosOrcamentariosDTO: '.(is_array( $planosOrcamentariosDTO ) ? sizeof($planosOrcamentariosDTO) : 0).'\n* ProgramasDTO: '.(is_array( $programasDTO ) ? sizeof($programasDTO) : 0).'\n* RegionalizacoesDTO: '.(is_array( $regionalizacoesDTO ) ? sizeof($regionalizacoesDTO) : 0).'");
						window.location.href = "elabrev.php?modulo=sistema/comunica/importaQualitativo&acao=A";
				</script>';
			exit();
			
		} else {
			$msg = $arrResponse->return->mensagensErro;
			echo "<script>alert('".$msg."!');
						window.location.href = 'elabrev.php?modulo=sistema/comunica/importaQualitativo&acao=A';
				</script>";
			exit();
		}
	} elseif( $documento == 'obterTabelasApoio'){
		$arrResponse = $obResponse->obterTabelasApoio();
		//ver($arrResponse,d);
		if( $arrResponse->return->sucesso ){
			$produtoDTO 				= $arrResponse->return->produtosDTO;
			$unidadeMedidaDTO 			= $arrResponse->return->unidadesMedidaDTO;
			
			insereProdutosDTO($produtoDTO);
			insereUnidademedidaDTO($unidadeMedidaDTO);
			
			echo '<script>
						alert("Dados atualizado com sucesso:\n* ProdutosDTO: '.(is_array( $produtoDTO ) ? sizeof($produtoDTO) : 0).'\n* MedidaDTO: '.(is_array( $unidadeMedidaDTO ) ? sizeof($unidadeMedidaDTO) : 0).'");
						window.location.href = "elabrev.php?modulo=sistema/comunica/importaQualitativo&acao=A";
				</script>';
			exit();
			
		} else {
			$msg = $arrResponse->return->mensagensErro;
			echo "<script>alert('".$msg."!');
						window.location.href = 'elabrev.php?modulo=sistema/comunica/importaQualitativo&acao=A';
				</script>";
			exit();
		}
	}
	
	echo "<script>alert('Dados atualizado com sucesso!');
			window.location.href = 'elabrev.php?modulo=sistema/comunica/importaQualitativo&acao=A';
	</script>";
	exit();
}

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Qualitativos');

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "280px";
$altura 	= "150px";
$id 		= "div_auth";
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<style>	
	.popup_alerta
	{
		width:<?php echo $largura ?>;
		height:<?php echo $altura ?>;
		position:absolute;
		z-index:0;
		top:50%;
		left:50%;
		margin-top:-<?php echo $altura/2 ?>;
		margin-left:-<?php echo $largura/2 ?>;
		border:solid 2px black;
		background-color:#FFFFFF;
		display:none;
		overflow: auto;
	}
</style>
<style>
	label { cursor: pointer; }
</style>
<form method="post" name="formulario" id="formulario">

<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td class="subtitulodireita" width="30%"><b>Funções:</b></td>
		<td style="padding: 0 20px 20px 20px;" width="70%">
		<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
			<tr><td>
				<label>
				<input type="radio" name="documento" id="documento_obterProgramacaoCompletaQualitativo" value="obterProgramacaoCompletaQualitativo" onclick="mostraCombo('obterProgramacaoCompletaQualitativo')"/>
				Obter Programacao Completa Qualitativo:
					<div id="obterProgramacaoCompletaQualitativo" style="display: none;">
						<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
							<tr>
								<td class="subtitulodireita" width="30%">Retornar Orgaos:</td>
								<td width="70%"><input type="radio" name="retornarOrgaos" value="1">Sim <input type="radio" name="retornarOrgaos" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Programas:</td>
								<td><input type="radio" name="retornarProgramas" value="1">Sim <input type="radio" name="retornarProgramas" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Indicadores:</td>
								<td><input type="radio" name="retornarIndicadores" value="1">Sim <input type="radio" name="retornarIndicadores" value="0" checked="checked">Não</td>
							</tr>							
							<tr>
								<td class="subtitulodireita">Retornar Objetivos:</td>
								<td><input type="radio" name="retornarObjetivos" value="1">Sim <input type="radio" name="retornarObjetivos" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Iniciativas:</td>
								<td><input type="radio" name="retornarIniciativas" value="1">Sim <input type="radio" name="retornarIniciativas" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Ações:</td>
								<td><input type="radio" name="retornarAcoes" value="1">Sim <input type="radio" name="retornarAcoes" value="0" checked="checked">Não</td>
							</tr>							
							<tr>
								<td class="subtitulodireita">Retornar Localizadores:</td>
								<td><input type="radio" name="retornarLocalizadores" value="1">Sim <input type="radio" name="retornarLocalizadores" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Metas:</td>
								<td><input type="radio" name="retornarMetas" value="1">Sim <input type="radio" name="retornarMetas" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Regionalizações:</td>
								<td><input type="radio" name="retornarRegionalizacoes" value="1">Sim <input type="radio" name="retornarRegionalizacoes" value="0" checked="checked">Não</td>
							</tr>							
							<tr>
								<td class="subtitulodireita">Retornar Planos Orcamentarios:</td>
								<td><input type="radio" name="retornarPlanosOrcamentarios" value="1">Sim <input type="radio" name="retornarPlanosOrcamentarios" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Agenda Sam:</td>
								<td><input type="radio" name="retornarAgendaSam" value="1">Sim <input type="radio" name="retornarAgendaSam" value="0" checked="checked">Não</td>
							</tr>
							<tr>
								<td class="subtitulodireita">Retornar Medidas Institucionais Normativas:</td>
								<td><input type="radio" name="retornarMedidasInstitucionaisNormativas" value="1">Sim <input type="radio" name="retornarMedidasInstitucionaisNormativas" value="0" checked="checked">Não</td>
							</tr>
						</table>
					</div>
				</label>
			</td></tr>
			<tr><td>
				<label>
				<input type="radio" name="documento" id="documento_obterTabelasApoio" value="obterTabelasApoio" onclick="mostraCombo('obterTabelasApoio')"/>
				Obter Tabelas Apoio:
					<div id="obterTabelasApoio" style="display: none;">
					<table align="center" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
						<!-- <tr>
							<td class="subtitulodireita" width="30%">Retornar Momentos:</td>
							<td width="70%"><input type="radio" name="retornarMomentos" value="1">Sim <input type="radio" name="retornarMomentos" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Esferas:</td>
							<td width="70%"><input type="radio" name="retornarEsferas" value="1">Sim <input type="radio" name="retornarEsferas" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Tipos Inclusao:</td>
							<td width="70%"><input type="radio" name="retornarTiposInclusao" value="1">Sim <input type="radio" name="retornarTiposInclusao" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retonar Funcoes:</td>
							<td width="70%"><input type="radio" name="retonarFuncoes" value="1">Sim <input type="radio" name="retonarFuncoes" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar SubFuncoes:</td>
							<td width="70%"><input type="radio" name="retornarSubFuncoes" value="1">Sim <input type="radio" name="retornarSubFuncoes" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Tipos Acao:</td>
							<td width="70%"><input type="radio" name="retornarTiposAcao" value="1">Sim <input type="radio" name="retornarTiposAcao" value="0" checked="checked">Não</td>
						</tr>-->
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Produtos:</td>
							<td width="70%"><input type="radio" name="retornarProdutos" value="1">Sim <input type="radio" name="retornarProdutos" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Unidades Medida:</td>
							<td width="70%"><input type="radio" name="retornarUnidadesMedida" value="1">Sim <input type="radio" name="retornarUnidadesMedida" value="0" checked="checked">Não</td>
						</tr>
						<!-- <tr>
							<td class="subtitulodireita" width="30%">Retornar Regioes:</td>
							<td width="70%"><input type="radio" name="retornarRegioes" value="1">Sim <input type="radio" name="retornarRegioes" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Perfis:</td>
							<td width="70%"><input type="radio" name="retornarPerfis" value="1">Sim <input type="radio" name="retornarPerfis" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Tipos Programa:</td>
							<td width="70%"><input type="radio" name="retornarTiposPrograma" value="1">Sim <input type="radio" name="retornarTiposPrograma" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Macro Desafios:</td>
							<td width="70%"><input type="radio" name="retornarMacroDesafios" value="1">Sim <input type="radio" name="retornarMacroDesafios" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Unidades Medida Indicador:</td>
							<td width="70%"><input type="radio" name="retornarUnidadesMedidaIndicador" value="1">Sim <input type="radio" name="retornarUnidadesMedidaIndicador" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Periodicidades:</td>
							<td width="70%"><input type="radio" name="retornarPeriodicidades" value="1">Sim <input type="radio" name="retornarPeriodicidades" value="0" checked="checked">Não</td>
						</tr>
						<tr>
							<td class="subtitulodireita" width="30%">Retornar Bases Geograficas:</td>
							<td width="70%"><input type="radio" name="retornarBasesGeograficas" value="1">Sim <input type="radio" name="retornarBasesGeograficas" value="0" checked="checked">Não</td>
						</tr> -->
					</table>
					</div>
				</label>
			</td></tr>
		</table>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Código do Momento:</b></td>
		<td><? echo campo_texto("codigomomento","S","S","codigomomento","22","","[#]","","","","","id='codigomomento'", '', '2000'); ?></td>
	</tr>
	<tr>
		<td colspan="2" align="center" style="background-color:#c0c0c0;"><input type="button" class="botao" value="Enviar Solicitação" onclick="solicitarExecucao();"></td>
	</tr>
</table>
<br>
<div id="erro"><?=$html; ?></div>

<input type="hidden" value="" name="requisicao" id="requisicao">
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php
				$wsusuario = $usuario; 
				echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'", '', WEB_SERVICE_SIOP_USUARIO) ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?php echo WEB_SERVICE_SIOP_SENHA; ?>" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" onclick="enviaSolicitacao()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
</form>
<script type="text/javascript">

/*$(document).ready(function(){
	document.getElementById('<?php echo $id ?>').style.display='none';
});*/

var form = document.getElementById('formulario');

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function mostraCombo(tipo){
	
	for( i=0; i<form.length; i++ ){
		if( form.elements[i].type == 'select-multiple' ){
			if( document.getElementById('documento_'+tipo).checked == true && tipo != 'obterProgramacaoCompletaQualitativo' && tipo != 'obterTabelasApoio' ){
				document.getElementById('tr_acaid_'+tipo).style.display = '';
				document.getElementById( 'acaid_'+tipo+'_campo_on' ).style.display = '';
				document.getElementById( 'acaid_'+tipo+'_campo_off' ).style.display = 'none';
				document.getElementById( 'acaid_'+tipo+'_campo_flag' ).value = '1';
			}
			
			var arObjeto = form.elements[i].id.split('_');
			if( tipo != arObjeto[1] ){
				//alert(arObjeto[1]);
				//document.getElementById('debug1').innerHTML = arObjeto[1]; 
				document.getElementById('tr_acaid_'+arObjeto[1]).style.display = 'none';
				document.getElementById( 'acaid_'+arObjeto[1]+'_campo_on' ).style.display = 'none';
				document.getElementById( 'acaid_'+arObjeto[1]+'_campo_off' ).style.display = 'none';
				document.getElementById( 'acaid_'+arObjeto[1]+'_campo_flag' ).value = '0';	
			}
		}
	}
	$('#obterTabelasApoio').hide();
	$('#obterProgramacaoCompletaQualitativo').hide();
	if( tipo == 'obterProgramacaoCompletaQualitativo'){
		$('#obterProgramacaoCompletaQualitativo').show(); 
	}
	if( tipo == 'obterTabelasApoio' ){
		$('#obterTabelasApoio').show();
	}
}

function solicitarExecucao(){
	var docObterProg 		= document.getElementById('documento_obterProgramacaoCompletaQualitativo').checked;
	var docObterTabApoio	= document.getElementById('documento_obterTabelasApoio').checked;

	
	if( docObterProg == false && docObterTabApoio == false ){
		alert('Selecione uma função');
		return false;
	} else {
		document.getElementById('div_auth').style.display = 'block';
	}
}
function enviaSolicitacao(){
				
	var form = document.getElementById('formulario');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	var codigomomento = document.getElementById('codigomomento').value;
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	
	if(!codigomomento) {
		alert('Favor informar o Código do Momento!');
		return false;
	}
	
	$('#requisicao').val('enviasolicitacao');
	$('#formulario').submit();	
}

function limpaCombo(){
	for( a=0; a<form.length; a++ ){
		if( form.elements[a].type == 'select-multiple' ){
			var arObjeto = form.elements[a].id.split('_');
			
			var codigo = document.getElementById( 'acaid_'+arObjeto[1] ).value;
			var campo_select = document.getElementById( 'acaid_'+arObjeto[1] );
			
			var removeu = false;
			for( var i = 0; i <= campo_select.length-1; i++ ){
				if ( codigo == campo_select.options[i].value ){
					campo_select.options[i] = null;
					removeu = true;
				}
			}
			if ( campo_select.options.length == 0 ){
				campo_select.options[0] = new Option( 'Duplo clique para selecionar da lista', '', false, false );
			}
		}
	}
}

</script>
<?
function insereAcoesDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_acaodto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arAcoes) {
			$sql = "INSERT INTO elabrev.ws_acaodto(baselegal, codigoacao, codigoesfera, codigofuncao, codigoiniciativa, codigomomento, codigoobjetivo, codigoorgao,
  						codigoproduto, codigoprograma, codigosubfuncao, codigotipoacao, codigotipoinclusaoacao, codigounidademedida, descricao, detalhamentoimplementacao,
  						especificacaoproduto, exercicio, finalidade, formaacompanhamento, identificacaosazonalidade, identificadorunico, insumosutilizados, sndescentralizada,
  						sndireta, snexclusaologica, snlinhacredito, sntransferenciaobrigatoria, sntransferenciavoluntaria, titulo, unidaderesponsavel) 
					VALUES ('{$arAcoes->baseLegal}', '{$arAcoes->codigoAcao}', '{$arAcoes->codigoEsfera}', '{$arAcoes->codigoFuncao}', '{$arAcoes->codigoIniciativa}', 
							'{$arAcoes->codigoMomento}', '{$arAcoes->codigoObjetivo}', '{$arAcoes->codigoOrgao}', '{$arAcoes->codigoProduto}', '{$arAcoes->codigoPrograma}', 
							'{$arAcoes->codigoSubFuncao}', '{$arAcoes->codigoTipoAcao}', '{$arAcoes->codigoTipoInclusaoAcao}', '".trim($arAcoes->codigoUnidadeMedida)."', 
							'".str_ireplace("'", "",$arAcoes->descricao)."', '".str_ireplace("'", "",$arAcoes->detalhamentoImplementacao)."',
 							'".str_ireplace("'", "",$arAcoes->especificacaoProduto)."', '{$arAcoes->exercicio}', '{$arAcoes->finalidade}', '{$arAcoes->formaAcompanhamento}', 
							'{$arAcoes->identificacaoSazonalidade}', '{$arAcoes->identificadorUnico}', '{$arAcoes->insumosUtilizados}', '{$arAcoes->snDescentralizada}',
 							'{$arAcoes->snDireta}', '{$arAcoes->snExclusaoLogica}', '{$arAcoes->snLinhaCredito}', '{$arAcoes->snTransferenciaObrigatoria}', 
 							'{$arAcoes->snTransferenciaVoluntaria}', '".str_ireplace("'", "",$arAcoes->titulo)."', '".str_ireplace("'", "",$arAcoes->unidadeResponsavel)."')";
 			//ver($sql,d);
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereAgendasSamDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_agendassamdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arAgenda) {
			$sql = "INSERT INTO elabrev.ws_agendassamdto(agendasam, codigoagendasam, descricao, exercicio) 
					VALUES ('{$arAgenda->agendaSam}', '{$arAgenda->codigoAgendaSam}', '{$arAgenda->descricao}', '{$_SESSION['exercicio']}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereIndicadoresDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_indicadoresdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_indicadoresdto(codigobasegeografica, codigoindicador, codigomomento, codigoperiodicidade, codigoprograma, codigounidademedidaindicador,
  						dataapuracao, descricao, exercicio, fonte, formula, identificadorunico, snapuracaoreferencia, snexclusaologica, valorreferencia) 
					VALUES ('{$arDado->codigoBaseGeografica}', '{$arDado->codigoIndicador}', '{$arDado->codigoMomento}', '{$arDado->codigoPeriodicidade}', '{$arDado->codigoPrograma}', '{$arDado->codigoUnidadeMedidaIndicador}',
 						'{$arDado->dataApuracao}', '{$arDado->descricao}', '{$arDado->exercicio}', '{$arDado->fonte}', '{$arDado->formula}', '{$arDado->identificadorUnico}', '{$arDado->snApuracaoReferencia}', '{$arDado->snExclusaoLogica}', '{$arDado->valorReferencia}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereIniciativasDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_iniciativasdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_iniciativasdto(codigoiniciativa, codigomomento, codigoobjetivo, codigoorgao, codigoprograma, exercicio, 
						identificadorunico, snexclusaologica, snindividualizada, titulo) 
					VALUES ('{$arDado->codigoIniciativa}', '{$arDado->codigoMomento}', '{$arDado->codigoObjetivo}', '{$arDado->codigoOrgao}', '{$arDado->codigoPrograma}', 
						'{$arDado->exercicio}', '{$arDado->identificadorUnico}', '{$arDado->snExclusaoLogica}', '{$arDado->snIndividualizada}', '{$arDado->titulo}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereLocalizadoresDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_localizadoresdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_localizadoresdto(anoinicio, anotermino, codigolocalizador, codigomomento, codigotipoinclusao, descricao, exercicio,
  						identificadorunicoacao, justificativarepercussao, municipio, regiao, siglaregiao, snexclusaologica, totalfinanceiro, totalfisico, uf) 
					VALUES ('{$arDado->anoInicio}', '{$arDado->anoTermino}', '{$arDado->codigoLocalizador}', '{$arDado->codigoMomento}', '{$arDado->codigoTipoInclusao}', 
						'". str_ireplace("'", "", $arDado->descricao ) ."', '{$arDado->exercicio}', '{$arDado->identificadorUnicoAcao}', '{$arDado->justificativaRepercussao}', '". str_ireplace("'", "", $arDado->municipio ) ."', 
						'{$arDado->regiao}', '{$arDado->siglaRegiao}', '{$arDado->snExclusaoLogica}', '{$arDado->totalFinanceiro}', '{$arDado->totalFisico}', '{$arDado->uf}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereMedidasInstitucionaisNormativasDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_medidasinstitucionaisnormativasdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_medidasinstitucionaisnormativasdto(codigomomento, codigoorgaosiorg, descricao, exercicio, identificadoriniciativa,
  						identificadorunico, produto, snexclusaologica)
					VALUES ('{$arDado->codigoMomento}', '{$arDado->codigoOrgaoSiorg}', '{$arDado->descricao}', '{$arDado->exercicio}', '{$arDado->identificadorIniciativa}', 
						'{$arDado->identificadorUnico}', '{$arDado->produto}', '{$arDado->snExclusaoLogica}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereMetasDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_metasdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_metasdto(codigometa, codigomomento, codigoobjetivo, codigoprograma, descricao, exercicio, identificadorunico)
					VALUES ('{$arDado->codigoMeta}', '{$arDado->codigoMomento}', '{$arDado->codigoObjetivo}', '{$arDado->codigoPrograma}', 
						'{$arDado->descricao}', '{$arDado->exercicio}', '{$arDado->identificadorUnico}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereObjetivosDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_objetivosdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_objetivosdto(codigomomento, codigoobjetivo, codigoorgao, codigoprograma, enunciado, exercicio, identificadorunico, snexclusaologica)
					VALUES ('{$arDado->codigoMomento}', '{$arDado->codigoObjetivo}', '{$arDado->codigoOrgao}', '{$arDado->codigoPrograma}', '{$arDado->enunciado}', 
					'{$arDado->exercicio}', '{$arDado->identificadorUnico}', '{$arDado->snExclusaoLogica}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereOrgaosDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_orgaodto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_orgaodto(codigoorgao, codigoorgaopai, descricao, descricaoabreviada, exercicio, orgaoid, orgaosiorg, tipoorgao, snativo)
					VALUES ('{$arDado->codigoOrgao}', '{$arDado->codigoOrgaoPai}', '{$arDado->descricao}', '{$arDado->descricaoAbreviada}', '{$arDado->exercicio}', 
						'{$arDado->orgaoId}', '{$arDado->orgaoSiorg}', '{$arDado->tipoOrgao}', '{$arDado->snAtivo}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function inserePlanosOrcamentariosDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_planosorcamentariosdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_planosorcamentariosdto(codigoindicadorplanoorcamentario, codigomomento, codigoproduto, codigounidademedida, datahoraalteracao,
  						detalhamento, exercicio, identificadorunico, identificadorunicoacao, planoorcamentario, titulo)
					VALUES ('{$arDado->codigoIndicadorPlanoOrcamentario}', '{$arDado->codigoMomento}', '{$arDado->codigoProduto}', '{$arDado->codigoUnidadeMedida}', 
						'{$arDado->dataHoraAlteracao}', '{$arDado->detalhamento}', '{$arDado->exercicio}', '{$arDado->identificadorUnico}', '{$arDado->identificadorUnicoAcao}', 
						'{$arDado->planoOrcamentario}', '{$arDado->titulo}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereProgramasDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_programasdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_programasdto(codigomacrodesafio, codigomomento, codigoorgao, codigoprograma, codigotipoprograma, estrategiaimplementacao, exercicio,
  						horizontetemporalcontinuo, identificadorunico, justificativa, objetivo, objetivogoverno, objetivosetorial, problema, publicoalvo, snexclusaologica, titulo, unidaderesponsavel)
					VALUES ('{$arDado->codigoMacroDesafio}', '{$arDado->codigoMomento}', '{$arDado->codigoOrgao}', '{$arDado->codigoPrograma}', '{$arDado->codigoTipoPrograma}', 
						'{$arDado->estrategiaImplementacao}', '{$arDado->exercicio}', '{$arDado->horizonteTemporalContinuo}', '{$arDado->identificadorUnico}', 
						'{$arDado->justificativa}', '{$arDado->objetivo}', '{$arDado->objetivoGoverno}', '{$arDado->objetivoSetorial}', '{$arDado->problema}', 
						'{$arDado->publicoAlvo}', '{$arDado->snExclusaoLogica}', '{$arDado->titulo}', '{$arDado->unidadeResponsavel}')";
 			$db->executar($sql);
		}
		return $db->commit();		
	}
}

function insereRegionalizacoesDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_regionalizacoesdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		
		foreach ($arrDados as $arDado) {
			$sql = "INSERT INTO elabrev.ws_regionalizacoesdto(codigometa, codigomomento, codigoobjetivo, codigoprograma, codigounidademedida, descricao,
  						exercicio, identificadorunicometa, regionalizacaoid, sigla, valor)
					VALUES ('{$arDado->codigoMeta}', '{$arDado->codigoMomento}', '{$arDado->codigoObjetivo}', '{$arDado->codigoPrograma}', '{$arDado->codigoUnidadeMedida}', 
						'{$arDado->descricao}', '{$arDado->exercicio}', '{$arDado->identificadorUnicoMeta}', '{$arDado->regionalizacaoId}', '{$arDado->sigla}', '{$arDado->valor}')";
 			$db->executar($sql);
		}
		return $db->commit();
	}
}

function insereProdutosDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_produtosdto WHERE exercicio = '{$_SESSION['exercicio']}'");
		$db->commit();
		foreach ($arrDados as $arDado) {
			$descricao = str_ireplace("'", "", $arDado->descricao);
			$sql = "INSERT INTO elabrev.ws_produtosdto(codigoproduto, datahoraalteracao, descricao, snativo, snexclusaologica, exercicio) 
					VALUES ({$arDado->codigoProduto}, '{$arDado->dataHoraAlteracao}', '{$descricao}', '{$arDado->snAtivo}', '{$arDado->snExclusaoLogica}', {$_SESSION['exercicio']})";
			$db->executar($sql);
		}
	}
	return $db->commit();
}

function insereUnidademedidaDTO( $arrDados ){
	global $db;
	
	if( is_array($arrDados) ){
		$db->executar("DELETE FROM elabrev.ws_unidademedidadto WHERE exercicio = '{$_SESSION['exercicio']}'");
		$db->commit();
		foreach ($arrDados as $arDado) {
			$descricao = str_ireplace("'", "", $arDado->descricao);
			$sql = "INSERT INTO elabrev.ws_unidademedidadto(codigounidademedida, datahoraalteracao, descricao, snativo, exercicio) 
					VALUES ({$arDado->codigoUnidadeMedida}, '{$arDado->dataHoraAlteracao}', '{$descricao}', '{$arDado->snAtivo}', {$_SESSION['exercicio']})";
			$db->executar($sql);
		}
	}
	return $db->commit();
}

?>