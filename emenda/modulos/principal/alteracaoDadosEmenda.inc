<?php
if ( $_REQUEST["emdid"] || $_SESSION["emenda"]["emdid"] ){
	
	if ($_POST['requisicao'] == 'excluir'){
		$sql = "DELETE FROM emenda.emendadetalhe WHERE emdid = {$_REQUEST["emdid"]}";
		$db->executar( $sql );
		$db->commit();
		$db->sucesso('principal/alteracaoDadosEmenda');
		exit();
	}
	
	if ($_POST['requisicao'] == 'salvar'){
		//ver( $_POST,d );
		$valorTotal = $_POST['emevalor'];
		$valorInformado = $_POST['valorjainformado'];
		$emdvalor = retiraPontos($_POST['emdvalor']);
		
		$valorInformado = $valorInformado + $emdvalor;		
		
		if( $valorTotal < $valorInformado && $_POST['tipoemenda'] <> 'R' ){
			$msg = "Não é possivél inserir uma nova modalidade, pois o valor já informado( R$ ".number_format($valorInformado, 2, ',', '.')." ) ultrapassa o valor total da emenda: R$ ".number_format($_POST['emevalor'], 2, ',', '.');
			if ($_POST['tipoemenda'] == 'E') $msg = "Não é possivél altera uma nova modalidade, pois o valor já informado( R$ ".number_format($valorInformado, 2, ',', '.')." ) ultrapassa o valor total da emenda: R$ ".number_format($_POST['emevalor'], 2, ',', '.');
			echo "<script>
						alert('$msg');
				  </script>";
		} else {
			if ($_POST['tipoemenda'] == 'E'){
				$sql = "UPDATE emenda.emendadetalhe SET 
					gndcod = ".$_POST[gndcod].",
					mapcod = '". $_POST[mapcod]."',
					foncod = ".($_POST[foncod] ? $_POST[foncod] : 'NULL').",
					emdvalor = '". retiraPontos($_POST[emdvalor]) ."',
					usucpfalteracao = '". $_SESSION[usucpf] ."',
					emddataalteracao = now(),
					emdtipo = '".$_POST['emdtipo']."' WHERE emdid = ". $_POST[emdid];
	
				$db->executar($sql);			
			} else {
				$sql = "INSERT INTO emenda.emendadetalhe (emeid,  
							".($_POST[gndcod] ? 'gndcod,' : '')."
							mapcod,
							".($_POST[foncod] ? 'foncod,' : '')."
							emdvalor,
							usucpfalteracao,
							emddataalteracao,
							emdtipo)
						VALUES ({$_SESSION[emenda][emeid]},
							".($_POST[gndcod] ? "'$_POST[gndcod]'," : '')."
							'$_POST[mapcod]',
							".($_POST[foncod] ? "'$_POST[foncod]'," : '')."
							'".retiraPontos($_POST[emdvalor])."',
							'{$_SESSION[usucpf]}',
							NOW(),
							'".$_POST['tipoemenda']."') RETURNING emdid";
							
				$db->executar($sql);
				//$emdid = $db->pegaUm($sql);
				//$_SESSION["emenda"]["emdid"] = $_REQUEST["emdid"] = $emdid;
			}
			if($db->commit()){
				$db->sucesso( 'principal/alteracaoDadosEmenda' );
			} else {
				$db->insucesso( 'Falha na operação' );
			}
		}
	}	
	
	if( $_POST['tipoemenda'] == 'E' ){
		$emdid = $_POST["emdid"];
		verificaEmenda( $emdid, 'detalheEmenda' );
		pegaEmendaPorDetalhe( $emdid );
		$dadosDetalheEmenda = pegaDadosDetalheEmenda( $emdid, true );
	} else {
		$_SESSION["emenda"]["emdid"] = $_REQUEST["emdid"] ? $_REQUEST["emdid"] : $_SESSION["emenda"]["emdid"];
		verificaEmenda( $_SESSION["emenda"]["emdid"], 'detalheEmenda' );
		pegaEmendaPorDetalhe( $_SESSION["emenda"]["emdid"] );
		$dadosDetalheEmenda = pegaDadosDetalheEmenda( $_SESSION["emenda"]["emdid"], false );
	}	
}else{
		
	print "<script>"
		. "		alert('Favor selecionar um detalhe da emenda!');"
		. "		history.back(-1);"
		. "</script>";
		
}
validaSessionPTA( $_SESSION["emenda"]["emdid"], false );
require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( 'Dados do Detalhe da Emenda', '');

$perfilEmenda = pegaPerfilArray( $_SESSION["usucpf"] );

$sql = "SELECT
			COALESCE(sum(ed.emdvalor), 0) as valortotal
		FROM
			emenda.emenda e
		    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
		WHERE
			ed.emeid = ".$_SESSION["emenda"]["emeid"]."
		    and ed.emdtipo <> 'R'
		    and e.emeano = '".$_SESSION['exercicio']."'";	
$valorJaInformado = $db->pegaUm( $sql );

if( $_POST['requisicao'] == 'alterar' || $_POST['requisicao'] == 'salvar' ) $valorJaInformado = $valorJaInformado - $dadosDetalheEmenda['valor'];
?>
<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
</head>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form action="emenda.php?modulo=principal/alteracaoDadosEmenda&acao=A" method="post" id="formEntidadeDetalhe" name="formEntidadeDetalhe">
	<input type="hidden" value="<?=$_POST['requisicao']; ?>" name="requisicao" id="requisicao"/>
	<input type="hidden" value="<?=$_POST['tipoemenda']; ?>" name="tipoemenda" id="tipoemenda"/>
	<input type="hidden" value="<?=$_POST["emdid"]; ?>" name="emdid" id="emdid"/>
	<input type="hidden" value="<?=$_POST["emdtipo"]; ?>" name="emdtipo" id="emdtipo"/>
	<input type="hidden" value="<?=$dadosDetalheEmenda['emevalor']; ?>" name="emevalor" id="emevalor"/>
	<input type="hidden" value="<?=$valorJaInformado; ?>" name="valorjainformado" id="valorjainformado"/>
	<input type="hidden" value="<?=$_POST['tipobotao']; ?>" name="tipobotao" id="tipobotao"/>
	<input type="hidden" value="<?=$_POST['botao']; ?>" name="botao" id="botao"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulodireita" width="200px">Número:</td>
			<td>
				<?php echo $dadosDetalheEmenda["numero"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Autor:</td>
			<td>
				<?php echo $dadosDetalheEmenda["autor"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Unidade Orçamentária:</td>
			<td>
				<?php echo $dadosDetalheEmenda["unidade"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Funcional Programática:</td>
			<td>
				<?php echo $dadosDetalheEmenda["funcional"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Subtítulo:</td>
			<td>
				<?php echo $dadosDetalheEmenda["subtitulo"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Responsável:</td>
			<td>
				<?php echo $dadosDetalheEmenda["responsavel"];?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor da Emenda:</td>
			<td>
				<?php echo 'R$ '.number_format($dadosDetalheEmenda["emevalor"], 2, ',', '.');?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">GND:</td>
			<td>	
				<?php
				
					$sql = "SELECT
								gndcod as codigo,
								gndcod||' - '||gnddsc as descricao
							FROM
								public.gnd
							WHERE
								gndstatus = 'A'
							ORDER BY
								gndcod";
					
				if ( !empty($_POST['emdtipo']) || $_POST['requisicao'] ){
					$gndcod = explode(" - ", $dadosDetalheEmenda["gnd1"]);
					$gndcod = $gndcod[0];
					if( $_POST['requisicao'] != 'alterar' ) $gndcod = $_POST['gndcod'];
				}
								 
				$db->monta_combo("gndcod", $sql, 'S', "Selecione...", '', '', '', '', 'S','gndcod');
				
				?>				
				
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Modalidade de Aplicação:</td>
			<td>			
				<?php

					$sql = "SELECT
								mapcod as codigo,
								mapcod||' - '||mapdsc as descricao
							FROM
								public.modalidadeaplicacao
							ORDER BY
								mapcod";				
				
				if ( !empty($_POST['emdtipo']) || $_POST['requisicao'] ){				
					$mapcod = explode(" - ", $dadosDetalheEmenda["mod"]);
					$mapcod = $mapcod[0] ? $mapcod[0] : $_POST['mapcod'];
					if( $_POST['requisicao'] != 'alterar' ) $mapcod = $_POST['mapcod'];
				}
				
				$db->monta_combo("mapcod", $sql, 'S', "Selecione...", '', '', '', '', 'S','mapcod');
				
				?>					
				
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Fonte:</td>
			<td>			
				<?php
				
					$sql = "SELECT
								foncod as codigo,
								foncod||' - '||fondsc as descricao
							FROM
								public.fonterecurso
							ORDER BY
								foncod";				

				if ( !empty($_POST['emdtipo']) || $_POST['requisicao'] ){
					$foncod = explode(" - ", $dadosDetalheEmenda["fonte"]);
					$foncod = $foncod[0] ? $foncod[0] : $_POST['foncod'];
					if( $_POST['requisicao'] != 'alterar' ) $foncod = $_POST['foncod'];
				}
								
				$db->monta_combo("foncod", $sql, 'S', "Selecione...", '', '', '', '', 'S','foncod');
				
				?>					
				
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor (R$):</td>
			<td>
				<?php
				//if ($_GET[novo]) unset($dadosDetalheEmenda["valor"]);
				if ( !empty($_POST['emdtipo']) || $_POST['requisicao'] ){
					$emdvalor = $dadosDetalheEmenda["valor"] ? number_format($dadosDetalheEmenda["valor"] , 2, ',', '.') : $_POST['emdvalor'];
					if( $_POST['requisicao'] != 'alterar' ) $emdvalor = $_POST['emdvalor'];
				}
				echo campo_texto( 'emdvalor', 'S', '', '', 16, 15, '###.###.###,##', '', 'left', '', 0, 'id="emdvalor"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value)");
				
				?>
			</td>
		</tr>
		<?php
			if( in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda ) || in_array( ASSESSORIA_PARLAMENTAR, $perfilEmenda ) || in_array( ADMINISTRADOR_INST, $perfilEmenda ) ){
				$disabled = 'onclick="salvaDadosDetalheEmenda();"  style="cursor: pointer;" ';
			} else {
				$disabled = 'disabled="disabled"';
			}
		?>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td> 
				<input type="button" value="<?=($_POST['tipobotao'] ? $_POST['tipobotao'] : 'Salvar'); ?>" name="btnSalvar" id="btnSalvar" <? echo $disabled;?> />
				<input type="button" value="Adicionar Recurso Extra" onclick="inserirRecurso();" name="btnExtrar" id="btnExtra"/>
				<input type="button" value="Adicionar Modalidade" onclick="inserirModalidade();" name="btnModalidade" id="btnModalidade"/>
				<input type="button" value="Cancelar" onclick="cancelarRequisicao();" name="btnCancelar" id="btnCancelar" />
				<input type="button" value="Voltar" onclick="window.location='emenda.php?modulo=principal/cadastroEmendas&acao=A';" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<div id='divTeste'></div>
<?
$emeid = $_SESSION["emenda"]["emeid"];
$cabecalho = array( "Ação", "Recurso", "GND", "Mod", "Fonte", "Valor (R$)", "Tipo" );

$sql = "SELECT 
				case when ed.emdtipo = 'E' then  
					'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar Dados da Emenda\" style=\"cursor:pointer;\" onclick=\"alterarDetalheEmenda(' || ed.emdid || ',\''||ed.emdtipo||'\');\">'||'&nbsp;'||				
					'<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir Dados da Emenda\" style=\"cursor:pointer;\"></center>'
				else 
					'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar Dados da Emenda\" style=\"cursor:pointer;\" onclick=\"alterarDetalheEmenda(' || ed.emdid || ',\''||ed.emdtipo||'\');\">'||'&nbsp;'||				
					'<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir Dados da Emenda\" style=\"cursor:pointer;\" onclick=\"excluirDetalheEmenda(' || ed.emdid || ',\''||ed.emdtipo||'\');\"></center>'				
				end as editar,
			ed.emdid,
			ed.gndcod||' - '||gn.gnddsc as gnd, 
		    ed.mapcod||' - '||map.mapdsc as mod,
		    ed.foncod||' - '||fon.fondsc as fonte,
			ed.emdvalor as valor,
			case when ed.emdtipo = 'R' then 'Recurso Extra'
				 when ed.emdtipo = 'M' then 'Modalidade'
			else 'Emenda' end as tipo
		FROM
			emenda.emendadetalhe ed
			left join public.gnd gn
		        on gn.gndcod = ed.gndcod
                and gn.gndstatus = 'A'
		    left join public.modalidadeaplicacao map
		        on map.mapcod = ed.mapcod
		    left join public.fonterecurso fon
		        on fon.foncod = ed.foncod 
                and fon.fonstatus = 'A'
		WHERE
			ed.emeid = {$emeid}";

$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', $tamanho);

?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	$(document).ready(function(){
		bloqueiaBotao( '<?=$_POST['botao']; ?>' );
		habilitaCampos();
	});

	/*if( document.getElementById( 'tipoemenda' ).value == '' ){
		document.getElementById( 'btnSalvar' ).disabled = true;
	} else {
		document.getElementById( 'btnSalvar' ).disabled = false;
	}*/
	function bloqueiaBotao( tipo ){
		if( tipo == 'extra' || tipo == 'modalidade' ){
			document.getElementById( 'btnExtra' ).disabled = true;
			document.getElementById( 'btnModalidade' ).disabled = true;
		}
		if( tipo == 'salvar' ){
		 	document.getElementById( 'btnModalidade' ).disabled = true;
		 	document.getElementById( 'btnExtra' ).disabled = true;
		}
	}
	
	
	function salvaDadosDetalheEmenda(){
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		if(erro == 0){
			document.getElementById( 'requisicao' ).value = 'salvar';
			document.getElementById( 'formEntidadeDetalhe' ).submit();
		}
	}
	function alterarDetalheEmenda( emdid, emdtipo ){
		document.getElementById( 'tipobotao' ).value = 'Salvar';
		document.getElementById( 'botao' ).value = 'salvar';
		document.getElementById( 'btnSalvar' ).disabled = false;
		document.getElementById( 'btnExtra' ).disabled = true;
		document.getElementById( 'btnModalidade' ).disabled = true;
	
		document.getElementById( 'requisicao' ).value = 'alterar';
		document.getElementById( 'emdid' ).value = emdid;
		document.getElementById( 'emdtipo' ).value = emdtipo;
		document.getElementById( 'tipoemenda' ).value = 'E';
		document.getElementById( 'formEntidadeDetalhe' ).submit();
	}
	function inserirRecurso(){
		document.getElementById( 'tipobotao' ).value = 'Salvar Recurso Extra';
		document.getElementById( 'btnSalvar' ).value = 'Salvar Recurso Extra';
		document.getElementById( 'botao' ).value = 'extra';
		document.getElementById( 'btnSalvar' ).disabled = false;
		document.getElementById( 'btnExtra' ).disabled = true;
		document.getElementById( 'btnModalidade' ).disabled = true;
		
		document.getElementById( 'emdtipo' ).value = '';
		document.getElementById( 'tipoemenda' ).value = 'R';
		document.getElementById( 'formEntidadeDetalhe' ).submit();
	}
	function excluirDetalheEmenda(emdid, emdtipo){
		document.getElementById( 'requisicao' ).value = 'excluir';
		document.getElementById( 'emdid' ).value = emdid;
		document.getElementById( 'emdtipo' ).value = emdtipo;
		document.getElementById( 'formEntidadeDetalhe' ).submit();
	}
	function habilitaCampos(){
		var tipo = document.getElementById( 'tipobotao' ).value;
		
		if( tipo == 'Salvar Modalidade' || tipo == 'Salvar Recurso Extra' || tipo == 'Salvar' ){
			$('#formEntidadeDetalhe select, #formEntidadeDetalhe input[type=text]').attr("disabled",false);
		} else {
			$('#formEntidadeDetalhe select, #formEntidadeDetalhe input[type=text]').attr("disabled",true);
		}		
	}
	function inserirModalidade(){
		document.getElementById( 'tipobotao' ).value = 'Salvar Modalidade';
		document.getElementById( 'btnSalvar' ).value = 'Salvar Modalidade';
		document.getElementById( 'botao' ).value = 'modalidade';
		document.getElementById( 'btnSalvar' ).disabled = false;
		document.getElementById( 'btnExtra' ).disabled = true;
		document.getElementById( 'btnModalidade' ).disabled = true;
	
		document.getElementById( 'tipobotao' ).value = 'Salvar Modalidade';
		document.getElementById( 'emdtipo' ).value = '';
		document.getElementById( 'tipoemenda' ).value = 'M';
		document.getElementById( 'formEntidadeDetalhe' ).submit();
	}
	function cancelarRequisicao(){
		window.location.href = window.location; 
	}
</script>